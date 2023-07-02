// 3D-Sound-Oscilloscope.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include <chrono>

#include "COscilloscope.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FREQUENCY 2

int main()
{
	COscilloscope oscilloscope(WINDOW_WIDTH, WINDOW_HEIGHT);

 //   double theta = 0;
	//double* signal1 = new double[BUFFER_SIZE];
	//double* signal2 = new double[BUFFER_SIZE];

	////memset(signal1, 0, sizeof(signal1));
	////memset(signal2, 0, sizeof(signal2));

	//for (int i = 0; i < BUFFER_SIZE; i++, theta += DECIM_ANG)
	//{
	//	signal2[i] = 0;
	//	for (int j = 1; j < 101; j += 2)
	//	{
	//		signal1[i] = sin(theta * FREQUENCY) + cos(theta * FREQUENCY + M_PI / 3) - cos(theta * (FREQUENCY * 2) + M_PI / 7);
	//		signal2[i] += sin(FREQUENCY * theta * j) / j;
	//	}
	//}

	//oscilloscope.CreateSignal(signal1);
	//oscilloscope.CreateSignal(signal2);
	oscilloscope.Run();

	return 0;
}
