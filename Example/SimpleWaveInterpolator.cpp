#pragma once

#include <vector>
#include <cmath>

class SimpleWaveInterpolator
{
public:
	SimpleWaveInterpolator(int wformsize)
	{
		this->wfsize = wformsize;

		std::vector<float> sine;
		for (int i = 0; i < wformsize; i++)
		{
			sine.push_back(std::sin(2 * 3.14 * i / wformsize)*0.5f + 0.5f);
		}

		std::vector<float> sawtooth;
		for (int i = 0; i < wformsize; i++)
		{
			sawtooth.push_back((float)i / wformsize);
		}

		std::vector<float> square;
		for (int i = 0; i < wformsize; i++)
		{
			square.push_back(i < wformsize / 2 ? 0.0f : 1.0f);
		}

		std::vector<float> triangle;
		for (int i = 0; i < wformsize; i++)
		{
			triangle.push_back(i < wformsize / 2 ? (float)i / (wformsize / 2) : 1.0f - (float)(i - wformsize / 2) / (wformsize / 2));
		}

		std::vector<float> cat;
		for (int i = 0; i < wformsize; i++)
		{
			cat.push_back(catvalues[i]);
		}

		std::vector<float> mouse;
		for (int i = 0; i < wformsize; i++)
		{
			mouse.push_back(mousevalues[i]);
		}

		std::vector<float> bird;
		for (int i = 0; i < wformsize; i++)
		{
			bird.push_back((30-birdints[i])*0.008f+0.6f);
		}

		_wforms.push_back(sine);
		_wforms.push_back(sawtooth);
		_wforms.push_back(square);
		_wforms.push_back(triangle);
		//_wforms.push_back(cat);
		//_wforms.push_back(mouse);
		//_wforms.push_back(bird);

		this->numwforms = _wforms.size();
	}

	void interpolate(float t, std::vector<float>& targetbuffer)
	{
		if (t < 0.0f || t >= numwforms) return;

		int wform1 = std::floor(t);
		int wform2 = (wform1 + 1) % numwforms;
		float alpha = t - wform1;

		for (int i = 0; i < wfsize; i++)
		{
			targetbuffer[i] = (1.0f - alpha) * _wforms[wform1][i] + alpha * _wforms[wform2][i];
		}
	}

	int getNumWaveforms()
	{
		return numwforms;
	}

private:
	int numwforms = 0;
	int wfsize = 0;
	std::vector<std::vector<float>> _wforms;

	float catvalues[128] = {
	0.0, 0.072, 0.104, 0.136, 0.16, 0.176, 0.264, 0.328, 0.376, 0.416,
	0.448, 0.48, 0.512, 0.536, 0.544, 0.552, 0.552, 0.56, 0.56, 0.56,
	0.552, 0.552, 0.544, 0.536, 0.528, 0.52, 0.512, 0.504, 0.504, 0.496,
	0.488, 0.48, 0.472, 0.464, 0.456, 0.448, 0.448, 0.44, 0.432, 0.424,
	0.432, 0.432, 0.44, 0.44, 0.44, 0.44, 0.448, 0.448, 0.448, 0.448,
	0.448, 0.456, 0.464, 0.464, 0.464, 0.464, 0.464, 0.464, 0.464, 0.464,
	0.464, 0.464, 0.464, 0.464,
	0.464, 0.464, 0.464, 0.464,	
	0.464, 0.464, 0.464, 0.464, 0.464, 0.464, 0.464, 0.464, 0.456, 0.448,
	0.448, 0.448, 0.448, 0.448, 0.44, 0.44, 0.44, 0.44, 0.432, 0.432, 
	0.424, 0.432, 0.44, 0.448, 0.448, 0.456, 0.464, 0.472, 0.48, 0.488,
	0.496, 0.504, 0.504, 0.512, 0.52, 0.528, 0.536, 0.544, 0.552, 0.552,
	0.56, 0.56, 0.56, 0.552, 0.552, 0.544, 0.536, 0.512, 0.48, 0.448,
	0.416, 0.376, 0.328, 0.264, 0.176, 0.16, 0.136, 0.104, 0.072, 0.0
	};

	float mousevalues[128] = {
	0.152, 0.16, 0.168, 0.168, 0.168, 0.176, 0.176, 0.176, 0.168, 0.16,
	0.16, 0.152, 0.144, 0.128, 0.112, 0.096, 0.072, 0.056, 0.048, 0.04,
	0.032, 0.024, 0.024, 0.016, 0.016, 0.024, 0.048, 0.144, 0.184, 0.216,
	0.24, 0.256, 0.272, 0.28, 0.288, 0.296, 0.304, 0.312, 0.32, 0.32,
	0.328, 0.344, 0.344, 0.352, 0.352, 0.352, 0.36, 0.36, 0.36, 0.368,
	0.368, 0.368, 0.368, 0.368, 0.376, 0.376, 0.376, 0.376, 0.376, 0.376,
	0.376, 0.368, 0.368, 0.368, 0.368, 0.36, 0.36, 0.36, 0.352, 0.344,
	0.328, 0.328, 0.32, 0.312, 0.304, 0.296, 0.288, 0.28, 0.272, 0.264,
	0.256, 0.248, 0.24, 0.232, 0.224, 0.216, 0.208, 0.176, 0.152, 0.144,
	0.136, 0.128, 0.112, 0.104, 0.088, 0.08, 0.088, 0.096, 0.112, 0.12,
	0.128, 0.136, 0.144, 0.144, 0.152, 0.152, 0.144, 0.112, 0.08, 0.072,
	0.064, 0.064, 0.056, 0.056, 0.056, 0.056, 0.048, 0.048, 0.04, 0.04,
	0.032, 0.032, 0.024, 0.016, 0.008, 0.0
	};

	int birdints[128] = {
	24, 23, 23, 22, 24, 20, 20, 19, 18, 18, 17, 16, 15, 15, 14, 13,
	12, 12, 11, 10, 9, 8, 8, 7, 6, 5, 5, 4, 3, 2, 2, 1,
	1, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 5,
	6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 12, 12, 13, 13, 14, 14,
	15, 16, 16, 15, 14, 13, 13, 14, 15, 16, 17, 16, 16, 15, 15, 14,
	14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8, 7, 7,
	6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 1, 1, 2,
	3, 4, 5, 6, 8, 9, 11, 12, 14, 16, 18, 21, 23, 25, 27, 30
	};


};