# Moonjelly Knob

An interactive JUCE circular knob with real-time interpolated curve display, ideal for audio applications needing smooth transitions



---



## Features



- **Real-Time Curve Display**: Displays an interpolated curve at its center, updating dynamically as the knob rotates.

- **Flexible Interpolation**: Designed to work with external interpolator classes to calculate curves from the knob's output.

- **Extensible Use Cases**:

  - Wavetable synthesis.

  - Audio envelope control.

  - Waveshaping.



---



## How It Works



1. **Knob Rotation**: The user rotates the knob, which outputs a value in the range `[0, numberOfCurves]`. 

2. **Interpolation**: This value is passed to an external interpolator (not included in the component), which computes the corresponding interpolated curve.

3. **Curve Display**: The computed curve is rendered at the center of the knob in real time.



![Tool demonstration](animals.gif)



---



## Example Use Case: Wavetable Synthesis



A typical application involves smooth interpolation between waveforms (e.g., sine, sawtooth, square). For convenience, an example class is provided in this repository that generates these common shapes and their linear interpolations.



> Note: The component does not include an interpolator itself, allowing users to integrate their own interpolation logic. However, the example class can be a starting point for customization.



View a demonstration of the Moonjelly Knob with audio: [Watch Video](demo_exampleproj.mp4)



### Advanced Use Case: Envelope Interpolation



For envelope interpolation, the knob can be integrated with a more sophisticated interpolator that supports advanced logic. If you're interested in this application, see [this related project](https://github.com/glesdora/envelopes-interpolator.git) for inspiration.



---



## Getting Started



### Requirements



- **JUCE Framework**: The component must be integrated into an existing JUCE project.



### Installation



1. Clone this repository or download the source files.

2. Add the component to your JUCE project.

3. Implement or include an interpolator class or function to handle curve calculations.



---



## License



This project is licensed under the MIT License. See the `LICENSE` file for details.
