#include <cstdio>  // for printing to stdout

#include "Gamma/Analysis.h"
#include "Gamma/Effects.h"
#include "Gamma/Envelope.h"
#include "Gamma/Gamma.h"
#include "Gamma/Oscillator.h"
#include "Gamma/Types.h"
#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/scene/al_PolySynth.hpp"
#include "al/scene/al_SynthSequencer.hpp"
#include "al/ui/al_ControlGUI.hpp"
#include "al/ui/al_Parameter.hpp"

using namespace gam;
using namespace al;
using namespace std;
class Sub : public SynthVoice {
public:

    // Unit generators
    float mNoiseMix;
    gam::Pan<> mPan;
    gam::ADSR<> mAmpEnv;
    gam::EnvFollow<> mEnvFollow;  // envelope follower to connect audio output to graphics
    
    gam::DSF<> mOsc;
    gam::DSF<> mOsc3;
    gam::DSF<> mOsc5;
    gam::DSF<> mOsc7;

    gam::NoiseWhite<> mNoise;
    gam::Reson<> mRes;
    gam::Env<2> mCFEnv;
    gam::Env<2> mBWEnv;
    // Additional members
    Mesh mMesh;

    // Initialize voice. This function will nly be called once per voice
    void init() override {
        mAmpEnv.curve(0); // linear segments
        mAmpEnv.levels(0,0.6,0.5,0); // These tables are not normalized, so scale to 0.3
        mAmpEnv.sustainPoint(2); // Make point 2 sustain until a release is issued
        mCFEnv.curve(0);
        mBWEnv.curve(0);

        mOsc.harmonics(12); //12
        mOsc3.harmonics(12); 
        mOsc5.harmonics(12); 
        mOsc7.harmonics(12); 
        // We have the mesh be a sphere
        addDisc(mMesh, 1.0, 30);

        createInternalTriggerParameter("amplitude", 1.0, 0.0, 1.0);
        createInternalTriggerParameter("frequency", 60, 20, 5000);
        createInternalTriggerParameter("attackTime", 0.1, 0.01, 3.0);
        createInternalTriggerParameter("releaseTime", 4.0, 0.1, 10.0);
        createInternalTriggerParameter("sustain", 0.4, 0.0, 1.0);
        createInternalTriggerParameter("curve", 4.0, -10.0, 10.0);
        createInternalTriggerParameter("noise", 0.0, 0.0, 1.0);
        createInternalTriggerParameter("envDur",1, 0.0, 5.0);
        createInternalTriggerParameter("cf1", 10.0, 10.0, 5000);
        createInternalTriggerParameter("cf2", 50.0, 10.0, 5000);
        createInternalTriggerParameter("cfRise", 0.1, 0.1, 2);
        createInternalTriggerParameter("bw1", 1200.0, 10.0, 5000);
        createInternalTriggerParameter("bw2", 600.0, 10.0, 5000);
        createInternalTriggerParameter("bwRise", 0.9, 0.1, 2);
        createInternalTriggerParameter("hmnum", 5, 5.0, 20.0);
        createInternalTriggerParameter("hmamp", 0.68, 0.0, 1.0);
        createInternalTriggerParameter("pan", 0.0, -1.0, 1.0);

    }

    //
    
    virtual void onProcess(AudioIOData& io) override {
        updateFromParameters();
        float amp = getInternalParameterValue("amplitude");
        float noiseMix = getInternalParameterValue("noise");
        while(io()){
            // mix oscillator with noise
            //float s1 = mOsc()*(1-noiseMix) + mNoise()*noiseMix;
            float s1 = mNoise()*noiseMix + (1-noiseMix)*(
                mOsc() * amp +
                mOsc3() * (amp/3.0) +
                mOsc5() * (amp/5.0) + 
                mOsc7() * (amp/7.0)
            );

            // apply resonant filter
            mRes.set(mCFEnv(), mBWEnv());
            s1 = mRes(s1);

            // appy amplitude envelope
            s1 *= mAmpEnv();

            float s2;
            mPan(s1, s1,s2);
            io.out(0) += s1;
            io.out(1) += s2;
        }
        
        
        if(mAmpEnv.done() && (mEnvFollow.value() < 0.001f)) free();
    }

   virtual void onProcess(Graphics &g) {
          float frequency = getInternalParameterValue("frequency");
          float amplitude = getInternalParameterValue("amplitude");
          g.pushMatrix();
          g.translate(amplitude,  amplitude, -4);
          //g.scale(frequency/2000, frequency/4000, 1);
          float scaling = 0.1;
          g.scale(scaling * frequency/200, scaling * frequency/400, scaling* 1);
          g.color(mEnvFollow.value(), frequency/1000, mEnvFollow.value()* 10, 0.4);
          g.draw(mMesh);
          g.popMatrix();
   }
    virtual void onTriggerOn() override {
        updateFromParameters();
        mAmpEnv.reset();
        mCFEnv.reset();
        mBWEnv.reset();
        
    }

    virtual void onTriggerOff() override {
        mAmpEnv.triggerRelease();
//        mCFEnv.triggerRelease();
//        mBWEnv.triggerRelease();
    }

    void updateFromParameters() {
        float f = getInternalParameterValue("frequency");
        mOsc.freq(f);
        mOsc3.freq(f*3);
        mOsc5.freq(f*5);
        mOsc7.freq(f*7);

        //mOsc.harmonics(getInternalParameterValue("hmnum"));
        //mOsc.ampRatio(getInternalParameterValue("hmamp"));
        /**
        mOsc3.harmonics(getInternalParameterValue("hmnum"));
        mOsc3.ampRatio(getInternalParameterValue("hmamp"));

        mOsc5.harmonics(getInternalParameterValue("hmnum"));
        mOsc5.ampRatio(getInternalParameterValue("hmamp"));

        mOsc7.harmonics(getInternalParameterValue("hmnum"));
        mOsc7.ampRatio(getInternalParameterValue("hmamp"));
        **/

        mAmpEnv.attack(getInternalParameterValue("attackTime"));
    //    mAmpEnv.decay(getInternalParameterValue("attackTime"));
        mAmpEnv.release(getInternalParameterValue("releaseTime"));
        mAmpEnv.levels()[1]=getInternalParameterValue("sustain");
        mAmpEnv.levels()[2]=getInternalParameterValue("sustain");

        mAmpEnv.curve(getInternalParameterValue("curve"));
        mPan.pos(getInternalParameterValue("pan"));
        mCFEnv.levels(getInternalParameterValue("cf1"),
                      getInternalParameterValue("cf2"),
                      getInternalParameterValue("cf1"));


        mCFEnv.lengths()[0] = getInternalParameterValue("cfRise");
        mCFEnv.lengths()[1] = 1 - getInternalParameterValue("cfRise");
        mBWEnv.levels(getInternalParameterValue("bw1"),
                      getInternalParameterValue("bw2"),
                      getInternalParameterValue("bw1"));
        mBWEnv.lengths()[0] = getInternalParameterValue("bwRise");
        mBWEnv.lengths()[1] = 1- getInternalParameterValue("bwRise");

        mCFEnv.totalLength(getInternalParameterValue("envDur"));
        mBWEnv.totalLength(getInternalParameterValue("envDur"));
    }
};


