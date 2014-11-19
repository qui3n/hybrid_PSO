//#include <SFML/Graphics.hpp>
//#include <iostream>
//
//#define WINDOW_WIDTH 1600
//#define WINDOW_HEIGHT 900
//#define STEP_TIME_MS 1000
//#define PARTICLES 100
//
//void drawParticle(sf::RenderWindow &window, float x, float y);
//
//struct Point
//{
//	float x;
//	float y;
//	float vx;
//	float vy;
//};
//
//int mainO()
//{
//	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PUNTINI CHE SI MUOVONO!!!1!");
//	srand((unsigned)time(NULL));
//
//	Point points[PARTICLES];
//	for (int i = 0; i < PARTICLES; i++)
//	{
//		points[i].x = rand() % WINDOW_WIDTH;
//		points[i].y = rand() % WINDOW_HEIGHT;
//		points[i].vx = 0;
//		points[i].vy = 0;
//	}
//
//	sf::Clock clock;
//	while (window.isOpen())
//	{
//		sf::Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed)
//				window.close();
//		}
//
//		for (int i = 0; i < PARTICLES; i++)
//		{
//			points[i].vx += ((rand() % 2) - 1) * 0.5;
//			points[i].vy += ((rand() % 2) - 1) * 0.5;
//
//			if (points[i].vx < -1)
//				points[i].vx = 1;
//			if (points[i].vx > 1)
//				points[i].vx = -1;
//			if (points[i].vy < -1)
//				points[i].vy = 1;
//			if (points[i].vy > 1)
//				points[i].vy = -1;
//		}
//
//		float dt = ((float) clock.restart().asMilliseconds()) / 60;
//		std::cout << dt << std::endl;
//
//		window.clear(sf::Color::Green);
//		for (int i = 0; i < PARTICLES; i++)
//		{
//			points[i].x += points[i].vx * dt;
//			points[i].y += points[i].vy * dt;
//
//			if (points[i].x < 0)
//				points[i].x = 0;
//			if (points[i].x > WINDOW_WIDTH)
//				points[i].x = WINDOW_WIDTH;
//			if (points[i].y < 0)
//				points[i].y = 0;
//			if (points[i].y > WINDOW_HEIGHT)
//				points[i].y = WINDOW_HEIGHT;
//
//			drawParticle(window, points[i].x, points[i].y);
//		}
//		window.display();
//		sf::sleep(sf::milliseconds(100));
//	}
//
//	return 0;
//}
//
//void drawParticle(sf::RenderWindow &window, float x, float y)
//{
//	sf::CircleShape shape(10);
//	shape.setFillColor(sf::Color::Red);
//	shape.setPosition(x, y);
//
//	if (window.isOpen())
//	{
//		window.draw(shape);
//	}
//	
//}