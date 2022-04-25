import numpy as np

from pythonosc.udp_client import SimpleUDPClient

#ip = "127.0.0.1"
ip = 'localhost'
port = 9010

client = SimpleUDPClient(ip, port)  # Create client

#client.send_message("/test", 12)   # Send float message

import cv2
import mediapipe as mp
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

finger_denote = []
for point in mp_hands.HandLandmark:
  finger_denote.append(point)

# For webcam input:
cap = cv2.VideoCapture(0)
with mp_hands.Hands(
    model_complexity=0,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5) as hands:
  while cap.isOpened():
    success, image = cap.read()
    if not success:
      print("Ignoring empty camera frame.")
      # If loading a video, use 'break' instead of 'continue'.
      continue

    # To improve performance, optionally mark the image as not writeable to
    # pass by reference.
    image.flags.writeable = False
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = hands.process(image)

    imageHeight, imageWidth, _ = image.shape


    if results.multi_hand_landmarks != None:
      '''for handLandmarks in results.multi_hand_landmarks:
        for point in mp_hands.HandLandmark:

            normalizedLandmark = handLandmarks.landmark[point]
            pixelCoordinatesLandmark = mp_drawing._normalized_to_pixel_coordinates(normalizedLandmark.x, normalizedLandmark.y, imageWidth, imageHeight)

            print(point)
            print(pixelCoordinatesLandmark)
            print(normalizedLandmark)
      '''

      for handLandmarks in results.multi_hand_landmarks:
        thumb_norm = handLandmarks.landmark[finger_denote[4]]
        index_norm = handLandmarks.landmark[finger_denote[8]]

        t_vec = np.array([thumb_norm.x, thumb_norm.y, thumb_norm.z])
        i_vec = np.array([index_norm.x, index_norm.y, index_norm.z])

        avg_x = (thumb_norm.x + index_norm.x) / 2.0

        dist = np.linalg.norm(t_vec-i_vec)
        
        if(dist <= 0.1):
          client.send_message("/m1", 1-avg_x)

    # Draw the hand annotations on the image.
    image.flags.writeable = True
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
    if results.multi_hand_landmarks:
      for hand_landmarks in results.multi_hand_landmarks:
        mp_drawing.draw_landmarks(
            image,
            hand_landmarks,
            mp_hands.HAND_CONNECTIONS,
            mp_drawing_styles.get_default_hand_landmarks_style(),
            mp_drawing_styles.get_default_hand_connections_style())
    # Flip the image horizontally for a selfie-view display.
    cv2.imshow('MediaPipe Hands', cv2.flip(image, 1))
    if cv2.waitKey(5) & 0xFF == 27:
      break
cap.release()