/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "maximilian.h"
#include "libs/maxiFFT.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,private Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

    void timerCallback() override;
    void drawFrame (Graphics& g);
    
private:
    //==============================================================================
    // Your private member variables go here...
    maxiSample s1, s2;
    maxiFFT fft_1, fft_2, fft_3;
    maxiIFFT ifft;
    maxiFilter bp;
    TextButton sampleSwitch;
    Label switchLabel;
    
    bool option = true;
    
    maxiSVF f1, f2;
    
    int cutoff1Hz = 8000;
    int cutoff2Hz = 20;
    float gain_1 = 1;
    float gain_2 = 1;
    float resonance1 = 0.2;
    float resonance2 = 0.2;
    float sampleSpeed1 = 1;
    float sampleSpeed2 = 1;
    
    bool nextFFTblockReady1 = false;
    
    // Sliders:
    Slider cutoff1, res1, cutoff2, res2, speed1, speed2, gain1, gain2;
    Label cutoffLabel1, resLabel1, cutoffLabel2, resLabel2, speedLabel1, speedLabel2, gainLabel1, gainLabel2;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
