//Autor: Kerem Okumus
//ge�ndert am: 25.01.2021(Datei erstellt und ersten Code geschrieben); 26.01.2021 (Code ver�ndert und Men� hinzugef�gt)


#include<SFML/Graphics.hpp>
#include<iostream>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Audio.hpp>

#include<Windows.h>

//Selbstgeschriebene Header-Dateien
#include "Button.h"


//Eine Einstellungsklasse, um es einfach aufzurufen, wann man es braucht!
void Einstellungen() {

    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        printf("Konnte die Schriftart nicht laden! Bitte konsultieren Sie die Bedienungsanleitung!");
    }

    sf::RenderWindow settings(sf::VideoMode(800, 800), "Einstellungen");

    Button applybtn1("Anwenden", { 800, 100 }, 30, sf::Color::Green, sf::Color::Black);
    applybtn1.setFont(font);
    applybtn1.setPosition({ 0.0f, 700.0f });

    while (settings.isOpen()) {

        //Zeichne Men�
        sf::ConvexShape convexShape;
        convexShape.setPointCount(4);
        convexShape.setFillColor(sf::Color::White);
        //rechtsunten
        convexShape.setPoint(0, sf::Vector2f(750.0f, 700.0f));
        //Rechtsoben
        convexShape.setPoint(1, sf::Vector2f(750.0f, 50.0f));
        //linksoben
        convexShape.setPoint(2, sf::Vector2f(50.0f, 50.0f));
        //linksunten
        convexShape.setPoint(3, sf::Vector2f(50.0f, 700.0f));

        sf::Event event;
        while (settings.pollEvent(event))
        {
            // Schlie�t das Fenster, falls schlie�en gedr�ckt wird
            if (event.type == sf::Event::Closed)
                settings.close();

            switch (event.type) {
                //Wenn es geklickt hat
                case sf::Event::MouseButtonPressed:
                    if (applybtn1.isMouseOver(settings)) {
                        //�bernehm die Einstellungen
                        printf("Einstellungen wurden uebernommen!");
                        settings.close();
                    }
            }

        }

        settings.clear(sf::Color(64, 224, 208, 255));

        applybtn1.drawTo(settings);
        settings.draw(convexShape);
        
        settings.display();
        
    }
}


//Main-Funktion - Hier befinden sich alle Hauptfunktionen wie Erzeugung des Bildschirms etc.
int main()
{
    //Erzeuge das Fenster-Objekt
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Las Vegas");

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
    Button playbtn1("Start", { 200, 100 }, 30, sf::Color(0,255,0,255), sf::Color::Black);
    playbtn1.setFont(font);
    playbtn1.setPosition({ 350.0f, 500.0f });

    //LadenKnopf
    Button loadbtn1("Laden", { 200, 100 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
    loadbtn1.setFont(font);
    loadbtn1.setPosition({ 350.0f, 650.0f });

    //Beendenknopf
    Button leavebtn1("Beenden", { 200, 100 }, 30, sf::Color(255, 0, 0, 255), sf::Color::Black);
    leavebtn1.setFont(font);
    leavebtn1.setPosition({ 650.0f, 650.0f });

    //Einstellungknopf
    Button settingsbtn1("Einstellungen", { 200, 100 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
    settingsbtn1.setFont(font);
    settingsbtn1.setPosition({ 650.0f, 500.0f });

    // Eine Schleife, dass das Programm solange laufen l�sst, bis es geschlossen wird
    while (window.isOpen())
    {
        

        // �berpr�ft alle Eventtrigger nach dem letzten Frame
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Schlie�t das Fenster, falls schlie�en gedr�ckt wird
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // Switch, um zu pr�fen, welcher Event passiert ist und was getan werden soll
        switch (event.type) {
            //Wenn es geklickt hat
            case sf::Event::MouseButtonPressed:
                if (playbtn1.isMouseOver(window)) {
                    //Wenn Spielen-Knopf gedr�ckt wird
                    //debug printf("Knopf wurde gedrueckt!");
                }

                if (leavebtn1.isMouseOver(window)) {
                    //Wenn Beenden-Knopf gedr�ckt wird
                    window.close();
                }

                if (settingsbtn1.isMouseOver(window)) {
                    //�ffne Einstellungen
                    Einstellungen();
                }


                if (loadbtn1.isMouseOver(window)) {
                    //�ffne Ladebildschirm f�r Spielstand
                    
                }

                //Wenn sich die Maus bewegt hat
            case sf::Event::MouseMoved:
                //�ber dem Spielen-Knopf
                if (playbtn1.isMouseOver(window)) {
                    playbtn1.setBackColor(sf::Color(0, 127, 0, 255));
                }
                else {
                    playbtn1.setBackColor(sf::Color(0, 255, 0, 255));
                }

                //�ber dem Beenden-Konpf
                if (leavebtn1.isMouseOver(window)) {
                    leavebtn1.setBackColor(sf::Color(127, 10, 16, 255));
                }
                else {
                    leavebtn1.setBackColor(sf::Color(255, 0, 0, 255));
                }

                //�ber dem Einstellungen-Knopf
                if (settingsbtn1.isMouseOver(window)) {
                    settingsbtn1.setBackColor(sf::Color(127, 127, 127, 255));
                }
                else {
                    settingsbtn1.setBackColor(sf::Color(195, 195, 195, 255));
                }

                //�ber dem Laden-Knopf
                if (loadbtn1.isMouseOver(window)) {
                    loadbtn1.setBackColor(sf::Color(0, 100, 127, 255));
                }
                else {
                    loadbtn1.setBackColor(sf::Color(0, 162, 232, 255));
                }

                //Wenn die Escape-Taste gedr�ckt wird
                if (sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::End) {
                        printf("Escape-Taste wird gedr�ckt!");
                        window.close();
                    }
                }

                break;
        }

        // setze das Bildschirm zur�ck
        window.clear(sf::Color::White);
        
        // Erzeuge einen Text
        sf::Text text("Willkommen zu Las Vegas! Bitte w�hlen Sie eine der folgenden Optionen.", font);
        text.setCharacterSize(30);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setPosition(230.0f, 430.0f);

        //Versiontext
        sf::Text vertext("Version: 0.2 Alpha", font);
        vertext.setCharacterSize(25);
        vertext.setStyle(sf::Text::Regular);
        vertext.setFillColor(sf::Color::Black);
        vertext.setPosition(20.0f, 760.0f);

        //Zeichne Umrandung um Buttons
        sf::ConvexShape convexShape;
        convexShape.setPointCount(4);
        convexShape.setFillColor(sf::Color(245, 245, 220, 255));
        convexShape.setOutlineThickness(3.0f);
        convexShape.setPoint(0, sf::Vector2f(300.0f, 800.0f));
        convexShape.setPoint(1, sf::Vector2f(300.0f, 475.0f));
        convexShape.setPoint(2, sf::Vector2f(900.0f, 475.0f));
        convexShape.setPoint(3, sf::Vector2f(900.0f, 800.0f));

        //Zeichne Elemente in das Frame
        window.draw(text);
        window.draw(vertext);
        window.draw(sprite);
        window.draw(convexShape);
        playbtn1.drawTo(window);
        leavebtn1.drawTo(window);
        settingsbtn1.drawTo(window);
        loadbtn1.drawTo(window);

        // Zeige den Frame an
        window.display();
    }
}