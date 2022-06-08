## Allolib playground & Evan Nguyen S22
[![Build Status](https://travis-ci.org/AlloSphere-Research-Group/allolib_playground.svg?branch=master)](https://travis-ci.org/AlloSphere-Research-Group/allolib_playground)

Please see the original repository, found here: https://github.com/AlloSphere-Research-Group/allolib_playground

### ALL OF MY WORK IS CONTAINED INSIDE THE LAB FOLDER. Please consult that for the main bulk of the work I've done.

This passion project was designed to accomplish a few things:
 - Bridge the gap between external applications and allolib, using the handy property of allolib being server-based.
 - Develop a 'server' that would handle the messages of a python UDP client, and provide a clear example for both.
 - Demonstrate coding skills and become more acquainted with larger codebases; splitting files in allolib, throttling UDP requests, class permeance, efficient algorithms, etc.
 - Manipulate hand-tracking data into usable, translatable information; come up with interesting solutions for determining 'gestures'.
 - Understand primitive music theory concepts, chord progression, note-frequency correlation, etc.
 - Attempt subtractive synthesis for an acoustic piano.

This repository contains several branches that I will progressively publish to main, because I like compartmentalizing my work.
# localpython 
This is my main project. This uses a UDP server from allolib (with courtesy of Lance Putnam, 2014) combined with a UDP client on python - by me. The goal here is to bridge the gap between more complex apps that are more suitably ran on python, to still interact within a c++ allolib environment. The project itself is an attempt at hand-controlled/gesture instrumentals, which will give the user full control through hand motion alone (webcam needed).

# bazel 
Initial tests in C++ to import mediapipe through bazel. It successfully compiles and all the windows build commandlines (which were modified for my system), can be found in log.txt. This is a deprecated branch since it seems like linking allolib with bazel seems like a horrible idea, just to get some information for hand landmarks. Scrapped and eventually opted for a 'server' approach.

