//Autor: Kerem Okumus, Kiriakos Avramidis, Moritz Biedenbacher
//Las Vegas Spiel als grafische Anwendung
//Dieses Programm nutzt die SFML-Bibliothek, dass zur Programmierung von grafischen Benutzeroberflächen erleichtern soll!
//geändert am: 25.01.2021(Datei erstellt und ersten Code geschrieben); 26.01.2021 (Code verändert und Menü hinzugefügt)

//Einbindung wichtiger Bibliotheken
#include<SFML/Graphics.hpp>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Audio.hpp>
#include<SFML/System.hpp>

#include<iostream>
#include<Windows.h>
#include<fstream>
#include<cstdlib>
#include<conio.h>
#include<random>
#include<numeric>
#include<filesystem>
 
using namespace std;
using namespace sf;

//Selbstgeschriebene Header-Dateien
#include "Button.h"
#include "Dice.h"

//Wichtige Variablen
float wuerfelpos[8] = { 20.0f, 120.0f, 220.0f, 320.0f, 420.0f, 520.0f, 620.0f, 720.0f };
string wuerfelaugen[6] = { "./res/Bilder/würfel/dice-png-1.png", "./res/Bilder/würfel/dice-png-2.png", "./res/Bilder/würfel/dice-png-3.png", "./res/Bilder/würfel/dice-png-4.png", "./res/Bilder/würfel/dice-png-5.png", "./res/Bilder/würfel/dice-png-6.png" };
int geldwerte[6] = { 10000, 20000, 30000, 50000, 70000, 90000 };
int casinogelder1[5];
int casinogelder2[5];
int casinogelder3[5];
int casinogelder4[5];
int casinogelder5[5];
int casinogelder6[5];
int casinowuerfelanzahl1[3];
int casinowuerfelanzahl2[3];
int casinowuerfelanzahl3[3];
int casinowuerfelanzahl4[3];
int casinowuerfelanzahl5[3];
int casinowuerfelanzahl6[3];
int summe1 = 0;
int summe2 = 0;
int summe3 = 0;
int summe4 = 0;
int summe5 = 0;
int summe6 = 0;
int wuerfelanzahl[3] = { 8, 8, 8 };
int wuerfelwert[8];
int spielernummer;
int ausgewählteAuge = 0;
Color farben[3] = { Color::Cyan, Color::Green, Color::Yellow };
string subtextstring;
bool aufWuerfelGedrueckt = false;
bool spielBeendet = false;
bool speichertbool = false;
bool zugbeendet = false;
string vergangenezeitstring;

//Dateiformat Speicher

struct spieldaten {
    int wuerfelwerte[8];
    int kontostand;
    int casino1, casino2, casino3, casino4, casino5, casino6;
    int wuerfelanzahl[3];
};

