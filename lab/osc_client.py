from pythonosc.udp_client import SimpleUDPClient

ip = "127.0.0.1"
port = 9010

client = SimpleUDPClient(ip, port)  # Create client

client.send_message("/test", 12)   # Send float message
