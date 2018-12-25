/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once
#include "maximilian.h"
#include "libs/maxim.h"
#include "../JuceLibraryCode/JuceHeader.h"

// Look and Feel Class:
class OtherLookAndFeel : public LookAndFeel_V4
{
public: 
        OtherLookAndFeel()
    {
        setColour(Slider::thumbColourId, Colours::darkslategrey); // 0x1001300
        setColour(Slider::backgroundColourId, Colours::blue); // 0x1001200, defined below
        setColour(Slider::rotarySliderFillColourId, Colours::orange); // covered values
        setColour(Slider::rotarySliderOutlineColourId, Colours::darkslategrey); // outline colour when not covered
    }
    
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        // JUCE code:
        
        auto outline = slider.findColour (Slider::rotarySliderOutlineColourId);
        auto fill    = slider.findColour (Slider::rotarySliderFillColourId);
        
        //auto bounds = Juce::Rectangle<int>(x, y, width, height).toFloat().reduced (10);
        auto radius = jmin(width, height) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		float rad = radius * 0.5;
        auto lineW = jmin(8.0f, rad);
        auto arcRadius = radius - lineW * 0.5f;
        
        // my circle:
        float centreX = x + width * 0.5f;
                float centreY = y + height * 0.5f;
                float rx = centreX - radius;
                float ry = centreY - radius;
                float rw = radius * 2.0f;
        
        g.setColour(Colours::whitesmoke);
                g.fillEllipse(rx, ry, rw, rw);
        
        // my small circle:
        // draw a smaller filled circle:
                float rx2 = centreX - (radius/2);
                float ry2 = centreY - (radius/2);
                // fill:
                g.setColour(Colours::darkslategrey);
                g.fillEllipse(rx2, ry2, (rw/2), (rw/2));
        //
        
        Path backgroundArc;
        backgroundArc.addCentredArc (width/2,
                                     height/2,
                                     arcRadius,
                                     arcRadius,
                                     0.0f,
                                     rotaryStartAngle,
                                     rotaryEndAngle,
                                     true);
        
        g.setColour (outline);
        g.strokePath (backgroundArc, PathStrokeType (lineW, PathStrokeType::beveled, PathStrokeType::butt));
        
        if (slider.isEnabled())
        {
            Path valueArc;
            valueArc.addCentredArc (width/2,
                                    height/2,
                                    arcRadius/1.1,
                                    arcRadius/1.1,
                                    0.0f,
                                    rotaryStartAngle,
                                    toAngle,
                                    true);
			
            g.setColour (fill);
            g.strokePath (valueArc, PathStrokeType (15, PathStrokeType::beveled, PathStrokeType::butt));
        }

		if (slider.isEnabled())
		{
			Path valueArc;
			valueArc.addCentredArc(width / 2,
				height / 2,
				arcRadius / 1.5,
				arcRadius / 1.5,
				0.0f,
				rotaryStartAngle,
				toAngle,
				true);

			g.setColour(Colours::grey);
			g.strokePath(valueArc, PathStrokeType(10, PathStrokeType::beveled, PathStrokeType::butt));
		}

		//if (slider.isEnabled())
		//{
			Path valueArc;
			valueArc.addCentredArc(width / 2,
				height / 2,
				arcRadius / 2.0,
				arcRadius / 2.0,
				0.0f,
				rotaryStartAngle,
				rotaryEndAngle,
				true);

			g.setColour(fill);
			g.strokePath(valueArc, PathStrokeType(5, PathStrokeType::mitered, PathStrokeType::butt));
		//}
        
        // my dial:
        float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        Path p;
                float pointerLength = radius * 0.53f;
                float pointerThickness = 10.0f;
                p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
                p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
        
                // fill the pointer path:
                g.setColour(Colours::darkslategrey);
                g.fillPath(p);

    }
};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, private Timer
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
	void drawFrame(Graphics& g);

private:
    //==============================================================================
    // Your private member variables go here...
	maxiSample s1, s2;
	
	maxiFFT fft_1, fft_2, fft_3;
	maxiIFFT ifft;
	TextButton sampleSwitch;
	TextButton sample1;
	TextButton sample2;
	bool playSample1 = false;
	bool playSample2 = false;
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
    
	OtherLookAndFeel otherLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
