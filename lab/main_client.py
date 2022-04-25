from hands import *

from pythonosc.udp_client import SimpleUDPClient

#ip = "127.0.0.1"
#port = 9010
mediapipe_camera_hands(SimpleUDPClient('127.0.0.1', 9010))

