//Autor: Kerem Okumus, Kiriakos Avramidis, Moritz Biedenbacher
//Las Vegas Spiel als grafische Anwendung
//Dieses Programm nutzt die SFML-Bibliothek, dass zur Programmierung von grafischen Benutzeroberfl�chen erleichtern soll!
//ge�ndert am: 25.01.2021(Datei erstellt und ersten Code geschrieben); 26.01.2021 (Code ver�ndert und Men� hinzugef�gt)


//Einbindung wichtiger Bibliotheken
#include<SFML/Graphics.hpp>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Audio.hpp>
#include<SFML/System.hpp>

#include<iostream>
#include<windows.h>
#include<fstream>
#include<cstdlib>
#include<conio.h>
#include<random>
#include<numeric>
#include<algorithm>
#include <string.h>

using namespace std;
using namespace sf;

//Selbstgeschriebene Header-Dateien
#include "Button.h"
#include "Dice.h"
#include "infosystem.h"
#include "dateienmanager.h"

//Wichtige Spiel-Variablen
float wuerfelpos[8] = { 20.0f, 120.0f, 220.0f, 320.0f, 420.0f, 520.0f, 620.0f, 720.0f };
string wuerfelaugen[6] = { "./res/Bilder/w�rfel/dice-png-1.png", "./res/Bilder/w�rfel/dice-png-2.png", "./res/Bilder/w�rfel/dice-png-3.png", "./res/Bilder/w�rfel/dice-png-4.png", "./res/Bilder/w�rfel/dice-png-5.png", "./res/Bilder/w�rfel/dice-png-6.png" };
int geldwerte[6] = { 10000, 20000, 30000, 50000, 70000, 90000 };
int kontostand[3] = {0, 0, 0};
int casinogelder1[5];
int casinogelder2[5];
int casinogelder3[5];
int casinogelder4[5];
int casinogelder5[5];
int casinogelder6[5];
int casinowuerfelanzahl1[3] = { 0, 0, 0 };
int casinowuerfelanzahl2[3] = { 0, 0, 0 };
int casinowuerfelanzahl3[3] = { 0, 0, 0 };
int casinowuerfelanzahl4[3] = { 0, 0, 0 };
int casinowuerfelanzahl5[3] = { 0, 0, 0 };
int casinowuerfelanzahl6[3] = { 0, 0, 0 };
int summe1 = 0;
int summe2 = 0;
int summe3 = 0;
int summe4 = 0;
int summe5 = 0;
int summe6 = 0;
int wuerfelanzahl[3] = { 8, 8, 8 };
int wuerfelwert[8];
int spieler;
int spielernummer;
int rundenzahl = 0;
int ausgew�hlteAuge[2]; //Ein leeres Array erstellen f�r den ersten Index, welcher W�rfel und der zweite Index, wie viele.
bool spielerhabenw�rfel = true;
Color farben[3] = { Color::Cyan, Color::Green, Color::Yellow };
string subtextstring;
int rundenzahlf�rtext = rundenzahl + 1; 
string kontostandtext1 = "Kontostand: $" + to_string(kontostand[0]);
string kontostandtext2 = "Kontostand: $" + to_string(kontostand[1]);
string kontostandtext3 = "Kontostand: $" + to_string(kontostand[2]);
string wuerfelzahl1, wuerfelzahl2, wuerfelzahl3;
string letzteaktion[3] = { "Zuletzt: Nichts passiert!", "Zuletzt: Nichts passiert!", "Zuletzt: Nichts passiert!" };
string speichername1, speichername2, speichername3, speichername4;
string rundentext;
bool aufWuerfelGedrueckt = false;
bool spielBeendet = false;
bool zugbeendet = false;
bool spielgeladen;

void neuesSpiel();
int main();

//Systemvariablen (BITTE NICHT VER�NDERN!)
bool neustartgefordert = false;
bool speichertbool = false;

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
                        einstellungsdatei�nderung(p, v);
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

//
// AKTUELLER CODE
//

void speicherdateilesen(int speichernummer) {
    string name;
    switch (speichernummer) {
    case 1:
        name = "data1.txt";
        break;
    case 2:
        name = "data2.txt";
        break;
    case 3:
        name = "data3.txt";
        break;
    case 4:
        name = "data4.txt";
        break;
    default:
        fehleranzeige("Programmier-Fehler", "Es gibt nur 4 Speicherdateien!");
    }
    ifstream leser(name, std::ios::in);
    string x[128];
    
    getline(leser, x[0]);
    kontostand[0] = stoi(x[0]);
    getline(leser, x[1]);
    kontostand[1] = stoi(x[1]);
    getline(leser, x[2]);
    kontostand[2] = stoi(x[2]);

    getline(leser, x[3]);
    casinogelder1[0] = stoi(x[3]);
    getline(leser, x[4]);
    casinogelder2[0] = stoi(x[4]);
    getline(leser, x[5]);
    casinogelder3[0] = stoi(x[5]);
    getline(leser, x[6]);
    casinogelder4[0] = stoi(x[6]);
    getline(leser, x[7]);
    casinogelder5[0] = stoi(x[7]);
    getline(leser, x[8]);
    casinogelder6[0] = stoi(x[8]);

    getline(leser, x[9]);
    casinogelder1[1] = stoi(x[9]);
    getline(leser, x[10]);
    casinogelder2[1] = stoi(x[10]);
    getline(leser, x[11]);
    casinogelder3[1] = stoi(x[11]);
    getline(leser, x[12]);
    casinogelder4[1] = stoi(x[12]);
    getline(leser, x[13]);
    casinogelder5[1] = stoi(x[13]);
    getline(leser, x[14]);
    casinogelder6[1] = stoi(x[14]);

    getline(leser, x[15]);
    casinogelder1[2] = stoi(x[15]);
    getline(leser, x[16]);
    casinogelder2[2] = stoi(x[16]);
    getline(leser, x[17]);
    casinogelder3[2] = stoi(x[17]);
    getline(leser, x[18]);
    casinogelder4[2] = stoi(x[18]);
    getline(leser, x[19]);
    casinogelder5[2] = stoi(x[19]);
    getline(leser, x[20]);
    casinogelder6[2] = stoi(x[20]);

    getline(leser, x[21]);
    casinogelder1[3] = stoi(x[21]);
    getline(leser, x[22]);
    casinogelder2[3] = stoi(x[22]);
    getline(leser, x[23]);
    casinogelder3[3] = stoi(x[23]);
    getline(leser, x[24]);
    casinogelder4[3] = stoi(x[24]);
    getline(leser, x[25]);
    casinogelder5[3] = stoi(x[25]);
    getline(leser, x[26]);
    casinogelder6[3] = stoi(x[26]);

    getline(leser, x[27]);
    casinogelder1[4] = stoi(x[27]);
    getline(leser, x[28]);
    casinogelder2[4] = stoi(x[28]);
    getline(leser, x[29]);
    casinogelder3[4] = stoi(x[29]);
    getline(leser, x[30]);
    casinogelder4[4] = stoi(x[30]);
    getline(leser, x[31]);
    casinogelder5[4] = stoi(x[31]);
    getline(leser, x[32]);
    casinogelder6[4] = stoi(x[32]);

    getline(leser, x[33]);
    casinowuerfelanzahl1[0] = stoi(x[33]);
    getline(leser, x[34]);
    casinowuerfelanzahl2[0] = stoi(x[34]);
    getline(leser, x[35]);
    casinowuerfelanzahl3[0] = stoi(x[35]);
    getline(leser, x[36]);
    casinowuerfelanzahl4[0] = stoi(x[36]);
    getline(leser, x[37]);
    casinowuerfelanzahl5[0] = stoi(x[37]);
    getline(leser, x[38]);
    casinowuerfelanzahl6[0] = stoi(x[38]);

    getline(leser, x[39]);
    casinowuerfelanzahl1[1] = stoi(x[39]);
    getline(leser, x[40]);
    casinowuerfelanzahl2[1] = stoi(x[40]);
    getline(leser, x[41]);
    casinowuerfelanzahl3[1] = stoi(x[41]);
    getline(leser, x[42]);
    casinowuerfelanzahl4[1] = stoi(x[42]);
    getline(leser, x[43]);
    casinowuerfelanzahl5[1] = stoi(x[43]);
    getline(leser, x[44]);
    casinowuerfelanzahl6[1] = stoi(x[44]);

    getline(leser, x[45]);
    casinowuerfelanzahl1[2] = stoi(x[45]);
    getline(leser, x[46]);
    casinowuerfelanzahl2[2] = stoi(x[46]);
    getline(leser, x[47]);
    casinowuerfelanzahl3[2] = stoi(x[47]);
    getline(leser, x[48]);
    casinowuerfelanzahl4[2] = stoi(x[48]);
    getline(leser, x[49]);
    casinowuerfelanzahl5[2] = stoi(x[49]);
    getline(leser, x[50]);
    casinowuerfelanzahl6[2] = stoi(x[50]);

    getline(leser, x[51]);
    wuerfelanzahl[0] = stoi(x[51]);
    getline(leser, x[52]);
    wuerfelanzahl[1] = stoi(x[52]);
    getline(leser, x[53]);
    wuerfelanzahl[2] = stoi(x[53]);

    getline(leser, x[54]);
    wuerfelwert[0] = stoi(x[54]);
    getline(leser, x[55]);
    wuerfelwert[1] = stoi(x[55]);
    getline(leser, x[56]);
    wuerfelwert[2] = stoi(x[56]);
    getline(leser, x[57]);
    wuerfelwert[3] = stoi(x[57]);
    getline(leser, x[58]);
    wuerfelwert[4] = stoi(x[58]);
    getline(leser, x[59]);
    wuerfelwert[5] = stoi(x[59]);
    getline(leser, x[60]);
    wuerfelwert[6] = stoi(x[60]);
    getline(leser, x[61]);
    wuerfelwert[7] = stoi(x[61]);

    getline(leser, x[62]);
    spieler = stoi(x[62]);
    spielernummer = stoi(x[62]);

    getline(leser, x[63]);
    rundenzahl = stoi(x[63]);

    getline(leser, x[64]);
    letzteaktion[0] = x[64];

    getline(leser, x[65]);
    letzteaktion[1] = x[65];

    getline(leser, x[66]);
    letzteaktion[2] = x[66];

    kontostandtext1 = "Kontostand: $" + to_string(kontostand[0]);
    kontostandtext2 = "Kontostand: $" + to_string(kontostand[1]);
    kontostandtext3 = "Kontostand: $" + to_string(kontostand[2]);

    rundentext = "Runde " + to_string(rundenzahlf�rtext) + " von 4";

    spielgeladen = true;
    printf("Spiel wurde geladen!\n");
}

