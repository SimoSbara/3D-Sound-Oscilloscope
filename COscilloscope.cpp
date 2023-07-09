#include "COscilloscope.h"

void actionThread(void* lpParam);

COscilloscope::COscilloscope(int width, int height)
{
	this->width = width;
	this->height = height;

	this->window = new sf::RenderWindow(sf::VideoMode(width, height), "Oscilloscopio");

	this->fps = 0;

	needDrawing = false;
}

COscilloscope::~COscilloscope()
{
	ClearAllSignals();
}

void actionThread(void* lpParam)
{
	COscilloscope* instance = (COscilloscope*)lpParam;

	while (instance->isRunning)
	{
		if (instance->needDrawing)
		{
			double yMin, yMax;

			for (int j = 0; j < instance->signalsToDraw.size(); j++)
			{
				yMin = DBL_MAX;
				yMax = DBL_MIN;

				for (int i = 0; i < BUFFER_SIZE; i++)
				{
					double y = instance->soundBuffers[j][i];

					if (yMin > y)
						yMin = y;
					else if (yMax < y)
						yMax = y;
				}

				double yDiff = 1.0 / (yMax - yMin);
				double xDiff = 1.0 / 3600;

				double X1, X2, Y1, Y2;
				for (int i = 0; i < BUFFER_SIZE - 1; i++)
				{
					sf::Vertex v1;
					sf::Vertex v2;

					double normalizedX1 = i * xDiff;
					double normalizedY1 = (instance->soundBuffers[j][i] - yMin) * yDiff;

					double normalizedX2 = (i + 1) * xDiff;
					double normalizedY2 = (instance->soundBuffers[j][i + 1] - yMin) * yDiff;

					X1 = normalizedX1 * (double)instance->getWidth();
					X2 = normalizedX2 * (double)instance->getWidth();

					Y1 = normalizedY1 * (double)instance->getHeight();
					Y2 = normalizedY2 * (double)instance->getHeight();

					v1.color = instance->signalsColor[j];
					v2.color = instance->signalsColor[j];

					v1.position.x = X1;
					v1.position.y = Y1;

					v2.position.x = X2;
					v2.position.y = Y2;

					instance->signalsToDraw[j].append(v1);
					instance->signalsToDraw[j].append(v2);
				}
			}
			instance->needDrawing = false;
		}
	}

	return;
}

void COscilloscope::Run()
{
	isRunning = true;

	std::thread threadActions(actionThread, this);
	threadActions.detach(); //indipendent execution
	ImGui::SFML::Init(*window);

	bool single = false;
	sf::Clock deltaClock;
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window->close();
		}

		ImGui::SFML::Update(*window, deltaClock.restart());

		sprintf(titleWindow, "fps: %f", fps);

		sf::String title(titleWindow);

		start = std::chrono::high_resolution_clock::now();

		window->setTitle(title);

		ImGui::Begin("signal parameters");
		if (ImGui::Button("Add Sin(x)"))
		{
			CreateSine();
		}
		if (ImGui::Button("Add Cos(x)"))
		{
			CreateCosine();
		}
		if (ImGui::Button("Add Random Signal"))
		{
			CreateRandomSignal();
		}
		if (ImGui::Button("Add Square Signal"))
		{
			CreateSquareSignal();
		}
		if (ImGui::Button("Add Triangle Signal"))
		{
			CreateTriangleSignal();
		}
		if (ImGui::Button("Add Saw Tooth Signal"))
		{
			CreateSawToothSignal();
		}
		if (ImGui::Button("Delete all Signals"))
		{
			ClearAllSignals();
		}

		for (int i = 0; i < signalsEnable.size(); i++)
		{
			std::string name = "Signal ";
			name += std::to_string(i + 1);

			ImGui::Checkbox(name.c_str(), signalsEnable[i]);
		}
		ImGui::End();
				
		if (!needDrawing)
		{
			window->clear(sf::Color::Black);

			for (int i = 0; i < signalsToDraw.size(); i++)
			{
				if (*signalsEnable[i])
					window->draw(signalsToDraw[i]);
			}
		}

		ImGui::SFML::Render(*window);

		window->display();

		end = std::chrono::high_resolution_clock::now();

		fps = (float)1e9 / (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	}

	threadActions.~thread();
	ImGui::SFML::Shutdown();
}