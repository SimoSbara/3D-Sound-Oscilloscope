#pragma once
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <chrono>
#include <Windows.h>
#include <math.h>

#include "muParser.h"

#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-audio.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "muparser.lib")

#define BUFFER_SIZE 3600
#define ANG_DEG_FROM_RAD   M_PI / 180.0
#define DECIM_ANG          ANG_DEG_FROM_RAD / 10 

#define M_PI_BY_4		   M_PI / 4.0

class COscilloscope
{
public:
	COscilloscope(int width, int height);
	~COscilloscope();
	void Run();

	bool isRunning;
	std::atomic<bool> needDrawing;

	std::vector<double*> soundBuffers;//intero periodo del seno o coseno preciso al decimo del grado: 3600 punti
	std::vector<sf::VertexArray> signalsToDraw;// (sf::Lines);
	std::vector<sf::Color> signalsColor;
	std::vector<bool*> signalsEnable;

	void ClearAllSignals()
	{
		for (int i = 0; i < soundBuffers.size(); i++)
		{
			delete soundBuffers[i];
			delete signalsEnable[i];
		}

		soundBuffers.clear();
		signalsEnable.clear();
		signalsColor.clear();
		signalsToDraw.clear();
	}

	void CreateSignal(double* signal)
	{
		soundBuffers.push_back(signal);
		signalsToDraw.push_back(sf::VertexArray(sf::Lines));

		BYTE r = rand() % 255;
		BYTE g = rand() % 255;
		BYTE b = rand() % 255;

		if (r < 100)
			r = 100;

		if (g < 100)
			g = 100;

		if (b < 100)
			b = 100;

		sf::Color color(r, g, b);
		signalsColor.push_back(color);
		signalsEnable.push_back(new bool(true));

		needDrawing = TRUE;
	}

	const wchar_t* GetWC(const char* c)
	{
		const size_t cSize = strlen(c) + 1;
		size_t converted;
		wchar_t* wc = new wchar_t[cSize];
		mbstowcs_s(&converted, wc, cSize, c, strlen(c));

		return wc;
	}

	void CreateCustomSignal(std::string expression)
	{
		double* customSignal = new double[BUFFER_SIZE];

		memset(customSignal, 0, BUFFER_SIZE * sizeof(double));

		double theta = 0;

		double var_k = 1, var_x = 0;
		mu::Parser p;
		p.DefineVar("k", &var_k);
		p.DefineVar("x", &var_x);
		//p.DefineFun("MySqr", MySqr);

		//wchar_t* expr = (wchar_t*)GetWC(expression.c_str());

		p.SetExpr(expression);

		try
		{
			for (std::size_t k = 1; k < 100; ++k)
			{
				var_k = k;

				for (int i = 0; i < BUFFER_SIZE; i++, theta += DECIM_ANG)
				{

						var_x = theta;
						customSignal[i] += p.Eval();

				}
			}
		}
		catch (mu::Parser::exception_type& e)
		{
			std::cout << e.GetMsg() << std::endl;
			delete customSignal;

			return;
		}

		CreateSignal(customSignal);
	}

	void CreateSquareSignal()
	{
		double* squareSignal = new double[BUFFER_SIZE];
		double theta = 0;
		int frequency = rand() % 7 + 1;

		for (int i = 0; i < BUFFER_SIZE; i++, theta += DECIM_ANG)
		{
			squareSignal[i] = 0;

			for (int j = 1; j < 101; j += 2)
			{
				squareSignal[i] += sin(frequency * theta * j) / j;
			}
		}

		CreateSignal(squareSignal);
	}


	void CreateTriangleSignal()
	{
		double* triangleSignal = new double[BUFFER_SIZE];
		double theta = 0;
		int frequency = rand() % 7 + 1;
		double factor = 8.0 / (M_PI * M_PI);

		for (int i = 0; i < BUFFER_SIZE; i++, theta += DECIM_ANG)
		{
			triangleSignal[i] = 0;

			for (int j = 0; j < 200; j++)
			{
				int n = 2 * j + 1;

				triangleSignal[i] += (pow(-1, j) * sin(2 * M_PI * frequency * n * theta)) / (n * n);
			}

			triangleSignal[i] *= factor;
		}
		
		CreateSignal(triangleSignal);
	}

	//dente di sega xd (codegoneeee)
	void CreateSawToothSignal()
	{
		double* sawToothSignal = new double[BUFFER_SIZE];
		double theta = 0;
		int frequency = rand() % 7 + 1;
		double factor =  - 2.0 / M_PI;

		for (int i = 0; i < BUFFER_SIZE; i++, theta += DECIM_ANG)
		{
			sawToothSignal[i] = 0;

			for (int j = 1; j < 200; j++)
			{
				sawToothSignal[i] += (pow(-1, j) * sin(2 * M_PI * frequency * j * theta)) / j;
			}

			sawToothSignal[i] *= factor;
		}

		CreateSignal(sawToothSignal);
	}

	void CreateRandomSignal()
	{
		double* randomSignal = new double[BUFFER_SIZE];
		double theta = 0;
		int numHarmonics = rand() % 30;

		for (int i = 0; i < BUFFER_SIZE; i++)
			randomSignal[i] = 0;

		for (int j = 0; j < numHarmonics; j++)
		{
			int choice = rand() % 2;
			int frequency = rand() % 10;
			int phase = rand() % 7;
			int amplitude = rand() % 10;

			for (int i = 0; i < BUFFER_SIZE; i++, theta += DECIM_ANG)
			{
				switch (choice)
				{
				case 0:
					randomSignal[i] += amplitude * sin(theta * frequency + phase);
					break;
				case 1:
					randomSignal[i] += amplitude * cos(theta * frequency + phase);
					break;
				case 2:
					randomSignal[i] += amplitude * (sin(theta * frequency + phase) + cos(theta * frequency + phase));
					break;
				}
			}
		}
		CreateSignal(randomSignal);
	}

	void CreateCosine()
	{
		double* cosine = new double[BUFFER_SIZE];
		double theta = 0;

		for (int i = 0; i < BUFFER_SIZE; i++, theta += DECIM_ANG)
		{
			cosine[i] = cos(theta);
		}

		CreateSignal(cosine);
	}

	void CreateSine()
	{
		double* sine = new double[BUFFER_SIZE];
		double theta = 0;

		for (int i = 0; i < BUFFER_SIZE; i++, theta += DECIM_ANG)
		{
			sine[i] = sin(theta);
		}

		CreateSignal(sine);
	}

	int getWidth() { return this->width; }
	int getHeight() { return this->height; }

private:
	int width, height;

	sf::RenderWindow* window;

	double fps;

	char titleWindow[128];

	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;

	HANDLE  threadActions;
	DWORD   threadActID;
};

