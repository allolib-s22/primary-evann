#CREATED BY EVAN NGUYEN.
#THIS APP WORKS BEST BY SEEING YOUR ENTIRE PALM; FOR BEST RESULTS, FACE HANDS PERPENDICULAR TO CAMERA, SUCH THAT YOUR FINGERS POINT VERTICAL. 

#Instructions----------------------------------------
#The way this code works is to extract 'landmarks' from hand positions, the diagram of which you should consult on my github repo.
#We deem that a finger is 'pressed', when the tip of a finger and its knuckle are the same distance to your palm - if you press the way I do.
#Try making this shape, it looks like a question mark or a hook of sorts.

#IF THIS APP FAILS TO DETECT YOUR PRESSES IN CONSOLE, PLEASE SEE THE press_threshold VALUE BELOW
#PRESETS FOLLOW

import numpy as np

import mediapipe as mp
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

import math
import random
from chords import *

finger_denote = []
for point in mp_hands.HandLandmark:
  finger_denote.append(point)

key_map_w = [0, 2, 4, 5, 7, 9, 11]
key_map_b = [1, 3, -1, 6, 8, 10, -1]

#---PRESETS-------------------------------------------------------------------------------------
#THIS IS THE THRESHOLD FOR PRESSING. THE LOWER, THE MORE STRICT. HIGHER VALUES MEAN MORE LIKELY TO DETECT
press_threshold_finger = 5/100  #These numbers need to be heavily tweaked, or set with some 'calibration' sequence
press_threshold_thumb = 10/100  #This is for the thumb. Kind of hard to detect, since it has fewer landmarks than a normal finger.

A_4 = 440
octL = 12 + (3*12) #Lowest C position possible, usually multiples of 12 from 24.  
                   #Here, it is in the 48'th position, or the fourth octave.
SPAN = 7 * 2 #SPAN IN WHITE KEYS, so usually a multiple of 7.
             #This value corresponds to how many 'white-key' lengths the window screen will support. 
             #In this case, the entire width of the screen maps to 14 keys. 
             #Shouldn't really be higher than 20.
MODE = 1 #1 for chords on the left hand (for now)

#For chord mode
last_played_chord = "I"
next_chord_options = get_next_set(last_played_chord) #Start in root position
chord_selector_index = 0

#---FUNCTIONS-----------------------------------------------------------------------------------

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
note = {}
chord_playing = {}
for x in range(0, len(fingers)*2):
  pressed[str(x)] = 0
  note[str(x)] = 0
  chord_playing[str(x)] = []

detecting = False
current_hover_note = octL
temp_chord_buffer = []
hover_chord = "I"

