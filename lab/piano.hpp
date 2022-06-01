#ifndef PIANO_H
#define PIANO_H

#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/scene/al_PolySynth.hpp"
#include "al/scene/al_SynthSequencer.hpp"
#include "al/ui/al_ControlGUI.hpp"
#include "al/ui/al_Parameter.hpp"

#include "al/graphics/al_Font.hpp"

#include <iostream>
#include <unordered_map>
#include <string>
#include <string.h>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace al;
using namespace std;

class Piano {
    public:
        vector<string> allChords = {"I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"};
  unordered_map< string, vector<string> > major = 
    {
      {"I", vector<string>({"I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"})},
      {"I6", vector<string>({"I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"})},
      {"I64", vector<string>({"I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"})},
      {"ii", vector<string>({"I", "V", "viid", "I6", "V6", "viid6", "I64", "V64", "vii64"})},
      {"ii6", vector<string>({"I", "V", "viid", "I6", "V6", "viid6", "I64", "V64", "vii64"})},
      {"ii64", vector<string>({"I", "V", "viid", "I6", "V6", "viid6", "I64", "V64", "vii64"})},
      {"iii", vector<string>({"I", "ii", "IV", "vi", "I6", "ii6", "IV6", "vi6", "I64", "ii64", "IV64", "vi64"})},
      {"iii6", vector<string>({"I", "ii", "IV", "vi", "I6", "ii6", "IV6", "vi6", "I64", "ii64", "IV64", "vi64"})},
      {"iii64", vector<string>({"I", "ii", "IV", "vi", "I6", "ii6", "IV6", "vi6", "I64", "ii64", "IV64", "vi64"})},
      {"IV", vector<string>({"I", "ii", "iii", "V", "viid", "I6", "ii6", "iii6", "V6", "viid6", "I64", "ii64", "iii64", "V64", "viid64"})},
      {"IV6", vector<string>({"I", "ii", "iii", "V", "viid", "I6", "ii6", "iii6", "V6", "viid6", "I64", "ii64", "iii64", "V64", "viid64"})},
      {"IV64", vector<string>({"I", "ii", "iii", "V", "viid", "I6", "ii6", "iii6", "V6", "viid6", "I64", "ii64", "iii64", "V64", "viid64"})},
      {"V", vector<string>({"I", "vi", "I6", "vi6", "I64", "vi64"})},
      {"V6", vector<string>({"I", "vi", "I6", "vi6", "I64", "vi64"})},
      {"V64", vector<string>({"I", "vi", "I6", "vi6", "I64", "vi64"})},
      {"vi", vector<string>({"I", "ii", "iii", "IV", "V", "I6", "ii6", "iii6", "IV6", "V6", "I64", "ii64", "iii64", "IV64", "V64"})},
      {"vi6", vector<string>({"I", "ii", "iii", "IV", "V", "I6", "ii6", "iii6", "IV6", "V6", "I64", "ii64", "iii64", "IV64", "V64"})},
      {"vi64", vector<string>({"I", "ii", "iii", "IV", "V", "I6", "ii6", "iii6", "IV6", "V6", "I64", "ii64", "iii64", "IV64", "V64"})},
      {"viid", vector<string>({"I", "iii", "I6", "iii6", "I64", "iii64"})},
      {"viid6", vector<string>({"I", "iii", "I6", "iii6", "I64", "iii64"})},
      {"viid64", vector<string>({"I", "iii", "I6", "iii6", "I64", "iii64"})}   
    };
        
        // Mesh and variables for drawing piano keys
        Mesh meshKey;
        float keyWidth, keyHeight;
        float keyPadding = 2.f;
        float fontSize;

        int N;
        float screenWidth, screenHeight;
        
        bool* press; // key down
        int* key_map_black;
        int* key_map_white;

        // Font renderer
        //FontRenderer fontRender;

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
            press = new bool[1024];
            key_map_black = new int[1024];
            key_map_white = new int[1024];

            float w = screenWidth;
            float h = screenHeight;
            keyWidth = w / 10.f - keyPadding * 2.f;
            keyHeight = h / 2.f - keyPadding * 2.f;
            //fontSize = keyWidth * 0.2;

            for (int i = 0; i < 1024; i++) {
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
            //fontRender.load(Font::defaultFont().c_str(), 60, 1024);
        }
        
        void Piano::draw(Graphics& g){
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

                float modifier_press = press[key_map_black[i]] ? 1 : 0;

                g.translate(x, y);
                g.scale(1, 0.5);
                g.color(c + modifier_press);
                g.tint(c);
                g.draw(meshKey);
                
                g.scale(1, 2);
                //fontRender.write(blackkeyLabels[i].c_str(), fontSize);
                //fontRender.renderAt(g, {keyWidth * 0.5 - 5, keyHeight * 0.1, 0.0});

                g.popMatrix();
            }
        }
};

#endif