//
//Eine Fehlermethode, der eine Fehlermeldung auf dem Bildschirm erscheinen lässt
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
            // Schließt das Fenster, falls schließen gedrückt wird

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
    * BITTE DIESE LISTE BEI ÄNDERUNGEN ERGÄNZEN!!!
    */

    //Erstelle eine Struktur für Daten
    struct einstellung {
        int stellung;
    };

    //Schreibe eine Einstellungsdatei in Binär
    ofstream schreiber("einstellungen.einstellungen", std::ios::out | std::ios::binary);

    //Nenne alles, was geschrieben werden muss
    //Siehe oben, um zu verstehen, für was was steht!
    int writer[3];
    writer[0] = 1;
    writer[1] = 50;

    //schreiber.write((char*)&writer[0].stellung, sizeof(einstellung));

    //Schreibe alle Daten ein
    printf("Schreiben wird gestartet!\n");
    for (int i = 0; i < 2; i++) {
        schreiber.write((char*) &writer[i], sizeof(einstellung));
    }

    //Datei schließen
    schreiber.close();

    //Prüfe, ob alles erfolgreich war!
    if (!schreiber.good()) {
        fehleranzeige("Datei-Fehler", "Einstellungsdatei konnte nicht geöffnet werden.");
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

    //Erstelle eine Struktur für Daten
    struct einstellung {
        int stellung;
    };

    //Einstellungendatei im Lesen-Modus öffnen
    ifstream leser("einstellungen.einstellungen", std::ios::out | std::ios::binary);
    if (!leser) {
        fehleranzeige("Datei-Fehler", "Einstellungsdatei konnte nicht geöffnet werden.");
        return;
    }

    //Array für gelesene Daten erstellen
    int items[3];
    //einstellung items[1];

    //Daten von Datei einlesen
    for(int i = 0; i < 2; i++) {
        leser.read((char*) &items[i], sizeof(einstellung));
        //printf("%p", &items[i]);
    }

    //Datei schließen
    leser.close();

    //Falls Fehler, anzeigen
    if (!leser.good()) {
        fehleranzeige("Datei-Fehler", "Einstellungsdatei konnte nicht gelesen werden.");
        return;
    }
    
    //Variablen für Button
    string text;
    sf::Color farbe;
    int p = items[0];

    //Variablen für Lautstärke
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
        //Zeichne Menü
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
        //Hier alles, was man mit dem Programm interagieren kann (Maus über Dinge, Klick und co.)) schreiben.
        sf::Event event;
        while (settings.pollEvent(event))
        {
            // Schließt das Fenster, falls schließen gedrückt wird
            if (event.type == sf::Event::Closed)
                settings.close();

            switch (event.type) {
                //Wenn es geklickt hat
                case sf::Event::MouseButtonPressed:
                    //Wenn es über dem Anwenden-Button geklickt hat
                    if (applybtn1.isMouseOver(settings)) {
                        //Übernehm die Einstellungen
                        ofstream schreiber("einstellungen.einstellungen", std::ios::out | std::ios::trunc);
                        //Array für Daten erstellen und beschriften
                        int items[2];
                        //einstellung items[1];
                        //p = Ob Sound laufen soll
                        //v = Lautstärke
                        items[0]= p;
                        items[1]= v;

                        //Indizen in die Datei schreiben
                        for (int i = 0; i < 2; i++) {
                            schreiber.write((char*)&items[i], sizeof(einstellung));
                        }

                        //Datei schließen
                        schreiber.close();
                        printf("Einstellungen wurden uebernommen! \n");
                        //Einstellungen schließen
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

        //Alles auf die Oberfläche zeichnen
        settings.clear(sf::Color(64, 224, 208, 255));
        applybtn1.drawTo(settings);
        settings.draw(convexShape);
        soundbutton.drawTo(settings);
        settings.draw(soundtext);
        
        //Einstellungen anzeigen
        settings.display();
    }
}


//
//MAIN SPIEL (TEST)
//
//Eine Function fürs neue Spiel erstellen (nicht vollständig)
void neuesSpieltest() {
    sf::RenderWindow spiel(sf::VideoMode(1600, 1000), "Las Vegas");

    bool gewonnen = false;
    //Schriftart laden
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        fehleranzeige("Font-Fehler", "Schriftart konnte nicht geladen werden!");
    }

    // Icon für das Spiel setzen
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
        //Kasten für Casinos
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

        //Menü unten
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

        //Schrifttab über Menü
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
        wuerfel.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        wuerfel.setSmooth(true);
        sf::Sprite wuerfelsprite;
        //wuerfelsprite.setColor(sf::Color::Blue);
        wuerfelsprite.setTexture(wuerfel, true);
        wuerfelsprite.setPosition({ 20.0f, 800.0f });
        wuerfelsprite.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite);

        sf::Texture wuerfel2;
        wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        wuerfel2.setSmooth(true);
        sf::Sprite wuerfelsprite2;
        //wuerfelsprite2.setColor(sf::Color::Blue);
        wuerfelsprite2.setTexture(wuerfel2, true);
        wuerfelsprite2.setPosition({ 120.0f, 800.0f });
        wuerfelsprite2.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite2);

        sf::Texture wuerfel3;
        wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        wuerfel3.setSmooth(true);
        sf::Sprite wuerfelsprite3;
        //wuerfelsprite3.setColor(sf::Color::Blue);
        wuerfelsprite3.setTexture(wuerfel3, true);
        wuerfelsprite3.setPosition({ 220.0f, 800.0f });
        wuerfelsprite3.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite3);

        sf::Texture wuerfel4;
        wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        wuerfel4.setSmooth(true);
        sf::Sprite wuerfelsprite4;
        //wuerfelsprite4.setColor(sf::Color::Blue);
        wuerfelsprite4.setTexture(wuerfel4, true);
        wuerfelsprite4.setPosition({ 320.0f, 800.0f });
        wuerfelsprite4.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite4);

        sf::Texture wuerfel5;
        wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        wuerfel5.setSmooth(true);
        sf::Sprite wuerfelsprite5;
        //wuerfelsprite5.setColor(sf::Color::Blue);
        wuerfelsprite5.setTexture(wuerfel5, true);
        wuerfelsprite5.setPosition({ 420.0f, 800.0f });
        wuerfelsprite5.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite5);

        sf::Texture wuerfel6;
        wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        wuerfel6.setSmooth(true);
        sf::Sprite wuerfelsprite6;
        //wuerfelsprite6.setColor(sf::Color::Blue);
        wuerfelsprite6.setTexture(wuerfel6, true);
        wuerfelsprite6.setPosition({ 520.0f, 800.0f });
        wuerfelsprite6.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite6);

        sf::Texture wuerfel7;
        wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        wuerfel7.setSmooth(true);
        sf::Sprite wuerfelsprite7;
        //wuerfelsprite7.setColor(sf::Color::Blue);
        wuerfelsprite7.setTexture(wuerfel7, true);
        wuerfelsprite7.setPosition({ 620.0f, 800.0f });
        wuerfelsprite7.setScale(0.15, 0.15);
        spiel.draw(wuerfelsprite7);

        sf::Texture wuerfel8;
        wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-2.png");
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


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void neuesSpieltest2() {
    sf::RenderWindow spiel(sf::VideoMode(1600, 1000), "Las Vegas");

    //Schriftart laden
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        fehleranzeige("Font-Fehler", "Schriftart konnte nicht geladen werden!");
    }

    // Icon für das Spiel setzen
    sf::Image icon;
    if (!icon.loadFromFile("res/Bilder/icon.jpg")) {
        //fehleranzeige("Icon-Fehler", "Icon konnte nicht geladen werden.");
        spiel.close();
        return;
    }
    spiel.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //Würfel würfeln für jeden Spieler
    for (int i = 0; i <= 7; i++) {
        wuerfelwert[i] = gen();
        cout << wuerfelwert[i];
    }
    printf("\n");
    
    
    for (int i = 0; i <= 6; i++)
    {
        for (int j = i + 1; j <= 7; j++)
        {
            if (wuerfelwert[i] > wuerfelwert[j])
            {
                //-----Tausch-----
                int h = wuerfelwert[i];
                wuerfelwert[i] = wuerfelwert[j];
                wuerfelwert[j] = h;
            }
        }
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
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-1.png");
            break;
        case 2:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-2.png");
            break;
        case 3:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-3.png");
            break;
        case 4:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-4.png");
            break;
        case 5:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-5.png");
            break;
        case 6:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
        wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        break;
    case 2:
        wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        break;
    case 3:
        wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        break;
    case 4:
        wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        break;
    case 5:
        wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        break;
    case 6:
        wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
        wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        break;
    case 2:
        wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        break;
    case 3:
        wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        break;
    case 4:
        wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        break;
    case 5:
        wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        break;
    case 6:
        wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
        wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        break;
    case 2:
        wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        break;
    case 3:
        wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        break;
    case 4:
        wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        break;
    case 5:
        wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        break;
    case 6:
        wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
        wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        break;
    case 2:
        wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        break;
    case 3:
        wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        break;
    case 4:
        wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        break;
    case 5:
        wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        break;
    case 6:
        wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
        wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        break;
    case 2:
        wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        break;
    case 3:
        wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        break;
    case 4:
        wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        break;
    case 5:
        wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        break;
    case 6:
        wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
        wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        break;
    case 2:
        wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        break;
    case 3:
        wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        break;
    case 4:
        wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        break;
    case 5:
        wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        break;
    case 6:
        wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
        wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        break;
    case 2:
        wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        break;
    case 3:
        wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        break;
    case 4:
        wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        break;
    case 5:
        wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        break;
    case 6:
        wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
        //Kasten für Casinos
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

        //Menü unten
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

        //Schrifttab über Menü
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

        //Eigentliches Spiel
        //Speicherdatei schreiben
        ofstream schreiber("daten.save", ios::out || ios::binary);
        int i = 0;
        wuerfelsprite.setColor(farben[i]);
        wuerfelsprite2.setColor(farben[i]);
        wuerfelsprite3.setColor(farben[i]);
        wuerfelsprite4.setColor(farben[i]);
        wuerfelsprite5.setColor(farben[i]);
        wuerfelsprite6.setColor(farben[i]);
        wuerfelsprite7.setColor(farben[i]);
        wuerfelsprite8.setColor(farben[i]);
        spiel.draw(wuerfelsprite);
        spiel.draw(wuerfelsprite2);
        spiel.draw(wuerfelsprite3);
        spiel.draw(wuerfelsprite4);
        spiel.draw(wuerfelsprite5);
        spiel.draw(wuerfelsprite6);
        spiel.draw(wuerfelsprite7);
        spiel.draw(wuerfelsprite8);

        printf("Test Hauptschleife \n");
        while (spielBeendet == false) {
            printf("Test diese Schleife\n");
        }

        //Event-Handling
        sf::Event event;
        while (spiel.pollEvent(event)) {
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                // transform the mouse position from window coordinates to world coordinates
                sf::Vector2f mouse = spiel.mapPixelToCoords(sf::Mouse::getPosition(spiel));

                // retrieve the bounding box of the sprite
                sf::FloatRect wuerfelcoords1 = wuerfelsprite.getGlobalBounds();
                sf::FloatRect wuerfelcoords2 = wuerfelsprite2.getGlobalBounds();
                sf::FloatRect wuerfelcoords3 = wuerfelsprite3.getGlobalBounds();
                sf::FloatRect wuerfelcoords4 = wuerfelsprite4.getGlobalBounds();
                sf::FloatRect wuerfelcoords5 = wuerfelsprite5.getGlobalBounds();
                sf::FloatRect wuerfelcoords6 = wuerfelsprite6.getGlobalBounds();
                sf::FloatRect wuerfelcoords7 = wuerfelsprite7.getGlobalBounds();
                sf::FloatRect wuerfelcoords8 = wuerfelsprite8.getGlobalBounds();

                // hit test
                if (wuerfelcoords1.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[0];
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5: 
                        break;
                    case 6:
                        break;
                    }
                }
                if (wuerfelcoords2.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[1];
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords3.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[2];
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords4.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[3];
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords5.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[4];
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords6.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[5];
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords7.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[6];
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords8.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[7];
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
            }
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
// AKTUELLER CODE
//

//Eine Funktion für das Pausenmenü
void Pausenmenu() {
    sf::RenderWindow pause(sf::VideoMode(500, 500), "Pause", sf::Style::None);

    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        fehleranzeige("Font-Fehler", "Font konnte nicht geladen werden!");
    }

    sf::Event event;
    while (pause.isOpen())
    {
        pause.clear(sf::Color::White);

        sf::Text titeltext("Pausemenü", font);
        titeltext.setCharacterSize(40);
        titeltext.setFillColor(sf::Color::Black);
        titeltext.setPosition(20.0f, 20.0f);

        Button fortsetzenbutton("Fortsetzen", { 200, 100 }, 30, sf::Color::Magenta, sf::Color::Black);
        fortsetzenbutton.setFont(font);
        fortsetzenbutton.setPosition(sf::Vector2f(20.0f, 100.0f));
        fortsetzenbutton.drawTo(pause);

        Button neuesspielbutton("Neues Spiel", { 200, 100 }, 30, sf::Color(0, 255, 0, 255), sf::Color::Black);
        neuesspielbutton.setFont(font);
        neuesspielbutton.setPosition({ 270.0f, 100.0f });
        neuesspielbutton.drawTo(pause);

        Button spielladenbutton("Spiel laden", { 200, 100 }, 30, sf::Color(0, 162, 232, 255), sf::Color::Black);
        spielladenbutton.setFont(font);
        spielladenbutton.setPosition({ 20.0f, 350.0f });
        spielladenbutton.drawTo(pause);

        Button beendenbtn("Beenden", { 200, 100 }, 30, sf::Color(255, 0, 0, 255), sf::Color::Black);
        beendenbtn.setFont(font);
        beendenbtn.setPosition({ 270.0f, 350.0f });
        beendenbtn.drawTo(pause);

        pause.draw(titeltext);
        pause.display();
        while (pause.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    pause.close();
                }
            }
        }
    }
}

