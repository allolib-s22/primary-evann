# This is the main folder for all my work in S22.
Here's the files included in this folder.
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

`SineEnv.cpp` or `SubSyn.cpp` - this is where the voice comes from. Can be interchanged with other sound files, just make sure you import a valid SynthVoice.

`sample/osc_client.py` - a really barebones client that will send UDP.

`sample/010_SimpleSineEnv.cpp` - demonstration file that I based alot of the sound features from.