void dateienmanager(bool speichern) {
    sf::RenderWindow dateifenster(sf::VideoMode(800, 800), "Spielspeicher-Dateienmanager", sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        fehleranzeige("Font-Fehler", "Font konnte nicht geladen werden!");
    }

    while (dateifenster.isOpen()) {
        dateifenster.clear(Color::Yellow);

        //Pr�fen, ob die Dateien leer sind!
        ifstream pr�fer("data1.txt", std::ios::in);
        string stemp1;
        getline(pr�fer, stemp1);
        if (stemp1 == "leer") {
            speichername1 = "Spielspeicher 1 - LEER";
        }
        else {
            speichername1 = "Spielspeicher 1 - VOLL";
        }

        ifstream pr�fer2("data2.txt", std::ios::in);
        string stemp2;
        getline(pr�fer2, stemp2);
        if (stemp2 == "leer") {
            speichername2 = "Spielspeicher 2 - LEER";
        }
        else {
            speichername2 = "Spielspeicher 2 - VOLL";
        }

        ifstream pr�fer3("data3.txt", std::ios::in);
        string stemp3;
        getline(pr�fer3, stemp3);
        if (stemp3 == "leer") {
            speichername3 = "Spielspeicher 3 - LEER";
        }
        else {
            speichername3 = "Spielspeicher 3 - VOLL";
        }

        ifstream pr�fer4("data4.txt", std::ios::in);
        string stemp4;
        getline(pr�fer4, stemp4);
        if (stemp4 == "leer") {
            speichername4 = "Spielspeicher 4 - LEER";
        }
        else {
            speichername4 = "Spielspeicher 4 - VOLL";
        }

        Button Spielspeicher1(speichername1, { 650, 100 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
        Spielspeicher1.setFont(font);
        Spielspeicher1.setPosition({ 25.0f, 50.0f });

        Button Spielspeicher2(speichername2, { 650, 100 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
        Spielspeicher2.setFont(font);
        Spielspeicher2.setPosition({ 25.0f, 200.0f });

        Button Spielspeicher3(speichername3, { 650, 100 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
        Spielspeicher3.setFont(font);
        Spielspeicher3.setPosition({ 25.0f, 350.0f });

        Button Spielspeicher4(speichername4, { 650, 100 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
        Spielspeicher4.setFont(font);
        Spielspeicher4.setPosition({ 25.0f, 500.0f });

        Button Speicher1l�schen("L�schen", { 75, 100 }, 20, sf::Color::Red, sf::Color::Black);
        Speicher1l�schen.setFont(font);
        Speicher1l�schen.setPosition({ 700.0f, 50.0f });

        Button Speicher2l�schen("L�schen", { 75, 100 }, 20, sf::Color::Red, sf::Color::Black);
        Speicher2l�schen.setFont(font);
        Speicher2l�schen.setPosition({ 700.0f, 200.0f });

        Button Speicher3l�schen("L�schen", { 75, 100 }, 20, sf::Color::Red, sf::Color::Black);
        Speicher3l�schen.setFont(font);
        Speicher3l�schen.setPosition({ 700.0f, 350.0f });

        Button Speicher4l�schen("L�schen", { 75, 100 }, 20, sf::Color::Red, sf::Color::Black);
        Speicher4l�schen.setFont(font);
        Speicher4l�schen.setPosition({ 700.0f, 500.0f });

        Button Abbrechenbutton("Abbrechen", { 800.0f, 100.0f },30,  sf::Color::Magenta, sf::Color::Black);
        

        Spielspeicher1.drawTo(dateifenster);
        Spielspeicher2.drawTo(dateifenster);
        Spielspeicher3.drawTo(dateifenster);
        Spielspeicher4.drawTo(dateifenster);
        Speicher1l�schen.drawTo(dateifenster);
        Speicher2l�schen.drawTo(dateifenster);
        Speicher3l�schen.drawTo(dateifenster);
        Speicher4l�schen.drawTo(dateifenster);
        dateifenster.display();
        sf::Event event;
        while (dateifenster.pollEvent(event))
        {
            // Schlie�t das Fenster, falls schlie�en gedr�ckt wird
            if (event.type == sf::Event::Closed) {
                dateifenster.close();

            }

            switch (event.type) {
            case sf::Event::MouseButtonPressed:
                if (Spielspeicher1.isMouseOver(dateifenster)) {
                    if (speichern == true) {
                        speicherdateien�nderung(1, kontostand, casinogelder1, casinogelder2, casinogelder3, casinogelder4, casinogelder5, casinogelder6, casinowuerfelanzahl1, casinowuerfelanzahl2, casinowuerfelanzahl3, casinowuerfelanzahl4, casinowuerfelanzahl5, casinowuerfelanzahl6, wuerfelanzahl, wuerfelwert, spielernummer, rundenzahl, letzteaktion);
                        dateifenster.close();
                    }
                    else if (speichern == false) {
                        //Pr�fen, ob Datei leer ist
                        ifstream pr�fer("data1.txt", std::ios::in);
                        string stemp;
                        getline(pr�fer, stemp);
                        if (stemp == "leer") {
                            break;
                        }
                        printf("Spiel 1 wird geladen!\n");
                        //Spiel laden
                        speicherdateilesen(1);
                        dateifenster.close();
                        neuesSpiel();
                    }
                }
                if (Spielspeicher2.isMouseOver(dateifenster)) {
                    if (speichern == true) {
                        speicherdateien�nderung(2, kontostand, casinogelder1, casinogelder2, casinogelder3, casinogelder4, casinogelder5, casinogelder6, casinowuerfelanzahl1, casinowuerfelanzahl2, casinowuerfelanzahl3, casinowuerfelanzahl4, casinowuerfelanzahl5, casinowuerfelanzahl6, wuerfelanzahl, wuerfelwert, spielernummer, rundenzahl, letzteaktion);
                        dateifenster.close();
                    }
                    else if (speichern == false) {
                        //Pr�fen, ob Datei leer ist
                        ifstream pr�fer("data2.txt", std::ios::in);
                        string stemp;
                        getline(pr�fer, stemp);
                        if (stemp == "leer") {
                            break;
                        }
                        printf("Spiel 2 wird geladen!\n");
                        //Spiel laden
                        speicherdateilesen(2);
                        dateifenster.close();
                        neuesSpiel();
                    }
                }
                if (Spielspeicher3.isMouseOver(dateifenster)) {
                    if (speichern == true) {
                        speicherdateien�nderung(3, kontostand, casinogelder1, casinogelder2, casinogelder3, casinogelder4, casinogelder5, casinogelder6, casinowuerfelanzahl1, casinowuerfelanzahl2, casinowuerfelanzahl3, casinowuerfelanzahl4, casinowuerfelanzahl5, casinowuerfelanzahl6, wuerfelanzahl, wuerfelwert, spielernummer, rundenzahl, letzteaktion);
                        dateifenster.close();
                    }
                    else if (speichern == false) {
                        //Pr�fen, ob Datei leer ist
                        ifstream pr�fer("data3.txt", std::ios::in);
                        string stemp;
                        getline(pr�fer, stemp);
                        if (stemp == "leer") {
                            break;
                        }
                        printf("Spiel 3 wird geladen!\n");
                        //Spiel laden
                        speicherdateilesen(3);
                        dateifenster.close();
                        neuesSpiel();
                    }
                }
                if (Spielspeicher4.isMouseOver(dateifenster)) {
                    if (speichern == true) {
                        speicherdateien�nderung(4, kontostand, casinogelder1, casinogelder2, casinogelder3, casinogelder4, casinogelder5, casinogelder6, casinowuerfelanzahl1, casinowuerfelanzahl2, casinowuerfelanzahl3, casinowuerfelanzahl4, casinowuerfelanzahl5, casinowuerfelanzahl6, wuerfelanzahl, wuerfelwert, spielernummer, rundenzahl, letzteaktion);
                        dateifenster.close();
                    }
                    else if (speichern == false) {
                        //Pr�fen, ob Datei leer ist
                        ifstream pr�fer("data4.txt", std::ios::in);
                        string stemp;
                        getline(pr�fer, stemp);
                        if (stemp == "leer") {
                            break;
                        }
                        printf("Spiel 4 wird geladen!\n");
                        //Spiel laden
                        speicherdateilesen(4);
                        dateifenster.close();
                        neuesSpiel();
                    }
                }
                if (Speicher1l�schen.isMouseOver(dateifenster)) {
                    speicherdateienl�schung(1);
                }
                if (Speicher2l�schen.isMouseOver(dateifenster)) {
                    speicherdateienl�schung(2);
                }
                if (Speicher3l�schen.isMouseOver(dateifenster)) {
                    speicherdateienl�schung(3);
                }
                if (Speicher4l�schen.isMouseOver(dateifenster)) {
                    speicherdateienl�schung(4);
                }
            }
        }
    }
}

//Eine Funktion f�r das Pausenmen�
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

        sf::Text titeltext("Pausemen�", font);
        titeltext.setCharacterSize(40);
        titeltext.setFillColor(sf::Color::Black);
        titeltext.setPosition(20.0f, 20.0f);

        Button fortsetzenbutton("Fortsetzen", { 200, 100 }, 30, sf::Color::Magenta, sf::Color::Black);
        fortsetzenbutton.setFont(font);
        fortsetzenbutton.setPosition(sf::Vector2f(20.0f, 100.0f));
        fortsetzenbutton.drawTo(pause);

        Button neuesspielbutton("Hauptmen�", { 200, 100 }, 30, sf::Color(0, 255, 0, 255), sf::Color::Black);
        neuesspielbutton.setFont(font);
        neuesspielbutton.setPosition({ 270.0f, 100.0f });
        neuesspielbutton.drawTo(pause);

        Button speichernbutton("Speichern", { 200, 100 }, 30, sf::Color::Cyan, sf::Color::Black);
        speichernbutton.setFont(font);
        speichernbutton.setPosition({ 20.0f, 225.0f });
        speichernbutton.drawTo(pause);

        Button einstellungsbutton("Einstellungen", { 200, 100 }, 30, sf::Color::Yellow, sf::Color::Black);
        einstellungsbutton.setFont(font);
        einstellungsbutton.setPosition({ 270.0f, 225.0f });
        einstellungsbutton.drawTo(pause);

        Button hilfebutton("Hilfe", { 200, 100 }, 30, sf::Color(0, 162, 232, 255), sf::Color::Black);
        hilfebutton.setFont(font);
        hilfebutton.setPosition({ 20.0f, 350.0f });
        hilfebutton.drawTo(pause);

        Button beendenbtn("Beenden", { 200, 100 }, 30, sf::Color(255, 0, 0, 255), sf::Color::Black);
        beendenbtn.setFont(font);
        beendenbtn.setPosition({ 270.0f, 350.0f });
        beendenbtn.drawTo(pause);

        pause.draw(titeltext);
        
        while (pause.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    pause.close();
                }
            }
        }

        switch (event.type) {
        case sf::Event::MouseButtonPressed:
            if (fortsetzenbutton.isMouseOver(pause)) {
                pause.close();
            }

            if (neuesspielbutton.isMouseOver(pause)) {
                neustartgefordert = true;
            }

            if (speichernbutton.isMouseOver(pause)) {
                dateienmanager(true);
            }

            if (hilfebutton.isMouseOver(pause)) {
                //Bedienungsanleitung �ffnen
                system("start winword \"./Bedienungsanleitung Las Vegas.docx\"");
            }

            if (beendenbtn.isMouseOver(pause)) {
                //Nach Speicherung warnen/fragen vllt.
                bool meldung = best�tigungsmeldung("M�chten Sie den Spielstand speichern?");
                if (meldung == true) {
                    //Jetzt Speichern
                    dateienmanager(true);   
                }
                else {
                    exit(0);
                }
            }
        }
        pause.display();
    }
}

//Wichtige Spielfunktionen
//Dient zur Aussortierung von W�rfel, auf den geklickt wurde
void setzeW�rfel(int spieler, int wert) {
    //Pr�fen, ob schon ein W�rfel angeklickt wurde!
    if (aufWuerfelGedrueckt == false) {
        //Da auf ein W�rfel gedr�ckt wurde, w�rde ich auch lieber den Bool daf�r auf true setzen XD
        aufWuerfelGedrueckt = true;

        //Variable f�r das Z�hlen der W�rfel, mit den gleichen Wert
        int anzahl = 0;
        switch (wert) {
        case 1:
            //Schaue nach weiteren W�rfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (wuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der W�rfel verschwindet
                    wuerfelwert[i] = 8;
                }
            }
            ausgew�hlteAuge[0] = wert;
            ausgew�hlteAuge[1] = anzahl;

            //W�rfel in Casinoarray setzen
            casinowuerfelanzahl1[spielernummer] += anzahl;

            break;
        case 2:
            //Schaue nach weiteren W�rfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (wuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der W�rfel verschwindet
                    wuerfelwert[i] = 8;
                }
            }
            ausgew�hlteAuge[0] = wert;
            ausgew�hlteAuge[1] = anzahl;

            casinowuerfelanzahl2[spielernummer] += anzahl;

            break;
        case 3:
            //Schaue nach weiteren W�rfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (wuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der W�rfel verschwindet
                    wuerfelwert[i] = 8;
                }
            }
            ausgew�hlteAuge[0] = wert;
            ausgew�hlteAuge[1] = anzahl;

            casinowuerfelanzahl3[spielernummer] += anzahl;

            break;
        case 4:
            //Schaue nach weiteren W�rfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (wuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der W�rfel verschwindet
                    wuerfelwert[i] = 8;
                }
            }
            ausgew�hlteAuge[0] = wert;
            ausgew�hlteAuge[1] = anzahl;

            casinowuerfelanzahl4[spielernummer] += anzahl;

            break;
        case 5:
            //Schaue nach weiteren W�rfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (wuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der W�rfel verschwindet
                    wuerfelwert[i] = 8;
                }
            }
            ausgew�hlteAuge[0] = wert;
            ausgew�hlteAuge[1] = anzahl;

            casinowuerfelanzahl5[spielernummer] += anzahl;

            break;
        case 6:
            //Schaue nach weiteren W�rfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (wuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der W�rfel verschwindet
                    wuerfelwert[i] = 8;
                }
            }
            ausgew�hlteAuge[0] = wert;
            ausgew�hlteAuge[1] = anzahl;

            casinowuerfelanzahl6[spielernummer] += anzahl;

            break;
        }
        //Abschlie�ende Sortierung der W�rfel starten und anzeigen
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
        //Gesamtw�rfelzahl des Spielers �ndern
        wuerfelanzahl[spielernummer] -= anzahl;

        //String des Spielers �ndern
        letzteaktion[spielernummer] = "Zuletzt: " + to_string(anzahl) + " W�rfeln auf Casino " + to_string(wert);
    }
}

void setzeSchrittZur�ck() {
    //Pr�fen, ob die Funktion �berhaupt was bringen w�rde
    if(aufWuerfelGedrueckt == true){
        aufWuerfelGedrueckt = false;
        int wert = ausgew�hlteAuge[0];
        int anzahl = ausgew�hlteAuge[1];

        //Switch f�r den Casinoarrays
        switch (wert) {
        case 1:
            casinowuerfelanzahl1[spielernummer] -= anzahl;
            break;
        case 2:
            casinowuerfelanzahl2[spielernummer] -= anzahl;
            break;
        case 3:
            casinowuerfelanzahl3[spielernummer] -= anzahl;
            break;
        case 4:
            casinowuerfelanzahl4[spielernummer] -= anzahl;
            break;
        case 5:
            casinowuerfelanzahl5[spielernummer] -= anzahl;
            break;
        case 6:
            casinowuerfelanzahl6[spielernummer] -= anzahl;
            break;
        }

        printf("Anzahl Wuerfel auf Casino 1 zurueckgezogen: %i\n", casinowuerfelanzahl1[0]);


        //Einsetzungsverfahren
        for (int i = 0; i <= 7; i++) {
            if (wuerfelwert[i] == 8) {
                if (anzahl != 0) {
                    printf("Wuerfel wird zur%cckgesetzt!\n", (char)129);
                    wuerfelwert[i] = wert;
                    anzahl--;
                }
            }
        }

        //W�rfel sortieren
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

        //Abschlie�ende Sortierung der W�rfel starten und anzeigen
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
        //W�rfelanzahl zur�cksetzen
        wuerfelanzahl[spielernummer] += anzahl;

        //Array zur�cksetzen
        ausgew�hlteAuge[0] = 0;
        ausgew�hlteAuge[1] = 0;
    }
}

void para() {
    //Erforderliche Funktionen aufrufen und ausf�hren und dann alles resetten
    //Geld auf Konten einzahlen
    
    //Casino1
    //Wiederhole solange, bis wir keine Preise oder Spieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(casinowuerfelanzahl1),
            size(casinogelder1));
        cashcounter++)
    {
        //Finde die Position vom W�rfel nit dem h�chsten Wert
        auto location = max_element(begin(casinowuerfelanzahl1),
            end(casinowuerfelanzahl1));
        //Transformiere Position zu einem Index
        auto index = distance(begin(casinowuerfelanzahl1), location);

        //Steigere den Kontostand dieses Spielers
        kontostand[index] += casinogelder1[cashcounter];

        //Setze den Wert auf 0 , um den Spieler nicht erneut gewinnen zu lassen
        casinowuerfelanzahl1[index] = 0;
    }

    //Kontost�nde ausdrucken lassen
    for (const auto& val : kontostand) {
        cout << val << endl;
    }

    //Casino2
    //Wiederhole solange, bis wir keine Preise oder Spieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(casinowuerfelanzahl2),
            size(casinogelder2));
        cashcounter++)
    {
        //Finde die Position vom W�rfel nit dem h�chsten Wert
        auto location = max_element(begin(casinowuerfelanzahl2),
            end(casinowuerfelanzahl2));
        //Transformiere Position zu einem Index
        auto index = distance(begin(casinowuerfelanzahl2), location);

        //Steigere den Kontostand dieses Spielers
        kontostand[index] += casinogelder2[cashcounter];

        //Setze den Wert auf 0 , um den Spieler nicht erneut gewinnen zu lassen
        casinowuerfelanzahl2[index] = 0;
    }

    //Kontost�nde ausdrucken lassen
    for (const auto& val : kontostand) {
        cout << val << endl;
    }

    //Casino3
    //Wiederhole solange, bis wir keine Preise oder Spieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(casinowuerfelanzahl3),
            size(casinogelder3));
        cashcounter++)
    {
        //Finde die Position vom W�rfel nit dem h�chsten Wert
        auto location = max_element(begin(casinowuerfelanzahl3),
            end(casinowuerfelanzahl3));
        //Transformiere Position zu einem Index
        auto index = distance(begin(casinowuerfelanzahl3), location);

        //Steigere den Kontostand dieses Spielers
        kontostand[index] += casinogelder3[cashcounter];

        //Setze den Wert auf 0 , um den Spieler nicht erneut gewinnen zu lassen
        casinowuerfelanzahl3[index] = 0;
    }

    //Kontost�nde ausdrucken lassen
    for (const auto& val : kontostand) {
        cout << val << endl;
    }

    //Casino4
    //Wiederhole solange, bis wir keine Preise oder Spieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(casinowuerfelanzahl4),
            size(casinogelder4));
        cashcounter++)
    {
        //Finde die Position vom W�rfel nit dem h�chsten Wert
        auto location = max_element(begin(casinowuerfelanzahl4),
            end(casinowuerfelanzahl4));
        //Transformiere Position zu einem Index
        auto index = distance(begin(casinowuerfelanzahl4), location);

        //Steigere den Kontostand dieses Spielers
        kontostand[index] += casinogelder4[cashcounter];

        //Setze den Wert auf 0 , um den Spieler nicht erneut gewinnen zu lassen
        casinowuerfelanzahl4[index] = 0;
    }

    //Kontost�nde ausdrucken lassen
    for (const auto& val : kontostand) {
        cout << val << endl;
    }

    //Casino5
    //Wiederhole solange, bis wir keine Preise oder Spieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(casinowuerfelanzahl5),
            size(casinogelder5));
        cashcounter++)
    {
        //Finde die Position vom W�rfel nit dem h�chsten Wert
        auto location = max_element(begin(casinowuerfelanzahl5),
            end(casinowuerfelanzahl5));
        //Transformiere Position zu einem Index
        auto index = distance(begin(casinowuerfelanzahl1), location);

        //Steigere den Kontostand dieses Spielers
        kontostand[index] += casinogelder5[cashcounter];

        //Setze den Wert auf 0 , um den Spieler nicht erneut gewinnen zu lassen
        casinowuerfelanzahl5[index] = 0;
    }

    //Kontost�nde ausdrucken lassen
    for (const auto& val : kontostand) {
        cout << val << endl;
    }

    //Casino6
    //Wiederhole solange, bis wir keine Preise oder Spieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(casinowuerfelanzahl6),
            size(casinogelder6));
        cashcounter++)
    {
        //Finde die Position vom W�rfel nit dem h�chsten Wert
        auto location = max_element(begin(casinowuerfelanzahl6),
            end(casinowuerfelanzahl6));
        //Transformiere Position zu einem Index
        auto index = distance(begin(casinowuerfelanzahl6), location);

        //Steigere den Kontostand dieses Spielers
        kontostand[index] += casinogelder6[cashcounter];

        //Setze den Wert auf 0 , um den Spieler nicht erneut gewinnen zu lassen
        casinowuerfelanzahl6[index] = 0;
    }

    //Kontost�nde ausdrucken lassen
    for (const auto& val : kontostand) {
        cout << val << endl;
    }

    //Resetten von alles n�tigen und wiederanfangen
    for (int i = 0; i <= 2; i++) {
        casinowuerfelanzahl1[i] = 0;
        casinowuerfelanzahl2[i] = 0;
        casinowuerfelanzahl3[i] = 0;
        casinowuerfelanzahl4[i] = 0;
        casinowuerfelanzahl5[i] = 0;
        casinowuerfelanzahl6[i] = 0;
        wuerfelanzahl[i] = 8;
        
    }
    spielerhabenw�rfel = true;
    spielernummer -= 1;

    //Strings f�r UI �ndern
    kontostandtext1 = "Kontostand: $" + to_string(kontostand[0]);
    kontostandtext2 = "Kontostand: $" + to_string(kontostand[1]);
    kontostandtext3 = "Kontostand: $" + to_string(kontostand[2]);
}

