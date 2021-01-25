//Autor: Kerem Okumus
//geändert am: 25.01.2021(Datei erstellt und ersten Code geschrieben)


#include<SFML/Graphics.hpp>
#include<iostream>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Audio.hpp>

//Selbstgeschriebene Header-Dateien
#include "Button.h"


int main()
{
    //Erzeuge das Fenster-Objekt
    sf::RenderWindow window(sf::VideoMode(1280, 700), "Las Vegas");

    //Erzeuge das Audio-Objekt und fange an zu spielen!
    sf::Music music;
    music.openFromFile("res/Audio/titelmusik.wav");
    music.setVolume(50);
    music.setLoop(true);
    music.play();
    
    
    //Lade die Schriftart
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        printf("Konnte die Schriftart nicht laden!");
    }

    //Lade das Titelbild
    sf::Texture texture;
    texture.loadFromFile("res/Bilder/titelbild.png");
    sf::Sprite sprite(texture);

    //SpielenKnopf
    Button playbtn1("Start", { 100, 50 }, 30, sf::Color(195,195,195,255), sf::Color::Black);
    playbtn1.setFont(font);
    playbtn1.setPosition({ 350.0f, 500.0f });

    //Beendenknopf
    Button leavebtn1("Beenden", { 100, 50 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
    leavebtn1.setFont(font);
    leavebtn1.setPosition({ 650.0f, 500.0f });

    // Eine Schleife, dass das Programm solange laufen lässt, bis es geschlossen wird
    while (window.isOpen())
    {
        // Überprüft alle Eventtrigger nach dem letzten Frame
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Schließt das Fenster, falls schließen gedrückt wird
            if (event.type == sf::Event::Closed)
                window.close();
        }

        switch (event.type) {
            case sf::Event::MouseButtonPressed:
                if (playbtn1.isMouseOver(window)) {
                    printf("Knopf wurde gedrueckt!");
                }
                if (leavebtn1.isMouseOver(window)) {
                    window.close();
                }
            case sf::Event::MouseMoved:
                if (playbtn1.isMouseOver(window)) {
                    playbtn1.setBackColor(sf::Color(127,127,127,255));
                }
                else {
                    playbtn1.setBackColor(sf::Color(195,195,195,255));
                }

                if (leavebtn1.isMouseOver(window)) {
                    leavebtn1.setBackColor(sf::Color(127, 127, 127, 255));
                }
                else {
                    leavebtn1.setBackColor(sf::Color(195, 195, 195, 255));
                }
                break;
        }

        // setze das Bildschirm zurück
        window.clear(sf::Color::White);
        
        // Erzeuge einen Text
        sf::Text text("Willkommen zu Las Vegas! Bitte wählen Sie eine der folgenden Optionen.", font);
        text.setCharacterSize(30);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setPosition(250.0f, 430.0f);

        //Zeichne Elemente in das Frame
        window.draw(text);
        window.draw(sprite);
        playbtn1.drawTo(window);
        leavebtn1.drawTo(window);

        // Zeige den Frame an
        window.display();
    }
}