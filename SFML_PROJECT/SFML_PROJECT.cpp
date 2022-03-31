#include "pch.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <iostream>

const float G = 6.6743f;
const float PI = 3.14159265f;

sf::Vector2f normalize(sf::Vector2f v) {
	sf::Vector2f result;
	float length = sqrt(v.x * v.x + v.y * v.y);
	result.x = v.x / length;
	result.y = v.y / length;
	return result;
}

class Star {
	private:
		float mass;
		float radius;
		sf::CircleShape star;
		sf::Vector2f pos;

	public:
		sf::Vector2f vlc = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f aVlc = sf::Vector2f(0.0f, 0.0f);

		Star(float mass, sf::Vector2f pos, sf::Vector2f vlc) {
			this->mass = mass;
			this->radius = this->mass*1.05f;
			this->star.setRadius(this->radius);
			this->star.setPosition(pos.x-radius,pos.y-radius);
			this->pos = pos;
			this->vlc = vlc;
		}

		float getMass() {
			return this->mass;
		}

		float getRadius() {
			return this->radius;
		}

		sf::Vector2f getPos() {
			return this->pos;
		}

		sf::CircleShape getStar() {
			return this->star;
		}

		sf::Vector2f collision(Star targetStar) {
			sf::Vector2f vN = (normalize(sf::Vector2f(targetStar.pos.x - pos.x, targetStar.pos.y - pos.y)));
			sf::Vector2f hN(-vN.y, vN.x);

			float s1v = vlc.x * vN.x + vlc.y * vN.y;
			float s1h = vlc.x * hN.x + vlc.y * hN.y;
			float s2v = targetStar.vlc.x * vN.x + targetStar.vlc.y * vN.y;
			float s2h = targetStar.vlc.x * hN.x + targetStar.vlc.y * hN.y;

			s1v = ((mass - targetStar.mass)*s1v + 2 * targetStar.mass*s2v) / (mass + targetStar.mass);

			sf::Vector2f s1vVector(vN.x * s1v, vN.y * s1v);
			sf::Vector2f s1hVector(hN.x * s1h, hN.y * s1h);
			return sf::Vector2f(s1vVector.x + s1hVector.x, s1vVector.y + s1hVector.y);
		}

		void move() {
			pos.x += vlc.x;
			pos.y += vlc.y;
			star.setPosition(pos.x - radius, pos.y - radius);
			vlc.x += aVlc.x;
			vlc.y += aVlc.y;
		}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML");
	window.setFramerateLimit(144);

	sf::Font agencyb;
	agencyb.loadFromFile(".\\font\\AGENCYB.TTF");

	sf::Text info;
	info.setFont(agencyb);
	info.setCharacterSize(24);
	
	Star s1(20.2f, sf::Vector2f(640.f, 260.f), sf::Vector2f(0.f, 0.f));
	//Star s2(30.0f, sf::Vector2f(640.f, 360.f), sf::Vector2f(0.f, 0.f));
	Star s3(20.2f, sf::Vector2f(640.f, 460.f), sf::Vector2f(1.f, 0.f));
	
	Star stars[2] = {s1,s3};
	int starNum = sizeof(stars) / sizeof(stars[0]);

	bool gravity = true;
	bool collision = true;
	bool border = true;

	while (window.isOpen())
	{
		//event
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}
		//physics
		//gravity
		if (gravity == true) {
			for (int i = 0; i < starNum; i++) {
				float xF = 0, yF = 0;
				for (int j = 0; j < starNum; j++) {
					if (i == j) continue;
					float distance = sqrt(pow((stars[i].getPos().x - stars[j].getPos().x), 2) + pow((stars[i].getPos().y - stars[j].getPos().y), 2));
					xF += G * stars[i].getMass() * stars[j].getMass() / pow(distance, 2) / distance * (stars[j].getPos().x - stars[i].getPos().x);
					yF += G * stars[i].getMass() * stars[j].getMass() / pow(distance, 2) / distance * (stars[j].getPos().y - stars[i].getPos().y);
				}
				stars[i].aVlc.x = xF / stars[i].getMass();
				stars[i].aVlc.y = yF / stars[i].getMass();
			}
		}
		
		//collision
		if (collision == true) {
			sf::Vector2f* newVlc = new sf::Vector2f[starNum];
			bool* coli = new bool[starNum];

			for (int i = 0; i < starNum; i++) {
				newVlc[i] = sf::Vector2f(0, 0);
				coli[i] = false;
			}

			for (int i = 0; i < starNum; i++) {
				for (int j = 0; j < starNum; j++) {
					if (i == j) continue;
					float distance;
					distance = sqrt(pow((stars[i].getPos().x - stars[j].getPos().x), 2) + pow((stars[i].getPos().y - stars[j].getPos().y), 2));
					if (distance <= stars[i].getRadius() + stars[j].getRadius()) {
						coli[i] = true;
						newVlc[i] += stars[i].collision(stars[j]);
					}
					info.setString("distance :" + std::to_string(distance));
				}
			}
			for (int i = 0; i < starNum; i++) {
				if (coli[i]) {
					stars[i].vlc = newVlc[i];
				}
				if (border == true) {
					if (stars[i].getPos().x - stars[i].getRadius() < 0) stars[i].vlc.x = abs(stars[i].vlc.x);
					else if (stars[i].getPos().x + stars[i].getRadius() > 1280) stars[i].vlc.x = -abs(stars[i].vlc.x);
					if (stars[i].getPos().y - stars[i].getRadius() < 0)	stars[i].vlc.y = abs(stars[i].vlc.y);
					else if (stars[i].getPos().y + stars[i].getRadius() > 720) stars[i].vlc.y = -abs(stars[i].vlc.y);
				}
			}
			delete[] newVlc;
			delete[] coli;
		}
		
		//render
		window.clear();
		for (int i = 0; i < sizeof(stars) / sizeof(stars[0]); i++) {
			stars[i].move();
			window.draw(stars[i].getStar());
		}
		window.draw(info);
		window.display();
	}

	return 0;
}