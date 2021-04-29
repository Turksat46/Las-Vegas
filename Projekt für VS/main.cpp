//Autor: Kerem Okumus, Kiriakos Avramidis, Moritz Biedenbacher
//Las Vegas Spiel als grafische Anwendung
//Dieses Programm nutzt die SFML-Bibliothek, dass zur Programmierung von grafischen Benutzeroberfl�chen erleichtern soll!
//ge�ndert am: 25.01.2021(Datei erstellt und ersten Code geschrieben); 26.01.2021 (Code ver�ndert und Men� hinzugef�gt)

//Einbindung wichtiger Bibliotheken
#include<SFML/Graphics.hpp>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Audio.hpp>

#include<iostream>
#include<Windows.h>
#include<fstream>
#include<cstdlib>
#include<conio.h>
#include<random>
 
using namespace std;
using namespace sf;

//Selbstgeschriebene Header-Dateien
#include "Button.h"
#include "Dice.h"

//Wichtige Variablen
float wuerfelpos[8] = { 20.0f, 120.0f, 220.0f, 320.0f, 420.0f, 520.0f, 620.0f, 720.0f };
string wuerfelaugen[6] = { "./res/Bilder/w�rfel/dice-png-1.png", "./res/Bilder/w�rfel/dice-png-2.png", "./res/Bilder/w�rfel/dice-png-3.png", "./res/Bilder/w�rfel/dice-png-4.png", "./res/Bilder/w�rfel/dice-png-5.png", "./res/Bilder/w�rfel/dice-png-6.png" };
int wuerfelwert[8];
bool spielBeendet = false;

