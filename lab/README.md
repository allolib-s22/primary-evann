# This is the main folder for all my work in S22.
The hand-tracking client supports two modes, one that allows normal presses with all the main fingers (thumb is still buggy), and a mode that has automatic chord progression detection, based on the position of the index finger and the middle key of the chord. The solution is made by google, and I used the landmark data that it outputs, which you can see here: https://google.github.io/mediapipe/solutions/hands.html (Fig 2.21 hand landmarks).

<img src="https://cdn.discordapp.com/attachments/599549030707625985/983985858401148988/unknown.png" width=50% height=50%>
<img src="https://cdn.discordapp.com/attachments/599549030707625985/983986413383061514/unknown.png" width=50% height=50%>

###### Above: A normal keystroke in mode 0, where in mode 1 a lightly blue hover outline of a chord appears.

# Here's the files included in this folder.
In order to start the app, please run the server using `./run.sh lab/OSC_server.cpp`
Then run the python app `main_client.py`, either through a debugger or terminal.

You should be able to find more details on setting parameters and presets in `func.py` and `OSC_server.cpp`.

## All libs and components to run the app successfully

`func.py` - processing functions for the hand landmarks

`hands.py` - mediapipe's hand solution, but put into a function that accepts a client.

`main_client.py` - main executable. This is the client that will send hand-tracking messages over UDP to allolib. RUN THIS.

`chords.py` - With courtesy from Ryan He in S22, this is the python-equivalent of his chord progression vector/map. The original is under sample/chords.hpp.

`OSC_server.cpp` - this is the allolib OSC server that MUST be concurrently running with main_client.py in order to receive messages. RUN THIS.

`piano.hpp` - This is a helper library that I wrote that handles the piano gui entirely. This allows customizable animation effects, i.e. decay time for key highlighting, key colors, number of keys, key width, etc. 

## Minor files for demonstration/plug and play.

`SineEnv.cpp` or `SubSyn.cpp` - this is where the voice comes from. Can be interchanged with other sound files, just make sure you import a valid SynthVoice. SubSyn.cpp is a personal attempt at an acoustic piano sound. However, with limited time I could only wrap my head around the basics of synthesizing one, and left it incomplete.

`sample/osc_client.py` - a really barebones client that will send UDP.

`sample/010_SimpleSineEnv.cpp` - demonstration file that I based alot of the sound features from.

## Further notes

This application has been a rough series of debugging sessions after one and every iteration of an idea. There exists many parameters inside func.py with proper documentation to help someone 'calibrate' the system to their needs, but I reckon that this won't be enough. Always does a miniscule bug, or a single line of code act up every now and then. And sometimes, the detection for presses can be quite sporadic if your finger straddles the border between two notes, or two states- leading to a spasm of sounds. This is just to preface that this app is FAR from perfect, but it's the best state I could get it in. I am severely limited by my knowledge in ML, and the speed of my system. 

If it does work for someone, that will be a dream come true for me.