def process(landmarks, handedness, client, shape):
  fact = round(shape[0]/shape[1], 2)
  halfway = shape[1]/2
  width = shape[0]

  global detecting

  global current_hover_note
  global last_played_chord
  global chord_selector_index

  global temp_chord_buffer
  global chord_playing
  global hover_chord 

  confidence = 0

  if not detecting and landmarks:
    detecting = True
    print(detecting)
    print(confidence)
  elif detecting and not landmarks:
    confidence = 0

    detecting = False
    print(detecting)

    #client.send_message("/hoverOff/", current_hover_note)
    #for n in range(0, len(temp_chord_buffer)):
    #  client.send_message("/hoverOff/", n)

    for l in range(0, SPAN):
      client.send_message("/hoverOff/", l+octL)

    for x in range(0, len(fingers)*2):
      pressed[str(x)] = 0
      client.send_message("/triggerOff/", note[str(x)])

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
            press_threshold = press_threshold_finger 
          else:                    
            press_threshold = press_threshold_thumb 

          if MODE and h[l] and i == 2:
            pos = math.ceil((1-b.x) * SPAN)
            if(b.y <= 0.5): #Select black notes
              if(key_map_b[pos % 7]) != -1:
                midiNote = octL + key_map_b[pos % 7] + 12 * (pos // 7)
            else: #Select white notes
              midiNote = octL + key_map_w[pos % 7] + 12 * (pos // 7)

            if(key_map_b[pos % 7] != -1 and current_hover_note != midiNote):
              if(str(midiNote % 12) in ascii_map.keys()):
                character = ascii_map[str(midiNote % 12)]
                if(character in next_chord_options.keys()):
                  hover_chord = next_chord_options[character][chord_selector_index]
                  chord_notes = chordToNotes(hover_chord)
                  c = random.random()
                  
                  for pr in temp_chord_buffer:
                    client.send_message("/hoverOff/", pr)
                  temp_chord_buffer = []

                  client.send_message("/hoverOff/", current_hover_note)
                  current_hover_note = midiNote
                  print("hovering ", current_hover_note)

                  for n in chord_notes:
                    v = note_map[n] + midiNote - (midiNote % 12)
                    temp_chord_buffer.append(v)
                    client.send_message("/noteColor/", [v, c])
              else:
                client.send_message("/hoverOff/", current_hover_note)
                
                for pr in temp_chord_buffer:
                    client.send_message("/hoverOff/", pr)
                temp_chord_buffer = []
                current_hover_note = midiNote
                client.send_message("/hoverOn/", midiNote)
          
          if(d < press_threshold) and not pressed[str(i)]:  
    
            if MODE and h[l] and i == 2:
              #test = 1
              chord_press_handler(client, temp_chord_buffer, i)
            elif i > len(fingers):
              normal_gesture_press_handler(client, i, b)

          elif(d >= press_threshold) and pressed[str(i)]:
            pressed[str(i)] = 0
            print(i, " released")

            if MODE and h[l] and i == 2:
              #test = 1
              chord_lift_handler(client, i)
            elif i > len(fingers):
              normal_gesture_lift_handler(client, i)
  
          i += 1

        l += 1
    

def normal_gesture_press_handler(client, i, b):
  #MAIN MODIFICATION OF MIDI NOTE HERE.
  #midiNote = int((1-b.x) * (80-60) + 60)

  pos = math.ceil((1-b.x) * SPAN)
  pressed[str(i)] = 1

  if(b.y <= 0.5): #Select black notes
    if(key_map_b[pos % 7]) != -1:
      midiNote = octL + key_map_b[pos % 7] + 12 * (pos // 7)
  else: #Select white notes
    midiNote = octL + key_map_w[pos % 7] + 12 * (pos // 7)


  if(key_map_b[pos % 7]) != -1 or b.y > 0.5:
    o = pow(2, ((midiNote - 69) / 12)) * A_4 #We don't need to do the calculation here, but since the other end is a 'server', we want to reduce its features.
    #o = (600-200)*(1-b.x) + 200

    #midiNote = max(midiNote, octL)
    note[str(i)]=midiNote #Clamp our value

    print(i, " pressed note ", midiNote)

    client.send_message("/frequency/", o)
    client.send_message("/triggerOn/", note[str(i)])

def normal_gesture_lift_handler(client, i):
  client.send_message("/triggerOff/", note[str(i)])

def chord_press_handler(client, noteList, i):
  last_played_chord = hover_chord
  print("played ", last_played_chord)
  next_chord_options = get_next_set(last_played_chord)

  chord_playing[str(i)] = noteList
  pressed[str(i)] = 1
  for n in chord_playing[str(i)]:
    o = pow(2, ((n - 69) / 12)) * A_4
    client.send_message("/frequency/", o)
    client.send_message("/triggerOn/", n)

def chord_lift_handler(client, i):
  for n in chord_playing[str(i)]:
    client.send_message("/triggerOff/", n)

'''Deprecated. Merged into the process function.
def chord_gesture_hover_handler(client, b):
  pos = math.ceil((1-b.x) * SPAN)
  if(b.y <= 0.5): #Select black notes
    if(key_map_b[pos % 7]) != -1:
      midiNote = octL + key_map_b[pos % 7] + 12 * (pos // 7)
  else: #Select white notes
    midiNote = octL + key_map_w[pos % 7] + 12 * (pos // 7)

  if current_hover_note != midiNote:
    current_hover_note = midiNote
    client.send_message("/hoverOn/", midiNote)
'''