/*
Allocore Example: OSC Server
Description:
This is a simple OSC server that listens for packets with the address "/test"
and containing a string and int.
You should run the OSC client example AFTER running this program.
Author:
Lance Putnam, Oct. 2014
*/

#include <iostream>
#include <string>
#include "al/app/al_App.hpp"

#include <cstdio>  // for printing to stdout

#include "SineEnv.cpp"

#include "Gamma/Analysis.h"
#include "Gamma/Effects.h"
#include "Gamma/Envelope.h"
#include "Gamma/Oscillator.h"

#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/scene/al_PolySynth.hpp"
#include "al/scene/al_SynthSequencer.hpp"
#include "al/ui/al_ControlGUI.hpp"
#include "al/ui/al_Parameter.hpp"

using namespace al;

// App has osc::PacketHandler as base class
struct MyApp : public App {


  SynthGUIManager<SineEnv> synthManager{"SineEnv"};


  // can give params in ctor
  // osc::Recv server {16447, "", 0.05};

  // or open later with `open` interface (at onCreate in this example)
  osc::Recv server;

  // MyApp()
  /* Constructor args:
          port, client IP address ("" for any), timeout in seconds
  The port MUST match the client's port. The timeout specifies how often
  the server checks for new data on the on the port. You will	want to tune
  this to rate at which the client is sending packets.*/
  // :	server(16447, "", 0.05)
  void onCreate() override {
    // Print out our IP address
    // std::cout << "SERVER: My IP is " << Socket::hostIP() << "\n";

    // port, address, timeout
    // "" as address for localhost
    server.open(16447, "localhost", 0.05);

    // Register ourself (osc::PacketHandler) with the server so onMessage
    // gets called.
    server.handler(oscDomain()->handler());

    // Start a thread to handle incoming packets
    server.start();

    // initWindow(Window::Dim(160), "OSC server");

    

    //SECTION BREAK ---------------------------------------------------



    navControl().active(false);  // Disable navigation via keyboard, since we
                                 // will be using keyboard for note triggering

    // Set sampling rate for Gamma objects from app's audio
    gam::sampleRate(audioIO().framesPerSecond());

    imguiInit();

    // Play example sequence. Comment this line to start from scratch
    // synthManager.synthSequencer().playSequence("synth1.synthSequence");
    synthManager.synthRecorder().verbose(true);
  }

  // The audio callback function. Called when audio hardware requires data
  void onSound(AudioIOData& io) override {
    synthManager.render(io);  // Render audio
  }

  void onAnimate(double dt) override {
    // The GUI is prepared here
    imguiBeginFrame();
    // Draw a window that contains the synth control panel
    synthManager.drawSynthControlPanel();
    imguiEndFrame();
  }

  // The graphics callback function.
  void onDraw(Graphics& g) override {
    g.clear();
    // Render the synth's graphics
    synthManager.render(g);

    // GUI is drawn here
    imguiDraw();
  }

  // Whenever a key is pressed, this function is called
  bool onKeyDown(Keyboard const& k) override {
    // const float A4 = 432.f;
    const float A4 = 440.f;
    if (ParameterGUI::usingKeyboard()) {  // Ignore keys if GUI is using
                                          // keyboard
      return true;
    }
    if (k.shift()) {
      // If shift pressed then keyboard sets preset
      int presetNumber = asciiToIndex(k.key());
      synthManager.recallPreset(presetNumber);
    } else {
      // Otherwise trigger note for polyphonic synth
      int midiNote = asciiToMIDI(k.key());
      std::cout << midiNote << std::endl;
      if (midiNote > 0) {
        synthManager.voice()->setInternalParameterValue(
            "frequency", ::pow(2.f, (midiNote - 69.f) / 12.f) * A4);
        synthManager.triggerOn(midiNote);
      }
    }
    return true;
  }

  // Whenever a key is released this function is called
  bool onKeyUp(Keyboard const& k) override {
    int midiNote = asciiToMIDI(k.key());
    if (midiNote > 0) {
      synthManager.triggerOff(midiNote);
    }
    return true;
  }

  void onExit() override { imguiShutdown(); }






  // This gets called whenever we receive a packet
  void onMessage(osc::Message& m) override {
    //For debugging
    //m.print(); 

    if(m.addressPattern() == "/test"){
      const float A4 = 440.f;
      int midiNote; 
      m >> midiNote;
      synthManager.voice()->setInternalParameterValue(
          "frequency", ::pow(2.f, (midiNote - 69.f) / 12.f) * A4);
      synthManager.triggerOn(midiNote);

      std::cout << "received note " << midiNote << std::endl;
      std::cout << m.addressPattern() << std::endl;

      //synthManager.triggerOff(midiNote);
    }


    if(m.addressPattern() == "/frequency/"){
      const float A4 = 440.f;

      float input;
      m >> input;

      float midiNote = input*(88-60) + 60;
      synthManager.voice()->setInternalParameterValue(
          "frequency", ::pow(2.f, (midiNote - 69.f) / 12.f) * A4);
    
    }
  }
};



int main() { 
  // Create app instance
  MyApp app;

  // Set up audio
  app.configureAudio(48000., 512, 2, 0);

  app.start();

  return 0;
}