//
//Eine Fehlermethode, der eine Fehlermeldung auf dem Bildschirm erscheinen l�sst
//
//Nutzung: fehleranzeige(TITEL, NACHRICHT);
void fehleranzeige(string titel, string nachricht) {
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        printf("FONT-FEHLER: SCHRIFTART KONNTE NICHT GELADEN WERDEN!\nBitte konsultieren Sie die Bedienungsanleitung!\nDr%ccken Sie eine Taste um das Programm zu beenden!", (char)129);
        _getch();
        exit(0);
    }

    sf::RenderWindow fehler(sf::VideoMode(600, 200), "Fehlermeldung", sf::Style::None);

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
        verlassbutton.setPosition(sf::Vector2f(250.0f, 150.0f));

        sf::Event event;
        while (fehler.pollEvent(event)) {
            // Schlie�t das Fenster, falls schlie�en gedr�ckt wird

            switch (event.type) {
                //Wenn es geklickt hat
                case sf::Event::MouseButtonPressed:
                    if (verlassbutton.isMouseOver(fehler)) {
                        fehler.close();
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


//
//Eine Startklasse, die den Start des Spiels vorbereitet. (Wird nur einmal aufgerufen!)
//
void startup() {
    /*INFO:
    * IN DER ARRAY GIBT ES FOLGENDE EINSTELLUNGEN:
    * 0 = Ob Sound laufen soll
    * 1 = Wie laut es sein soll (Standart: 50)
    * 
    * BITTE DIESE LISTE BEI �NDERUNGEN ERG�NZEN!!!
    */

    //Erstelle eine Struktur f�r Daten
    struct einstellung {
        int stellung;
    };

    //Schreibe eine Einstellungsdatei in Bin�r
    ofstream schreiber("einstellungen.einstellungen", std::ios::out | std::ios::binary);

    //Nenne alles, was geschrieben werden muss
    //Siehe oben, um zu verstehen, f�r was was steht!
    int writer[3];
    writer[0] = 1;
    writer[1] = 50;

    //schreiber.write((char*)&writer[0].stellung, sizeof(einstellung));

    //Schreibe alle Daten ein
    printf("Schreiben wird gestartet!\n");
    for (int i = 0; i < 2; i++) {
        schreiber.write((char*) &writer[i], sizeof(einstellung));
    }

    //Datei schlie�en
    schreiber.close();

    //Pr�fe, ob alles erfolgreich war!
    if (!schreiber.good()) {
        fehleranzeige("Datei-Fehler", "Einstellungsdatei konnte nicht ge�ffnet werden.");
        return;
    }

    //Musik starten, da es keine Einstellung existiert!
    //Erzeuge das Audio-Objekt und fange an zu spielen!
    sf::Music music;
    music.openFromFile("res/Audio/titelmusik.wav");
    music.setVolume(50);
    music.setLoop(true);
    music.play();
}

//
//Eine Einstellungsklasse, um es einfach aufzurufen, wann man es braucht!
//
void Einstellungen() {
    //Lade die Schriftart
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        fehleranzeige("Font-Fehler", "Schriftart konnte nicht geladen werden!");
    }

    //Erstelle ein Einstellungsfenster-Objekt
    sf::RenderWindow settings(sf::VideoMode(800, 800), "Einstellungen");

    //Erstelle den Speichern-Knopf
    Button applybtn1("Anwenden", { 800, 100 }, 30, sf::Color::Green, sf::Color::Black);
    applybtn1.setFont(font);
    applybtn1.setPosition({ 0.0f, 700.0f });

    //Erstelle eine Struktur f�r Daten
    struct einstellung {
        int stellung;
    };

    //Einstellungendatei im Lesen-Modus �ffnen
    ifstream leser("einstellungen.einstellungen", std::ios::out | std::ios::binary);
    if (!leser) {
        fehleranzeige("Datei-Fehler", "Einstellungsdatei konnte nicht ge�ffnet werden.");
        return;
    }

    //Array f�r gelesene Daten erstellen
    int items[3];
    //einstellung items[1];

    //Daten von Datei einlesen
    for(int i = 0; i < 2; i++) {
        leser.read((char*) &items[i], sizeof(einstellung));
        //printf("%p", &items[i]);
    }

    //Datei schlie�en
    leser.close();

    //Falls Fehler, anzeigen
    if (!leser.good()) {
        fehleranzeige("Datei-Fehler", "Einstellungsdatei konnte nicht gelesen werden.");
        return;
    }
    
    //Variablen f�r Button
    string text;
    sf::Color farbe;
    int p = items[0];

    //Variablen f�r Lautst�rke
    string volumetext;
    int v = items[1];

    //Knopfalgorithmus
    if (p == 1) {
        text = "An";
        farbe = sf::Color::Green;
    }
    else {
        text = "Aus";
        farbe = sf::Color::Red;
    }

    while (settings.isOpen()) {
        //
        //Zeichne Men�
        //
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
        //Sound Button
        Button soundbutton(text, { 200, 100 }, 30, farbe, sf::Color::Black);
        soundbutton.setPosition({520.0f, 80.0f});
        soundbutton.setFont(font);

        //Eventhandler
        //Hier alles, was man mit dem Programm interagieren kann (Maus �ber Dinge, Klick und co.)) schreiben.
        sf::Event event;
        while (settings.pollEvent(event))
        {
            // Schlie�t das Fenster, falls schlie�en gedr�ckt wird
            if (event.type == sf::Event::Closed)
                settings.close();

            switch (event.type) {
                //Wenn es geklickt hat
                case sf::Event::MouseButtonPressed:
                    //Wenn es �ber dem Anwenden-Button geklickt hat
                    if (applybtn1.isMouseOver(settings)) {
                        //�bernehm die Einstellungen
                        ofstream schreiber("einstellungen.einstellungen", std::ios::out | std::ios::trunc);
                        //Array f�r Daten erstellen und beschriften
                        int items[2];
                        //einstellung items[1];
                        //p = Ob Sound laufen soll
                        //v = Lautst�rke
                        items[0]= p;
                        items[1]= v;

                        //Indizen in die Datei schreiben
                        for (int i = 0; i < 2; i++) {
                            schreiber.write((char*)&items[i], sizeof(einstellung));
                        }

                        //Datei schlie�en
                        schreiber.close();
                        printf("Einstellungen wurden uebernommen! \n");
                        //Einstellungen schlie�en
                        settings.close();
                    }

                    if (soundbutton.isMouseOver(settings)) {
                        printf("Wurde auf Soundbutton geklickt! \n");
                        //�nder die Einstellung
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

        //Alles auf die Oberfl�che zeichnen
        settings.clear(sf::Color(64, 224, 208, 255));
        applybtn1.drawTo(settings);
        settings.draw(convexShape);
        soundbutton.drawTo(settings);
        settings.draw(soundtext);
        
        //Einstellungen anzeigen
        settings.display();
    }
}

void wuerfel(float positionx, int augenanzahl) {
    float position = positionx; //< Position des W�rfels
    int augen = augenanzahl;
    sf::Texture wuerfeltextur;
    Sprite wuerfelsprite;
    wuerfeltextur.loadFromFile(wuerfelaugen[augen]);
    //wuerfeltextur.setSmooth(true);
    //wuerfelsprite.setColor(sf::Color::Blue);
    wuerfelsprite.setTexture(wuerfeltextur, true);
    wuerfelsprite.setPosition(sf::Vector2f(position, 800.0f));
    wuerfelsprite.setScale(0.15, 0.15);
}


//
//MAIN SPIEL (TEST)
//
//Eine Function f�rs neue Spiel erstellen (nicht vollst�ndig)
void neuesSpieltest() {
    sf::RenderWindow spiel(sf::VideoMode(1600, 1000), "Las Vegas");

    bool gewonnen = false;
    //Schriftart laden
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        fehleranzeige("Font-Fehler", "Schriftart konnte nicht geladen werden!");
    }

    // Icon f�r das Spiel setzen
    sf::Image icon;
    if (!icon.loadFromFile("res/Bilder/icon.jpg")) {
        //fehleranzeige("Icon-Fehler", "Icon konnte nicht geladen werden.");
        spiel.close();
        return;
    }
    spiel.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //Hauptschleife des Spiels
    while(spiel.isOpen()){
        spiel.clear(sf::Color(53, 104, 45, 255));

        //TestKarte
        //Dies wird mit Punkten gezeichnet sprich an jeder Ecke ein Punkt
        /*sf::ConvexShape testkarte;
        testkarte.setPointCount(4);
        testkarte.setFillColor(sf::Color(192, 126, 220, 255));
        testkarte.setOutlineThickness(3.0f);
        //Linksoben
        testkarte.setPoint(0, sf::Vector2f(100.0f, 100.0f));
        //Rechtsoben
        testkarte.setPoint(1, sf::Vector2f(300.0f, 100.0f));
        //Rechtsunten
        testkarte.setPoint(2, sf::Vector2f(300.0f, 600.0f));
        //Linksunten
        testkarte.setPoint(3, sf::Vector2f(100.0f, 600.0f));
        //Zeichnen
        spiel.draw(testkarte);
        spiel.display();*/

        //Las Vegas Logo
        sf::Texture logo;
        logo.loadFromFile("res/Bilder/Assets/vegas.gif");
        logo.setSmooth(true);
        sf::Sprite logosprite(logo);
        logosprite.setPosition({1300.0f, 10.0f});
        logosprite.setScale(0.5f, 0.5f);
        spiel.draw(logosprite);
        //
        // Trennlinie
        sf::ConvexShape trennlinie;
        trennlinie.setPointCount(4);
        trennlinie.setFillColor(sf::Color::Black);
        trennlinie.setPoint(0, sf::Vector2f(1250.0f, 0.0f));
        trennlinie.setPoint(1, sf::Vector2f(1250.0f, 900.0f));
        trennlinie.setPoint(2, sf::Vector2f(1251.0f, 900.0f));
        trennlinie.setPoint(3, sf::Vector2f(1251.0f, 0.0f));
        spiel.draw(trennlinie);

        //
        //Kasten f�r Casinos
        sf::ConvexShape goldennuggetkasten;
        goldennuggetkasten.setPointCount(4);
        goldennuggetkasten.setFillColor(sf::Color(251,209,100,165));
        goldennuggetkasten.setOutlineThickness(3.0f);
        goldennuggetkasten.setOutlineColor(sf::Color(251, 209, 100, 50));
        goldennuggetkasten.setPoint(0, sf::Vector2f(18.0f, 20.0f));
        goldennuggetkasten.setPoint(1, sf::Vector2f(202.0f, 20.0f));
        goldennuggetkasten.setPoint(2, sf::Vector2f(202.0f, 750.0f));
        goldennuggetkasten.setPoint(3, sf::Vector2f(18.0f, 750.0f));
        spiel.draw(goldennuggetkasten);

        sf::ConvexShape caesarskasten;
        caesarskasten.setPointCount(4);
        caesarskasten.setFillColor(sf::Color(248, 131, 49, 140));
        caesarskasten.setOutlineThickness(3.0f);
        caesarskasten.setOutlineColor(sf::Color(248, 131, 49, 50));
        caesarskasten.setPoint(0, sf::Vector2f(218.0f, 20.0f));
        caesarskasten.setPoint(1, sf::Vector2f(402.0f, 20.0f));
        caesarskasten.setPoint(2, sf::Vector2f(402.0f, 750.0f));
        caesarskasten.setPoint(3, sf::Vector2f(218.0f, 750.0f));
        spiel.draw(caesarskasten);

        sf::ConvexShape miragekasten;
        miragekasten.setPointCount(4);
        miragekasten.setFillColor(sf::Color(180, 110, 198, 145));
        miragekasten.setOutlineThickness(3.0f);
        miragekasten.setOutlineColor(sf::Color(180, 110, 198, 50));
        miragekasten.setPoint(0, sf::Vector2f(418.0f, 20.0f));
        miragekasten.setPoint(1, sf::Vector2f(602.0f, 20.0f));
        miragekasten.setPoint(2, sf::Vector2f(602.0f, 750.0f));
        miragekasten.setPoint(3, sf::Vector2f(418.0f, 750.0f));
        spiel.draw(miragekasten);

        sf::ConvexShape saharakasten;
        saharakasten.setPointCount(4);
        saharakasten.setFillColor(sf::Color(212, 76, 48, 122));
        saharakasten.setOutlineThickness(3.0f);
        saharakasten.setOutlineColor(sf::Color(212, 76, 48, 50));
        saharakasten.setPoint(0, sf::Vector2f(618.0f, 20.0f));
        saharakasten.setPoint(1, sf::Vector2f(802.0f, 20.0f));
        saharakasten.setPoint(2, sf::Vector2f(802.0f, 750.0f));
        saharakasten.setPoint(3, sf::Vector2f(618.0f, 750.0f));
        spiel.draw(saharakasten);

        sf::ConvexShape luxorkasten;
        luxorkasten.setPointCount(4);
        luxorkasten.setFillColor(sf::Color(83, 135, 149, 109));
        luxorkasten.setOutlineThickness(3.0f);
        luxorkasten.setOutlineColor(sf::Color(83, 135, 149, 50));
        luxorkasten.setPoint(0, sf::Vector2f(818.0f, 20.0f));
        luxorkasten.setPoint(1, sf::Vector2f(1002.0f, 20.0f));
        luxorkasten.setPoint(2, sf::Vector2f(1002.0f, 750.0f));
        luxorkasten.setPoint(3, sf::Vector2f(818.0f, 750.0f));
        spiel.draw(luxorkasten);

        sf::ConvexShape circuskasten;
        circuskasten.setPointCount(4);
        circuskasten.setFillColor(sf::Color(121, 147, 200, 151));
        circuskasten.setOutlineThickness(3.0f);
        circuskasten.setOutlineColor(sf::Color(121, 147, 200, 50));
        circuskasten.setPoint(0, sf::Vector2f(1018.0f, 20.0f));
        circuskasten.setPoint(1, sf::Vector2f(1202.0f, 20.0f));
        circuskasten.setPoint(2, sf::Vector2f(1202.0f, 750.0f));
        circuskasten.setPoint(3, sf::Vector2f(1018.0f, 750.0f));
        spiel.draw(circuskasten);

        //
        //Einzelne Casinos
        sf::Texture golden_nugget;
        golden_nugget.loadFromFile("res/Bilder/Assets/golden_nugget.png");
        golden_nugget.setSmooth(true);
        sf::Sprite golden_nuggetsprite(golden_nugget);
        golden_nuggetsprite.setPosition({ 20.0f, 350.0f });
        //golden_nuggetsprite.setScale(0.75, 0.75);
        spiel.draw(golden_nuggetsprite);
        
        sf::Texture caesars;
        caesars.loadFromFile("res/Bilder/Assets/caesars.png");
        caesars.setSmooth(true);
        sf::Sprite caesarssprite(caesars);
        caesarssprite.setPosition({ 220.0f, 350.0f });
        //caesarssprite.setScale(0.75, 0.75);
        spiel.draw(caesarssprite);

        sf::Texture mirage;
        mirage.loadFromFile("res/Bilder/Assets/mirage.png");
        mirage.setSmooth(true);
        sf::Sprite miragesprite(mirage);
        miragesprite.setPosition({ 420.0f, 350.0f });
        spiel.draw(miragesprite);

        sf::Texture sahara;
        sahara.loadFromFile("res/Bilder/Assets/sahara.png");
        sahara.setSmooth(true);
        sf::Sprite saharasprite(sahara);
        saharasprite.setPosition({ 620.0f, 350.0f });
        spiel.draw(saharasprite);

        sf::Texture luxor;
        luxor.loadFromFile("res/Bilder/Assets/luxor.png");
        luxor.setSmooth(true);
        sf::Sprite luxorsprite(luxor);
        luxorsprite.setPosition({ 820.0f, 350.0f });
        spiel.draw(luxorsprite);

        sf::Texture circus;
        circus.loadFromFile("res/Bilder/Assets/circus.png");
        circus.setSmooth(true);
        sf::Sprite circussprite(circus);
        circussprite.setPosition({ 1020.0f, 350.0f });
        spiel.draw(circussprite);

        //Men� unten
        sf::ConvexShape menuunten;
        menuunten.setPointCount(4);
        //Linksoben
        menuunten.setPoint(0, sf::Vector2f(0.0f, 935.0f));
        //Rechtsoben
        menuunten.setPoint(1, sf::Vector2f(1600.0f, 935.0f));
        //Rechtsunten
        menuunten.setPoint(2, sf::Vector2f(1600.0f, 1000.0f));
        //Linksunten
        menuunten.setPoint(3, sf::Vector2f(0.0f, 1000.0f));
        spiel.draw(menuunten);

        //Schrifttab �ber Men�
        sf::ConvexShape schrifttab;
        schrifttab.setPointCount(4);
        //Linksoben
        schrifttab.setPoint(0, sf::Vector2f(0.0f, 900.0f));
        //Rechtsoben
        schrifttab.setPoint(1, sf::Vector2f(1600.0f, 900.0f));
        //Rechtsunten
        schrifttab.setPoint(2, sf::Vector2f(1600.0f, 935.0f));
        //Linksunten
        schrifttab.setPoint(3, sf::Vector2f(0.0f, 935.0f));
        schrifttab.setFillColor(sf::Color(53, 53, 53, 150));
        spiel.draw(schrifttab);
        
        sf::Texture wuerfel;
        wuerfel.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        wuerfel.setSmooth(true);
        sf::Sprite wuerfelsprite;
        //wuerfelsprite.setColor(sf::Color::Blue);
        wuerfelsprite.setTexture(wuerfel, true);
        wuerfelsprite.setPosition({ 20.0f, 800.0f });
        wuerfelsprite.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite);

        sf::Texture wuerfel2;
        wuerfel2.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        wuerfel2.setSmooth(true);
        sf::Sprite wuerfelsprite2;
        //wuerfelsprite2.setColor(sf::Color::Blue);
        wuerfelsprite2.setTexture(wuerfel2, true);
        wuerfelsprite2.setPosition({ 120.0f, 800.0f });
        wuerfelsprite2.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite2);

        sf::Texture wuerfel3;
        wuerfel3.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        wuerfel3.setSmooth(true);
        sf::Sprite wuerfelsprite3;
        //wuerfelsprite3.setColor(sf::Color::Blue);
        wuerfelsprite3.setTexture(wuerfel3, true);
        wuerfelsprite3.setPosition({ 220.0f, 800.0f });
        wuerfelsprite3.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite3);

        sf::Texture wuerfel4;
        wuerfel4.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        wuerfel4.setSmooth(true);
        sf::Sprite wuerfelsprite4;
        //wuerfelsprite4.setColor(sf::Color::Blue);
        wuerfelsprite4.setTexture(wuerfel4, true);
        wuerfelsprite4.setPosition({ 320.0f, 800.0f });
        wuerfelsprite4.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite4);

        sf::Texture wuerfel5;
        wuerfel5.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        wuerfel5.setSmooth(true);
        sf::Sprite wuerfelsprite5;
        //wuerfelsprite5.setColor(sf::Color::Blue);
        wuerfelsprite5.setTexture(wuerfel5, true);
        wuerfelsprite5.setPosition({ 420.0f, 800.0f });
        wuerfelsprite5.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite5);

        sf::Texture wuerfel6;
        wuerfel6.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        wuerfel6.setSmooth(true);
        sf::Sprite wuerfelsprite6;
        //wuerfelsprite6.setColor(sf::Color::Blue);
        wuerfelsprite6.setTexture(wuerfel6, true);
        wuerfelsprite6.setPosition({ 520.0f, 800.0f });
        wuerfelsprite6.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite6);

        sf::Texture wuerfel7;
        wuerfel7.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        wuerfel7.setSmooth(true);
        sf::Sprite wuerfelsprite7;
        //wuerfelsprite7.setColor(sf::Color::Blue);
        wuerfelsprite7.setTexture(wuerfel7, true);
        wuerfelsprite7.setPosition({ 620.0f, 800.0f });
        wuerfelsprite7.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite7);

        sf::Texture wuerfel8;
        wuerfel8.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        wuerfel8.setSmooth(true);
        sf::Sprite wuerfelsprite8;
        //wuerfelsprite8.setColor(sf::Color::Blue);
        wuerfelsprite8.setTexture(wuerfel8, true);
        wuerfelsprite8.setPosition({ 720.0f, 800.0f });
        wuerfelsprite8.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite8);

        //Eigentliche Einstellungen
        sf::Text subtext("+++ Spieler 1 ist dran! +++", font);
        subtext.setCharacterSize(25);
        subtext.setPosition(sf::Vector2f(700.0f, 900.0f));
        subtext.setFillColor(sf::Color::Black);
        spiel.draw(subtext);

        //Event-Handling
        sf::Event event;
        while (spiel.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                spiel.close();
        }
        //Spiel zeichnen
        spiel.display();
    }
}

