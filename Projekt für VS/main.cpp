//Autor: Kerem Okumus,Kiriakos Avramidis,Moritz Biedenbacher
//Las Vegas Spiel als grafische Anwendung
//Dieses Programm nutzt die SFML-Bibliothek, dass zur Programmierung von grafischen Benutzeroberflächen erleichtern soll!
//geändert am: 25.01.2021(Datei erstellt und ersten Code geschrieben); 26.01.2021 (Code verändert und Menü hinzugefügt)


#include<SFML/Graphics.hpp>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Audio.hpp>

#include<iostream>
#include<Windows.h>
#include<fstream>
 
using namespace std;

//Selbstgeschriebene Header-Dateien
#include "Button.h"

//Eine Startklasse, die den Start des Spiels vorbereitet. (Wird nur einmal aufgerufen!)
void startup() {
    /*INFO:
    * IN DER ARRAY GIBT ES FOLGENDE EINSTELLUNGEN:
    * 0 = Ob Sound laufen soll
    * 1 = Wie laut es sein soll (Standart: 50)
    * 
    * BITTE DIESE LISTE BEI ÄNDERUNGEN ERGÄNZEN!!!
    */

    //Erstelle eine Struktur für Daten
    struct einstellung {
        int stellung;
    };

    //Schreibe eine Einstellungsdatei in Binär
    std::ofstream schreiber("einstellungen.einstellungen", std::ios::out | std::ios::binary);

    //Nenne alles, was geschrieben werden muss
    einstellung writer[1];
    writer[0].stellung = 1;
    writer[1].stellung = 50;

    //schreiber.write((char*)&writer[0].stellung, sizeof(einstellung));

    //Schreibe alle Daten ein
    for (int i = 0; i < 2; i++) {
        schreiber.write((char*) &writer[i], sizeof(einstellung));
    }

    schreiber.close();

    //Prüfe, ob alles erfolgreich war!
    if (!schreiber.good()) {
        printf("Einstellungsdatei konnte nicht geschrieben werden werden!");
        return;
    }
}

//Eine Einstellungsklasse, um es einfach aufzurufen, wann man es braucht!
void Einstellungen() {

    //Lade die Schriftart
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        printf("Konnte die Schriftart nicht laden! Bitte konsultieren Sie die Bedienungsanleitung! \n");
    }

    //Erstelle ein Einstellungsfenster-Objekt
    sf::RenderWindow settings(sf::VideoMode(800, 800), "Einstellungen");

    //Erstelle den Speichern-Knopf
    Button applybtn1("Anwenden", { 800, 100 }, 30, sf::Color::Green, sf::Color::Black);
    applybtn1.setFont(font);
    applybtn1.setPosition({ 0.0f, 700.0f });

    //Erstelle eine Struktur für Daten
    struct einstellung {
        int stellung;
    };

    //Einstellungendatei im Lesen-Modus öffnen
    std::ifstream leser("einstellungen.einstellungen", std::ios::out | std::ios::binary);
    if (!leser) {
        printf("Datei konnte nicht geöffnet werden!");
        return;
    }

    einstellung items[1];

    for(int i = 0; i < 2; i++) {
        leser.read((char*) &items[i], sizeof(einstellung));
        cout << "Ergebnis: " << items[i].stellung << endl;
    }

    //Datei schließen
    leser.close();

    //Falls Fehler, anzeigen
    if (!leser.good()) {
        printf("Einstellungdatei konnte nicht gelesen werden! \n");
        return;
    }
    
    //Variablen für Button
    string text;
    sf::Color farbe;
    int p = items[0].stellung;

    //Variablen für Lautstärke
    string volumetext;
    int v = items[1].stellung;

    //Knopfalgorithmus
    if (p == 1) {
        text = "An";
        farbe = sf::Color::Green;
    }
    else {
        text = "Aus";
        farbe = sf::Color::Red;
    }

    //Button soundbutton(text, { 200, 100 }, 30, farbe, sf::Color::Black);
    //soundbutton.setPosition({520.0f, 80.0f});
    //soundbutton.setFont(font);

    while (settings.isOpen()) {
        //Zeichne Menü
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

        //Eigentliche Einstellungen
        sf::Text soundtext("Musik", font);
        soundtext.setCharacterSize(30);
        soundtext.setPosition(sf::Vector2f(100.0f, 100.0f));
        soundtext.setFillColor(sf::Color::Black);

        Button soundbutton(text, { 200, 100 }, 30, farbe, sf::Color::Black);
        soundbutton.setPosition({520.0f, 80.0f});
        soundbutton.setFont(font);

        sf::Event event;
        while (settings.pollEvent(event))
        {
            // Schließt das Fenster, falls schließen gedrückt wird
            if (event.type == sf::Event::Closed)
                settings.close();

            switch (event.type) {
                //Wenn es geklickt hat
                case sf::Event::MouseButtonPressed:
                    if (applybtn1.isMouseOver(settings)) {
                        //Übernehm die Einstellungen
                        std::ofstream schreiber("einstellungen.einstellungen", std::ios::out | std::ios::trunc);
                        einstellung items[1];
                        items[0].stellung = p;
                        items[1].stellung = v;

                        for (int i = 0; i < 2; i++) {
                            schreiber.write((char*)&items[i], sizeof(einstellung));
                        }

                        schreiber.close();

                        printf("Einstellungen wurden uebernommen! \n");
                        settings.close();
                    }

                    if (soundbutton.isMouseOver(settings)) {
                        printf("Wurde auf Soundbutton geklickt! \n");
                        //Änder die Einstellung
                        if (p == 1)
                        {
                            p = 0;
                            text = "Aus";
                            farbe = sf::Color::Red;
                        }
                        else {
                            p = 1;
                            text = "An";
                            farbe = sf::Color::Green;
                        }
                    }
            }

        }

        //
        settings.clear(sf::Color(64, 224, 208, 255));
        applybtn1.drawTo(settings);
        settings.draw(convexShape);
        soundbutton.drawTo(settings);
        settings.draw(soundtext);
        
        settings.display();
    }
}


