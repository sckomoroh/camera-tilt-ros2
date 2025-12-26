import gi

gi.require_version("Gst", "1.0")
gi.require_version("GstRtspServer", "1.0")
from gi.repository import Gst, GstRtspServer, GLib

Gst.init(None)

PIPELINE = (
    "libcamerasrc  "
    "! video/x-raw,width=1920,height=1080,framerate=30/1,format=NV12  "
    "! queue leaky=downstream max-size-buffers=1 max-size-time=0 max-size-bytes=0  "
    "! videoconvert  "
    "! x264enc tune=zerolatency speed-preset=ultrafast key-int-max=30 bitrate=4000  "
    "! video/x-h264,profile=baseline  "
    "! h264parse config-interval=-1  "
    "! rtph264pay name=pay0 pt=96 config-interval=1"
)

class PipelineFactory(GstRtspServer.RTSPMediaFactory):
    def __init__(self):
        super().__init__()
        self.set_shared(True)

    def do_create_element(self, url):
        return Gst.parse_launch(PIPELINE)

def main():
    server = GstRtspServer.RTSPServer()
    server.set_service("8554")
    mounts = server.get_mount_points()
    mounts.add_factory("/cam", PipelineFactory())
    server.attach(None)

    print("RTSP ready: rtsp://0.0.0.0:8554/cam")
    GLib.MainLoop().run()


if __name__ == "__main__":
    main()