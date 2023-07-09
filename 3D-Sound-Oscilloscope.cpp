// 3D-Sound-Oscilloscope.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include <chrono>

#include "COscilloscope.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FREQUENCY 2

int main()
{
	COscilloscope oscilloscope(WINDOW_WIDTH, WINDOW_HEIGHT);
	oscilloscope.Run();

	return 0;
}
