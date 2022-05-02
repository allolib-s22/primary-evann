Here's the files included in this folder.

`func.py` - processing functions for the hand landmarks

`hands.py` - mediapipe's hand solution, but put into a function that accepts a client.

`main_client.py` - main executable. this is the client that will send hand-tracking messages over UDP to allolib.

`OSC_server.cpp` - this is the allolib OSC server that MUST be concurrently running with main_client.py in order to receive messages.

`SineEnv.cpp` - this is where the voice comes from. Can be interchanged with other sound files.

`sample/osc_client.py` - a really barebones client that will send UDP.

`sample/010_SimpleSineEnv.cpp` - demonstration file that I based alot of the sound features from.
