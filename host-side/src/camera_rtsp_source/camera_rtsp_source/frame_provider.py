import gi

gi.require_version("Gst", "1.0")
gi.require_version("GstRtspServer", "1.0")
from gi.repository import Gst, GstRtspServer, GLib

Gst.init(None)

PIPELINE = (
    "libcamerasrc"
    "! video/x-raw,width=1920,height=1080 "
    "! queue leaky=downstream max-size-buffers=1 max-size-time=0 max-size-bytes=0 "
    "! v4l2convert "
    "! v4l2h264enc "
    "! video/x-h264,level=(string)5.1,profile=baseline "
    "! rtph264pay pt=96 config-interval=1"
)


class Factory(GstRtspServer.RTSPMediaFactory):
    def __init__(self):
        super().__init__()
        self.set_shared(True)

    def do_create_element(self, url):
        return Gst.parse_launch(PIPELINE)


server = GstRtspServer.RTSPServer()
server.set_service("8554")
mounts = server.get_mount_points()
mounts.add_factory("/cam", Factory())
server.attach(None)

print("RTSP ready: rtsp://0.0.0.0:8554/cam")
GLib.MainLoop().run()