void gewinner(int spieler) {
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        printf("FONT-FEHLER: SCHRIFTART KONNTE NICHT GELADEN WERDEN!\nBitte konsultieren Sie die Bedienungsanleitung!\nDr%ccken Sie eine Taste um das Programm zu beenden!", (char)129);
        _getch();
        exit(0);
    }

    sf::RenderWindow gewinnerfenster(sf::VideoMode(1000, 600), "Gewinner", sf::Style::Titlebar);

    string gewinnertext;
    switch (spieler) {
    case 1:
        gewinnertext = "Spieler 1 hat gewonnen!";
        break;
    case 2:
        gewinnertext = "Spieler 2 hat gewonnen!";
        break;
    case 3:
        gewinnertext = "Spieler 3 hat gewonnen!";
        break;
    }

    sf::Event event;
    while (gewinnerfenster.isOpen()) {

        //Nachricht
        sf::Text nachrichtentext(gewinnertext, font);
        nachrichtentext.setCharacterSize(70);
        nachrichtentext.setFillColor(sf::Color::Black);
        //Setze den Text in die Mitte
        float xPos = (1000.0f / 2) - (nachrichtentext.getLocalBounds().width / 2);
        float yPos = (600.0f / 3) - (nachrichtentext.getLocalBounds().height / 2);
        nachrichtentext.setPosition(xPos, yPos);

        //Zeichne Umrandung um Buttons
        sf::ConvexShape menueleiste;
        menueleiste.setPointCount(4);
        menueleiste.setFillColor(sf::Color(245, 245, 220, 255));
        menueleiste.setOutlineThickness(3.0f);
        menueleiste.setPoint(0, sf::Vector2f(0.0f, 450.0f));
        menueleiste.setPoint(1, sf::Vector2f(1000.0f, 450.0f));
        menueleiste.setPoint(2, sf::Vector2f(1000.0f, 600.0f));
        menueleiste.setPoint(3, sf::Vector2f(0.0f, 600.0f));

        //Ja-Button
        Button neuesspielbutton("Neues Spiel", { 200, 100 }, 30, sf::Color::Green, sf::Color::Black);
        neuesspielbutton.setFont(font);
        neuesspielbutton.setPosition(sf::Vector2f(25.0f, 475.0f));

        //Nein-Button
        Button spielladenbutton("Spiel laden", { 200, 100 }, 30, sf::Color::Blue, sf::Color::Black);
        spielladenbutton.setFont(font);
        spielladenbutton.setPosition(sf::Vector2f(275.0f, 475.0f));

        Button menuebutton("Hauptmen�", { 200, 100 }, 30, sf::Color::Yellow, sf::Color::Black);
        menuebutton.setFont(font);
        menuebutton.setPosition(sf::Vector2f(525.0f, 475.0f));

        Button verlassenbutton("Beenden", { 200, 100 }, 30, sf::Color::Red, sf::Color::Black);
        verlassenbutton.setFont(font);
        verlassenbutton.setPosition(sf::Vector2f(775.0f, 475.0f));

        sf::Event event;
        while (gewinnerfenster.pollEvent(event)) {
            // Schlie�t das Fenster, falls schlie�en gedr�ckt wird

            switch (event.type) {
                //Wenn es geklickt hat
            case sf::Event::MouseButtonPressed:
                if (spielladenbutton.isMouseOver(gewinnerfenster)) {
                    gewinnerfenster.close();
                    dateienmanager(false);
                }

                if (neuesspielbutton.isMouseOver(gewinnerfenster)) {
                    gewinnerfenster.close();
                    neuesSpiel();
                }

                if (menuebutton.isMouseOver(gewinnerfenster)) {
                    gewinnerfenster.close();
                    main();
                }

                if (verlassenbutton.isMouseOver(gewinnerfenster)) {
                    exit(0);
                }

                break;

            }
        }

        //Meldung zeichnen
        gewinnerfenster.clear(sf::Color::Yellow);
        gewinnerfenster.draw(menueleiste);
        gewinnerfenster.draw(nachrichtentext);
        neuesspielbutton.drawTo(gewinnerfenster);
        spielladenbutton.drawTo(gewinnerfenster);
        verlassenbutton.drawTo(gewinnerfenster);
        menuebutton.drawTo(gewinnerfenster);

        //Meldung anzeigen
        gewinnerfenster.display();
    }
}

