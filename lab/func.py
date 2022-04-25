import numpy as np

import mediapipe as mp
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

finger_denote = []
for point in mp_hands.HandLandmark:
  finger_denote.append(point)

def process(landmarks, client):
    if landmarks:
        for handLandmarks in landmarks:
            thumb_norm = handLandmarks.landmark[finger_denote[4]]
            index_norm = handLandmarks.landmark[finger_denote[8]]

            t_vec = np.array([thumb_norm.x, thumb_norm.y])
            i_vec = np.array([index_norm.x, index_norm.y])

            avg_x = (thumb_norm.x + index_norm.x) / 2.0

            dist = np.linalg.norm(t_vec-i_vec)
            
            if(dist <= 0.1) and (np.linalg.norm(thumb_norm.z - index_norm.z) <= 0.05):
                client.send_message("/frequency/", 1-avg_x)