//Main-Funktion - Hier befinden sich alle Hauptfunktionen wie Erzeugung des Bildschirms etc.
int main()
{
    //Erzeuge das Fenster-Objekt
    sf::RenderWindow window(sf::VideoMode(1220, 800), "Las Vegas");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile("res/Bilder/icon.jpg")) {
        printf("Icon konnte nicht geladen! \n");
        return 0;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //Erstelle eine Struktur für Daten
    struct einstellung {
        int stellung;
    };

    //Prüfe, ob es Einstellungdatei existiert und erstelle sie, falls nicht
    std::ifstream leser("einstellungen.einstellungen", std::ios::out | std::ios::binary);
    if (!leser) {
        printf("Einstellungsdatei wird erstellt! \n");
        startup();
    }
    
    //Prüfe, ob Einstellung auf An ist
    einstellung items[1];

    for (int i = 0; i < 2; i++) {
        leser.read((char*)&items[i], sizeof(einstellung));
    }

    int e = items[0].stellung;

    //Erzeuge das Audio-Objekt und fange an zu spielen!
    sf::Music music;
    music.openFromFile("res/Audio/titelmusik.wav");
    music.setVolume(50);
    music.setLoop(true);
    if (e == 1) {
        music.play();
    }
    
    //Lade die Schriftart
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        printf("Konnte die Schriftart nicht laden!\n");
    }

    //Lade das Titelbild
    sf::Texture texture;
    texture.loadFromFile("res/Bilder/titelbild.png");
    sf::Sprite sprite(texture);

    //SpielenKnopf
    Button playbtn1("Neues Spiel", { 200, 100 }, 30, sf::Color(0,255,0,255), sf::Color::Black);
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
        
        // Switch, um zu prüfen, welcher Event passiert ist und was getan werden soll
        switch (event.type) {
            //Wenn es geklickt hat
            case sf::Event::MouseButtonPressed:
                if (playbtn1.isMouseOver(window)) {
                    //Wenn Spielen-Knopf gedrückt wird
                    //debug printf("Knopf wurde gedrueckt!");
                }

                if (leavebtn1.isMouseOver(window)) {
                    //Wenn Beenden-Knopf gedrückt wird
                    window.close();
                }

                if (settingsbtn1.isMouseOver(window)) {
                    //Öffne Einstellungen
                    Einstellungen();
                }


                if (loadbtn1.isMouseOver(window)) {
                    //Öffne Ladebildschirm für Spielstand
                    
                }

                //Wenn sich die Maus bewegt hat
            case sf::Event::MouseMoved:
                //Über dem Spielen-Knopf
                if (playbtn1.isMouseOver(window)) {
                    playbtn1.setBackColor(sf::Color(0, 127, 0, 255));
                }
                else {
                    playbtn1.setBackColor(sf::Color(0, 255, 0, 255));
                }

                //Über dem Beenden-Konpf
                if (leavebtn1.isMouseOver(window)) {
                    leavebtn1.setBackColor(sf::Color(127, 10, 16, 255));
                }
                else {
                    leavebtn1.setBackColor(sf::Color(255, 0, 0, 255));
                }

                //Über dem Einstellungen-Knopf
                if (settingsbtn1.isMouseOver(window)) {
                    settingsbtn1.setBackColor(sf::Color(127, 127, 127, 255));
                }
                else {
                    settingsbtn1.setBackColor(sf::Color(195, 195, 195, 255));
                }

                //Über dem Laden-Knopf
                if (loadbtn1.isMouseOver(window)) {
                    loadbtn1.setBackColor(sf::Color(0, 100, 127, 255));
                }
                else {
                    loadbtn1.setBackColor(sf::Color(0, 162, 232, 255));
                }

                //Wenn die Escape-Taste gedrückt wird
                if (sf::Event::KeyPressed) {
                    /*if (event.key.code == sf::Keyboard::End) {
                        printf("Escape-Taste wird gedrückt! \n");
                        window.close();
                    
                    */
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
        text.setPosition(230.0f, 430.0f);

        //Versiontext
        sf::Text vertext("Version: 0.3 Alpha", font);
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