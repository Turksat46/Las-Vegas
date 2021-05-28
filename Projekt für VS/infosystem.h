#pragma once
#include "Button.h"
void fehleranzeige(string titel, string nachricht) {
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        printf("FONT-FEHLER: SCHRIFTART KONNTE NICHT GELADEN WERDEN!\nBitte konsultieren Sie die Bedienungsanleitung!\nDr%ccken Sie eine Taste um das Programm zu beenden!", (char)129);
        _getch();
        exit(0);
    }

    sf::RenderWindow fehler(sf::VideoMode(600, 200), "Fehlermeldung", sf::Style::Titlebar);

    sf::Event event;
    while (fehler.isOpen()) {
        //Titel
        sf::Text titeltext(titel, font);
        titeltext.setCharacterSize(40);
        titeltext.setFillColor(sf::Color::Black);
        titeltext.setStyle(sf::Text::Bold);
        titeltext.setPosition(175.0f, 10.0f);

        //Nachricht
        sf::Text nachrichtentext(nachricht, font);
        nachrichtentext.setCharacterSize(30);
        nachrichtentext.setFillColor(sf::Color::Black);
        nachrichtentext.setPosition(100.0f, 80.0f);

        //Button
        Button verlassbutton("Beenden", { 100, 50 }, 30, sf::Color(209, 209, 209, 184), sf::Color::Black);
        verlassbutton.setFont(font);
        verlassbutton.setPosition(sf::Vector2f(250.0f, 125.0f));

        sf::Event event;
        while (fehler.pollEvent(event)) {
            // Schließt das Fenster, falls schließen gedrückt wird

            switch (event.type) {
                //Wenn es geklickt hat
            case sf::Event::MouseButtonPressed:
                if (verlassbutton.isMouseOver(fehler)) {
                    fehler.close();
                    exit(404);
                }
            }
        }

        //Fehler zeichnen
        fehler.clear(sf::Color(255, 255, 255, 255));
        fehler.draw(titeltext);
        fehler.draw(nachrichtentext);
        verlassbutton.drawTo(fehler);

        //Fehlermeldung anzeigen
        fehler.display();
    }
}

bool bestätigungsmeldung(string nachricht) {
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        printf("FONT-FEHLER: SCHRIFTART KONNTE NICHT GELADEN WERDEN!\nBitte konsultieren Sie die Bedienungsanleitung!\nDr%ccken Sie eine Taste um das Programm zu beenden!", (char)129);
        _getch();
        exit(0);
    }

    sf::RenderWindow meldung(sf::VideoMode(400, 200), "Meldung", sf::Style::Titlebar);

    sf::Event event;
    while (meldung.isOpen()) {
        
        //Nachricht
        sf::Text nachrichtentext(nachricht, font);
        nachrichtentext.setCharacterSize(25);
        nachrichtentext.setFillColor(sf::Color::Black);
        //Setze den Text in die Mitte
        float xPos = (400.0f / 2) - (nachrichtentext.getLocalBounds().width / 2);
        float yPos = (200.0f / 3) - (nachrichtentext.getLocalBounds().height / 2);
        nachrichtentext.setPosition(xPos, yPos);

        //Ja-Button
        Button jabutton("Ja", { 100, 50 }, 30, sf::Color::Green, sf::Color::Black);
        jabutton.setFont(font);
        jabutton.setPosition(sf::Vector2f(25.0f, 125.0f));

        //Nein-Button
        Button neinbutton("Nein", { 100, 50 }, 30, sf::Color::Red, sf::Color::Black);
        neinbutton.setFont(font);
        neinbutton.setPosition(sf::Vector2f(275.0f, 125.0f));

        Button abbrechenbutton("Abbrechen", { 100, 50 }, 25, sf::Color::Yellow, sf::Color::Black);
        abbrechenbutton.setFont(font);
        abbrechenbutton.setPosition(sf::Vector2f(150.0f, 125.0f));

        sf::Event event;
        while (meldung.pollEvent(event)) {
            // Schließt das Fenster, falls schließen gedrückt wird

            switch (event.type) {
                //Wenn es geklickt hat
                case sf::Event::MouseButtonPressed:
                    if (neinbutton.isMouseOver(meldung)) {
                        return false;
                    }

                    if (jabutton.isMouseOver(meldung)) {
                        return true;
                    }

                    if (abbrechenbutton.isMouseOver(meldung)) {
                        meldung.close();
                    }
                    break;
                    
            }
        }

        //Meldung zeichnen
        meldung.clear(sf::Color(150, 150, 150, 255));
        meldung.draw(nachrichtentext);
        jabutton.drawTo(meldung);
        neinbutton.drawTo(meldung);
        abbrechenbutton.drawTo(meldung);

        //Meldung anzeigen
        meldung.display();
    }
}