void Spielzeichnung() {
    sf::RenderWindow spiel(sf::VideoMode(1600, 1000), "Las Vegas", sf::Style::Close);
    spiel.setPosition(sf::Vector2i(0, 0));
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


    //while-schleife zeichnen
    while (spiel.isOpen()) {
        spiel.clear(sf::Color(53, 104, 45, 255));

        //Variablen!!
        //F�r Casinos
        //Variablen f�r Casino 1
        int c1spieler1 = casinowuerfelanzahl1[0];
        int c1spieler2 = casinowuerfelanzahl1[1];
        int c1spieler3 = casinowuerfelanzahl1[2];

        //Variablen f�r Casino 2
        int c2spieler1 = casinowuerfelanzahl2[0];
        int c2spieler2 = casinowuerfelanzahl2[1];
        int c2spieler3 = casinowuerfelanzahl2[2];

        //Variablen f�r Casino 3
        int c3spieler1 = casinowuerfelanzahl3[0];
        int c3spieler2 = casinowuerfelanzahl3[1];
        int c3spieler3 = casinowuerfelanzahl3[2];

        //Variablen f�r Casino 4
        int c4spieler1 = casinowuerfelanzahl4[0];
        int c4spieler2 = casinowuerfelanzahl4[1];
        int c4spieler3 = casinowuerfelanzahl4[2];

        //Variablen f�r Casino 5
        int c5spieler1 = casinowuerfelanzahl5[0];
        int c5spieler2 = casinowuerfelanzahl5[1];
        int c5spieler3 = casinowuerfelanzahl5[2];

        //Variablen f�r Casino 6
        int c6spieler1 = casinowuerfelanzahl6[0];
        int c6spieler2 = casinowuerfelanzahl6[1];
        int c6spieler3 = casinowuerfelanzahl6[2];

        //
        //W�rfel zeichnen
        //
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
        //wuerfelsprite.setScale(0.15, 0.15);

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
        //wuerfelsprite2.setScale(0.15, 0.15);

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
        //wuerfelsprite3.setScale(0.15, 0.15);

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
        //wuerfelsprite4.setScale(0.15, 0.15);

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
        //wuerfelsprite5.setScale(0.15, 0.15);

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
        //wuerfelsprite6.setScale(0.15, 0.15);

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
        //wuerfelsprite7.setScale(0.15, 0.15);

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
        //wuerfelsprite8.setScale(0.15, 0.15);

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
        //Rechts das Leaderboard

        sf::ConvexShape rundenkasten;
        rundenkasten.setPointCount(4);
        rundenkasten.setPoint(0, sf::Vector2f(1251.0f, 50.0f));
        rundenkasten.setPoint(1, sf::Vector2f(1251.0f, 150.0f));
        //rundenkasten.setPoint(2, )

        sf::Text rundentext(rundentext, font);
        rundentext.setCharacterSize(30);
        rundentext.setFillColor(sf::Color::Black);
        rundentext.setPosition(1350.0f, 100.0f);
        spiel.draw(rundentext);


        //Spieler 1 Kasten
        sf::ConvexShape spieler1kasten;
        spieler1kasten.setPointCount(4);
        spieler1kasten.setPoint(0, sf::Vector2f(1252.0f, 200.0f));
        spieler1kasten.setPoint(1, sf::Vector2f(1600.0f, 200.0f));
        spieler1kasten.setPoint(2, sf::Vector2f(1600.0f, 305.0f));
        spieler1kasten.setPoint(3, sf::Vector2f(1252.0f, 305.0f));
        spieler1kasten.setFillColor(farben[0] - sf::Color(0, 0, 0, 200));
        spiel.draw(spieler1kasten);

        sf::Text spieler1text("Spieler 1", font);
        spieler1text.setCharacterSize(25);
        spieler1text.setFillColor(farben[0]);
        spieler1text.setPosition(1385.0f, 205.0f);
        spiel.draw(spieler1text);

        sf::Text spieler1geld(kontostandtext1, font);
        spieler1geld.setCharacterSize(20);
        spieler1geld.setFillColor(farben[0]);
        spieler1geld.setPosition(1260.0f, 235.0f);
        spiel.draw(spieler1geld);

        sf::Text spieler1w�rfelzahl(wuerfelzahl1, font);
        spieler1w�rfelzahl.setCharacterSize(20);
        spieler1w�rfelzahl.setFillColor(farben[0]);
        spieler1w�rfelzahl.setPosition(1260.0f, 255.0f);
        spiel.draw(spieler1w�rfelzahl);

        sf::Text spieler1zuletztaktion(letzteaktion[0], font);
        spieler1zuletztaktion.setCharacterSize(20);
        spieler1zuletztaktion.setFillColor(farben[0]);
        spieler1zuletztaktion.setPosition(1260.0f, 275.0f);
        spiel.draw(spieler1zuletztaktion);

        //Spieler 2 Kasten
        sf::ConvexShape spieler2kasten;
        spieler2kasten.setPointCount(4);
        spieler2kasten.setPoint(0, sf::Vector2f(1252.0f, 400.0f));
        spieler2kasten.setPoint(1, sf::Vector2f(1600.0f, 400.0f));
        spieler2kasten.setPoint(2, sf::Vector2f(1600.0f, 505.0f));
        spieler2kasten.setPoint(3, sf::Vector2f(1252.0f, 505.0f));
        spieler2kasten.setFillColor(farben[1] - sf::Color(0, 0, 0, 200));
        spiel.draw(spieler2kasten);

        sf::Text spieler2text("Spieler 2", font);
        spieler2text.setCharacterSize(25);
        spieler2text.setFillColor(farben[1]);
        spieler2text.setPosition(1385.0f, 405.0f);
        spiel.draw(spieler2text);

        sf::Text spieler2geld(kontostandtext2, font);
        spieler2geld.setCharacterSize(20);
        spieler2geld.setFillColor(farben[1]);
        spieler2geld.setPosition(1260.0f, 435.0f);
        spiel.draw(spieler2geld);

        sf::Text spieler2w�rfelzahl(wuerfelzahl2, font);
        spieler2w�rfelzahl.setCharacterSize(20);
        spieler2w�rfelzahl.setFillColor(farben[1]);
        spieler2w�rfelzahl.setPosition(1260.0f, 455.0f);
        spiel.draw(spieler2w�rfelzahl);

        sf::Text spieler2zuletztaktion(letzteaktion[1], font);
        spieler2zuletztaktion.setCharacterSize(20);
        spieler2zuletztaktion.setFillColor(farben[1]);
        spieler2zuletztaktion.setPosition(1260.0f, 475.0f);
        spiel.draw(spieler2zuletztaktion);

        //Spieler 3 Kasten
        sf::ConvexShape spieler3kasten;
        spieler3kasten.setPointCount(4);
        spieler3kasten.setPoint(0, sf::Vector2f(1252.0f, 600.0f));
        spieler3kasten.setPoint(1, sf::Vector2f(1600.0f, 600.0f));
        spieler3kasten.setPoint(2, sf::Vector2f(1600.0f, 705.0f));
        spieler3kasten.setPoint(3, sf::Vector2f(1252.0f, 705.0f));
        spieler3kasten.setFillColor(farben[2] - sf::Color(0, 0, 0, 200));
        spiel.draw(spieler3kasten);

        sf::Text spieler3text("Spieler 3", font);
        spieler3text.setCharacterSize(25);
        spieler3text.setFillColor(farben[2]);
        spieler3text.setPosition(1385.0f, 605.0f);
        spiel.draw(spieler3text);

        sf::Text spieler3geld(kontostandtext3, font);
        spieler3geld.setCharacterSize(20);
        spieler3geld.setFillColor(farben[2]);
        spieler3geld.setPosition(1260.0f, 635.0f);
        spiel.draw(spieler3geld);

        sf::Text spieler3w�rfelzahl(wuerfelzahl3, font);
        spieler3w�rfelzahl.setCharacterSize(20);
        spieler3w�rfelzahl.setFillColor(farben[2]);
        spieler3w�rfelzahl.setPosition(1260.0f, 655.0f);
        spiel.draw(spieler3w�rfelzahl);

        sf::Text spieler3zuletztaktion(letzteaktion[2], font);
        spieler3zuletztaktion.setCharacterSize(20);
        spieler3zuletztaktion.setFillColor(farben[2]);
        spieler3zuletztaktion.setPosition(1260.0f, 675.0f);
        spiel.draw(spieler3zuletztaktion);

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
        golden_nuggetsprite.setPosition({ 20.0f, 400.0f });
        //golden_nuggetsprite.setScale(0.75, 0.75);
        spiel.draw(golden_nuggetsprite);

        sf::Texture caesars;
        caesars.loadFromFile("res/Bilder/Assets/caesars.png");
        caesars.setSmooth(true);
        sf::Sprite caesarssprite(caesars);
        caesarssprite.setPosition({ 220.0f, 400.0f });
        //caesarssprite.setScale(0.75, 0.75);
        spiel.draw(caesarssprite);

        sf::Texture mirage;
        mirage.loadFromFile("res/Bilder/Assets/mirage.png");
        mirage.setSmooth(true);
        sf::Sprite miragesprite(mirage);
        miragesprite.setPosition({ 420.0f, 400.0f });
        spiel.draw(miragesprite);

        sf::Texture sahara;
        sahara.loadFromFile("res/Bilder/Assets/sahara.png");
        sahara.setSmooth(true);
        sf::Sprite saharasprite(sahara);
        saharasprite.setPosition({ 620.0f, 400.0f });
        spiel.draw(saharasprite);

        sf::Texture luxor;
        luxor.loadFromFile("res/Bilder/Assets/luxor.png");
        luxor.setSmooth(true);
        sf::Sprite luxorsprite(luxor);
        luxorsprite.setPosition({ 820.0f, 400.0f });
        spiel.draw(luxorsprite);

        sf::Texture circus;
        circus.loadFromFile("res/Bilder/Assets/circus.png");
        circus.setSmooth(true);
        sf::Sprite circussprite(circus);
        circussprite.setPosition({ 1020.0f, 400.0f });
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
        menuunten.setFillColor(sf::Color(195, 195, 195, 184));
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

        //Eigentlicher Text
        sf::Text subtext(subtextstring, font);
        subtext.setCharacterSize(25);
        //Setze den Text in die Mitte
        float xPos = (0.0f + 1600.0f / 2) - (subtext.getLocalBounds().width / 2);
        float yPos = (900.0f + 35.0f / 2.6) - (subtext.getLocalBounds().height / 2);
        subtext.setPosition(xPos, yPos);
        //subtext.setPosition(sf::Vector2f(600.0f, 900.0f));
        subtext.setFillColor(sf::Color::Black);
        spiel.draw(subtext);

        //Buttonfarbe (195, 195, 195, 184)

        //Buttonfl�chen
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

        //Schritt zur�cksetzen Button
        Button undobutton("Einen Schritt zur�ck", { 170, 50 }, 20, sf::Color::Yellow, sf::Color::Black);
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
        c2gs4sprite.setPosition(sf::Vector2f(225.0f, 115.0f));
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

        //W�rfel auf Casinos zeichnen
        //Aufbau der Namen: c = Casino, z = Zeile, s = Spalte
        //Es wird von oben nach unten gez�hlt sprich von links nach rechts: c1z6s4 ist rechtsunten

        sf::Texture c1z1s1;
        c1z1s1.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z1s1.setSmooth(true);
        sf::Sprite c1z1s1sprite(c1z1s1);
        c1z1s1sprite.setPosition(20.0f, 520.0f);
        c1z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z1s2;
        c1z1s2.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z1s2.setSmooth(true);
        sf::Sprite c1z1s2sprite(c1z1s2);
        c1z1s2sprite.setPosition(65.0f, 520.0f);
        c1z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z1s3;
        c1z1s3.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z1s3.setSmooth(true);
        sf::Sprite c1z1s3sprite(c1z1s3);
        c1z1s3sprite.setPosition(110.0f, 520.0f);
        c1z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z1s4;
        c1z1s4.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z1s4.setSmooth(true);
        sf::Sprite c1z1s4sprite(c1z1s4);
        c1z1s4sprite.setPosition(155.0f, 520.0f);
        c1z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z2s1;
        c1z2s1.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z2s1.setSmooth(true);
        sf::Sprite c1z2s1sprite(c1z2s1);
        c1z2s1sprite.setPosition(20.0f, 565.0f);
        c1z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z2s2;
        c1z2s2.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z2s2.setSmooth(true);
        sf::Sprite c1z2s2sprite(c1z2s2);
        c1z2s2sprite.setPosition(65.0f, 565.0f);
        c1z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z2s3;
        c1z2s3.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z2s3.setSmooth(true);
        sf::Sprite c1z2s3sprite(c1z2s3);
        c1z2s3sprite.setPosition(110.0f, 565.0f);
        c1z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z2s4;
        c1z2s4.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z2s4.setSmooth(true);
        sf::Sprite c1z2s4sprite(c1z2s4);
        c1z2s4sprite.setPosition(155.0f, 565.0f);
        c1z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z3s1;
        c1z3s1.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z3s1.setSmooth(true);
        sf::Sprite c1z3s1sprite(c1z3s1);
        c1z3s1sprite.setPosition(20.0f, 610.0f);
        c1z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z3s2;
        c1z3s2.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z3s2.setSmooth(true);
        sf::Sprite c1z3s2sprite(c1z3s2);
        c1z3s2sprite.setPosition(65.0f, 610.0f);
        c1z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z3s3;
        c1z3s3.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z3s3.setSmooth(true);
        sf::Sprite c1z3s3sprite(c1z3s3);
        c1z3s3sprite.setPosition(110.0f, 610.0f);
        c1z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z3s4;
        c1z3s4.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z3s4.setSmooth(true);
        sf::Sprite c1z3s4sprite(c1z3s4);
        c1z3s4sprite.setPosition(155.0f, 610.0f);
        c1z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z4s1;
        c1z4s1.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z4s1.setSmooth(true);
        sf::Sprite c1z4s1sprite(c1z4s1);
        c1z4s1sprite.setPosition(20.0f, 655.0f);
        c1z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z4s2;
        c1z4s2.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z4s2.setSmooth(true);
        sf::Sprite c1z4s2sprite(c1z4s2);
        c1z4s2sprite.setPosition(65.0f, 655.0f);
        c1z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z4s3;
        c1z4s3.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z4s3.setSmooth(true);
        sf::Sprite c1z4s3sprite(c1z4s3);
        c1z4s3sprite.setPosition(110.0f, 655.0f);
        c1z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z4s4;
        c1z4s4.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z4s4.setSmooth(true);
        sf::Sprite c1z4s4sprite(c1z4s4);
        c1z4s4sprite.setPosition(155.0f, 655.0f);
        c1z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z5s1;
        c1z5s1.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z5s1.setSmooth(true);
        sf::Sprite c1z5s1sprite(c1z5s1);
        c1z5s1sprite.setPosition(20.0f, 700.0f);
        c1z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z5s2;
        c1z5s2.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z5s2.setSmooth(true);
        sf::Sprite c1z5s2sprite(c1z5s2);
        c1z5s2sprite.setPosition(65.0f, 700.0f);
        c1z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z5s3;
        c1z5s3.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z5s3.setSmooth(true);
        sf::Sprite c1z5s3sprite(c1z5s3);
        c1z5s3sprite.setPosition(110.0f, 700.0f);
        c1z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z5s4;
        c1z5s4.loadFromFile("res/Bilder/w�rfel/dice-png-1.png");
        c1z5s4.setSmooth(true);
        sf::Sprite c1z5s4sprite(c1z5s4);
        c1z5s4sprite.setPosition(155.0f, 700.0f);
        c1z5s4sprite.setScale(0.5f, 0.5f);

        //Casino 2
        sf::Texture c2z1s1;
        c2z1s1.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z1s1.setSmooth(true);
        sf::Sprite c2z1s1sprite(c2z1s1);
        c2z1s1sprite.setPosition(220.0f, 520.0f);
        c2z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z1s2;
        c2z1s2.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z1s2.setSmooth(true);
        sf::Sprite c2z1s2sprite(c2z1s2);
        c2z1s2sprite.setPosition(265.0f, 520.0f);
        c2z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z1s3;
        c2z1s3.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z1s3.setSmooth(true);
        sf::Sprite c2z1s3sprite(c2z1s3);
        c2z1s3sprite.setPosition(310.0f, 520.0f);
        c2z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z1s4;
        c2z1s4.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z1s4.setSmooth(true);
        sf::Sprite c2z1s4sprite(c2z1s4);
        c2z1s4sprite.setPosition(355.0f, 520.0f);
        c2z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z2s1;
        c2z2s1.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z2s1.setSmooth(true);
        sf::Sprite c2z2s1sprite(c2z2s1);
        c2z2s1sprite.setPosition(220.0f, 565.0f);
        c2z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z2s2;
        c2z2s2.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z2s2.setSmooth(true);
        sf::Sprite c2z2s2sprite(c2z2s2);
        c2z2s2sprite.setPosition(265.0f, 565.0f);
        c2z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z2s3;
        c2z2s3.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z2s3.setSmooth(true);
        sf::Sprite c2z2s3sprite(c2z2s3);
        c2z2s3sprite.setPosition(310.0f, 565.0f);
        c2z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z2s4;
        c2z2s4.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z2s4.setSmooth(true);
        sf::Sprite c2z2s4sprite(c2z2s4);
        c2z2s4sprite.setPosition(355.0f, 565.0f);
        c2z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z3s1;
        c2z3s1.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z3s1.setSmooth(true);
        sf::Sprite c2z3s1sprite(c2z3s1);
        c2z3s1sprite.setPosition(220.0f, 610.0f);
        c2z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z3s2;
        c2z3s2.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z3s2.setSmooth(true);
        sf::Sprite c2z3s2sprite(c2z3s2);
        c2z3s2sprite.setPosition(265.0f, 610.0f);
        c2z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z3s3;
        c2z3s3.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z3s3.setSmooth(true);
        sf::Sprite c2z3s3sprite(c2z3s3);
        c2z3s3sprite.setPosition(310.0f, 610.0f);
        c2z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z3s4;
        c2z3s4.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z3s4.setSmooth(true);
        sf::Sprite c2z3s4sprite(c2z3s4);
        c2z3s4sprite.setPosition(355.0f, 610.0f);
        c2z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z4s1;
        c2z4s1.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z4s1.setSmooth(true);
        sf::Sprite c2z4s1sprite(c2z4s1);
        c2z4s1sprite.setPosition(220.0f, 655.0f);
        c2z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z4s2;
        c2z4s2.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z4s2.setSmooth(true);
        sf::Sprite c2z4s2sprite(c2z4s2);
        c2z4s2sprite.setPosition(265.0f, 655.0f);
        c2z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z4s3;
        c2z4s3.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z4s3.setSmooth(true);
        sf::Sprite c2z4s3sprite(c2z4s3);
        c2z4s3sprite.setPosition(310.0f, 655.0f);
        c2z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z4s4;
        c2z4s4.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z4s4.setSmooth(true);
        sf::Sprite c2z4s4sprite(c2z4s4);
        c2z4s4sprite.setPosition(355.0f, 655.0f);
        c2z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z5s1;
        c2z5s1.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z5s1.setSmooth(true);
        sf::Sprite c2z5s1sprite(c2z5s1);
        c2z5s1sprite.setPosition(220.0f, 700.0f);
        c2z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z5s2;
        c2z5s2.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z5s2.setSmooth(true);
        sf::Sprite c2z5s2sprite(c2z5s2);
        c2z5s2sprite.setPosition(265.0f, 700.0f);
        c2z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z5s3;
        c2z5s3.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z5s3.setSmooth(true);
        sf::Sprite c2z5s3sprite(c2z5s3);
        c2z5s3sprite.setPosition(310.0f, 700.0f);
        c2z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z5s4;
        c2z5s4.loadFromFile("res/Bilder/w�rfel/dice-png-2.png");
        c2z5s4.setSmooth(true);
        sf::Sprite c2z5s4sprite(c2z5s4);
        c2z5s4sprite.setPosition(355.0f, 700.0f);
        c2z5s4sprite.setScale(0.5f, 0.5f);

        //Casino 3
        sf::Texture c3z1s1;
        c3z1s1.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z1s1.setSmooth(true);
        sf::Sprite c3z1s1sprite(c3z1s1);
        c3z1s1sprite.setPosition(420.0f, 520.0f);
        c3z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z1s2;
        c3z1s2.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z1s2.setSmooth(true);
        sf::Sprite c3z1s2sprite(c3z1s2);
        c3z1s2sprite.setPosition(465.0f, 520.0f);
        c3z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z1s3;
        c3z1s3.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z1s3.setSmooth(true);
        sf::Sprite c3z1s3sprite(c3z1s3);
        c3z1s3sprite.setPosition(510.0f, 520.0f);
        c3z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z1s4;
        c3z1s4.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z1s4.setSmooth(true);
        sf::Sprite c3z1s4sprite(c3z1s4);
        c3z1s4sprite.setPosition(555.0f, 520.0f);
        c3z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z2s1;
        c3z2s1.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z2s1.setSmooth(true);
        sf::Sprite c3z2s1sprite(c3z2s1);
        c3z2s1sprite.setPosition(420.0f, 565.0f);
        c3z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z2s2;
        c3z2s2.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z2s2.setSmooth(true);
        sf::Sprite c3z2s2sprite(c3z2s2);
        c3z2s2sprite.setPosition(465.0f, 565.0f);
        c3z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z2s3;
        c3z2s3.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z2s3.setSmooth(true);
        sf::Sprite c3z2s3sprite(c3z2s3);
        c3z2s3sprite.setPosition(510.0f, 565.0f);
        c3z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z2s4;
        c3z2s4.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z2s4.setSmooth(true);
        sf::Sprite c3z2s4sprite(c3z2s4);
        c3z2s4sprite.setPosition(555.0f, 565.0f);
        c3z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z3s1;
        c3z3s1.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z3s1.setSmooth(true);
        sf::Sprite c3z3s1sprite(c3z3s1);
        c3z3s1sprite.setPosition(420.0f, 610.0f);
        c3z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z3s2;
        c3z3s2.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z3s2.setSmooth(true);
        sf::Sprite c3z3s2sprite(c3z3s2);
        c3z3s2sprite.setPosition(465.0f, 610.0f);
        c3z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z3s3;
        c3z3s3.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z3s3.setSmooth(true);
        sf::Sprite c3z3s3sprite(c3z3s3);
        c3z3s3sprite.setPosition(510.0f, 610.0f);
        c3z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z3s4;
        c3z3s4.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z3s4.setSmooth(true);
        sf::Sprite c3z3s4sprite(c3z3s4);
        c3z3s4sprite.setPosition(555.0f, 610.0f);
        c3z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z4s1;
        c3z4s1.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z4s1.setSmooth(true);
        sf::Sprite c3z4s1sprite(c3z4s1);
        c3z4s1sprite.setPosition(420.0f, 655.0f);
        c3z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z4s2;
        c3z4s2.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z4s2.setSmooth(true);
        sf::Sprite c3z4s2sprite(c3z4s2);
        c3z4s2sprite.setPosition(465.0f, 655.0f);
        c3z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z4s3;
        c3z4s3.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z4s3.setSmooth(true);
        sf::Sprite c3z4s3sprite(c3z4s3);
        c3z4s3sprite.setPosition(510.0f, 655.0f);
        c3z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z4s4;
        c3z4s4.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z4s4.setSmooth(true);
        sf::Sprite c3z4s4sprite(c3z4s4);
        c3z4s4sprite.setPosition(555.0f, 655.0f);
        c3z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z5s1;
        c3z5s1.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z5s1.setSmooth(true);
        sf::Sprite c3z5s1sprite(c3z5s1);
        c3z5s1sprite.setPosition(420.0f, 700.0f);
        c3z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z5s2;
        c3z5s2.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z5s2.setSmooth(true);
        sf::Sprite c3z5s2sprite(c3z5s2);
        c3z5s2sprite.setPosition(465.0f, 700.0f);
        c3z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z5s3;
        c3z5s3.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z5s3.setSmooth(true);
        sf::Sprite c3z5s3sprite(c3z5s3);
        c3z5s3sprite.setPosition(510.0f, 700.0f);
        c3z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z5s4;
        c3z5s4.loadFromFile("res/Bilder/w�rfel/dice-png-3.png");
        c3z5s4.setSmooth(true);
        sf::Sprite c3z5s4sprite(c3z5s4);
        c3z5s4sprite.setPosition(555.0f, 700.0f);
        c3z5s4sprite.setScale(0.5f, 0.5f);

        //Casino 4
        sf::Texture c4z1s1;
        c4z1s1.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z1s1.setSmooth(true);
        sf::Sprite c4z1s1sprite(c4z1s1);
        c4z1s1sprite.setPosition(620.0f, 520.0f);
        c4z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z1s2;
        c4z1s2.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z1s2.setSmooth(true);
        sf::Sprite c4z1s2sprite(c4z1s2);
        c4z1s2sprite.setPosition(665.0f, 520.0f);
        c4z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z1s3;
        c4z1s3.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z1s3.setSmooth(true);
        sf::Sprite c4z1s3sprite(c4z1s3);
        c4z1s3sprite.setPosition(710.0f, 520.0f);
        c4z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z1s4;
        c4z1s4.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z1s4.setSmooth(true);
        sf::Sprite c4z1s4sprite(c4z1s4);
        c4z1s4sprite.setPosition(755.0f, 520.0f);
        c4z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z2s1;
        c4z2s1.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z2s1.setSmooth(true);
        sf::Sprite c4z2s1sprite(c4z2s1);
        c4z2s1sprite.setPosition(620.0f, 565.0f);
        c4z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z2s2;
        c4z2s2.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z2s2.setSmooth(true);
        sf::Sprite c4z2s2sprite(c4z2s2);
        c4z2s2sprite.setPosition(665.0f, 565.0f);
        c4z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z2s3;
        c4z2s3.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z2s3.setSmooth(true);
        sf::Sprite c4z2s3sprite(c4z2s3);
        c4z2s3sprite.setPosition(710.0f, 565.0f);
        c4z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z2s4;
        c4z2s4.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z2s4.setSmooth(true);
        sf::Sprite c4z2s4sprite(c4z2s4);
        c4z2s4sprite.setPosition(755.0f, 565.0f);
        c4z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z3s1;
        c4z3s1.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z3s1.setSmooth(true);
        sf::Sprite c4z3s1sprite(c4z3s1);
        c4z3s1sprite.setPosition(620.0f, 610.0f);
        c4z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z3s2;
        c4z3s2.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z3s2.setSmooth(true);
        sf::Sprite c4z3s2sprite(c4z3s2);
        c4z3s2sprite.setPosition(665.0f, 610.0f);
        c4z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z3s3;
        c4z3s3.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z3s3.setSmooth(true);
        sf::Sprite c4z3s3sprite(c4z3s3);
        c4z3s3sprite.setPosition(710.0f, 610.0f);
        c4z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z3s4;
        c4z3s4.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z3s4.setSmooth(true);
        sf::Sprite c4z3s4sprite(c4z3s4);
        c4z3s4sprite.setPosition(755.0f, 610.0f);
        c4z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z4s1;
        c4z4s1.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z4s1.setSmooth(true);
        sf::Sprite c4z4s1sprite(c4z4s1);
        c4z4s1sprite.setPosition(620.0f, 655.0f);
        c4z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z4s2;
        c4z4s2.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z4s2.setSmooth(true);
        sf::Sprite c4z4s2sprite(c4z4s2);
        c4z4s2sprite.setPosition(665.0f, 655.0f);
        c4z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z4s3;
        c4z4s3.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z4s3.setSmooth(true);
        sf::Sprite c4z4s3sprite(c4z4s3);
        c4z4s3sprite.setPosition(710.0f, 655.0f);
        c4z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z4s4;
        c4z4s4.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z4s4.setSmooth(true);
        sf::Sprite c4z4s4sprite(c4z4s4);
        c4z4s4sprite.setPosition(755.0f, 655.0f);
        c4z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z5s1;
        c4z5s1.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z5s1.setSmooth(true);
        sf::Sprite c4z5s1sprite(c4z5s1);
        c4z5s1sprite.setPosition(620.0f, 700.0f);
        c4z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z5s2;
        c4z5s2.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z5s2.setSmooth(true);
        sf::Sprite c4z5s2sprite(c4z5s2);
        c4z5s2sprite.setPosition(665.0f, 700.0f);
        c4z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z5s3;
        c4z5s3.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z5s3.setSmooth(true);
        sf::Sprite c4z5s3sprite(c4z5s3);
        c4z5s3sprite.setPosition(710.0f, 700.0f);
        c4z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z5s4;
        c4z5s4.loadFromFile("res/Bilder/w�rfel/dice-png-4.png");
        c4z5s4.setSmooth(true);
        sf::Sprite c4z5s4sprite(c4z5s4);
        c4z5s4sprite.setPosition(755.0f, 700.0f);
        c4z5s4sprite.setScale(0.5f, 0.5f);

        //Casino 5
        sf::Texture c5z1s1;
        c5z1s1.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z1s1.setSmooth(true);
        sf::Sprite c5z1s1sprite(c5z1s1);
        c5z1s1sprite.setPosition(820.0f, 520.0f);
        c5z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z1s2;
        c5z1s2.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z1s2.setSmooth(true);
        sf::Sprite c5z1s2sprite(c5z1s2);
        c5z1s2sprite.setPosition(865.0f, 520.0f);
        c5z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z1s3;
        c5z1s3.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z1s3.setSmooth(true);
        sf::Sprite c5z1s3sprite(c5z1s3);
        c5z1s3sprite.setPosition(910.0f, 520.0f);
        c5z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z1s4;
        c5z1s4.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z1s4.setSmooth(true);
        sf::Sprite c5z1s4sprite(c5z1s4);
        c5z1s4sprite.setPosition(955.0f, 520.0f);
        c5z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z2s1;
        c5z2s1.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z2s1.setSmooth(true);
        sf::Sprite c5z2s1sprite(c5z2s1);
        c5z2s1sprite.setPosition(820.0f, 565.0f);
        c5z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z2s2;
        c5z2s2.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z2s2.setSmooth(true);
        sf::Sprite c5z2s2sprite(c5z2s2);
        c5z2s2sprite.setPosition(865.0f, 565.0f);
        c5z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z2s3;
        c5z2s3.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z2s3.setSmooth(true);
        sf::Sprite c5z2s3sprite(c5z2s3);
        c5z2s3sprite.setPosition(910.0f, 565.0f);
        c5z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z2s4;
        c5z2s4.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z2s4.setSmooth(true);
        sf::Sprite c5z2s4sprite(c5z2s4);
        c5z2s4sprite.setPosition(955.0f, 565.0f);
        c5z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z3s1;
        c5z3s1.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z3s1.setSmooth(true);
        sf::Sprite c5z3s1sprite(c5z3s1);
        c5z3s1sprite.setPosition(820.0f, 610.0f);
        c5z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z3s2;
        c5z3s2.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z3s2.setSmooth(true);
        sf::Sprite c5z3s2sprite(c5z3s2);
        c5z3s2sprite.setPosition(865.0f, 610.0f);
        c5z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z3s3;
        c5z3s3.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z3s3.setSmooth(true);
        sf::Sprite c5z3s3sprite(c5z3s3);
        c5z3s3sprite.setPosition(910.0f, 610.0f);
        c5z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z3s4;
        c5z3s4.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z3s4.setSmooth(true);
        sf::Sprite c5z3s4sprite(c5z3s4);
        c5z3s4sprite.setPosition(955.0f, 610.0f);
        c5z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z4s1;
        c5z4s1.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z4s1.setSmooth(true);
        sf::Sprite c5z4s1sprite(c5z4s1);
        c5z4s1sprite.setPosition(820.0f, 655.0f);
        c5z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z4s2;
        c5z4s2.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z4s2.setSmooth(true);
        sf::Sprite c5z4s2sprite(c5z4s2);
        c5z4s2sprite.setPosition(865.0f, 655.0f);
        c5z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z4s3;
        c5z4s3.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z4s3.setSmooth(true);
        sf::Sprite c5z4s3sprite(c5z4s3);
        c5z4s3sprite.setPosition(910.0f, 655.0f);
        c5z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z4s4;
        c5z4s4.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z4s4.setSmooth(true);
        sf::Sprite c5z4s4sprite(c5z4s4);
        c5z4s4sprite.setPosition(955.0f, 655.0f);
        c5z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z5s1;
        c5z5s1.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z5s1.setSmooth(true);
        sf::Sprite c5z5s1sprite(c5z5s1);
        c5z5s1sprite.setPosition(820.0f, 700.0f);
        c5z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z5s2;
        c5z5s2.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z5s2.setSmooth(true);
        sf::Sprite c5z5s2sprite(c5z5s2);
        c5z5s2sprite.setPosition(865.0f, 700.0f);
        c5z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z5s3;
        c5z5s3.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z5s3.setSmooth(true);
        sf::Sprite c5z5s3sprite(c5z5s3);
        c5z5s3sprite.setPosition(910.0f, 700.0f);
        c5z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z5s4;
        c5z5s4.loadFromFile("res/Bilder/w�rfel/dice-png-5.png");
        c5z5s4.setSmooth(true);
        sf::Sprite c5z5s4sprite(c5z5s4);
        c5z5s4sprite.setPosition(955.0f, 700.0f);
        c5z5s4sprite.setScale(0.5f, 0.5f);

        //Casino 6
        sf::Texture c6z1s1;
        c6z1s1.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z1s1.setSmooth(true);
        sf::Sprite c6z1s1sprite(c6z1s1);
        c6z1s1sprite.setPosition(1020.0f, 520.0f);
        c6z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z1s2;
        c6z1s2.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z1s2.setSmooth(true);
        sf::Sprite c6z1s2sprite(c6z1s2);
        c6z1s2sprite.setPosition(1065.0f, 520.0f);
        c6z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z1s3;
        c6z1s3.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z1s3.setSmooth(true);
        sf::Sprite c6z1s3sprite(c6z1s3);
        c6z1s3sprite.setPosition(1110.0f, 520.0f);
        c6z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z1s4;
        c6z1s4.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z1s4.setSmooth(true);
        sf::Sprite c6z1s4sprite(c6z1s4);
        c6z1s4sprite.setPosition(1155.0f, 520.0f);
        c6z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z2s1;
        c6z2s1.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z2s1.setSmooth(true);
        sf::Sprite c6z2s1sprite(c6z2s1);
        c6z2s1sprite.setPosition(1020.0f, 565.0f);
        c6z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z2s2;
        c6z2s2.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z2s2.setSmooth(true);
        sf::Sprite c6z2s2sprite(c6z2s2);
        c6z2s2sprite.setPosition(1065.0f, 565.0f);
        c6z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z2s3;
        c6z2s3.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z2s3.setSmooth(true);
        sf::Sprite c6z2s3sprite(c6z2s3);
        c6z2s3sprite.setPosition(1110.0f, 565.0f);
        c6z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z2s4;
        c6z2s4.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z2s4.setSmooth(true);
        sf::Sprite c6z2s4sprite(c6z2s4);
        c6z2s4sprite.setPosition(1155.0f, 565.0f);
        c6z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z3s1;
        c6z3s1.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z3s1.setSmooth(true);
        sf::Sprite c6z3s1sprite(c6z3s1);
        c6z3s1sprite.setPosition(1020.0f, 610.0f);
        c6z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z3s2;
        c6z3s2.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z3s2.setSmooth(true);
        sf::Sprite c6z3s2sprite(c6z3s2);
        c6z3s2sprite.setPosition(1065.0f, 610.0f);
        c6z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z3s3;
        c6z3s3.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z3s3.setSmooth(true);
        sf::Sprite c6z3s3sprite(c6z3s3);
        c6z3s3sprite.setPosition(1110.0f, 610.0f);
        c6z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z3s4;
        c6z3s4.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z3s4.setSmooth(true);
        sf::Sprite c6z3s4sprite(c6z3s4);
        c6z3s4sprite.setPosition(1155.0f, 610.0f);
        c6z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z4s1;
        c6z4s1.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z4s1.setSmooth(true);
        sf::Sprite c6z4s1sprite(c6z4s1);
        c6z4s1sprite.setPosition(1020.0f, 655.0f);
        c6z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z4s2;
        c6z4s2.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z4s2.setSmooth(true);
        sf::Sprite c6z4s2sprite(c6z4s2);
        c6z4s2sprite.setPosition(1065.0f, 655.0f);
        c6z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z4s3;
        c6z4s3.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z4s3.setSmooth(true);
        sf::Sprite c6z4s3sprite(c6z4s3);
        c6z4s3sprite.setPosition(1110.0f, 655.0f);
        c6z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z4s4;
        c6z4s4.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z4s4.setSmooth(true);
        sf::Sprite c6z4s4sprite(c6z4s4);
        c6z4s4sprite.setPosition(1155.0f, 655.0f);
        c6z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z5s1;
        c6z5s1.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z5s1.setSmooth(true);
        sf::Sprite c6z5s1sprite(c6z5s1);
        c6z5s1sprite.setPosition(1020.0f, 700.0f);
        c6z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z5s2;
        c6z5s2.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z5s2.setSmooth(true);
        sf::Sprite c6z5s2sprite(c6z5s2);
        c6z5s2sprite.setPosition(1065.0f, 700.0f);
        c6z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z5s3;
        c6z5s3.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z5s3.setSmooth(true);
        sf::Sprite c6z5s3sprite(c6z5s3);
        c6z5s3sprite.setPosition(1110.0f, 700.0f);
        c6z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z5s4;
        c6z5s4.loadFromFile("res/Bilder/w�rfel/dice-png-6.png");
        c6z5s4.setSmooth(true);
        sf::Sprite c6z5s4sprite(c6z5s4);
        c6z5s4sprite.setPosition(1155.0f, 700.0f);
        c6z5s4sprite.setScale(0.5f, 0.5f);

        //Algorithmus f�r kleine W�rfel bei den Casinos
        //Casino 1
        //c1z1s1
        if (c1spieler1 != 0) {
            c1z1s1sprite.setColor(farben[0]);
            spiel.draw(c1z1s1sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z1s1sprite.setColor(farben[1]);
            spiel.draw(c1z1s1sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z1s1sprite.setColor(farben[2]);
            spiel.draw(c1z1s1sprite);
            c1spieler3--;   
        }
        else {
            //Mach nichts
        }
        
        //c1z1s2
        if (c1spieler1 != 0) {
            c1z1s2sprite.setColor(farben[0]);
            spiel.draw(c1z1s2sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z1s2sprite.setColor(farben[1]);
            spiel.draw(c1z1s2sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z1s2sprite.setColor(farben[2]);
            spiel.draw(c1z1s2sprite);
            c1spieler3--;
        }
        
        //c1z1s3
        if (c1spieler1 != 0) {
            c1z1s3sprite.setColor(farben[0]);
            spiel.draw(c1z1s3sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z1s3sprite.setColor(farben[1]);
            spiel.draw(c1z1s3sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z1s3sprite.setColor(farben[2]);
            spiel.draw(c1z1s3sprite);
            c1spieler3--;
        }

        //c1z1s4
        if (c1spieler1 != 0) {
            c1z1s4sprite.setColor(farben[0]);
            spiel.draw(c1z1s4sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z1s4sprite.setColor(farben[1]);
            spiel.draw(c1z1s4sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z1s4sprite.setColor(farben[2]);
            spiel.draw(c1z1s4sprite);
            c1spieler3--;
        }

        //c1z2s1
        if (c1spieler1 != 0) {
            c1z2s1sprite.setColor(farben[0]);
            spiel.draw(c1z2s1sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z2s1sprite.setColor(farben[1]);
            spiel.draw(c1z2s1sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z2s1sprite.setColor(farben[2]);
            spiel.draw(c1z2s1sprite);
            c1spieler3--;
        }

        //c1z2s2
        if (c1spieler1 != 0) {
            c1z2s2sprite.setColor(farben[0]);
            spiel.draw(c1z2s2sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z2s2sprite.setColor(farben[1]);
            spiel.draw(c1z2s2sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z2s2sprite.setColor(farben[2]);
            spiel.draw(c1z2s2sprite);
            c1spieler3--;
        }

        //c1z2s3
        if (c1spieler1 != 0) {
            c1z2s3sprite.setColor(farben[0]);
            spiel.draw(c1z2s3sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z2s3sprite.setColor(farben[1]);
            spiel.draw(c1z2s3sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z2s3sprite.setColor(farben[2]);
            spiel.draw(c1z2s3sprite);
            c1spieler3--;
        }

        //c1z2s4
        if (c1spieler1 != 0) {
            c1z2s4sprite.setColor(farben[0]);
            spiel.draw(c1z2s4sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z2s4sprite.setColor(farben[1]);
            spiel.draw(c1z2s4sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z2s4sprite.setColor(farben[2]);
            spiel.draw(c1z2s4sprite);
            c1spieler3--;
        }

        //c1z3s1
        if (c1spieler1 != 0) {
            c1z3s1sprite.setColor(farben[0]);
            spiel.draw(c1z3s1sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z3s1sprite.setColor(farben[1]);
            spiel.draw(c1z3s1sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z3s1sprite.setColor(farben[2]);
            spiel.draw(c1z3s1sprite);
            c1spieler3--;
        }

        //c1z3s2
        if (c1spieler1 != 0) {
            c1z3s2sprite.setColor(farben[0]);
            spiel.draw(c1z3s2sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z3s2sprite.setColor(farben[1]);
            spiel.draw(c1z3s2sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z3s2sprite.setColor(farben[2]);
            spiel.draw(c1z3s2sprite);
            c1spieler3--;
        }

        //c1z3s3
        if (c1spieler1 != 0) {
            c1z3s3sprite.setColor(farben[0]);
            spiel.draw(c1z3s3sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z3s3sprite.setColor(farben[1]);
            spiel.draw(c1z3s3sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z3s3sprite.setColor(farben[2]);
            spiel.draw(c1z3s3sprite);
            c1spieler3--;
        }

        //c1z3s4
        if (c1spieler1 != 0) {
            c1z3s4sprite.setColor(farben[0]);
            spiel.draw(c1z3s4sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z3s4sprite.setColor(farben[1]);
            spiel.draw(c1z3s4sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z3s4sprite.setColor(farben[2]);
            spiel.draw(c1z3s4sprite);
            c1spieler3--;
        }

        //c1z4s1
        if (c1spieler1 != 0) {
            c1z4s1sprite.setColor(farben[0]);
            spiel.draw(c1z4s1sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z4s1sprite.setColor(farben[1]);
            spiel.draw(c1z4s1sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z4s1sprite.setColor(farben[2]);
            spiel.draw(c1z4s1sprite);
            c1spieler3--;
        }

        //c1z4s2
        if (c1spieler1 != 0) {
            c1z4s2sprite.setColor(farben[0]);
            spiel.draw(c1z4s2sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z4s2sprite.setColor(farben[1]);
            spiel.draw(c1z4s2sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z4s2sprite.setColor(farben[2]);
            spiel.draw(c1z4s2sprite);
            c1spieler3--;
        }

        //c1z4s3
        if (c1spieler1 != 0) {
            c1z4s3sprite.setColor(farben[0]);
            spiel.draw(c1z4s3sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z4s3sprite.setColor(farben[1]);
            spiel.draw(c1z4s3sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z4s3sprite.setColor(farben[2]);
            spiel.draw(c1z4s3sprite);
            c1spieler3--;
        }

        //c1z4s4
        if (c1spieler1 != 0) {
            c1z4s4sprite.setColor(farben[0]);
            spiel.draw(c1z4s4sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z4s4sprite.setColor(farben[1]);
            spiel.draw(c1z4s4sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z4s4sprite.setColor(farben[2]);
            spiel.draw(c1z4s4sprite);
            c1spieler3--;
        }

        //c1z5s1
        if (c1spieler1 != 0) {
            c1z5s1sprite.setColor(farben[0]);
            spiel.draw(c1z5s1sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z5s1sprite.setColor(farben[1]);
            spiel.draw(c1z5s1sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z5s1sprite.setColor(farben[2]);
            spiel.draw(c1z5s1sprite);
            c1spieler3--;
        }

        //c1z5s2
        if (c1spieler1 != 0) {
            c1z5s2sprite.setColor(farben[0]);
            spiel.draw(c1z5s2sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z5s2sprite.setColor(farben[1]);
            spiel.draw(c1z5s2sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z5s2sprite.setColor(farben[2]);
            spiel.draw(c1z5s2sprite);
            c1spieler3--;
        }

        //c1z5s3
        if (c1spieler1 != 0) {
            c1z5s3sprite.setColor(farben[0]);
            spiel.draw(c1z5s3sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z5s3sprite.setColor(farben[1]);
            spiel.draw(c1z5s3sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z5s3sprite.setColor(farben[2]);
            spiel.draw(c1z5s3sprite);
            c1spieler3--;
        }

        //c1z5s4
        if (c1spieler1 != 0) {
            c1z5s4sprite.setColor(farben[0]);
            spiel.draw(c1z5s4sprite);
            c1spieler1--;
        }
        else if (c1spieler2 != 0) {
            c1z5s4sprite.setColor(farben[1]);
            spiel.draw(c1z5s4sprite);
            c1spieler2--;
        }
        else if (c1spieler3 != 0) {
            c1z5s4sprite.setColor(farben[2]);
            spiel.draw(c1z5s4sprite);
            c1spieler3--;
        }

        //Casino 2
        //c2z1s1
        if (c2spieler1 != 0) {
            c2z1s1sprite.setColor(farben[0]);
            spiel.draw(c2z1s1sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z1s1sprite.setColor(farben[1]);
            spiel.draw(c2z1s1sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z1s1sprite.setColor(farben[2]);
            spiel.draw(c2z1s1sprite);
            c2spieler3--;
        }

        //c2z1s2
        if (c2spieler1 != 0) {
            c2z1s2sprite.setColor(farben[0]);
            spiel.draw(c2z1s2sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z1s2sprite.setColor(farben[1]);
            spiel.draw(c2z1s2sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z1s2sprite.setColor(farben[2]);
            spiel.draw(c2z1s2sprite);
            c2spieler3--;
        }

        //c2z1s3
        if (c2spieler1 != 0) {
            c2z1s3sprite.setColor(farben[0]);
            spiel.draw(c2z1s3sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z1s3sprite.setColor(farben[1]);
            spiel.draw(c2z1s3sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z1s3sprite.setColor(farben[2]);
            spiel.draw(c2z1s3sprite);
            c2spieler3--;
        }

        //c2z1s4
        if (c2spieler1 != 0) {
            c2z1s4sprite.setColor(farben[0]);
            spiel.draw(c2z1s4sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z1s4sprite.setColor(farben[1]);
            spiel.draw(c2z1s4sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z1s4sprite.setColor(farben[2]);
            spiel.draw(c2z1s4sprite);
            c2spieler3--;
        }

        //c2z2s1
        if (c2spieler1 != 0) {
            c2z2s1sprite.setColor(farben[0]);
            spiel.draw(c2z2s1sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z2s1sprite.setColor(farben[1]);
            spiel.draw(c2z2s1sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z2s1sprite.setColor(farben[2]);
            spiel.draw(c2z2s1sprite);
            c2spieler3--;
        }

        //c2z2s2
        if (c2spieler1 != 0) {
            c2z2s2sprite.setColor(farben[0]);
            spiel.draw(c2z2s2sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z2s2sprite.setColor(farben[1]);
            spiel.draw(c2z2s2sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z2s2sprite.setColor(farben[2]);
            spiel.draw(c2z2s2sprite);
            c2spieler3--;
        }

        //c2z2s3
        if (c2spieler1 != 0) {
            c2z2s3sprite.setColor(farben[0]);
            spiel.draw(c2z2s3sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z2s3sprite.setColor(farben[1]);
            spiel.draw(c2z2s3sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z2s3sprite.setColor(farben[2]);
            spiel.draw(c2z2s3sprite);
            c2spieler3--;
        }

        //c2z2s4
        if (c2spieler1 != 0) {
            c2z2s4sprite.setColor(farben[0]);
            spiel.draw(c2z2s4sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z2s4sprite.setColor(farben[1]);
            spiel.draw(c2z2s4sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z2s4sprite.setColor(farben[2]);
            spiel.draw(c2z2s4sprite);
            c2spieler3--;
        }

        //c2z3s1
        if (c2spieler1 != 0) {
            c2z3s1sprite.setColor(farben[0]);
            spiel.draw(c2z3s1sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z3s1sprite.setColor(farben[1]);
            spiel.draw(c2z3s1sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z3s1sprite.setColor(farben[2]);
            spiel.draw(c2z3s1sprite);
            c2spieler3--;
        }

        //c2z3s2
        if (c2spieler1 != 0) {
            c2z3s2sprite.setColor(farben[0]);
            spiel.draw(c2z3s2sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z3s2sprite.setColor(farben[1]);
            spiel.draw(c2z3s2sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z3s2sprite.setColor(farben[2]);
            spiel.draw(c2z3s2sprite);
            c2spieler3--;
        }

        //c2z3s3
        if (c2spieler1 != 0) {
            c2z3s3sprite.setColor(farben[0]);
            spiel.draw(c2z3s3sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z3s3sprite.setColor(farben[1]);
            spiel.draw(c2z3s3sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z3s3sprite.setColor(farben[2]);
            spiel.draw(c2z3s3sprite);
            c2spieler3--;
        }

        //c2z3s4
        if (c2spieler1 != 0) {
            c2z3s4sprite.setColor(farben[0]);
            spiel.draw(c2z3s4sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z3s4sprite.setColor(farben[1]);
            spiel.draw(c2z3s4sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z3s4sprite.setColor(farben[2]);
            spiel.draw(c2z3s4sprite);
            c2spieler3--;
        }

        //c2z4s1
        if (c2spieler1 != 0) {
            c2z4s1sprite.setColor(farben[0]);
            spiel.draw(c2z4s1sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z4s1sprite.setColor(farben[1]);
            spiel.draw(c2z4s1sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z4s1sprite.setColor(farben[2]);
            spiel.draw(c2z4s1sprite);
            c2spieler3--;
        }

        //c2z4s2
        if (c2spieler1 != 0) {
            c2z4s2sprite.setColor(farben[0]);
            spiel.draw(c2z4s2sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z4s2sprite.setColor(farben[1]);
            spiel.draw(c2z4s2sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z4s2sprite.setColor(farben[2]);
            spiel.draw(c2z4s2sprite);
            c2spieler3--;
        }

        //c2z4s3
        if (c2spieler1 != 0) {
            c2z4s3sprite.setColor(farben[0]);
            spiel.draw(c2z4s3sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z4s3sprite.setColor(farben[1]);
            spiel.draw(c2z4s3sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z4s3sprite.setColor(farben[2]);
            spiel.draw(c2z4s3sprite);
            c2spieler3--;
        }

        //c2z4s4
        if (c2spieler1 != 0) {
            c2z4s4sprite.setColor(farben[0]);
            spiel.draw(c2z4s4sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z4s4sprite.setColor(farben[1]);
            spiel.draw(c2z4s4sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z4s4sprite.setColor(farben[2]);
            spiel.draw(c2z4s4sprite);
            c2spieler3--;
        }

        //c2z5s1
        if (c2spieler1 != 0) {
            c2z5s1sprite.setColor(farben[0]);
            spiel.draw(c2z5s1sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z5s1sprite.setColor(farben[1]);
            spiel.draw(c2z5s1sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z5s1sprite.setColor(farben[2]);
            spiel.draw(c2z5s1sprite);
            c2spieler3--;
        }

        //c2z5s2
        if (c2spieler1 != 0) {
            c2z5s2sprite.setColor(farben[0]);
            spiel.draw(c2z5s2sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z5s2sprite.setColor(farben[1]);
            spiel.draw(c2z5s2sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z5s2sprite.setColor(farben[2]);
            spiel.draw(c2z5s2sprite);
            c2spieler3--;
        }

        //c2z5s3
        if (c2spieler1 != 0) {
            c2z5s3sprite.setColor(farben[0]);
            spiel.draw(c2z5s3sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z5s3sprite.setColor(farben[1]);
            spiel.draw(c2z5s3sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z5s3sprite.setColor(farben[2]);
            spiel.draw(c2z5s3sprite);
            c2spieler3--;
        }

        //c2z5s4
        if (c2spieler1 != 0) {
            c2z5s4sprite.setColor(farben[0]);
            spiel.draw(c2z5s4sprite);
            c2spieler1--;
        }
        else if (c2spieler2 != 0) {
            c2z5s4sprite.setColor(farben[1]);
            spiel.draw(c2z5s4sprite);
            c2spieler2--;
        }
        else if (c2spieler3 != 0) {
            c2z5s4sprite.setColor(farben[2]);
            spiel.draw(c2z5s4sprite);
            c2spieler3--;
        }

        //Casino 3
        //c3z1s1
        if (c3spieler1 != 0) {
            c3z1s1sprite.setColor(farben[0]);
            spiel.draw(c3z1s1sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z1s1sprite.setColor(farben[1]);
            spiel.draw(c3z1s1sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z1s1sprite.setColor(farben[2]);
            spiel.draw(c3z1s1sprite);
            c3spieler3--;
        }

        //c3z1s2
        if (c3spieler1 != 0) {
            c3z1s2sprite.setColor(farben[0]);
            spiel.draw(c3z1s2sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z1s2sprite.setColor(farben[1]);
            spiel.draw(c3z1s2sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z1s2sprite.setColor(farben[2]);
            spiel.draw(c3z1s2sprite);
            c3spieler3--;
        }

        //c3z1s3
        if (c3spieler1 != 0) {
            c3z1s3sprite.setColor(farben[0]);
            spiel.draw(c3z1s3sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z1s3sprite.setColor(farben[1]);
            spiel.draw(c3z1s3sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z1s3sprite.setColor(farben[2]);
            spiel.draw(c3z1s3sprite);
            c3spieler3--;
        }

        //c3z1s4
        if (c3spieler1 != 0) {
            c3z1s4sprite.setColor(farben[0]);
            spiel.draw(c3z1s4sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z1s4sprite.setColor(farben[1]);
            spiel.draw(c3z1s4sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z1s4sprite.setColor(farben[2]);
            spiel.draw(c3z1s4sprite);
            c3spieler3--;
        }

        //c3z2s1
        if (c3spieler1 != 0) {
            c3z2s1sprite.setColor(farben[0]);
            spiel.draw(c3z2s1sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z2s1sprite.setColor(farben[1]);
            spiel.draw(c3z2s1sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z2s1sprite.setColor(farben[2]);
            spiel.draw(c3z2s1sprite);
            c3spieler3--;
        }

        //c3z2s2
        if (c3spieler1 != 0) {
            c3z2s2sprite.setColor(farben[0]);
            spiel.draw(c3z2s2sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z2s2sprite.setColor(farben[1]);
            spiel.draw(c3z2s2sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z2s2sprite.setColor(farben[2]);
            spiel.draw(c3z2s2sprite);
            c3spieler3--;
        }

        //c3z2s3
        if (c3spieler1 != 0) {
            c3z2s3sprite.setColor(farben[0]);
            spiel.draw(c3z2s3sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z2s3sprite.setColor(farben[1]);
            spiel.draw(c3z2s3sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z2s3sprite.setColor(farben[2]);
            spiel.draw(c3z2s3sprite);
            c3spieler3--;
        }

        //c3z2s4
        if (c3spieler1 != 0) {
            c3z2s4sprite.setColor(farben[0]);
            spiel.draw(c3z2s4sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z2s4sprite.setColor(farben[1]);
            spiel.draw(c3z2s4sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z2s4sprite.setColor(farben[2]);
            spiel.draw(c3z2s4sprite);
            c3spieler3--;
        }

        //c3z3s1
        if (c3spieler1 != 0) {
            c3z3s1sprite.setColor(farben[0]);
            spiel.draw(c3z3s1sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z3s1sprite.setColor(farben[1]);
            spiel.draw(c3z3s1sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z3s1sprite.setColor(farben[2]);
            spiel.draw(c3z3s1sprite);
            c3spieler3--;
        }

        //c3z3s2
        if (c3spieler1 != 0) {
            c3z3s2sprite.setColor(farben[0]);
            spiel.draw(c3z3s2sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z3s2sprite.setColor(farben[1]);
            spiel.draw(c3z3s2sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z3s2sprite.setColor(farben[2]);
            spiel.draw(c3z3s2sprite);
            c3spieler3--;
        }

        //c3z3s3
        if (c3spieler1 != 0) {
            c3z3s3sprite.setColor(farben[0]);
            spiel.draw(c3z3s3sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z3s3sprite.setColor(farben[1]);
            spiel.draw(c3z3s3sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z3s3sprite.setColor(farben[2]);
            spiel.draw(c3z3s3sprite);
            c3spieler3--;
        }

        //c3z3s4
        if (c3spieler1 != 0) {
            c3z3s4sprite.setColor(farben[0]);
            spiel.draw(c3z3s4sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z3s4sprite.setColor(farben[1]);
            spiel.draw(c3z3s4sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z3s4sprite.setColor(farben[2]);
            spiel.draw(c3z3s4sprite);
            c3spieler3--;
        }

        //c3z4s1
        if (c3spieler1 != 0) {
            c3z4s1sprite.setColor(farben[0]);
            spiel.draw(c3z4s1sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z4s1sprite.setColor(farben[1]);
            spiel.draw(c3z4s1sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z4s1sprite.setColor(farben[2]);
            spiel.draw(c3z4s1sprite);
            c3spieler3--;
        }

        //c3z4s2
        if (c3spieler1 != 0) {
            c3z4s2sprite.setColor(farben[0]);
            spiel.draw(c3z4s2sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z4s2sprite.setColor(farben[1]);
            spiel.draw(c3z4s2sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z4s2sprite.setColor(farben[2]);
            spiel.draw(c3z4s2sprite);
            c3spieler3--;
        }

        //c3z4s3
        if (c3spieler1 != 0) {
            c3z4s3sprite.setColor(farben[0]);
            spiel.draw(c3z4s3sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z4s3sprite.setColor(farben[1]);
            spiel.draw(c3z4s3sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z4s3sprite.setColor(farben[2]);
            spiel.draw(c3z4s3sprite);
            c3spieler3--;
        }

        //c3z4s4
        if (c3spieler1 != 0) {
            c3z4s4sprite.setColor(farben[0]);
            spiel.draw(c3z4s4sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z4s4sprite.setColor(farben[1]);
            spiel.draw(c3z4s4sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z4s4sprite.setColor(farben[2]);
            spiel.draw(c3z4s4sprite);
            c3spieler3--;
        }

        //c3z5s1
        if (c3spieler1 != 0) {
            c3z5s1sprite.setColor(farben[0]);
            spiel.draw(c3z5s1sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z5s1sprite.setColor(farben[1]);
            spiel.draw(c3z5s1sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z5s1sprite.setColor(farben[2]);
            spiel.draw(c3z5s1sprite);
            c3spieler3--;
        }

        //c3z5s2
        if (c3spieler1 != 0) {
            c3z5s2sprite.setColor(farben[0]);
            spiel.draw(c3z5s2sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z5s2sprite.setColor(farben[1]);
            spiel.draw(c3z5s2sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z5s2sprite.setColor(farben[2]);
            spiel.draw(c3z5s2sprite);
            c3spieler3--;
        }

        //c3z5s3
        if (c3spieler1 != 0) {
            c3z5s3sprite.setColor(farben[0]);
            spiel.draw(c3z5s3sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z5s3sprite.setColor(farben[1]);
            spiel.draw(c3z5s3sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z5s3sprite.setColor(farben[2]);
            spiel.draw(c3z5s3sprite);
            c3spieler3--;
        }

        //c3z5s4
        if (c3spieler1 != 0) {
            c3z5s4sprite.setColor(farben[0]);
            spiel.draw(c3z5s4sprite);
            c3spieler1--;
        }
        else if (c3spieler2 != 0) {
            c3z5s4sprite.setColor(farben[1]);
            spiel.draw(c3z5s4sprite);
            c3spieler2--;
        }
        else if (c3spieler3 != 0) {
            c3z5s4sprite.setColor(farben[2]);
            spiel.draw(c3z5s4sprite);
            c3spieler3--;
        }

        //Casino 4
        //c4z1s1
        if (c4spieler1 != 0) {
            c4z1s1sprite.setColor(farben[0]);
            spiel.draw(c4z1s1sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z1s1sprite.setColor(farben[1]);
            spiel.draw(c4z1s1sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z1s1sprite.setColor(farben[2]);
            spiel.draw(c4z1s1sprite);
            c4spieler3--;
        }

        //c4z1s2
        if (c4spieler1 != 0) {
            c4z1s2sprite.setColor(farben[0]);
            spiel.draw(c4z1s2sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z1s2sprite.setColor(farben[1]);
            spiel.draw(c4z1s2sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z1s2sprite.setColor(farben[2]);
            spiel.draw(c4z1s2sprite);
            c4spieler3--;
        }

        //c4z1s3
        if (c4spieler1 != 0) {
            c4z1s3sprite.setColor(farben[0]);
            spiel.draw(c4z1s3sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z1s3sprite.setColor(farben[1]);
            spiel.draw(c4z1s3sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z1s3sprite.setColor(farben[2]);
            spiel.draw(c4z1s3sprite);
            c4spieler3--;
        }

        //c4z1s4
        if (c4spieler1 != 0) {
            c4z1s4sprite.setColor(farben[0]);
            spiel.draw(c4z1s4sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z1s4sprite.setColor(farben[1]);
            spiel.draw(c4z1s4sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z1s4sprite.setColor(farben[2]);
            spiel.draw(c4z1s4sprite);
            c4spieler3--;
        }

        //c4z2s1
        if (c4spieler1 != 0) {
            c4z2s1sprite.setColor(farben[0]);
            spiel.draw(c4z2s1sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z2s1sprite.setColor(farben[1]);
            spiel.draw(c4z2s1sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z2s1sprite.setColor(farben[2]);
            spiel.draw(c4z2s1sprite);
            c4spieler3--;
        }

        //c4z2s2
        if (c4spieler1 != 0) {
            c4z2s2sprite.setColor(farben[0]);
            spiel.draw(c4z2s2sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z2s2sprite.setColor(farben[1]);
            spiel.draw(c4z2s2sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z2s2sprite.setColor(farben[2]);
            spiel.draw(c4z2s2sprite);
            c4spieler3--;
        }

        //c4z2s3
        if (c4spieler1 != 0) {
            c4z2s3sprite.setColor(farben[0]);
            spiel.draw(c4z2s3sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z2s3sprite.setColor(farben[1]);
            spiel.draw(c4z2s3sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z2s3sprite.setColor(farben[2]);
            spiel.draw(c4z2s3sprite);
            c4spieler3--;
        }

        //c4z2s4
        if (c4spieler1 != 0) {
            c4z2s4sprite.setColor(farben[0]);
            spiel.draw(c4z2s4sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z2s4sprite.setColor(farben[1]);
            spiel.draw(c4z2s4sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z2s4sprite.setColor(farben[2]);
            spiel.draw(c4z2s4sprite);
            c4spieler3--;
        }

        //c4z3s1
        if (c4spieler1 != 0) {
            c4z3s1sprite.setColor(farben[0]);
            spiel.draw(c4z3s1sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z3s1sprite.setColor(farben[1]);
            spiel.draw(c4z3s1sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z3s1sprite.setColor(farben[2]);
            spiel.draw(c4z3s1sprite);
            c4spieler3--;
        }

        //c4z3s2
        if (c4spieler1 != 0) {
            c4z3s2sprite.setColor(farben[0]);
            spiel.draw(c4z3s2sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z3s2sprite.setColor(farben[1]);
            spiel.draw(c4z3s2sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z3s2sprite.setColor(farben[2]);
            spiel.draw(c4z3s2sprite);
            c4spieler3--;
        }

        //c4z3s3
        if (c4spieler1 != 0) {
            c4z3s3sprite.setColor(farben[0]);
            spiel.draw(c4z3s3sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z3s3sprite.setColor(farben[1]);
            spiel.draw(c4z3s3sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z3s3sprite.setColor(farben[2]);
            spiel.draw(c4z3s3sprite);
            c4spieler3--;
        }

        //c4z3s4
        if (c4spieler1 != 0) {
            c4z3s4sprite.setColor(farben[0]);
            spiel.draw(c4z3s4sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z3s4sprite.setColor(farben[1]);
            spiel.draw(c4z3s4sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z3s4sprite.setColor(farben[2]);
            spiel.draw(c4z3s4sprite);
            c4spieler3--;
        }

        //c4z4s1
        if (c4spieler1 != 0) {
            c4z4s1sprite.setColor(farben[0]);
            spiel.draw(c4z4s1sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z4s1sprite.setColor(farben[1]);
            spiel.draw(c4z4s1sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z4s1sprite.setColor(farben[2]);
            spiel.draw(c4z4s1sprite);
            c4spieler3--;
        }

        //c4z4s2
        if (c4spieler1 != 0) {
            c4z4s2sprite.setColor(farben[0]);
            spiel.draw(c4z4s2sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z4s2sprite.setColor(farben[1]);
            spiel.draw(c4z4s2sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z4s2sprite.setColor(farben[2]);
            spiel.draw(c4z4s2sprite);
            c4spieler3--;
        }

        //c4z4s3
        if (c4spieler1 != 0) {
            c4z4s3sprite.setColor(farben[0]);
            spiel.draw(c4z4s3sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z4s3sprite.setColor(farben[1]);
            spiel.draw(c4z4s3sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z4s3sprite.setColor(farben[2]);
            spiel.draw(c4z4s3sprite);
            c4spieler3--;
        }

        //c4z4s4
        if (c4spieler1 != 0) {
            c4z4s4sprite.setColor(farben[0]);
            spiel.draw(c4z4s4sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z4s4sprite.setColor(farben[1]);
            spiel.draw(c4z4s4sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z4s4sprite.setColor(farben[2]);
            spiel.draw(c4z4s4sprite);
            c4spieler3--;
        }

        //c4z5s1
        if (c4spieler1 != 0) {
            c4z5s1sprite.setColor(farben[0]);
            spiel.draw(c4z5s1sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z5s1sprite.setColor(farben[1]);
            spiel.draw(c4z5s1sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z5s1sprite.setColor(farben[2]);
            spiel.draw(c4z5s1sprite);
            c4spieler3--;
        }

        //c4z5s2
        if (c4spieler1 != 0) {
            c4z5s2sprite.setColor(farben[0]);
            spiel.draw(c4z5s2sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z5s2sprite.setColor(farben[1]);
            spiel.draw(c4z5s2sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z5s2sprite.setColor(farben[2]);
            spiel.draw(c4z5s2sprite);
            c4spieler3--;
        }

        //c4z5s3
        if (c4spieler1 != 0) {
            c4z5s3sprite.setColor(farben[0]);
            spiel.draw(c4z5s3sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z5s3sprite.setColor(farben[1]);
            spiel.draw(c4z5s3sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z5s3sprite.setColor(farben[2]);
            spiel.draw(c4z5s3sprite);
            c4spieler3--;
        }

        //c4z5s4
        if (c4spieler1 != 0) {
            c4z5s4sprite.setColor(farben[0]);
            spiel.draw(c4z5s4sprite);
            c4spieler1--;
        }
        else if (c4spieler2 != 0) {
            c4z5s4sprite.setColor(farben[1]);
            spiel.draw(c4z5s4sprite);
            c4spieler2--;
        }
        else if (c4spieler3 != 0) {
            c4z5s4sprite.setColor(farben[2]);
            spiel.draw(c4z5s4sprite);
            c4spieler3--;
        }

        //Casino 5
        //c5z1s1
        if (c5spieler1 != 0) {
            c5z1s1sprite.setColor(farben[0]);
            spiel.draw(c5z1s1sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z1s1sprite.setColor(farben[1]);
            spiel.draw(c5z1s1sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z1s1sprite.setColor(farben[2]);
            spiel.draw(c5z1s1sprite);
            c5spieler3--;
        }

        //c5z1s2
        if (c5spieler1 != 0) {
            c5z1s2sprite.setColor(farben[0]);
            spiel.draw(c5z1s2sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z1s2sprite.setColor(farben[1]);
            spiel.draw(c5z1s2sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z1s2sprite.setColor(farben[2]);
            spiel.draw(c5z1s2sprite);
            c5spieler3--;
        }

        //c5z1s3
        if (c5spieler1 != 0) {
            c5z1s3sprite.setColor(farben[0]);
            spiel.draw(c5z1s3sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z1s3sprite.setColor(farben[1]);
            spiel.draw(c5z1s3sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z1s3sprite.setColor(farben[2]);
            spiel.draw(c5z1s3sprite);
            c5spieler3--;
        }

        //c5z1s4
        if (c5spieler1 != 0) {
            c5z1s4sprite.setColor(farben[0]);
            spiel.draw(c5z1s4sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z1s4sprite.setColor(farben[1]);
            spiel.draw(c5z1s4sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z1s4sprite.setColor(farben[2]);
            spiel.draw(c5z1s4sprite);
            c5spieler3--;
        }

        //c5z2s1
        if (c5spieler1 != 0) {
            c5z2s1sprite.setColor(farben[0]);
            spiel.draw(c5z2s1sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z2s1sprite.setColor(farben[1]);
            spiel.draw(c5z2s1sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z2s1sprite.setColor(farben[2]);
            spiel.draw(c5z2s1sprite);
            c5spieler3--;
        }

        //c5z2s2
        if (c5spieler1 != 0) {
            c5z2s2sprite.setColor(farben[0]);
            spiel.draw(c5z2s2sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z2s2sprite.setColor(farben[1]);
            spiel.draw(c5z2s2sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z2s2sprite.setColor(farben[2]);
            spiel.draw(c5z2s2sprite);
            c5spieler3--;
        }

        //c5z2s3
        if (c5spieler1 != 0) {
            c5z2s3sprite.setColor(farben[0]);
            spiel.draw(c5z2s3sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z2s3sprite.setColor(farben[1]);
            spiel.draw(c5z2s3sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z2s3sprite.setColor(farben[2]);
            spiel.draw(c5z2s3sprite);
            c5spieler3--;
        }

        //c5z2s4
        if (c5spieler1 != 0) {
            c5z2s4sprite.setColor(farben[0]);
            spiel.draw(c5z2s4sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z2s4sprite.setColor(farben[1]);
            spiel.draw(c5z2s4sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z2s4sprite.setColor(farben[2]);
            spiel.draw(c5z2s4sprite);
            c5spieler3--;
        }

        //c5z3s1
        if (c5spieler1 != 0) {
            c5z3s1sprite.setColor(farben[0]);
            spiel.draw(c5z3s1sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z3s1sprite.setColor(farben[1]);
            spiel.draw(c5z3s1sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z3s1sprite.setColor(farben[2]);
            spiel.draw(c5z3s1sprite);
            c5spieler3--;
        }

        //c5z3s2
        if (c5spieler1 != 0) {
            c5z3s2sprite.setColor(farben[0]);
            spiel.draw(c5z3s2sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z3s2sprite.setColor(farben[1]);
            spiel.draw(c5z3s2sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z3s2sprite.setColor(farben[2]);
            spiel.draw(c5z3s2sprite);
            c5spieler3--;
        }

        //c5z3s3
        if (c5spieler1 != 0) {
            c5z3s3sprite.setColor(farben[0]);
            spiel.draw(c5z3s3sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z3s3sprite.setColor(farben[1]);
            spiel.draw(c5z3s3sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z3s3sprite.setColor(farben[2]);
            spiel.draw(c5z3s3sprite);
            c5spieler3--;
        }

        //c5z3s4
        if (c5spieler1 != 0) {
            c5z3s4sprite.setColor(farben[0]);
            spiel.draw(c5z3s4sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z3s4sprite.setColor(farben[1]);
            spiel.draw(c5z3s4sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z3s4sprite.setColor(farben[2]);
            spiel.draw(c5z3s4sprite);
            c5spieler3--;
        }

        //c5z4s1
        if (c5spieler1 != 0) {
            c5z4s1sprite.setColor(farben[0]);
            spiel.draw(c5z4s1sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z4s1sprite.setColor(farben[1]);
            spiel.draw(c5z4s1sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z4s1sprite.setColor(farben[2]);
            spiel.draw(c5z4s1sprite);
            c5spieler3--;
        }

        //c5z4s2
        if (c5spieler1 != 0) {
            c5z4s2sprite.setColor(farben[0]);
            spiel.draw(c5z4s2sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z4s2sprite.setColor(farben[1]);
            spiel.draw(c5z4s2sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z4s2sprite.setColor(farben[2]);
            spiel.draw(c5z4s2sprite);
            c5spieler3--;
        }

        //c5z4s3
        if (c5spieler1 != 0) {
            c5z4s3sprite.setColor(farben[0]);
            spiel.draw(c5z4s3sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z4s3sprite.setColor(farben[1]);
            spiel.draw(c5z4s3sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z4s3sprite.setColor(farben[2]);
            spiel.draw(c5z4s3sprite);
            c5spieler3--;
        }

        //c5z4s4
        if (c5spieler1 != 0) {
            c5z4s4sprite.setColor(farben[0]);
            spiel.draw(c5z4s4sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z4s4sprite.setColor(farben[1]);
            spiel.draw(c5z4s4sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z4s4sprite.setColor(farben[2]);
            spiel.draw(c5z4s4sprite);
            c5spieler3--;
        }

        //c5z5s1
        if (c5spieler1 != 0) {
            c5z5s1sprite.setColor(farben[0]);
            spiel.draw(c5z5s1sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z5s1sprite.setColor(farben[1]);
            spiel.draw(c5z5s1sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z5s1sprite.setColor(farben[2]);
            spiel.draw(c5z5s1sprite);
            c5spieler3--;
        }

        //c5z5s2
        if (c5spieler1 != 0) {
            c5z5s2sprite.setColor(farben[0]);
            spiel.draw(c5z5s2sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z5s2sprite.setColor(farben[1]);
            spiel.draw(c5z5s2sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z5s2sprite.setColor(farben[2]);
            spiel.draw(c5z5s2sprite);
            c5spieler3--;
        }

        //c5z5s3
        if (c5spieler1 != 0) {
            c5z5s3sprite.setColor(farben[0]);
            spiel.draw(c5z5s3sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z5s3sprite.setColor(farben[1]);
            spiel.draw(c5z5s3sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z5s3sprite.setColor(farben[2]);
            spiel.draw(c5z5s3sprite);
            c5spieler3--;
        }

        //c5z5s4
        if (c5spieler1 != 0) {
            c5z5s4sprite.setColor(farben[0]);
            spiel.draw(c5z5s4sprite);
            c5spieler1--;
        }
        else if (c5spieler2 != 0) {
            c5z5s4sprite.setColor(farben[1]);
            spiel.draw(c5z5s4sprite);
            c5spieler2--;
        }
        else if (c5spieler3 != 0) {
            c5z5s4sprite.setColor(farben[2]);
            spiel.draw(c5z5s4sprite);
            c5spieler3--;
        }

        //Casino 6
        //c6z1s1
        if (c6spieler1 != 0) {
            c6z1s1sprite.setColor(farben[0]);
            spiel.draw(c6z1s1sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z1s1sprite.setColor(farben[1]);
            spiel.draw(c6z1s1sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z1s1sprite.setColor(farben[2]);
            spiel.draw(c6z1s1sprite);
            c6spieler3--;
        }

        //c6z1s2
        if (c6spieler1 != 0) {
            c6z1s2sprite.setColor(farben[0]);
            spiel.draw(c6z1s2sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z1s2sprite.setColor(farben[1]);
            spiel.draw(c6z1s2sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z1s2sprite.setColor(farben[2]);
            spiel.draw(c6z1s2sprite);
            c6spieler3--;
        }

        //c6z1s3
        if (c6spieler1 != 0) {
            c6z1s3sprite.setColor(farben[0]);
            spiel.draw(c6z1s3sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z1s3sprite.setColor(farben[1]);
            spiel.draw(c6z1s3sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z1s3sprite.setColor(farben[2]);
            spiel.draw(c6z1s3sprite);
            c6spieler3--;
        }

        //c6z1s4
        if (c6spieler1 != 0) {
            c6z1s4sprite.setColor(farben[0]);
            spiel.draw(c6z1s4sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z1s4sprite.setColor(farben[1]);
            spiel.draw(c6z1s4sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z1s4sprite.setColor(farben[2]);
            spiel.draw(c6z1s4sprite);
            c6spieler3--;
        }

        //c6z2s1
        if (c6spieler1 != 0) {
            c6z2s1sprite.setColor(farben[0]);
            spiel.draw(c6z2s1sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z2s1sprite.setColor(farben[1]);
            spiel.draw(c6z2s1sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z2s1sprite.setColor(farben[2]);
            spiel.draw(c6z2s1sprite);
            c6spieler3--;
        }

        //c6z2s2
        if (c6spieler1 != 0) {
            c6z2s2sprite.setColor(farben[0]);
            spiel.draw(c6z2s2sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z2s2sprite.setColor(farben[1]);
            spiel.draw(c6z2s2sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z2s2sprite.setColor(farben[2]);
            spiel.draw(c6z2s2sprite);
            c6spieler3--;
        }

        //c6z2s3
        if (c6spieler1 != 0) {
            c6z2s3sprite.setColor(farben[0]);
            spiel.draw(c6z2s3sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z2s3sprite.setColor(farben[1]);
            spiel.draw(c6z2s3sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z2s3sprite.setColor(farben[2]);
            spiel.draw(c6z2s3sprite);
            c6spieler3--;
        }

        //c6z2s4
        if (c6spieler1 != 0) {
            c6z2s4sprite.setColor(farben[0]);
            spiel.draw(c6z2s4sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z2s4sprite.setColor(farben[1]);
            spiel.draw(c6z2s4sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z2s4sprite.setColor(farben[2]);
            spiel.draw(c6z2s4sprite);
            c6spieler3--;
        }

        //c6z3s1
        if (c6spieler1 != 0) {
            c6z3s1sprite.setColor(farben[0]);
            spiel.draw(c6z3s1sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z3s1sprite.setColor(farben[1]);
            spiel.draw(c6z3s1sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z3s1sprite.setColor(farben[2]);
            spiel.draw(c6z3s1sprite);
            c6spieler3--;
        }

        //c6z3s2
        if (c6spieler1 != 0) {
            c6z3s2sprite.setColor(farben[0]);
            spiel.draw(c6z3s2sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z3s2sprite.setColor(farben[1]);
            spiel.draw(c6z3s2sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z3s2sprite.setColor(farben[2]);
            spiel.draw(c6z3s2sprite);
            c6spieler3--;
        }

        //c6z3s3
        if (c6spieler1 != 0) {
            c6z3s3sprite.setColor(farben[0]);
            spiel.draw(c6z3s3sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z3s3sprite.setColor(farben[1]);
            spiel.draw(c6z3s3sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z3s3sprite.setColor(farben[2]);
            spiel.draw(c6z3s3sprite);
            c6spieler3--;
        }

        //c6z3s4
        if (c6spieler1 != 0) {
            c6z3s4sprite.setColor(farben[0]);
            spiel.draw(c6z3s4sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z3s4sprite.setColor(farben[1]);
            spiel.draw(c6z3s4sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z3s4sprite.setColor(farben[2]);
            spiel.draw(c6z3s4sprite);
            c6spieler3--;
        }

        //c6z4s1
        if (c6spieler1 != 0) {
            c6z4s1sprite.setColor(farben[0]);
            spiel.draw(c6z4s1sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z4s1sprite.setColor(farben[1]);
            spiel.draw(c6z4s1sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z4s1sprite.setColor(farben[2]);
            spiel.draw(c6z4s1sprite);
            c6spieler3--;
        }

        //c6z4s2
        if (c6spieler1 != 0) {
            c6z4s2sprite.setColor(farben[0]);
            spiel.draw(c6z4s2sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z4s2sprite.setColor(farben[1]);
            spiel.draw(c6z4s2sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z4s2sprite.setColor(farben[2]);
            spiel.draw(c6z4s2sprite);
            c6spieler3--;
        }

        //c6z4s3
        if (c6spieler1 != 0) {
            c6z4s3sprite.setColor(farben[0]);
            spiel.draw(c6z4s3sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z4s3sprite.setColor(farben[1]);
            spiel.draw(c6z4s3sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z4s3sprite.setColor(farben[2]);
            spiel.draw(c6z4s3sprite);
            c6spieler3--;
        }

        //c6z4s4
        if (c6spieler1 != 0) {
            c6z4s4sprite.setColor(farben[0]);
            spiel.draw(c6z4s4sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z4s4sprite.setColor(farben[1]);
            spiel.draw(c6z4s4sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z4s4sprite.setColor(farben[2]);
            spiel.draw(c6z4s4sprite);
            c6spieler3--;
        }

        //c6z5s1
        if (c6spieler1 != 0) {
            c6z5s1sprite.setColor(farben[0]);
            spiel.draw(c6z5s1sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z5s1sprite.setColor(farben[1]);
            spiel.draw(c6z5s1sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z5s1sprite.setColor(farben[2]);
            spiel.draw(c6z5s1sprite);
            c6spieler3--;
        }

        //c6z5s2
        if (c6spieler1 != 0) {
            c6z5s2sprite.setColor(farben[0]);
            spiel.draw(c6z5s2sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z5s2sprite.setColor(farben[1]);
            spiel.draw(c6z5s2sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z5s2sprite.setColor(farben[2]);
            spiel.draw(c6z5s2sprite);
            c6spieler3--;
        }

        //c6z5s3
        if (c6spieler1 != 0) {
            c6z5s3sprite.setColor(farben[0]);
            spiel.draw(c6z5s3sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z5s3sprite.setColor(farben[1]);
            spiel.draw(c6z5s3sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z5s3sprite.setColor(farben[2]);
            spiel.draw(c6z5s3sprite);
            c6spieler3--;
        }

        //c6z5s4
        if (c6spieler1 != 0) {
            c6z5s4sprite.setColor(farben[0]);
            spiel.draw(c6z5s4sprite);
            c6spieler1--;
        }
        else if (c6spieler2 != 0) {
            c6z5s4sprite.setColor(farben[1]);
            spiel.draw(c6z5s4sprite);
            c6spieler2--;
        }
        else if (c6spieler3 != 0) {
            c6z5s4sprite.setColor(farben[2]);
            spiel.draw(c6z5s4sprite);
            c6spieler3--;
        }

        //W�rfelsprites zeichnen
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
                //Dies mit einer Speicherwarung oder Speichern �ndern
                bool meldung = best�tigungsmeldung("M�chten Sie den Spielstand speichern?");
                if (meldung == true) {
                    //Speichern
                    dateienmanager(true);
                }
                else {
                    spiel.close();
                    exit(0);
                }
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
                        aufWuerfelGedrueckt = false;
                    }
                }

                if (undobutton.isMouseOver(spiel)) { 
                    setzeSchrittZur�ck();
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

                // Hit-test: �berpr�fe, ob W�rfel angeklickt wurde
                if (wuerfelcoords1.contains(mouse))
                {
                    // mouse is on sprite!
                    //TODO: Setze W�rfel auf Casino
                    int wuerfelplatzwert = wuerfelwert[0];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeW�rfel(spielernummer, 1);
                        break;
                    case 2:
                        setzeW�rfel(spielernummer, 2);
                        break;
                    case 3:
                        setzeW�rfel(spielernummer, 3);
                        break;
                    case 4:
                        setzeW�rfel(spielernummer, 4);
                        break;
                    case 5:
                        setzeW�rfel(spielernummer, 5);
                        break;
                    case 6:
                        setzeW�rfel(spielernummer, 6);
                        break;
                    }
                }
                if (wuerfelcoords2.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[1];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeW�rfel(spielernummer, 1);
                        break;
                    case 2:
                        setzeW�rfel(spielernummer, 2);
                        break;
                    case 3:
                        setzeW�rfel(spielernummer, 3);
                        break;
                    case 4:
                        setzeW�rfel(spielernummer, 4);
                        break;
                    case 5:
                        setzeW�rfel(spielernummer, 5);
                        break;
                    case 6:
                        setzeW�rfel(spielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords3.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[2];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeW�rfel(spielernummer, 1);
                        break;
                    case 2:
                        setzeW�rfel(spielernummer, 2);
                        break;
                    case 3:
                        setzeW�rfel(spielernummer, 3);
                        break;
                    case 4:
                        setzeW�rfel(spielernummer, 4);
                        break;
                    case 5:
                        setzeW�rfel(spielernummer, 5);
                        break;
                    case 6:
                        setzeW�rfel(spielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords4.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[3];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeW�rfel(spielernummer, 1);
                        break;
                    case 2:
                        setzeW�rfel(spielernummer, 2);
                        break;
                    case 3:
                        setzeW�rfel(spielernummer, 3);
                        break;
                    case 4:
                        setzeW�rfel(spielernummer, 4);
                        break;
                    case 5:
                        setzeW�rfel(spielernummer, 5);
                        break;
                    case 6:
                        setzeW�rfel(spielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords5.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[4];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeW�rfel(spielernummer, 1);
                        break;
                    case 2:
                        setzeW�rfel(spielernummer, 2);
                        break;
                    case 3:
                        setzeW�rfel(spielernummer, 3);
                        break;
                    case 4:
                        setzeW�rfel(spielernummer, 4);
                        break;
                    case 5:
                        setzeW�rfel(spielernummer, 5);
                        break;
                    case 6:
                        setzeW�rfel(spielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords6.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[5];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeW�rfel(spielernummer, 1);
                        break;
                    case 2:
                        setzeW�rfel(spielernummer, 2);
                        break;
                    case 3:
                        setzeW�rfel(spielernummer, 3);
                        break;
                    case 4:
                        setzeW�rfel(spielernummer, 4);
                        break;
                    case 5:
                        setzeW�rfel(spielernummer, 5);
                        break;
                    case 6:
                        setzeW�rfel(spielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords7.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[6];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeW�rfel(spielernummer, 1);
                        break;
                    case 2:
                        setzeW�rfel(spielernummer, 2);
                        break;
                    case 3:
                        setzeW�rfel(spielernummer, 3);
                        break;
                    case 4:
                        setzeW�rfel(spielernummer, 4);
                        break;
                    case 5:
                        setzeW�rfel(spielernummer, 5);
                        break;
                    case 6:
                        setzeW�rfel(spielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords8.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = wuerfelwert[7];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeW�rfel(spielernummer, 1);
                        break;
                    case 2:
                        setzeW�rfel(spielernummer, 2);
                        break;
                    case 3:
                        setzeW�rfel(spielernummer, 3);
                        break;
                    case 4:
                        setzeW�rfel(spielernummer, 4);
                        break;
                    case 5:
                        setzeW�rfel(spielernummer, 5);
                        break;
                    case 6:
                        setzeW�rfel(spielernummer, 6);
                        break;
                    }

                }
            }
        }

        spiel.display();
    }
}

void neuesSpiel() { 
    printf("Neues Spiel wird gestartet! \n");
    //Zeichenthread erstellen
    sf::Thread zeichenthread(&Spielzeichnung);
    zeichenthread.launch();
    //Ganz normale Spiellogik
    while(spielBeendet == false) {
        //Rundentext ver�ndern

        //Geldscheine zuf�llig ausw�hlen und anzeigen (i <=5 machen nicht vergessen)
        srand(time(NULL));
        if (spielgeladen == false) {
            for (int i = 0; i <= 5; i++) {
                switch (i) {
                case 0:
                    for (int k = 0; ; k++) {
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
        }
        //Geldscheine sortieren
        //Casino 1
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
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
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
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
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
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
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
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
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
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
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
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

        while (spielerhabenw�rfel == true) {
            rundentext = "Runde " + to_string(rundenzahlf�rtext) + " von 4";
            //W�rfeln, bis keiner mehr w�rfeln mehr hat
            if (spielgeladen == false) {
                spieler = 0;
            }
            for (spieler; spieler <= 2; spieler++) {
                //Schreiben von Arrays
                wuerfelzahl1 = "Spieler 1 hat " + to_string(wuerfelanzahl[0]) + " W�rfeln �brig";
                wuerfelzahl2 = "Spieler 2 hat " + to_string(wuerfelanzahl[1]) + " W�rfeln �brig";
                wuerfelzahl3 = "Spieler 3 hat " + to_string(wuerfelanzahl[2]) + " W�rfeln �brig";
                rundenzahlf�rtext = rundenzahl + 1;
                rundentext = "Runde " + to_string(rundenzahlf�rtext) + " von 4";
                //Tempor�re Variable f�rs �berpr�fen, ob ein Spieler kein W�rfel hat
                int tempanzwuerfel = wuerfelanzahl[spieler];
                if (tempanzwuerfel == 0) {
                    continue;
                }
                spielernummer = spieler;
                int anzahlwuerfel = wuerfelanzahl[spieler];

                if(spielgeladen == false){
                    //W�rfel w�rfeln
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
                }
                spielgeladen = false;
                //W�rfeln sortieren
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

                //Warten bis auf W�rfel gedr�ckt wird
                while (zugbeendet == false) {
                    //Unten den Nutzer drauf hinweisen, auf einen W�rfel zu klicken!
                    switch (spielernummer) {
                    case 0:
                        if (aufWuerfelGedrueckt == false) {
                            subtextstring = "Spieler 1: Bitte w�hlen Sie einen W�rfel!";
                        }
                        else {
                            subtextstring = "Spieler 1: Beenden Sie den Zug oder setzen Sie den Schritt zur�ck!";
                        }
                        break;
                    case 1:
                        if (aufWuerfelGedrueckt == false) {
                            subtextstring = "Spieler 2: Bitte w�hlen Sie einen W�rfel!";
                        }
                        else {
                            subtextstring = "Spieler 2: Beenden Sie den Zug oder setzen Sie den Schritt zur�ck!";
                        }
                        break;
                    case 2:
                        if (aufWuerfelGedrueckt == false) {
                            subtextstring = "Spieler 3: Bitte w�hlen Sie einen W�rfel!";
                        }
                        else {
                            subtextstring = "Spieler 3: Beenden Sie den Zug oder setzen Sie den Schritt zur�ck!";
                        }
                        break;
                    default:
                        break;
                    }
                }
                //Resetten
                aufWuerfelGedrueckt = false;
                zugbeendet = false;

                if (wuerfelanzahl[0] == 0 && wuerfelanzahl[1] == 0 && wuerfelanzahl[2] == 0) {
                    para();
                    if (rundenzahl == 3) {
                        //Gewinner zeigen, dass er gewonnen hat!
                        printf("Spiel zu Ende!\n");
                        spielBeendet = true;
                        if (kontostand[0] > kontostand[1] && kontostand[0] > kontostand[2]) {
                            zeichenthread.terminate();
                            gewinner(1);
                        }
                        else if (kontostand[0]< kontostand[1] && kontostand[1] > kontostand[2]) {
                            zeichenthread.terminate();
                            gewinner(2);
                        }
                        else if (kontostand[0] < kontostand[2] && kontostand[1] < kontostand[2]) {
                            zeichenthread.terminate();
                            gewinner(3);
                        }
                    }
                    else {
                        printf("Rundenzahl ist unter 3!\n");
                        rundenzahl++;
                        rundenzahlf�rtext = rundenzahl + 1;
                    }
                }
            }

            /*
            if (wuerfelanzahl[0] == 0 && wuerfelanzahl[1] == 0 && wuerfelanzahl[2] == 0) {
                printf("Spieler haben keine W�rfel mehr!\n");
                para();
                if (rundenzahl <= 3) {
                    //Funktion f�rs Resetten und Geldzuteilen
                    
                    rundenzahl++;
                }
                else {
                    //Spiel vorbei!
                    spielBeendet = true;
                }

            }*/
        }

    }
    //Ende vom Spiel
    //Gewinner anzeigen und sterben oder so und Verlierer wird ein Kopfgeld gesetzt


}

//Dieses Thread arbeitet im Hintergrund und sorgt f�r einen problemlosen Lauf des Spiels
//Er �berpr�ft auf Probleme und Fehler und korrigiert diese. In diesem Thread werden auch wichtige Funktionen aufgerufen.
void Systemthread() {
    
    //Check als erstes, ob wichtige Dateien vorhanden sind!
    if (einstellungsdateipr�fer() == false) {
        printf("[WARNUNG] Einstellungsdatei nicht gefunden! Wird erstellt!\n");
        einstellungsdateierstellung();
    }
    else {
        //G�hnix
        printf("[INFO] Einstellungsdatei vorhanden!\n");
    }

    if (speicherdatenpr�fung() == false) {
        printf("[WARNUNG]Speicherdateien nicht gefunden! Dateien werden erstellt!");
        speicherdateienerstellung();
    }
    else {
        printf("[INFO] Speicherdateien vorhanden!\n");
    }
    
    //Variablenpr�fung
    while (true) {
        
    }

}

//
//Main-Funktion - Hier befinden sich alle Hauptfunktionen wie Erzeugung des Bildschirms etc.
//
int main(){
    //Konsolenausgabe
    printf("Las Vegas - Das Spiel\nDiese Konsole wird als Fehlerausgabe benutzt!\nSprich, falls etwas falsch laufen sollte,\nwird es hier ausgegeben!\n\n");

    //Erzeuge das Fenster-Objekt
    sf::RenderWindow window(sf::VideoMode(1220, 800), "Las Vegas", sf::Style::Close);

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
        fehleranzeige("Fehler", "Einstellungsdatei nicht gefunden! Es wird neuerstellt!");
        printf("Einstellungsdatei wird erstellt! \n");
        einstellungsdateierstellung();
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

    //Den Systemthread initialisieren und starten
    Thread systemthread(&Systemthread);
    systemthread.launch();
  
    // Eine Schleife, dass das Programm solange laufen l�sst, bis es geschlossen wird
    while (window.isOpen())
    {
        // �berpr�ft alle Eventtrigger nach dem letzten Frame
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Schlie�t das Fenster, falls schlie�en gedr�ckt wird
            if (event.type == sf::Event::Closed) {
                  window.close();
                  exit(0);
            }
              

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
                    window.setVisible(false);
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
                    dateienmanager(false);
                    //gewinner(3);
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
        sf::Text vertext("Version: 0.9.6 Alpha", font);
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