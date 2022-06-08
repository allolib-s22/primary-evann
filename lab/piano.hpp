#ifndef PIANO_H
#define PIANO_H

#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/scene/al_PolySynth.hpp"
#include "al/scene/al_SynthSequencer.hpp"
#include "al/ui/al_ControlGUI.hpp"
#include "al/ui/al_Parameter.hpp"

#include "al/graphics/al_Font.hpp"

#include <string>
#include <vector>

using namespace al;
using namespace std;

//Small GUI library written by Evan Nguyen. As of now, supports key number, key glow delay, key colors, and valid chord progressions.
//Built to be used in tandem with my server.cpp and hands.py client, main_client.py. 

class Piano {
    public:
        // Mesh and variables for drawing piano keys
        Mesh meshKey;
        float keyWidth, keyHeight;
        float keyPadding = 2.f;
        float fontSize;

        float GLOBAL_DECAY = 0.95;

        int N; //Number of WHITE keys
        float screenWidth, screenHeight;
        
        float* press; // key down
        float* decay;
        int* key_map_black;
        int* key_map_white;
        float* hsv_s;
        float* hsv_h;

        // Font renderer
        //FontRenderer fontRender;

        // This is just for the purposes of demonstrating chords, not really a feature of the main GUI:
        int max_colors = 21;

        vector<string> valid_progressions;

        Piano::Piano(){
            N = 1;

            screenWidth = 1;
            screenHeight = 1;

            this->selfInit();
        }

        Piano::Piano(int keys){
            N = keys;

            screenWidth = 1;
            screenHeight = 1;

            this->selfInit();
        }

        Piano::Piano(int keys, float scrWidth, float scrHeight){
            N = keys;
            
            screenWidth = scrWidth;
            screenHeight = scrHeight;

            this->selfInit();
        }

        Piano::~Piano(){
            delete[] press;
            delete[] decay;
            delete[] hsv_s;
            delete[] hsv_h;
            delete[] key_map_black;
            delete[] key_map_white;
        }

        void Piano::setWidth(float scrWidth){
            screenWidth = scrWidth;
            this->selfInit();
        }

        void Piano::setHeight(float scrHeight){
            screenHeight = scrHeight;
            this->selfInit();
        }

        void Piano::selfInit(){
            press = new float[200]; //Initialized to 112 because of typical keyboard limitations. 
            decay = new float[200]; //Should not specify more than 112.
            hsv_s = new float[200]; 
            hsv_h = new float[200];
            key_map_black = new int[200];
            key_map_white = new int[200];

            float w = screenWidth;
            float h = screenHeight;
            keyWidth = w / 10.f - keyPadding * 2.f;
            keyHeight = h / 2.f - keyPadding * 2.f;
            //fontSize = keyWidth * 0.2;

            for (int i = 0; i < 112; i++) {
                press[i] = 0;
                decay[i] = 1;
                hsv_s[i] = 0;
                hsv_h[i] = 0;
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
            //fontRender.load(Font::defaultFont().c_str(), 60, 1024);
        }
        
        void Piano::draw(Graphics& g){
            for (int i = 0; i < 112; i++) {
                if(abs(press[i]) <= 0.03){
                    hsv_s[i] = 0;
                    press[i] = 0;
                }else{
                    press[i] *= decay[i];
                    hsv_s[i] *= decay[i];
                }
            }
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

                float modifier_press = press[key_map_white[i]];

                g.translate(x, y);
                g.color(HSV(hsv_h[key_map_white[i]], hsv_s[key_map_white[i]], c + modifier_press));
                g.tint(c);
                g.draw(meshKey);
                
                g.tint(1);
                //fontRender.write(whitekeyLabels[i].c_str(), fontSize);
                //fontRender.renderAt(g, {keyWidth * 0.5 - 5, keyHeight * 0.1, 0.0});

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

                float modifier_press = press[key_map_black[i]];

                g.translate(x, y);
                g.scale(1, 0.5);
                g.color(HSV(hsv_h[key_map_black[i]], hsv_s[key_map_black[i]], c + 0.4*hsv_s[key_map_black[i]] + modifier_press));
                g.tint(c);
                g.draw(meshKey);
                
                g.scale(1, 2);
                //fontRender.write(blackkeyLabels[i].c_str(), fontSize);
                //fontRender.renderAt(g, {keyWidth * 0.5 - 5, keyHeight * 0.1, 0.0});

                g.popMatrix();
            }
        }

        void keyDown(int key){
            this->press[key] = true;
            this->decay[key] = 1;
            this->hsv_s[key] = 0;
        }

        void keyDown(int key, float highlight){
            this->press[key] = highlight;
            this->decay[key] = 1;
        }

        void keyDown(int key, float highlight, float color){
            this->press[key] = highlight;
            this->decay[key] = 1;
            this->hsv_s[key] = 0.45;
            this->hsv_h[key] = color;
        }

        void keyUp(int key){
            this->decay[key] = GLOBAL_DECAY;
        }

        void keyUp(int key, float decay_rate){
            this->decay[key] = decay_rate;
        }
};

#endif