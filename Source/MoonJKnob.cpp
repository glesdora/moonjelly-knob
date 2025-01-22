#include "MoonJKnob.h"

MoonJKnob::MoonJKnob(const String& componentName) : Slider(componentName), numberOfShapes(0), display()
{
    addAndMakeVisible(display);
    
    setSliderStyle(RotaryHorizontalVerticalDrag);
    setTextBoxStyle(NoTextBox, true, 0, 0);
    setRotaryParameters(0, 2*float_Pi, false);
	setRange(0.0, 1.0);
}

MoonJKnob::~MoonJKnob()
{
	setComponentEffect(nullptr);
}

void MoonJKnob::paint(Graphics& g)
{
    auto bounds = getLocalBounds();
    auto center = bounds.getCentre();

    auto sliderPos =  static_cast<float>(valueToProportionOfLength(getValue()));
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.setColour(lcdPalette[back]);
    g.fillEllipse(center.x - innerradius, center.y - innerradius, innerradius * 2, innerradius * 2);

	g.setColour(lcdPalette[border]);
	g.strokePath(borderBand, PathStrokeType(borderwidth));
    
    g.setColour(lcdPalette[back]);
    g.strokePath(arrowBand, PathStrokeType(arrowbandwidth));

	g.setColour(lcdPalette[back].darker());
	g.strokePath(outline, PathStrokeType(0.5f));

    if (lcdPalette[ledsband].getAlpha() > 0.0f)
    {
        g.setColour(lcdPalette[ledsband]);
        g.strokePath(ledsBand, PathStrokeType(ledsbandwidth));
    }

    Point<float> indicatorPos(
        center.x + std::sin(toAngle) * arrowbandradius,
        center.y + std::cos(toAngle) * arrowbandradius * (-1.0f)
    );

    float zoom = 1.0f;

	Rectangle<float> arrowBounds(-arrowheight, -arrowheight, arrowheight * 2.0f, arrowheight * 2.0f);
	Image arrowImage = Image(Image::ARGB, arrowheight * zoom * 2.0f, arrowheight * zoom * 2.0f, true);
    Graphics arrowGraphics(arrowImage);
    AffineTransform transform = AffineTransform::rotation(toAngle).translated(arrowheight * zoom, arrowheight * zoom);

	arrowGraphics.setColour(lcdPalette[led]);
	arrowGraphics.fillPath(arrowPath, transform);

    float glowradius = 0.1f * arrowheight * zoom;
    GlowEffect glow;
    glow.setGlowProperties(glowradius, lcdPalette[led]);
    Image glowedImage = arrowImage.createCopy();
    Graphics glowGraphics(glowedImage);

    glow.applyEffect(arrowImage, glowGraphics, 1.0f, 1.0f);
    g.drawImage(glowedImage, arrowBounds.translated(indicatorPos.x, indicatorPos.y), RectanglePlacement::stretchToFit);

    if (numberOfShapes) {
        gaussianIntensity(sliderPos*numberOfLeds, 0.75f, leds);

        for (int i = 0; i < numberOfLeds; ++i) {
			ledComponents[i]->setBrightness(leds[i]);
        }
    }
}

void MoonJKnob::resized()
{
    auto bounds = getLocalBounds();
    auto center = bounds.getCentre();
    Rectangle<int> displaybounds;

	float incradius = 0.0f;

    innerradius = jmin (bounds.getWidth(), bounds.getHeight()) / 3.5f;
    incradius = innerradius;
    borderwidth = innerradius * 0.032f;
    borderradius = innerradius + borderwidth * 0.5f;
	incradius += borderwidth;
    arrowbandwidth = innerradius * 0.2f;
    arrowbandradius = incradius + arrowbandwidth * 0.5f;
	incradius += arrowbandwidth;
    outlineradius = incradius;
    voidwidth = innerradius * 0.075f;
	incradius += voidwidth;
    ledsbandwidth = innerradius * 0.25f;
	ledsbandradius = incradius + ledsbandwidth * 0.5f;
    ledsdiameter = ledsbandwidth * 0.6f;    //0.3f
	ledswidth = ledsdiameter * 0.66f;
    arrowheight =  arrowbandwidth * 0.55f;      // * 0.83f if dark

    display.setBounds(bounds.withSizeKeepingCentre(innerradius * 1.25f, innerradius * 1.25f));    //the multiplier is a bit less then the sqrt(2), to leave some space

	borderBand.clear();
	arrowBand.clear();
	outline.clear();
	ledsBand.clear();
	arrowPath.clear();

	borderBand.addCentredArc(center.x, center.y, borderradius, borderradius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    arrowBand.addCentredArc(center.x, center.y, arrowbandradius, arrowbandradius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
	outline.addCentredArc(center.x, center.y, outlineradius, outlineradius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    ledsBand.addCentredArc(center.x, center.y, ledsbandradius, ledsbandradius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    arrowPath.addTriangle(-arrowheight * 0.5f, arrowheight * 0.5f, arrowheight * 0.5f, arrowheight * 0.5f, 0.0f, -arrowheight * 0.5f);

    if (numberOfShapes) {
        for (int i = 0; i < numberOfLeds; ++i) {
            float angle = rotaryStartAngle + i * (rotaryEndAngle - rotaryStartAngle) / numberOfLeds;

            Point<float> shapePos(
                center.x + std::sin(angle) * (ledsbandradius),
                center.y + std::cos(angle) * (ledsbandradius) * (-1.0f)
            );

            ledTransform = AffineTransform::rotation(angle).translated(shapePos.x, shapePos.y);

            MoonJLed* led = ledComponents[i].get();
            Rectangle<int> ledBounds;

			if (!led->isLineType())
                ledBounds = Rectangle<int>(-ledsdiameter * 0.5f, -ledsdiameter * 0.5f, ledsdiameter, ledsdiameter);
			else
                ledBounds = Rectangle<int>(-ledswidth * 0.5f, -ledsbandwidth * 0.875f, ledswidth, ledsbandwidth * 1.5f);

            ledComponents[i]->setBounds(ledBounds);
            ledComponents[i]->setTransform(ledTransform);
        }
    }   
}

void MoonJKnob::setNumberOfShapes(int n) {
    numberOfShapes = n;
    numberOfLeds = numberOfShapes * round(24.0 / numberOfShapes);
    leds.resize(numberOfLeds);

	ledComponents.clear();
    ledComponents.resize(numberOfLeds);

	for (int i = 0; i < numberOfLeds; ++i) {
        if (i % (numberOfLeds / numberOfShapes)) {
            ledComponents[i].reset(new MoonJLed(false));
		}
		else {
			ledComponents[i].reset(new MoonJLed(true));
		}
		
		addAndMakeVisible(ledComponents[i].get());
    }

    setRange(0.0, (double)numberOfShapes, 0.01);
}

void MoonJKnob::gaussianIntensity(float p, float d0, std::vector<float>& leds) {
    int n = leds.size();

    for (int i = 0; i < n; ++i) {
        float d = jmin(abs(i-p), n-abs(i-p));
        leds[i] = exp(-d*d/(2*d0*d0));
    }
}

void MoonJKnob::setData(DoubleBuffer& dbuf) {
    display.setData(dbuf);
}

void MoonJKnob::updateShapePath() {
	display.updatePath();
}