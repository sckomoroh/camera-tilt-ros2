import gi

gi.require_version("Gst", "1.0")
from gi.repository import Gst, GLib

from ultralytics import RTDETR
import torch
import math

import numpy as np
import cv2

import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
#from esp_msgs.msg import ServoStatus
from sensor_msgs.msg import Image, JointState
from std_msgs.msg import Float64MultiArray

from control_msgs.action import FollowJointTrajectory
from trajectory_msgs.msg import JointTrajectoryPoint
from builtin_interfaces.msg import Duration
from rclpy.executors import SingleThreadedExecutor

from threading import Thread


class CameraHelper:
    @staticmethod
    def clamp(v, lo, hi):
        return lo if v < lo else hi if v > hi else v

    @staticmethod
    def bbox2positions(
        x1,
        y1,
        x2,
        y2,
        frame_width,
        frame_height,
        tilt_position,
        camera_position,
        fov_x,
        fov_y,
        Kp_x,
        Kp_y,
        logger
    ):
        # angle_clamp = 0.01745
        center_x = 0.5 * (x1 + x2)
        center_y = 0.5 * (y1 + y2)

        nx = (center_x - 0.5 * frame_width) / (0.5 * frame_width)
        ny = (center_y - 0.5 * frame_height) / (0.5 * frame_height)

        correct_fov_x = abs(0.5 * frame_width -center_x) / (0.5 * frame_width)
        correct_fov_y = abs(0.5 * frame_height -center_y) / (0.5 * frame_height)
        
        err_tile = nx * (fov_x * 0.5)
        err_camera = ny * (fov_y * 0.5)
        
        tilt_offset = Kp_x * err_tile
        camera_offset = Kp_y * err_camera
        
        # tilt_offset = CameraHelper.clamp(tilt_offset, -angle_clamp, angle_clamp)
        # camera_offset = CameraHelper.clamp(camera_offset, -angle_clamp, angle_clamp)
        
        
        if abs(math.degrees(tilt_offset)) < 3.0:
            tilt_offset = 0.0

        if abs(math.degrees(camera_offset)) < 3.0:
            camera_offset = 0.0
        
        logger.info(f"Detection at ({center_x}, {center_y}),offset: ({math.degrees(tilt_offset):.4f}, {math.degrees(camera_offset):.4f}) corrected_fov: {correct_fov_x:.2f}, {correct_fov_y:.2f}")
        
        tilt_t = tilt_position + tilt_offset
        camera_t = camera_position + camera_offset

        tilt_t = CameraHelper.clamp(tilt_t, -math.pi, math.pi)  # PAN ±180°
        camera_t = CameraHelper.clamp(camera_t, -math.pi / 4, math.pi / 2)

        return tilt_t, -camera_t
        # return tilt_t, 0


class InferenceNode(Node):
    CONFIG_PARAMS = {
        "host": "10.42.0.29",
        "port": 8554,
        "path": "cam",
        "tilt_joint_name": "pt_tilt_joint",
        "camera_joint_name": "pt_camera_joint",
        "fov_x": math.radians(120.0),
        "fov_y": math.radians(90.0),
        "Kp_x": 0.6,
        "Kp_y": 0.6,
        "frame_width": 1920,
        "frame_height": 1080,
    }

    def __init__(self):
        super().__init__("inference_node")

        self.config = {}
        self.current_tilt_position = 0.0
        self.current_camera_position = 0.0

        self.joint_names = []
        self._configure()

        self.model = RTDETR("rtdetr-l.pt")

        self.camera_helper = CameraHelper()
        self.frame_publisher = self.create_publisher(Image, "/camera_frame", 10)

        self.frame_subscriber = self.create_subscription(
            Image, "/pt_camera/image", self._frame_handler, 10
        )


        self.servo_status_sub = self.create_subscription(
            JointState, "/joint_states", self._servo_status_callback, 10
        )

        self.pos_publisher = self.create_publisher(
            Float64MultiArray, "/forward_position_controller/commands", 10)

        self.statuses = {}
        
        self.get_logger().info("Inference Node initialized with config:")
        for param_name, value in self.config.items():
            self.get_logger().info(f"  {param_name}: {value}")

    def _configure(self):
        for param_name, default_value in self.CONFIG_PARAMS.items():
            self.declare_parameter(param_name, default_value)
            self.config[param_name] = self.get_parameter_or(
                param_name, default_value
            ).value

        self.joint_names = [
            self.config["tilt_joint_name"],
            self.config["camera_joint_name"],
        ]

    def _frame_handler(self, frame_msg):
        # Convert ROS Image message to OpenCV image
        frame = np.frombuffer(frame_msg.data, dtype=np.uint8).reshape(
            (frame_msg.height, frame_msg.width, 3)
        )
        
        results = self.model.track(
                frame, show=False, tracker="bytetrack.yaml", classes=[67], verbose=False
                # frame, show=False, tracker="bytetrack.yaml", classes=[47], verbose=False
            )[0]
        
        detections = results.boxes
        if len(detections) > 0:
            idx = torch.argmax(detections.conf)
            if detections.conf[idx] > 0.4:
                x1, y1, x2, y2 = map(int, detections.xyxy[idx])
                self._on_detection(frame.copy(), x1, y1, x2, y2)
                self.get_logger().info(f"Detection confidence: {detections.conf[idx]:.4f}")
                return
            
        self.get_logger().info("No valid detections")


    def _on_detection(self, frame, x1, y1, x2, y2):
        current_time = self.get_clock().now()
        current_tilt_position = None
        current_camera_position = None
        for status in self.statuses:
            if (current_time.nanoseconds - status) < 200 * 1e6:  # 200ms
                current_tilt_position = self.statuses[status]["tilt"]
                current_camera_position = self.statuses[status]["camera"]
                break
            
        if current_tilt_position is None:
            self.get_logger().error("No recent servo status available, skipping detection.")
            return
            
        target_tilt_position, target_camera_position = CameraHelper.bbox2positions(
            x1,
            y1,
            x2,
            y2,
            self.config["frame_width"],
            self.config["frame_height"],
            current_tilt_position,
            current_camera_position,
            self.config["fov_x"],
            self.config["fov_y"],
            self.config["Kp_x"],
            self.config["Kp_y"],
            self.get_logger()
        )

        msg = Float64MultiArray()
        msg.data = [target_tilt_position, -target_camera_position]
        self.pos_publisher.publish(msg)
        
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
        msg = Image()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.height = frame.shape[0]
        msg.width = frame.shape[1]
        msg.encoding = "bgr8"
        msg.is_bigendian = False
        msg.step = frame.shape[1] * 3
        msg.data = frame.tobytes()
        self.frame_publisher.publish(msg)

    def _servo_status_callback(self, msg: JointState):
        self.current_tilt_position = msg.position[1]
        self.current_camera_position = msg.position[0]
        
        if len(self.statuses) > 20:
            self.statuses.pop(min(self.statuses.keys()))
        
        self.statuses[self.get_clock().now().nanoseconds] = {
            "tilt": self.current_tilt_position,
            "camera": self.current_camera_position
        }


def main():
    Gst.init(None)
    rclpy.init()

    rclpy.logging.get_logger("inference_node").info("Starting Inference Node")
    node = InferenceNode()
    
    rclpy.spin(node)
    
    node.destroy_node()

    rclpy.shutdown()


if __name__ == "__main__":
    main()
