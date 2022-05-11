import numpy as np

import mediapipe as mp
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

finger_denote = []
for point in mp_hands.HandLandmark:
  finger_denote.append(point)

A4 = 440.0

#todo: 
#must develop a method of getting 'rays', where each limb closest to the hand goes.
#when the tip diverges from the ray, is when the press starts

def lineseg_dist(p, a, b):

    # normalized tangent vector
    d = np.divide(b - a, np.linalg.norm(b - a))

    # signed parallel distance components
    s = np.dot(a - p, d)
    t = np.dot(p - b, d)

    # clamped parallel distance
    h = np.maximum.reduce([s, t, 0])

    # perpendicular distance component
    c = np.cross(p - a, d)

    return np.hypot(h, np.linalg.norm(c))

def vect(landmark, scale_x=1, scale_y=1, weight_z=1):
  return np.array([landmark.x*scale_x, landmark.y*scale_y, landmark.z*weight_z])



#Legacy processing function
'''
thumb_norm = handLandmarks.landmark[finger_denote[4]]
index_norm = handLandmarks.landmark[finger_denote[8]]

t_vec = np.array([thumb_norm.x, thumb_norm.y])
i_vec = np.array([index_norm.x, index_norm.y])

avg_x = (thumb_norm.x + index_norm.x) / 2.0

dist = np.linalg.norm(t_vec-i_vec)

if(dist <= 0.1) and (np.linalg.norm(thumb_norm.z - index_norm.z) <= 0.05):
    client.send_message("/frequency/", 1-avg_x)
'''

#fingers = [[5, 6, 8], [9, 10, 12], [13, 14, 16], [17, 18, 20]]
'''fingers = [[5, 6, 8]]
segDist = []
for f in fingers:
  a = handLandmarks.landmark[finger_denote[f[0]]]
  b = handLandmarks.landmark[finger_denote[f[1]]]
  c = handLandmarks.landmark[finger_denote[f[2]]]

  
  segDist.append(lineseg_dist(vect(c), vect(a), vect(b)))

print(segDist)
'''



fingers = [[4, 2], [8, 5], [12, 9], [16, 13], [20, 17]]

pressed = {}
for x in range(0, len(fingers)*2):
  pressed[str(x)] = 0

detecting = False

def process(landmarks, handedness, client, shape):
  fact = round(shape[0]/shape[1], 2)

  global detecting
  confidence = 0

  if not detecting and landmarks:
    detecting = True
    print(detecting)
    print(confidence)
  elif detecting and not landmarks:
    confidence = 0

    detecting = False
    print(detecting)

    for x in range(0, len(fingers)*2):
      pressed[str(x)] = 0
      client.send_message("/triggerOff/", x)

  if landmarks:
    l = 0 #num of hands#
    h = [] #define hands

    for idx, hand_handedness in enumerate(handedness):
      h.append(hand_handedness.classification[0].label == 'Right') # 1 = Right handed, 0 = Left
      confidence = hand_handedness.classification[0].score

    if confidence >= 0.0:
      for handLandmarks in landmarks:

        a = handLandmarks.landmark[finger_denote[0]]
        if(h[l]):
          i = 0
        else:
          i = len(fingers)

        for f in fingers:
          b = handLandmarks.landmark[finger_denote[f[0]]]
          c = handLandmarks.landmark[finger_denote[f[1]]]

          d = abs(np.linalg.norm(vect(a, scale_y=fact, weight_z=1)-vect(b, scale_y=fact, weight_z=1))-np.linalg.norm(vect(a, scale_y=fact, weight_z=1)-vect(c, scale_y=fact, weight_z=1))) 

          if(i == 5) or (i == 0):
            press_threshold = 4/100 #These numbers need to be heavily tweaked, or set with some 'calibration' sequence
          else:
            press_threshold = 10/100
          
          if(d < press_threshold) and not pressed[str(i)]:
            pressed[str(i)] = 1
            
            A_4 = 440
            midiNote = int((1-b.x) * (80-40) + 40)
            o = pow(2, ((midiNote - 69) / 12)) * A_4
            print(o)
            #o = (600-200)*(1-b.x) + 200

            print(i, " pressed note ", midiNote)
            client.send_message("/frequency/", o)
            client.send_message("/triggerOn/", i)

          elif(d >= press_threshold) and pressed[str(i)]:
            pressed[str(i)] = 0
            print(i, " released")
            client.send_message("/triggerOff/", i)

          i += 1

        l += 1
    





      
      