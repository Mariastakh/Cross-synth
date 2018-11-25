/*
  ==============================================================================

 Cross synthesis of two samples, with a filter applied to each sample. The frequency spectrums
 of the original filtered samples and the cross-synthesised result are plotted.

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // /Users/default/Documents/JUCE/Filters_&_CS/project_chord.wav
    // /Users/default/Documents/JUCE/Filters_&_CS/chime.wav
    ///Users/default/Documents/JUCE/Filters_&_CS/FemChoirC.wav
    ///Users/default/Documents/JUCE/Filters_&_CS/MaleChoirC.wav
    s1.load("/Users/default/Documents/JUCE/Filters_&_CS/project_chord.wav");
    s2.load("/Users/default/Documents/JUCE/Filters_&_CS/MaleChoirC.wav");
    // /Users/default/Documents/JUCE/Filters_&_CS/house.wav
    // /Users/default/Documents/JUCE/Filters_&_CS/beat2.wav
    
    
    // Three FFTs, one for each sample, and one to analyse the cross-synthesis result:
    fft_1.setup(1024, 512, 256);
    // fttSize, windowSize, hopSize
    fft_2.setup(1024, 512, 256);
    fft_3.setup(1024, 512, 256);
    // INVERSE FFT: this is the function that will do the cross synthesis itself:
    ifft.setup(1024, 512, 256);

    
    // GUI:
    // Button to swap over the samples, as cross-synthesising the magnitudes of one sample
    // with the phases of another, will bear different results vice versa.
    addAndMakeVisible (sampleSwitch);
    sampleSwitch.setButtonText ("Switch Samples");
    sampleSwitch.onClick = [this] { option= !option; };   // [8]
    sampleSwitch.setColour(TextButton::buttonColourId, Colours::skyblue);
    
    // Filter Sliders
    cutoff1.setSliderStyle(Slider::Rotary);
    cutoff1.setRange(20, 8000);
    cutoff1.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    cutoff1.setColour(Slider::textBoxTextColourId, Colours::black);
    cutoff1.onValueChange = [this] { cutoff1Hz = cutoff1.getValue(); };
    cutoff1.setValue(cutoff1Hz);
    addAndMakeVisible(cutoff1);
    
    cutoffLabel1.setText("Cutoff 1", dontSendNotification);
    cutoffLabel1.setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(cutoffLabel1);
    
    res1.setSliderStyle(Slider::Rotary);
    res1.setRange(0.1, 2.5);
    res1.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    res1.setColour(Slider::textBoxTextColourId, Colours::black);
    res1.onValueChange = [this] { resonance1 = res1.getValue(); };
    res1.setValue(resonance1);
    addAndMakeVisible(res1);
    
    resLabel1.setText("Resonance 1", dontSendNotification);
    resLabel1.setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(resLabel1);
    
    ///
    
    cutoff2.setSliderStyle(Slider::Rotary);
    cutoff2.setRange(20, 2000);
    cutoff2.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    cutoff2.setColour(Slider::textBoxTextColourId, Colours::black);
    cutoff2.onValueChange = [this] { cutoff2Hz = cutoff2.getValue(); };
    cutoff2.setValue(cutoff2Hz);
    addAndMakeVisible(cutoff2);
    
    cutoffLabel2.setText("Cutoff 2", dontSendNotification);
    cutoffLabel2.setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(cutoffLabel2);
    
    res2.setSliderStyle(Slider::Rotary);
    res2.setRange(0.1, 2.5);
    res2.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    res2.setColour(Slider::textBoxTextColourId, Colours::black);
    res2.onValueChange = [this] { resonance2 = res2.getValue(); };
    res2.setValue(resonance2);
    addAndMakeVisible(res2);
    
    resLabel2.setText("Resonance 2", dontSendNotification);
    resLabel2.setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(resLabel2);
    
    // Sample playback speed sliders:
    speed1.setSliderStyle(Slider::Rotary);
    speed1.setRange(0, 10);
    speed1.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    speed1.setColour(Slider::textBoxTextColourId, Colours::black);
    speed1.onValueChange = [this] { sampleSpeed1 = speed1.getValue(); };
    speed1.setValue(sampleSpeed1);
    addAndMakeVisible(speed1);
    
    speedLabel1.setText("Speed 1", dontSendNotification);
    speedLabel1.setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(speedLabel1);
    ///
    
    speed2.setSliderStyle(Slider::Rotary);
    speed2.setRange(0, 10);
    speed2.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    speed2.setColour(Slider::textBoxTextColourId, Colours::black);
    speed2.onValueChange = [this] { sampleSpeed2 = speed2.getValue(); };
    speed2.setValue(sampleSpeed2);
    addAndMakeVisible(speed2);
    
    speedLabel2.setText("Speed 2", dontSendNotification);
    speedLabel2.setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(speedLabel2);

    // Gain Sliders:
    gain1.setSliderStyle(Slider::Rotary);
    gain1.setRange(0, 1);
    gain1.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    gain1.setColour(Slider::textBoxTextColourId, Colours::black);
    gain1.onValueChange = [this] { gain_1 = gain1.getValue(); };
    gain1.setValue(gain_1);
    addAndMakeVisible(gain1);
    
    gainLabel1.setText("Gain 1", dontSendNotification);
    gainLabel1.setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(gainLabel2);
    
    gain2.setSliderStyle(Slider::Rotary);
    gain2.setRange(0, 1);
    gain2.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    gain2.setColour(Slider::textBoxTextColourId, Colours::black);
    gain2.onValueChange = [this] { gain_2 = gain2.getValue(); };
    gain2.setValue(gain_2);
    addAndMakeVisible(gain2);
    
    gainLabel2.setText("Gain 2", dontSendNotification);
    gainLabel2.setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(gainLabel2);
    
    // We need a timer callback in order to keep updating the graphics
    // with the latest batch of frequency spectrum results so we can represent them:
    startTimerHz (30);

    setSize (1050, 600);
    setAudioChannels (2, 2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Get   pointer to the left and right buffers:
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
   
    // The Audio Loop:
    for(int sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        // Grab samples from the wav data:
        double chords = s1.play(sampleSpeed1)*gain_1;
        double perc = s2.play(sampleSpeed2)*gain_2;
        
        // Apply a lowpass filter to the first sample:
        double filteredChords = f1.setCutoff(cutoff1Hz).setResonance(resonance1).play(chords, 1.0, 0.0, 0.0, 0.0);
        // Apply highpass filter to the second filter:
        double filteredPerc = f2.setCutoff(cutoff2Hz).setResonance(resonance2).play(perc, 0.0, 0.0, 1.0, 0.0);
        // Feed samples into the FFT function: This will convert a block of collected amplitude values into
        // phase and magnitude values (complex values).
        fft_1.process(filteredChords);
        fft_2.process(filteredPerc);
        
        double out = 0;
        
        // Depending on the toggle button, either combine phases of the first sample with the magnitudes of the second sample,
        // or combine the phases of the second sample with the magnitudes of the first sample.
        if(option == true)
        {
            out = ifft.process(fft_2.magnitudes, fft_1.phases);
        } else if(option == false)
        {
            out = ifft.process(fft_1.magnitudes, fft_2.phases);
        }
        
        // output
        leftBuffer[sample] = out*0.9;
        rightBuffer[sample] = leftBuffer[sample];
        // Pass output to another FFT so we can see the frequency spectrum of the cross-synthesis:
        fft_3.process(out);
    }
    // Pass the info to the graphics by triggering the callback timer:
    nextFFTblockReady1 = true;
}

void MainComponent::releaseResources()
{
 
}

void MainComponent::timerCallback()
{
    if (nextFFTblockReady1 == true)
    {
        nextFFTblockReady1 = false;
        repaint();
    }
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::white);
    drawFrame (g);
}
void MainComponent::drawFrame (Graphics& g)
{
    float xinc = 100 / 1024 * 2.0;
    
    // should be until less than 1024/2 (size of fft up to Nyquist)
    // but we'll only do the first 128 bins so we can fit the
    // graph into the screen space (most of the activity is happening
    // at the bottom):
    for(int i=0; i < 1024/8; i++) {
        float height1 = fft_1.magnitudes[i]*5;
        float height2 = fft_2.magnitudes[i]*5;
        float height3 = fft_3.magnitudes[i]*5;

        // Draw a rectangle for every frequency value:
        g.setColour (Colours::skyblue);
        g.fillRect(500.2 +(i*4), 150.1-height1, 4.0, height1);
        g.setColour(Colours::grey);
        g.fillRect(500.2 +(i*4), 350.1-height2, 4.0, height2);
        g.setColour (Colours::orange);
        g.fillRect(500.2 +(i*4), 550.1-height3, 4.0, height3);
    }
}

void MainComponent::resized()
{
    // Sliders for first sample:
    cutoff1.setBounds(10, 10, 150, 150);
    res1.setBounds(10, 200, 150, 150);
    // Labels:
    cutoffLabel1.setBounds(50, 160, 50, 50);
    resLabel1.setBounds(40, 350, 100, 50);
    
    // Sliders for second sample:
    cutoff2.setBounds(150, 10, 150, 150);
    res2.setBounds(150, 200, 150, 150);
    // Labels:
    cutoffLabel2.setBounds(190, 160, 100, 50);
    resLabel2.setBounds(190, 350, 100, 50);
    
    // Speed Slider 1:
    speed1.setBounds(10, 390, 150, 150);
    // Label
    speedLabel1.setBounds(40, 540, 100, 50);
    // Speed Slider 2:
    speed2.setBounds(150, 390, 150, 150);
    // Label:
    speedLabel2.setBounds(170, 540, 100, 50);
    
    // Button:
    sampleSwitch.setBounds(340, 260, 100, 70);
    
    // Gain sliders:
    gain1.setBounds(340, 140, 100, 100);
    gain2.setBounds(340, 340, 100, 100);

}
