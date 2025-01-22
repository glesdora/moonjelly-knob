#pragma once

#include <vector>
#include <JuceHeader.h>
#include "MoonJDisplay.h"
#include "MoonJPalette.h"
#include "MoonJLed.h"

using namespace juce;

class MoonJKnob : public juce::Slider
{
public:
  MoonJKnob(const String& componentName);
  ~MoonJKnob() override;
  void paint(Graphics& g) override;
  void resized() override;
  void setNumberOfShapes(int n);
  void setData(DoubleBuffer& dbuf);
  void updateShapePath();

private:
  MoonJDisplay display;

  int numberOfShapes;
  int numberOfLeds;
  std::vector<float> leds;
  std::vector<std::unique_ptr<MoonJLed>> ledComponents;

  float rotaryStartAngle = -float_Pi;
  float rotaryEndAngle = float_Pi;
  float innerradius;
  float borderwidth;
  float borderradius;
  float arrowbandwidth;
  float arrowbandradius;
  float outlineradius;
  float voidwidth;
  float ledsbandwidth;
  float ledsbandradius;
  float ledsdiameter;
  float ledswidth;
  float arrowheight;

  Path borderBand;
  Path arrowBand;
  Path outline;
  Path ledsBand;
  Path arrowPath;

  AffineTransform ledTransform;

  void gaussianIntensity(float p, float d0, std::vector<float>& leds);
};