void Spielzeichnung() {
    sf::RenderWindow spiel(sf::VideoMode(1600, 1000), "Las Vegas", sf::Style::Close);

    //Schriftart laden
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        fehleranzeige("Font-Fehler", "Schriftart konnte nicht geladen werden!");
    }

    // Icon für das Spiel setzen
    sf::Image icon;
    if (!icon.loadFromFile("res/Bilder/icon.jpg")) {
        //fehleranzeige("Icon-Fehler", "Icon konnte nicht geladen werden.");
        spiel.close();
        return;
    }
    spiel.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    //while-schleife zeichnen
    while (spiel.isOpen()) {
        spiel.clear(sf::Color(53, 104, 45, 255));
        //
        //Würfel zeichnen
        //
        sf::Texture wuerfel;
        switch (wuerfelwert[0]) {
        case 1:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-1.png");
            break;
        case 2:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-2.png");
            break;
        case 3:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-3.png");
            break;
        case 4:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-4.png");
            break;
        case 5:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-5.png");
            break;
        case 6:
            wuerfel.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
            wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-1.png");
            break;
        case 2:
            wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-2.png");
            break;
        case 3:
            wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-3.png");
            break;
        case 4:
            wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-4.png");
            break;
        case 5:
            wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-5.png");
            break;
        case 6:
            wuerfel2.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
            wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-1.png");
            break;
        case 2:
            wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-2.png");
            break;
        case 3:
            wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-3.png");
            break;
        case 4:
            wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-4.png");
            break;
        case 5:
            wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-5.png");
            break;
        case 6:
            wuerfel3.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
            wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-1.png");
            break;
        case 2:
            wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-2.png");
            break;
        case 3:
            wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-3.png");
            break;
        case 4:
            wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-4.png");
            break;
        case 5:
            wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-5.png");
            break;
        case 6:
            wuerfel4.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
            wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-1.png");
            break;
        case 2:
            wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-2.png");
            break;
        case 3:
            wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-3.png");
            break;
        case 4:
            wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-4.png");
            break;
        case 5:
            wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-5.png");
            break;
        case 6:
            wuerfel5.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
            wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-1.png");
            break;
        case 2:
            wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-2.png");
            break;
        case 3:
            wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-3.png");
            break;
        case 4:
            wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-4.png");
            break;
        case 5:
            wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-5.png");
            break;
        case 6:
            wuerfel6.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
            wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-1.png");
            break;
        case 2:
            wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-2.png");
            break;
        case 3:
            wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-3.png");
            break;
        case 4:
            wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-4.png");
            break;
        case 5:
            wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-5.png");
            break;
        case 6:
            wuerfel7.loadFromFile("res/Bilder/würfel/dice-png-6.png");
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
            wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-1.png");
            break;
        case 2:
            wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-2.png");
            break;
        case 3:
            wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-3.png");
            break;
        case 4:
            wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-4.png");
            break;
        case 5:
            wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-5.png");
            break;
        case 6:
            wuerfel8.loadFromFile("res/Bilder/würfel/dice-png-6.png");
            break;
        }
        wuerfel8.setSmooth(true);
        sf::Sprite wuerfelsprite8;
        //wuerfelsprite.setColor(sf::Color::Blue);
        wuerfelsprite8.setTexture(wuerfel8, true);
        wuerfelsprite8.setPosition({ 720.0f, 800.0f });
        wuerfelsprite8.setScale(0.15, 0.15);

        //
        //Main-Ansicht/Hintergrund zeichnen
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
        //Kasten für Casinos
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

        //Menü unten
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
        menuunten.setFillColor(sf::Color(195, 195, 195, 184));
        spiel.draw(menuunten);

        //Schrifttab über Menü
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

        //Eigentliche Einstellungen
        sf::Text subtext(subtextstring, font);
        subtext.setCharacterSize(25);
        subtext.setPosition(sf::Vector2f(600.0f, 900.0f));
        subtext.setFillColor(sf::Color::Black);
        spiel.draw(subtext);

        //Buttonfarbe (195, 195, 195, 184)

        //Buttonflächen
        sf::RectangleShape undobackground;
        undobackground.setSize({ 170, 50 });
        undobackground.setFillColor(sf::Color::Yellow);
        undobackground.setPosition({ 15.0f, 942.5f });
        spiel.draw(undobackground);

        sf::RectangleShape donebackground;
        donebackground.setSize({ 170, 50 });
        donebackground.setFillColor(sf::Color::Yellow);
        donebackground.setPosition({ 1410.0f, 942.5f });
        spiel.draw(donebackground);

        //Schritt zurücksetzen Button
        Button undobutton("Einen Schritt zurück", { 170, 50 }, 20, sf::Color::Yellow, sf::Color::Black);
        undobutton.setFont(font);
        undobutton.setPosition({15.0f, 942.5f});
        if (aufWuerfelGedrueckt == true) {
            undobutton.drawTo(spiel);
        }
        
        //Zug Beenden Button
        Button donebutton("Zug beenden", { 170, 50 }, 20, sf::Color::Yellow, sf::Color::Black);
        donebutton.setFont(font);
        donebutton.setPosition({ 1410.0f, 942.5f });
        if (aufWuerfelGedrueckt == true) {
            donebutton.drawTo(spiel);
        }

        //
        //Geldscheine
        sf::Texture c1gs1;
        int geldscheinc1gs1 = casinogelder1[0];
        switch (geldscheinc1gs1) {
            case 10000:
                c1gs1.loadFromFile("res/Bilder/Assets/10000.png");
                break;
            case 20000:
                c1gs1.loadFromFile("res/Bilder/Assets/20000.png");
                break;
            case 30000:
                c1gs1.loadFromFile("res/Bilder/Assets/30000.png");
                break;
            case 50000:
                c1gs1.loadFromFile("res/Bilder/Assets/50000.png");
                break;
            case 70000:
                c1gs1.loadFromFile("res/Bilder/Assets/70000.png");
                break;
            case 90000:
                c1gs1.loadFromFile("res/Bilder/Assets/90000.png");
                break;
        }
        c1gs1.setSmooth(true);
        sf::Sprite c1gs1sprite(c1gs1);
        c1gs1sprite.setPosition(sf::Vector2f( 25.0f, 25.0f ));
        c1gs1sprite.setScale(1.5f, 1.5f);
        spiel.draw(c1gs1sprite);

        sf::Texture c1gs2;
        switch (casinogelder1[1]) {
        case 10000:
            c1gs2.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c1gs2.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c1gs2.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c1gs2.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c1gs2.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c1gs2.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c1gs2.setSmooth(true);
        sf::Sprite c1gs2sprite(c1gs2);
        c1gs2sprite.setPosition(sf::Vector2f(25.0f, 55.0f));
        c1gs2sprite.setScale(1.5f, 1.5f);
        spiel.draw(c1gs2sprite);

        sf::Texture c1gs3;
        switch (casinogelder1[2]) {
        case 10000:
            c1gs3.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c1gs3.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c1gs3.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c1gs3.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c1gs3.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c1gs3.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c1gs3.setSmooth(true);
        sf::Sprite c1gs3sprite(c1gs3);
        c1gs3sprite.setPosition(sf::Vector2f(25.0f, 85.0f));
        c1gs3sprite.setScale(1.5f, 1.5f);
        spiel.draw(c1gs3sprite);

        sf::Texture c1gs4;
        switch (casinogelder1[3]) {
        case 10000:
            c1gs4.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c1gs4.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c1gs4.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c1gs4.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c1gs4.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c1gs4.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c1gs4.setSmooth(true);
        sf::Sprite c1gs4sprite(c1gs4);
        c1gs4sprite.setPosition(sf::Vector2f(25.0f, 115.0f));
        c1gs4sprite.setScale(1.5f, 1.5f);
        spiel.draw(c1gs4sprite);

        sf::Texture c1gs5;
        switch (casinogelder1[4]) {
        case 10000:
            c1gs5.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c1gs5.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c1gs5.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c1gs5.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c1gs5.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c1gs5.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c1gs5.setSmooth(true);
        sf::Sprite c1gs5sprite(c1gs5);
        c1gs5sprite.setPosition(sf::Vector2f(25.0f, 145.0f));
        c1gs5sprite.setScale(1.5f, 1.5f);
        spiel.draw(c1gs5sprite);

        sf::Texture c2gs1;
        switch (casinogelder2[0]) {
        case 10000:
            c2gs1.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c2gs1.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c2gs1.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c2gs1.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c2gs1.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c2gs1.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c2gs1.setSmooth(true);
        sf::Sprite c2gs1sprite(c2gs1);
        c2gs1sprite.setPosition(sf::Vector2f(225.0f, 25.0f));
        c2gs1sprite.setScale(1.5f, 1.5f);
        spiel.draw(c2gs1sprite);

        sf::Texture c2gs2;
        switch (casinogelder2[1]) {
        case 10000:
            c2gs2.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c2gs2.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c2gs2.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c2gs2.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c2gs2.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c2gs2.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c2gs2.setSmooth(true);
        sf::Sprite c2gs2sprite(c2gs2);
        c2gs2sprite.setPosition(sf::Vector2f(225.0f, 55.0f));
        c2gs2sprite.setScale(1.5f, 1.5f);
        spiel.draw(c2gs2sprite);

        sf::Texture c2gs3;
        switch (casinogelder2[2]) {
        case 10000:
            c2gs3.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c2gs3.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c2gs3.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c2gs3.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c2gs3.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c2gs3.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c2gs3.setSmooth(true);
        sf::Sprite c2gs3sprite(c2gs3);
        c2gs3sprite.setPosition(sf::Vector2f(225.0f, 85.0f));
        c2gs3sprite.setScale(1.5f, 1.5f);
        spiel.draw(c2gs3sprite);

        sf::Texture c2gs4;
        switch (casinogelder2[3]) {
        case 10000:
            c2gs4.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c2gs4.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c2gs4.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c2gs4.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c2gs4.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c2gs4.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c2gs4.setSmooth(true);
        sf::Sprite c2gs4sprite(c2gs4);
        c2gs1sprite.setPosition(sf::Vector2f(225.0f, 115.0f));
        c2gs4sprite.setScale(1.5f, 1.5f);
        spiel.draw(c2gs4sprite);

        sf::Texture c2gs5;
        switch (casinogelder2[4]) {
        case 10000:
            c2gs5.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c2gs5.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c2gs5.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c2gs5.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c2gs5.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c2gs5.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c2gs5.setSmooth(true);
        sf::Sprite c2gs5sprite(c2gs5);
        c2gs5sprite.setPosition(sf::Vector2f(225.0f, 145.0f));
        c2gs5sprite.setScale(1.5f, 1.5f);
        spiel.draw(c2gs5sprite);

        sf::Texture c3gs1;
        switch (casinogelder3[0]) {
        case 10000:
            c3gs1.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c3gs1.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c3gs1.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c3gs1.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c3gs1.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c3gs1.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c3gs1.setSmooth(true);
        sf::Sprite c3gs1sprite(c3gs1);
        c3gs1sprite.setPosition(sf::Vector2f(425.0f, 25.0f));
        c3gs1sprite.setScale(1.5f, 1.5f);
        spiel.draw(c3gs1sprite);

        sf::Texture c3gs2;
        switch (casinogelder3[1]) {
        case 10000:
            c3gs2.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c3gs2.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c3gs2.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c3gs2.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c3gs2.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c3gs2.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c3gs2.setSmooth(true);
        sf::Sprite c3gs2sprite(c3gs2);
        c3gs2sprite.setPosition(sf::Vector2f(425.0f, 55.0f));
        c3gs2sprite.setScale(1.5f, 1.5f);
        spiel.draw(c3gs2sprite);

        sf::Texture c3gs3;
        switch (casinogelder3[2]) {
        case 10000:
            c3gs3.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c3gs3.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c3gs3.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c3gs3.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c3gs3.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c3gs3.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c3gs3.setSmooth(true);
        sf::Sprite c3gs3sprite(c3gs3);
        c3gs3sprite.setPosition(sf::Vector2f(425.0f, 85.0f));
        c3gs3sprite.setScale(1.5f, 1.5f);
        spiel.draw(c3gs3sprite);

        sf::Texture c3gs4;
        switch (casinogelder3[3]) {
        case 10000:
            c3gs4.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c3gs4.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c3gs4.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c3gs4.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c3gs4.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c3gs4.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c3gs4.setSmooth(true);
        sf::Sprite c3gs4sprite(c3gs4);
        c3gs4sprite.setPosition(sf::Vector2f(425.0f, 115.0f));
        c3gs4sprite.setScale(1.5f, 1.5f);
        spiel.draw(c3gs4sprite);

        sf::Texture c3gs5;
        switch (casinogelder3[4]) {
        case 10000:
            c3gs5.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c3gs5.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c3gs5.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c3gs5.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c3gs5.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c3gs5.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c3gs5.setSmooth(true);
        sf::Sprite c3gs5sprite(c3gs5);
        c3gs5sprite.setPosition(sf::Vector2f(425.0f, 145.0f));
        c3gs5sprite.setScale(1.5f, 1.5f);
        spiel.draw(c3gs5sprite);

        sf::Texture c4gs1;
        switch (casinogelder4[0]) {
        case 10000:
            c4gs1.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c4gs1.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c4gs1.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c4gs1.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c4gs1.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c4gs1.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c4gs1.setSmooth(true);
        sf::Sprite c4gs1sprite(c4gs1);
        c4gs1sprite.setPosition(sf::Vector2f(625.0f, 25.0f));
        c4gs1sprite.setScale(1.5f, 1.5f);
        spiel.draw(c4gs1sprite);

        sf::Texture c4gs2;
        switch (casinogelder4[1]) {
        case 10000:
            c4gs2.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c4gs2.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c4gs2.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c4gs2.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c4gs2.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c4gs2.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c4gs2.setSmooth(true);
        sf::Sprite c4gs2sprite(c4gs2);
        c4gs2sprite.setPosition(sf::Vector2f(625.0f, 55.0f));
        c4gs2sprite.setScale(1.5f, 1.5f);
        spiel.draw(c4gs2sprite);

        sf::Texture c4gs3;
        switch (casinogelder4[2]) {
        case 10000:
            c4gs3.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c4gs3.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c4gs3.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c4gs3.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c4gs3.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c4gs3.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c4gs3.setSmooth(true);
        sf::Sprite c4gs3sprite(c4gs3);
        c4gs3sprite.setPosition(sf::Vector2f(625.0f, 85.0f));
        c4gs3sprite.setScale(1.5f, 1.5f);
        spiel.draw(c4gs3sprite);

        sf::Texture c4gs4;
        switch (casinogelder4[3]) {
        case 10000:
            c4gs4.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c4gs4.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c4gs4.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c4gs4.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c4gs4.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c4gs4.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c4gs4.setSmooth(true);
        sf::Sprite c4gs4sprite(c4gs4);
        c4gs4sprite.setPosition(sf::Vector2f(625.0f, 115.0f));
        c4gs4sprite.setScale(1.5f, 1.5f);
        spiel.draw(c4gs4sprite);

        sf::Texture c4gs5;
        switch (casinogelder4[4]) {
        case 10000:
            c4gs5.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c4gs5.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c4gs5.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c4gs5.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c4gs5.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c4gs5.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c4gs5.setSmooth(true);
        sf::Sprite c4gs5sprite(c4gs5);
        c4gs5sprite.setPosition(sf::Vector2f(625.0f, 145.0f));
        c4gs5sprite.setScale(1.5f, 1.5f);
        spiel.draw(c4gs5sprite);

        sf::Texture c5gs1;
        switch (casinogelder5[0]) {
        case 10000:
            c5gs1.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c5gs1.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c5gs1.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c5gs1.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c5gs1.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c5gs1.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c5gs1.setSmooth(true);
        sf::Sprite c5gs1sprite(c5gs1);
        c5gs1sprite.setPosition(sf::Vector2f(825.0f, 25.0f));
        c5gs1sprite.setScale(1.5f, 1.5f);
        spiel.draw(c5gs1sprite);

        sf::Texture c5gs2;
        switch (casinogelder5[1]) {
        case 10000:
            c5gs2.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c5gs2.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c5gs2.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c5gs2.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c5gs2.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c5gs2.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c5gs2.setSmooth(true);
        sf::Sprite c5gs2sprite(c5gs2);
        c5gs2sprite.setPosition(sf::Vector2f(825.0f, 55.0f));
        c5gs2sprite.setScale(1.5f, 1.5f);
        spiel.draw(c5gs2sprite);

        sf::Texture c5gs3;
        switch (casinogelder5[2]) {
        case 10000:
            c5gs3.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c5gs3.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c5gs3.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c5gs3.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c5gs3.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c5gs3.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c5gs3.setSmooth(true);
        sf::Sprite c5gs3sprite(c5gs3);
        c5gs3sprite.setPosition(sf::Vector2f(825.0f, 85.0f));
        c5gs3sprite.setScale(1.5f, 1.5f);
        spiel.draw(c5gs3sprite);

        sf::Texture c5gs4;
        switch (casinogelder5[3]) {
        case 10000:
            c5gs4.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c5gs4.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c5gs4.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c5gs4.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c5gs4.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c5gs4.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c5gs4.setSmooth(true);
        sf::Sprite c5gs4sprite(c5gs4);
        c5gs4sprite.setPosition(sf::Vector2f(825.0f, 115.0f));
        c5gs4sprite.setScale(1.5f, 1.5f);
        spiel.draw(c5gs4sprite);

        sf::Texture c5gs5;
        switch (casinogelder5[4]) {
        case 10000:
            c5gs5.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c5gs5.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c5gs5.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c5gs5.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c5gs5.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c5gs5.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c5gs5.setSmooth(true);
        sf::Sprite c5gs5sprite(c5gs5);
        c5gs5sprite.setPosition(sf::Vector2f(825.0f, 145.0f));
        c5gs5sprite.setScale(1.5f, 1.5f);
        spiel.draw(c5gs5sprite);

        sf::Texture c6gs1;
        switch (casinogelder6[0]) {
        case 10000:
            c6gs1.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c6gs1.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c6gs1.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c6gs1.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c6gs1.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c6gs1.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c6gs1.setSmooth(true);
        sf::Sprite c6gs1sprite(c6gs1);
        c6gs1sprite.setPosition(sf::Vector2f(1025.0f, 25.0f));
        c6gs1sprite.setScale(1.5f, 1.5f);
        spiel.draw(c6gs1sprite);

        sf::Texture c6gs2;
        switch (casinogelder6[1]) {
        case 10000:
            c6gs2.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c6gs2.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c6gs2.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c6gs2.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c6gs2.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c6gs2.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c6gs2.setSmooth(true);
        sf::Sprite c6gs2sprite(c6gs2);
        c6gs2sprite.setPosition(sf::Vector2f(1025.0f, 55.0f));
        c6gs2sprite.setScale(1.5f, 1.5f);
        spiel.draw(c6gs2sprite);

        sf::Texture c6gs3;
        switch (casinogelder6[2]) {
        case 10000:
            c6gs3.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c6gs3.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c6gs3.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c6gs3.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c6gs3.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c6gs3.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c6gs3.setSmooth(true);
        sf::Sprite c6gs3sprite(c6gs3);
        c6gs3sprite.setPosition(sf::Vector2f(1025.0f, 85.0f));
        c6gs3sprite.setScale(1.5f, 1.5f);
        spiel.draw(c6gs3sprite);

        sf::Texture c6gs4;
        switch (casinogelder6[3]) {
        case 10000:
            c6gs4.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c6gs4.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c6gs4.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c6gs4.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c6gs4.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c6gs4.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c6gs4.setSmooth(true);
        sf::Sprite c6gs4sprite(c6gs4);
        c6gs4sprite.setPosition(sf::Vector2f(1025.0f, 115.0f));
        c6gs4sprite.setScale(1.5f, 1.5f);
        spiel.draw(c6gs4sprite);

        sf::Texture c6gs5;
        switch (casinogelder6[4]) {
        case 10000:
            c6gs5.loadFromFile("res/Bilder/Assets/10000.png");
            break;
        case 20000:
            c6gs5.loadFromFile("res/Bilder/Assets/20000.png");
            break;
        case 30000:
            c6gs5.loadFromFile("res/Bilder/Assets/30000.png");
            break;
        case 50000:
            c6gs5.loadFromFile("res/Bilder/Assets/50000.png");
            break;
        case 70000:
            c6gs5.loadFromFile("res/Bilder/Assets/70000.png");
            break;
        case 90000:
            c6gs5.loadFromFile("res/Bilder/Assets/90000.png");
            break;
        }
        c6gs5.setSmooth(true);
        sf::Sprite c6gs5sprite(c6gs5);
        c6gs5sprite.setPosition(sf::Vector2f(1025.0f, 145.0f));
        c6gs5sprite.setScale(1.5f, 1.5f);
        spiel.draw(c6gs5sprite);

        //Würfelsprites zeichnen
        wuerfelsprite.setColor(farben[spielernummer]);
        wuerfelsprite2.setColor(farben[spielernummer]);
        wuerfelsprite3.setColor(farben[spielernummer]);
        wuerfelsprite4.setColor(farben[spielernummer]);
        wuerfelsprite5.setColor(farben[spielernummer]);
        wuerfelsprite6.setColor(farben[spielernummer]);
        wuerfelsprite7.setColor(farben[spielernummer]);
        wuerfelsprite8.setColor(farben[spielernummer]);
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
                //Dies mit einer Speicherwarung oder Speichern ändern
                spiel.close();
                exit(0);
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    Pausenmenu();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (donebutton.isMouseOver(spiel)) {
                    if (aufWuerfelGedrueckt == true) {
                        zugbeendet = true;
                    }
                }

                if (undobutton.isMouseOver(spiel)) { 
                    aufWuerfelGedrueckt = false;
                }
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                // transform the mouse position from window coordinates to world coordinates
                sf::Vector2f mouse = spiel.mapPixelToCoords(sf::Mouse::getPosition(spiel));

                // retrieve the bounding box of the sprite
                sf::FloatRect wuerfelcoords1 = wuerfelsprite.getGlobalBounds();
                sf::FloatRect wuerfelcoords2 = wuerfelsprite2.getGlobalBounds();
                sf::FloatRect wuerfelcoords3 = wuerfelsprite3.getGlobalBounds();
                sf::FloatRect wuerfelcoords4 = wuerfelsprite4.getGlobalBounds();
                sf::FloatRect wuerfelcoords5 = wuerfelsprite5.getGlobalBounds();
                sf::FloatRect wuerfelcoords6 = wuerfelsprite6.getGlobalBounds();
                sf::FloatRect wuerfelcoords7 = wuerfelsprite7.getGlobalBounds();
                sf::FloatRect wuerfelcoords8 = wuerfelsprite8.getGlobalBounds();

                // Hit-test: Überprüfe, ob Würfel angeklickt wurde
                if (wuerfelcoords1.contains(mouse))
                {
                    // mouse is on sprite!
                    //TODO: Setze Würfel auf Casino
                    int wuerfelplatzwert = wuerfelwert[0];
                    aufWuerfelGedrueckt = true;
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }
                }
                if (wuerfelcoords2.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[1];
                    aufWuerfelGedrueckt = true;
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords3.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[2];
                    aufWuerfelGedrueckt = true;
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords4.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[3];
                    aufWuerfelGedrueckt = true;
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords5.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[4];
                    aufWuerfelGedrueckt = true;
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords6.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[5];
                    aufWuerfelGedrueckt = true;
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords7.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[6];
                    aufWuerfelGedrueckt = true;
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
                if (wuerfelcoords8.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[7];
                    aufWuerfelGedrueckt = true;
                    switch (wuerfelplatzwert) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    }

                }
            }
        }

        spiel.display();
    }
}

void neuesSpiel() { 
    //Zeichenthread erstellen
    sf::Thread zeichenthread(&Spielzeichnung);
    zeichenthread.launch();
    //Ganz normale Spiellogik
    bool spielBeendet = false;
    while(spielBeendet == false) {
        //Geldscheine zufällig auswählen und anzeigen (i <=5 machen nicht vergessen)
        srand(time(NULL));
        for (int i = 0; i <= 5; i++) {
            switch (i) {
            case 0:
                for(int k = 0; ; k++){
                    //int k = 0;
                    int j = rand() % 6;
                    casinogelder1[k] = geldwerte[j];
                    printf("Gerade gezogen: %i\n", casinogelder1[k]);
                    summe1 += casinogelder1[k];
                    if (summe1 >= 50000) {
                        printf("Summe Casino 1: %i\n", summe1);
                        break;
                    }
                    
                }
                break;
            case 1:
                for (int k = 0; ; k++) {
                    //int k = 0;
                    int j = rand() % 6;
                    casinogelder2[k] = geldwerte[j];
                    printf("Gerade gezogen: %i\n", casinogelder2[k]);
                    summe2 += casinogelder2[k];
                    if (summe2 >= 50000) {
                        printf("Summe Casino 2: %i\n", summe2);
                        break;
                    }

                }
                break;
            case 2:
                for (int k = 0; ; k++) {
                    //int k = 0;
                    int j = rand() % 6;
                    casinogelder3[k] = geldwerte[j];
                    printf("Gerade gezogen: %i\n", casinogelder3[k]);
                    summe3 += casinogelder3[k];
                    if (summe3 >= 50000) {
                        printf("Summe Casino 3: %i\n", summe3);
                        break;
                    }
                }
                break;
            case 3:
                for (int k = 0; ; k++) {
                    //int k = 0;
                    int j = rand() % 6;
                    casinogelder4[k] = geldwerte[j];
                    printf("Gerade gezogen: %i\n", casinogelder4[k]);
                    summe4 += casinogelder4[k];
                    if (summe4 >= 50000) {
                        printf("Summe Casino 4: %i\n", summe4);
                        break;
                    }
                }
                break;
            case 4:
                for (int k = 0; ; k++) {
                    //int k = 0;
                    int j = rand() % 6;
                    casinogelder5[k] = geldwerte[j];
                    printf("Gerade gezogen: %i\n", casinogelder5[k]);
                    summe5 += casinogelder5[k];
                    if (summe5 >= 50000) {
                        printf("Summe Casino 5: %i\n", summe5);
                        break;
                    }
                }
                break;
            case 5:
                for (int k = 0; ; k++) {
                    //int k = 0;
                    int j = rand() % 6;
                    casinogelder6[k] = geldwerte[j];
                    printf("Gerade gezogen: %i\n", casinogelder6[k]);
                    summe6 += casinogelder6[k];
                    if (summe6 >= 50000) {
                        printf("Summe Casino 6: %i\n", summe6);
                        break;
                    }
                }
                break;
            }
        }

        //Geldscheine sortieren
        //Casino 1
        for (int i = 0; i <= 4; i++)
        {
            for (int j = i + 1; j <= 5; j++)
            {
                if (casinogelder1[i] < casinogelder1[j])
                {
                    //-----Tausch-----
                    int h = casinogelder1[i];
                    casinogelder1[i] = casinogelder1[j];
                    casinogelder1[j] = h;
                }
            }
        }

        //Casino 2
        for (int i = 0; i <= 4; i++)
        {
            for (int j = i + 1; j <= 5; j++)
            {
                if (casinogelder2[i] < casinogelder2[j])
                {
                    //-----Tausch-----
                    int h = casinogelder2[i];
                    casinogelder2[i] = casinogelder2[j];
                    casinogelder2[j] = h;
                }
            }
        }

        //Casino 3
        for (int i = 0; i <= 4; i++)
        {
            for (int j = i + 1; j <= 5; j++)
            {
                if (casinogelder3[i] < casinogelder3[j])
                {
                    //-----Tausch-----
                    int h = casinogelder3[i];
                    casinogelder3[i] = casinogelder3[j];
                    casinogelder3[j] = h;
                }
            }
        }

        //Casino 4
        for (int i = 0; i <= 4; i++)
        {
            for (int j = i + 1; j <= 5; j++)
            {
                if (casinogelder4[i] < casinogelder4[j])
                {
                    //-----Tausch-----
                    int h = casinogelder4[i];
                    casinogelder4[i] = casinogelder4[j];
                    casinogelder4[j] = h;
                }
            }
        }

        //Casino 5
        for (int i = 0; i <= 4; i++)
        {
            for (int j = i + 1; j <= 5; j++)
            {
                if (casinogelder5[i] < casinogelder5[j])
                {
                    //-----Tausch-----
                    int h = casinogelder5[i];
                    casinogelder5[i] = casinogelder5[j];
                    casinogelder5[j] = h;
                }
            }
        }

        //Casino 6
        for (int i = 0; i <= 4; i++)
        {
            for (int j = i + 1; j <= 5; j++)
            {
                if (casinogelder6[i] < casinogelder6[j])
                {
                    //-----Tausch-----
                    int h = casinogelder6[i];
                    casinogelder6[i] = casinogelder6[j];
                    casinogelder6[j] = h;
                }
            }
        }

        for (int spieler = 0; spieler <= 2; spieler++) {
            //Temporäre Variable fürs Überprüfen, ob ein Spieler kein Würfel hat
            int tempanzwuerfel = wuerfelanzahl[spieler];
            if(tempanzwuerfel == 0) {
                continue;
            }
            spielernummer = spieler;
            int anzahlwuerfel = wuerfelanzahl[spieler];

            //Würfel würfeln
            for (int i = 0; i <= 7; i++) {
                if (i > anzahlwuerfel - 1) {
                    wuerfelwert[i] = 8;
                }
                else {
                    wuerfelwert[i] = gen();
                    cout << wuerfelwert[i];
                }
            }
            printf("\n");

            //Würfeln sortieren
            for (int i = 0; i <= 6; i++)
            {
                for (int j = i + 1; j <= 7; j++)
                {
                    if (wuerfelwert[i] > wuerfelwert[j])
                    {
                        //-----Tausch-----
                        int h = wuerfelwert[i];
                        wuerfelwert[i] = wuerfelwert[j];
                        wuerfelwert[j] = h;
                    }
                }
            }

            //Warten bis auf Würfel gedrückt wird
            while (zugbeendet == false) {
                //Unten den Nutzer drauf hinweisen, auf einen Würfel zu klicken!
                switch (spielernummer) {
                    case 0:
                        subtextstring = "Spieler 1: Bitte wählen Sie einen Würfel!";
                        break;
                    case 1:
                        subtextstring = "Spieler 2: Bitte wählen Sie einen Würfel!";
                        break;
                    case 2:
                        subtextstring = "Spieler 3: Bitte wählen Sie einen Würfel!";
                        break;
                    default:
                        break;
                }
            }
            //Resetten
            aufWuerfelGedrueckt = false;
            zugbeendet = false;

            //Würfelableger-Algorithmus


        }
    }
    //Ende vom Spiel

}

//
//Main-Funktion - Hier befinden sich alle Hauptfunktionen wie Erzeugung des Bildschirms etc.
//
int main(){
    //Konsolenausgabe
    printf("Las Vegas - Das Spiel\nDiese Konsole wird als Fehlerausgabe benutzt!\nSprich, falls etwas falsch laufen sollte,\nwird es hier ausgegeben!\n\n");

    //Erzeuge das Fenster-Objekt
    sf::RenderWindow window(sf::VideoMode(1220, 800), "Las Vegas", sf::Style::Close);

    // Icon für das Spiel setzen
    sf::Image icon;
    if (!icon.loadFromFile("res/Bilder/icon.jpg")) {
        fehleranzeige("Icon-Fehler", "Icon konnte nicht geladen werden.");
        return 0;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //Erstelle eine Struktur für Daten
    struct einstellung {
        int stellung;
    };
    
    //Prüfe, ob es Einstellungdatei existiert und erstelle sie, falls nicht
    ifstream leser("einstellungen.einstellungen", std::ios::out | std::ios::binary);
    if (!leser) {
        fehleranzeige("Fehler", "Einstellungsdatei nicht gefunden! Es wird neuerstellt!");
        printf("Einstellungsdatei wird erstellt! \n");
        startup();
    }
    
    //Prüfe, ob Einstellung auf An ist
    einstellung items[1];
    //Daten lesen
    for (int i = 0; i < 2; i++) {
        leser.read((char*)&items[i], sizeof(einstellung));
    }
    //Erster Wert im Array als Variable speichern
    int e = items[0].stellung;

    //Erzeuge das Audio-Objekt und fange an zu spielen!
    //AUSLASSEN BITTE KEIN BOCK MEHR DASS ES SPACKT
    //Fehler: Es spackt, wenn man im Anruf ist und der ganze Audiotreiber ist dann verwirrt
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
    Button loadbtn1("Laden", { 200, 100 }, 30, sf::Color(0, 162, 232, 255), sf::Color::Black);
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
        
        // Switch, um zu prüfen, welches Event passiert ist und was getan werden soll
        switch (event.type) 
        {
            //Wenn es geklickt hat
            case sf::Event::MouseButtonPressed:
                if (playbtn1.isMouseOver(window)) {
                    //Wenn Spielen-Knopf gedrückt wird
                    //debug printf("Knopf wurde gedrueckt!");
                    //test fehleranzeige("Kritischer Fehler", "Fehler! Bitte starten Sie das Spiel neu!");
                    window.setActive(false);
                    window.setVisible(false);
                    neuesSpiel();
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

        // setze den Bildschirm zurück
        window.clear(sf::Color::White);
        
        // Erzeuge einen Text
        sf::Text text("Willkommen zu Las Vegas! Bitte wählen Sie eine der folgenden Optionen.", font);
        text.setCharacterSize(30);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setPosition(230.0f, 430.0f);

        //Versiontext
        sf::Text vertext("Version: 0.6.9 Alpha", font);
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
        //window.draw(convexShape);
        playbtn1.drawTo(window);
        leavebtn1.drawTo(window);
        settingsbtn1.drawTo(window);
        loadbtn1.drawTo(window);

        // Zeige den Frame an
        window.display();
    }
}