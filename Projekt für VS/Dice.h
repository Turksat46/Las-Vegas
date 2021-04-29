//Geschrieben von: Kerem Okumus, Kiriakos Avramidis, Moritz Biedenbacher

#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Audio.hpp>

#include<iostream>
#include<Windows.h>
#include<fstream>
#include<cstdlib>
#include<conio.h>

using namespace sf;
using namespace std;

class wuerfel {
	
public:
	wuerfel(float positionx, int augenanzahl) {
		float position = positionx; //< Position des Würfels
        int augen = augenanzahl;
        wuerfeltextur.loadFromFile(wuerfelaugen[augen]);
        wuerfeltextur.setSmooth(true);
        //wuerfelsprite.setColor(sf::Color::Blue);
        wuerfelsprite.setTexture(wuerfeltextur, true);
        wuerfelsprite.setPosition(sf::Vector2f(position, 800.0f));
        wuerfelsprite.setScale(0.15, 0.15);
	}
	
    void zeichnewuerfel(RenderWindow& spielfenster) {
        spielfenster.draw(wuerfelsprite);
    }

private:
    sf::Texture wuerfeltextur;
	Sprite wuerfelsprite;
    string wuerfelaugen[6] = { "./res/Bilder/würfel/dice-png-1.png", "./res/Bilder/würfel/dice-png-2.png", "./res/Bilder/würfel/dice-png-3.png", "./res/Bilder/würfel/dice-png-4.png", "./res/Bilder/würfel/dice-png-5.png", "./res/Bilder/würfel/dice-png-6.png" };
};