//Randomizer
int gen() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 6);
    return dist(gen);
}

void neuesSpiel() {
    sf::RenderWindow spiel(sf::VideoMode(1600, 1000), "Las Vegas");

    //Schriftart laden
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        fehleranzeige("Font-Fehler", "Schriftart konnte nicht geladen werden!");
    }

    // Icon f�r das Spiel setzen
    sf::Image icon;
    if (!icon.loadFromFile("res/Bilder/icon.jpg")) {
        //fehleranzeige("Icon-Fehler", "Icon konnte nicht geladen werden.");
        spiel.close();
        return;
    }
    spiel.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    for (int i = 0; i <=7; i++) {
        wuerfelwert[i] = gen();
        cout << wuerfelwert[i];
    }
    
    
    //wuerfel(120.0f, wuerfelwert[1]);
    //wuerfel()
    
    /*
    do {
        for (int spielerzahl = 0; spielerzahl <= 3; spielerzahl++) {
            for (int wuerfelzahl = 0; wuerfelzahl <= 9; wuerfelzahl++) {
                wuerfelwert[wuerfelzahl] = rand() % 6 + 1;
                switch (wuerfelwert[wuerfelzahl]) {
                    case 1:
                       //wuerfel.loadFromFile(wuerfelaugen[0]);
                        break;
                }
            }
        }
    } while (spielBeendet == false);*/

    sf::Texture wuerfel;
    switch (wuerfelwert[0]) {
        case 1:
            wuerfel.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
            break;
        case 2:
            wuerfel.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
            break;
        case 3:
            wuerfel.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
            break;
        case 4:
            wuerfel.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
            break;
        case 5:
            wuerfel.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
            break;
        case 6:
            wuerfel.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
            break;
    }
    wuerfel.setSmooth(true);
    sf::Sprite wuerfelsprite;
    //wuerfelsprite.setColor(sf::Color::Blue);
    wuerfelsprite.setTexture(wuerfel, true);
    wuerfelsprite.setPosition({ 20.0f, 800.0f });
    wuerfelsprite.setScale(0.15, 0.15);

    sf::Texture wuerfel2;
    switch (wuerfelwert[1]) {
    case 1:
        wuerfel2.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        break;
    case 2:
        wuerfel2.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        break;
    case 3:
        wuerfel2.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        break;
    case 4:
        wuerfel2.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        break;
    case 5:
        wuerfel2.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        break;
    case 6:
        wuerfel2.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        break;
    }
    wuerfel2.setSmooth(true);
    sf::Sprite wuerfelsprite2;
    //wuerfelsprite.setColor(sf::Color::Blue);
    wuerfelsprite2.setTexture(wuerfel2, true);
    wuerfelsprite2.setPosition({ 120.0f, 800.0f });
    wuerfelsprite2.setScale(0.15, 0.15);

    sf::Texture wuerfel3;
    switch (wuerfelwert[2]) {
    case 1:
        wuerfel3.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        break;
    case 2:
        wuerfel3.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        break;
    case 3:
        wuerfel3.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        break;
    case 4:
        wuerfel3.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        break;
    case 5:
        wuerfel3.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        break;
    case 6:
        wuerfel3.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        break;
    }
    wuerfel3.setSmooth(true);
    sf::Sprite wuerfelsprite3;
    //wuerfelsprite.setColor(sf::Color::Blue);
    wuerfelsprite3.setTexture(wuerfel3, true);
    wuerfelsprite3.setPosition({ 220.0f, 800.0f });
    wuerfelsprite3.setScale(0.15, 0.15);

    sf::Texture wuerfel4;
    switch (wuerfelwert[3]) {
    case 1:
        wuerfel4.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        break;
    case 2:
        wuerfel4.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        break;
    case 3:
        wuerfel4.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        break;
    case 4:
        wuerfel4.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        break;
    case 5:
        wuerfel4.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        break;
    case 6:
        wuerfel4.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        break;
    }
    wuerfel4.setSmooth(true);
    sf::Sprite wuerfelsprite4;
    //wuerfelsprite.setColor(sf::Color::Blue);
    wuerfelsprite4.setTexture(wuerfel4, true);
    wuerfelsprite4.setPosition({ 320.0f, 800.0f });
    wuerfelsprite4.setScale(0.15, 0.15);
    
    sf::Texture wuerfel5;
    switch (wuerfelwert[4]) {
    case 1:
        wuerfel5.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        break;
    case 2:
        wuerfel5.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        break;
    case 3:
        wuerfel5.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        break;
    case 4:
        wuerfel5.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        break;
    case 5:
        wuerfel5.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        break;
    case 6:
        wuerfel5.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        break;
    }
    wuerfel5.setSmooth(true);
    sf::Sprite wuerfelsprite5;
    //wuerfelsprite.setColor(sf::Color::Blue);
    wuerfelsprite5.setTexture(wuerfel5, true);
    wuerfelsprite5.setPosition({ 420.0f, 800.0f });
    wuerfelsprite5.setScale(0.15, 0.15);

    sf::Texture wuerfel6;
    switch (wuerfelwert[5]) {
    case 1:
        wuerfel6.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        break;
    case 2:
        wuerfel6.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        break;
    case 3:
        wuerfel6.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        break;
    case 4:
        wuerfel6.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        break;
    case 5:
        wuerfel6.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        break;
    case 6:
        wuerfel6.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        break;
    }
    wuerfel6.setSmooth(true);
    sf::Sprite wuerfelsprite6;
    //wuerfelsprite.setColor(sf::Color::Blue);
    wuerfelsprite6.setTexture(wuerfel6, true);
    wuerfelsprite6.setPosition({ 520.0f, 800.0f });
    wuerfelsprite6.setScale(0.15, 0.15);

    sf::Texture wuerfel7;
    switch (wuerfelwert[6]) {
    case 1:
        wuerfel7.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        break;
    case 2:
        wuerfel7.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        break;
    case 3:
        wuerfel7.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        break;
    case 4:
        wuerfel7.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        break;
    case 5:
        wuerfel7.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        break;
    case 6:
        wuerfel7.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        break;
    }
    wuerfel7.setSmooth(true);
    sf::Sprite wuerfelsprite7;
    //wuerfelsprite.setColor(sf::Color::Blue);
    wuerfelsprite7.setTexture(wuerfel7, true);
    wuerfelsprite7.setPosition({ 620.0f, 800.0f });
    wuerfelsprite7.setScale(0.15, 0.15);

    sf::Texture wuerfel8;
    switch (wuerfelwert[7]) {
    case 1:
        wuerfel8.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        break;
    case 2:
        wuerfel8.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        break;
    case 3:
        wuerfel8.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        break;
    case 4:
        wuerfel8.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        break;
    case 5:
        wuerfel8.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        break;
    case 6:
        wuerfel8.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        break;
    }
    wuerfel8.setSmooth(true);
    sf::Sprite wuerfelsprite8;
    //wuerfelsprite.setColor(sf::Color::Blue);
    wuerfelsprite8.setTexture(wuerfel8, true);
    wuerfelsprite8.setPosition({ 720.0f, 800.0f });
    wuerfelsprite8.setScale(0.15, 0.15);

    //Hauptschleife
    while (spiel.isOpen()) {
        spiel.clear(sf::Color(53, 104, 45, 255));

        //Main-Ansicht zeichnen
         //Las Vegas Logo
        sf::Texture logo;
        logo.loadFromFile("res/Bilder/Assets/vegas.gif");
        logo.setSmooth(true);
        sf::Sprite logosprite(logo);
        logosprite.setPosition({ 1300.0f, 10.0f });
        logosprite.setScale(0.5f, 0.5f);
        spiel.draw(logosprite);
        //
        // Trennlinie
        sf::ConvexShape trennlinie;
        trennlinie.setPointCount(4);
        trennlinie.setFillColor(sf::Color::Black);
        trennlinie.setPoint(0, sf::Vector2f(1250.0f, 0.0f));
        trennlinie.setPoint(1, sf::Vector2f(1250.0f, 900.0f));
        trennlinie.setPoint(2, sf::Vector2f(1251.0f, 900.0f));
        trennlinie.setPoint(3, sf::Vector2f(1251.0f, 0.0f));
        spiel.draw(trennlinie);

        //
        //Kasten f�r Casinos
        sf::ConvexShape goldennuggetkasten;
        goldennuggetkasten.setPointCount(4);
        goldennuggetkasten.setFillColor(sf::Color(251, 209, 100, 165));
        goldennuggetkasten.setOutlineThickness(3.0f);
        goldennuggetkasten.setOutlineColor(sf::Color(251, 209, 100, 50));
        goldennuggetkasten.setPoint(0, sf::Vector2f(18.0f, 20.0f));
        goldennuggetkasten.setPoint(1, sf::Vector2f(202.0f, 20.0f));
        goldennuggetkasten.setPoint(2, sf::Vector2f(202.0f, 750.0f));
        goldennuggetkasten.setPoint(3, sf::Vector2f(18.0f, 750.0f));
        spiel.draw(goldennuggetkasten);

        sf::ConvexShape caesarskasten;
        caesarskasten.setPointCount(4);
        caesarskasten.setFillColor(sf::Color(248, 131, 49, 140));
        caesarskasten.setOutlineThickness(3.0f);
        caesarskasten.setOutlineColor(sf::Color(248, 131, 49, 50));
        caesarskasten.setPoint(0, sf::Vector2f(218.0f, 20.0f));
        caesarskasten.setPoint(1, sf::Vector2f(402.0f, 20.0f));
        caesarskasten.setPoint(2, sf::Vector2f(402.0f, 750.0f));
        caesarskasten.setPoint(3, sf::Vector2f(218.0f, 750.0f));
        spiel.draw(caesarskasten);

        sf::ConvexShape miragekasten;
        miragekasten.setPointCount(4);
        miragekasten.setFillColor(sf::Color(180, 110, 198, 145));
        miragekasten.setOutlineThickness(3.0f);
        miragekasten.setOutlineColor(sf::Color(180, 110, 198, 50));
        miragekasten.setPoint(0, sf::Vector2f(418.0f, 20.0f));
        miragekasten.setPoint(1, sf::Vector2f(602.0f, 20.0f));
        miragekasten.setPoint(2, sf::Vector2f(602.0f, 750.0f));
        miragekasten.setPoint(3, sf::Vector2f(418.0f, 750.0f));
        spiel.draw(miragekasten);

        sf::ConvexShape saharakasten;
        saharakasten.setPointCount(4);
        saharakasten.setFillColor(sf::Color(212, 76, 48, 122));
        saharakasten.setOutlineThickness(3.0f);
        saharakasten.setOutlineColor(sf::Color(212, 76, 48, 50));
        saharakasten.setPoint(0, sf::Vector2f(618.0f, 20.0f));
        saharakasten.setPoint(1, sf::Vector2f(802.0f, 20.0f));
        saharakasten.setPoint(2, sf::Vector2f(802.0f, 750.0f));
        saharakasten.setPoint(3, sf::Vector2f(618.0f, 750.0f));
        spiel.draw(saharakasten);

        sf::ConvexShape luxorkasten;
        luxorkasten.setPointCount(4);
        luxorkasten.setFillColor(sf::Color(83, 135, 149, 109));
        luxorkasten.setOutlineThickness(3.0f);
        luxorkasten.setOutlineColor(sf::Color(83, 135, 149, 50));
        luxorkasten.setPoint(0, sf::Vector2f(818.0f, 20.0f));
        luxorkasten.setPoint(1, sf::Vector2f(1002.0f, 20.0f));
        luxorkasten.setPoint(2, sf::Vector2f(1002.0f, 750.0f));
        luxorkasten.setPoint(3, sf::Vector2f(818.0f, 750.0f));
        spiel.draw(luxorkasten);

        sf::ConvexShape circuskasten;
        circuskasten.setPointCount(4);
        circuskasten.setFillColor(sf::Color(121, 147, 200, 151));
        circuskasten.setOutlineThickness(3.0f);
        circuskasten.setOutlineColor(sf::Color(121, 147, 200, 50));
        circuskasten.setPoint(0, sf::Vector2f(1018.0f, 20.0f));
        circuskasten.setPoint(1, sf::Vector2f(1202.0f, 20.0f));
        circuskasten.setPoint(2, sf::Vector2f(1202.0f, 750.0f));
        circuskasten.setPoint(3, sf::Vector2f(1018.0f, 750.0f));
        spiel.draw(circuskasten);

        //
        //Einzelne Casinos
        sf::Texture golden_nugget;
        golden_nugget.loadFromFile("res/Bilder/Assets/golden_nugget.png");
        golden_nugget.setSmooth(true);
        sf::Sprite golden_nuggetsprite(golden_nugget);
        golden_nuggetsprite.setPosition({ 20.0f, 350.0f });
        //golden_nuggetsprite.setScale(0.75, 0.75);
        spiel.draw(golden_nuggetsprite);

        sf::Texture caesars;
        caesars.loadFromFile("res/Bilder/Assets/caesars.png");
        caesars.setSmooth(true);
        sf::Sprite caesarssprite(caesars);
        caesarssprite.setPosition({ 220.0f, 350.0f });
        //caesarssprite.setScale(0.75, 0.75);
        spiel.draw(caesarssprite);

        sf::Texture mirage;
        mirage.loadFromFile("res/Bilder/Assets/mirage.png");
        mirage.setSmooth(true);
        sf::Sprite miragesprite(mirage);
        miragesprite.setPosition({ 420.0f, 350.0f });
        spiel.draw(miragesprite);

        sf::Texture sahara;
        sahara.loadFromFile("res/Bilder/Assets/sahara.png");
        sahara.setSmooth(true);
        sf::Sprite saharasprite(sahara);
        saharasprite.setPosition({ 620.0f, 350.0f });
        spiel.draw(saharasprite);

        sf::Texture luxor;
        luxor.loadFromFile("res/Bilder/Assets/luxor.png");
        luxor.setSmooth(true);
        sf::Sprite luxorsprite(luxor);
        luxorsprite.setPosition({ 820.0f, 350.0f });
        spiel.draw(luxorsprite);

        sf::Texture circus;
        circus.loadFromFile("res/Bilder/Assets/circus.png");
        circus.setSmooth(true);
        sf::Sprite circussprite(circus);
        circussprite.setPosition({ 1020.0f, 350.0f });
        spiel.draw(circussprite);

        //Men� unten
        sf::ConvexShape menuunten;
        menuunten.setPointCount(4);
        //Linksoben
        menuunten.setPoint(0, sf::Vector2f(0.0f, 935.0f));
        //Rechtsoben
        menuunten.setPoint(1, sf::Vector2f(1600.0f, 935.0f));
        //Rechtsunten
        menuunten.setPoint(2, sf::Vector2f(1600.0f, 1000.0f));
        //Linksunten
        menuunten.setPoint(3, sf::Vector2f(0.0f, 1000.0f));
        spiel.draw(menuunten);

        //Schrifttab �ber Men�
        sf::ConvexShape schrifttab;
        schrifttab.setPointCount(4);
        //Linksoben
        schrifttab.setPoint(0, sf::Vector2f(0.0f, 900.0f));
        //Rechtsoben
        schrifttab.setPoint(1, sf::Vector2f(1600.0f, 900.0f));
        //Rechtsunten
        schrifttab.setPoint(2, sf::Vector2f(1600.0f, 935.0f));
        //Linksunten
        schrifttab.setPoint(3, sf::Vector2f(0.0f, 935.0f));
        schrifttab.setFillColor(sf::Color(53, 53, 53, 150));
        spiel.draw(schrifttab);

        spiel.draw(wuerfelsprite);
        spiel.draw(wuerfelsprite2);
        spiel.draw(wuerfelsprite3);
        spiel.draw(wuerfelsprite4);
        spiel.draw(wuerfelsprite5);
        spiel.draw(wuerfelsprite6);
        spiel.draw(wuerfelsprite7);
        spiel.draw(wuerfelsprite8);


        //Event-Handling
        sf::Event event;
        while (spiel.pollEvent(event)) {
            //Wenn Fenster geschlossen wird
            if (event.type == sf::Event::Closed) {
                spiel.close();
            }
            //Spiel zeichnen
            spiel.display();
        }
    }
}

