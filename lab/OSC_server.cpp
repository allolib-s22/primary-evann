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

#include "al/graphics/al_Shapes.hpp" //Piano
#include "al/graphics/al_Font.hpp"

using namespace al;

#define N 20

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

  // Mesh and variables for drawing piano keys
  Mesh meshKey;
  float keyWidth, keyHeight;
  float keyPadding = 2.f;
  float fontSize;
  
  bool press[N*2]; // key down
  int key_map_black[N];
  int key_map_white[N];

  std::string whitekeyLabels[20] = {"Z","X","C","V","B","N","M",",",".","/",
                                    "Q","W","E","R","T","Y","U","I","O","P"};
  std::string blackkeyLabels[20] = {"S","D","","G","H","J","","L",";","",
                                    "2","3","","5","6","7","","9","0",""};
  // Font renderder
  FontRenderer fontRender;

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
    // Calculate the size of piano keys based on the app window size
    float w = float(width());
    float h = float(height());
    keyWidth = w / 10.f - keyPadding * 2.f;
    keyHeight = h / 2.f - keyPadding * 2.f;
    fontSize = keyWidth * 0.2;

    for (int i = 0; i < N*2; i++) {
      press[i] = false;
    }

    int w_it = 0, b_it = 1;
    for (int i = 0; i < N; i++){
      //std::cout << w_it << " " << b_it << std::endl;
      key_map_white[i] = w_it;
      key_map_black[i] = b_it;
      if(i%7 == 2 || i%7 == 6){
        w_it += 1;
      }else{
        w_it += 2;
      }

      if(i%7 == 1 || i%7 == 5){
        b_it += 1;
      }else{
        b_it += 2;
      }
    }

    // Create a mesh that will be drawn as piano keys
    addRect(meshKey, 0, 0, keyWidth, keyHeight);

    // Set the font renderer
    fontRender.load(Font::defaultFont().c_str(), 60, 1024);

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

    // This example uses only the orthogonal projection for 2D drawing
    g.camera(Viewpoint::ORTHO_FOR_2D);  // Ortho [0:width] x [0:height]

    // Drawing white piano keys
    for(int i = 0; i < N; i++) {
      int index = i % N;
      g.pushMatrix();
      
      float c = 0.9;
      float x = (keyWidth + keyPadding * 2) * index + keyPadding;
      float y = 0;

      /**
      if(i >= 10) {
        y = keyHeight + keyPadding * 2;
      }
      **/

      float modifier_press = press[key_map_white[i]] ? 1 : 0;

      g.translate(x, y);
      g.color(c + modifier_press);
      g.tint(c);
      g.draw(meshKey);
      
      g.tint(1);
      fontRender.write(whitekeyLabels[i].c_str(), fontSize);
      fontRender.renderAt(g, {keyWidth * 0.5 - 5, keyHeight * 0.1, 0.0});

      g.popMatrix();
    }

    // Drawing black piano keys
    for(int i = 0; i < N; i++) {
      int index = i % N;
      if(index%7 == 2 || index%7 == 6) continue;

      g.pushMatrix();
      
      float c = 0.5;      
      float x = (keyWidth + keyPadding * 2) * index + keyPadding + keyWidth * 0.5;
      float y = keyHeight * 0.5;

      /**
      if(i >= 10) {
        y = y + keyHeight + keyPadding * 2;
      }
      **/

      float modifier_press = press[key_map_black[i]] ? 1 : 0;

      g.translate(x, y);
      g.scale(1, 0.5);
      g.color(c + modifier_press);
      g.tint(c);
      g.draw(meshKey);
      
      g.scale(1, 2);
      fontRender.write(blackkeyLabels[i].c_str(), fontSize);
      fontRender.renderAt(g, {keyWidth * 0.5 - 5, keyHeight * 0.1, 0.0});
      
      g.popMatrix();
    }
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

    /**
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
    **/

    if(m.addressPattern() == "/frequency/"){
      float i;
      m >> i;

      synthManager.voice()->setInternalParameterValue(
          "frequency", i);
    }

    if(m.addressPattern() == "/triggerOn/"){
      int i;
      m >> i;

      press[i] = true;
      synthManager.triggerOn(i);
    }

    if(m.addressPattern() == "/triggerOff/"){
      int i;
      m >> i;

      press[i] = false;
      synthManager.triggerOff(i);
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