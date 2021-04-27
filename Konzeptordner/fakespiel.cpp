//Autor: Kerem Okumus
//geschrieben am: 26.01.2020
//Beschreibung: Dient als Darstellungshilfe

#include<SFML/Graphics.hpp>
#include<iostream>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Audio.hpp>



#include<Windows.h>

//Selbstgeschriebene Header-Dateien
#include "Button.h"

void fakespielmain() {
    
        //Schließ das Menü

        sf::RenderWindow fakewindow(sf::VideoMode(1600, 800), "FAKE - Las Vegas");
        sf::Font font;
        if (!font.loadFromFile("res/Fonts/font.ttf")) {
            printf("Konnte die Schriftart nicht laden! Bitte konsultieren Sie die Bedienungsanleitung!");
        }

        while (fakewindow.isOpen()) {
            fakewindow.display();
            sf::Event event;
            while (fakewindow.pollEvent(event))
            {
                // Schließt das Fenster, falls schließen gedrückt wird
                if (event.type == sf::Event::Closed) {
                    fakewindow.close();

                }
            }
        }

        fakewindow.display();
    }