//
//Main-Funktion - Hier befinden sich alle Hauptfunktionen wie Erzeugung des Bildschirms etc.
//
int main(){
    //Konsolenausgabe
    printf("Las Vegas - Das Spiel\nDiese Konsole wird als Fehlerausgabe benutzt!\nSprich, falls etwas falsch laufen sollte,\nwird es hier ausgegeben!\n\n");

    //Erzeuge das Fenster-Objekt
    sf::RenderWindow window(sf::VideoMode(1220, 800), "Las Vegas");

    // Icon f�r das Spiel setzen
    sf::Image icon;
    if (!icon.loadFromFile("res/Bilder/icon.jpg")) {
        fehleranzeige("Icon-Fehler", "Icon konnte nicht geladen werden.");
        return 0;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //Erstelle eine Struktur f�r Daten
    struct einstellung {
        int stellung;
    };
    
    //Pr�fe, ob es Einstellungdatei existiert und erstelle sie, falls nicht
    ifstream leser("einstellungen.einstellungen", std::ios::out | std::ios::binary);
    if (!leser) {
        printf("Einstellungsdatei wird erstellt! \n");
        startup();
    }
    
    //Pr�fe, ob Einstellung auf An ist
    einstellung items[1];
    //Daten lesen
    for (int i = 0; i < 2; i++) {
        leser.read((char*)&items[i], sizeof(einstellung));
    }
    //Erster Wert im Array als Variable speichern
    int e = items[0].stellung;

    //Erzeuge das Audio-Objekt und fange an zu spielen!
    //AUSLASSEN BITTE KEIN BOCK MEHR DASS ES SPACKT
    /*sf::Music music;
    music.openFromFile("res/Audio/titelmusik.wav");
    music.setVolume(50);
    music.setLoop(true);
    //Musik spielen, falls Einstellung erlaubt
    if (e == 1) {
        music.play();
    }*/
    
    //Lade die Schriftart
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        fehleranzeige("Font-Fehler", "Schriftart konnte nicht geladen werden.");
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
        
        // Switch, um zu pr�fen, welches Event passiert ist und was getan werden soll
        switch (event.type) 
        {
            //Wenn es geklickt hat
            case sf::Event::MouseButtonPressed:
                if (playbtn1.isMouseOver(window)) {
                    //Wenn Spielen-Knopf gedr�ckt wird
                    //debug printf("Knopf wurde gedrueckt!");
                    //test fehleranzeige("Kritischer Fehler", "Fehler! Bitte starten Sie das Spiel neu!");
                    window.setActive(false);
                    neuesSpiel();
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
                    /*if (event.key.code == sf::Keyboard::End) {
                        printf("Escape-Taste wird gedr�ckt! \n");
                        window.close();
                    
                    */
                }

                break;
        }

        // setze den Bildschirm zur�ck
        window.clear(sf::Color::White);
        
        // Erzeuge einen Text
        sf::Text text("Willkommen zu Las Vegas! Bitte w�hlen Sie eine der folgenden Optionen.", font);
        text.setCharacterSize(30);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setPosition(230.0f, 430.0f);

        //Versiontext
        sf::Text vertext("Version: 0.3.6 Alpha", font);
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