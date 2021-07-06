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
int igeldwerte[6] = { 10000, 20000, 30000, 50000, 70000, 90000 };
int ikontostand[3] = {0, 0, 0};
int iicasinogelder1[5];
int iicasinogelder2[5];
int iicasinogelder3[5];
int iicasinogelder4[5];
int iicasinogelder5[5];
int iicasinogelder6[5];
int icasinoiiwuerfelanzahl1[3] = { 0, 0, 0 };
int icasinoiiwuerfelanzahl2[3] = { 0, 0, 0 };
int icasinoiiwuerfelanzahl3[3] = { 0, 0, 0 };
int icasinoiiwuerfelanzahl4[3] = { 0, 0, 0 };
int icasinoiiwuerfelanzahl5[3] = { 0, 0, 0 };
int icasinoiiwuerfelanzahl6[3] = { 0, 0, 0 };
int isumme1 = 0;
int isumme2 = 0;
int isumme3 = 0;
int isumme4 = 0;
int isumme5 = 0;
int isumme6 = 0;
int iiwuerfelanzahl[3] = { 8, 8, 8 };
int iiwuerfelwert[8];
int ispieler;
int iispielernummer;
int iirundenzahl = 0;
int iausgewählteAuge[2]; //Ein leeres Array erstellen für den ersten Index, welcher Würfel und der zweite Index, wie viele.
bool ispielerhabenwürfel = true;
Color farben[3] = { Color::Cyan, Color::Green, Color::Yellow };
string ssubtextstring;
int iirundenzahlfürtext = iirundenzahl + 1; 
string ikontostandtext1 = "ikontostand: $" + to_string(ikontostand[0]);
string ikontostandtext2 = "ikontostand: $" + to_string(ikontostand[1]);
string ikontostandtext3 = "ikontostand: $" + to_string(ikontostand[2]);
string swuerfelzahl1, swuerfelzahl2, swuerfelzahl3;
string ssletzteaktion[3] = { "Zuletzt: Nichts passiert!", "Zuletzt: Nichts passiert!", "Zuletzt: Nichts passiert!" };
string sspeichersname1, sspeichersname2, sspeichersname3, sspeichersname4;
string srundentext;
bool baufwuerfelgedrueckt = false;
bool bspielbeendet = false;
bool bzugbeendet = false;
bool bspielgeladen;

void neuesSpiel();
int main();

//Systemvariablen (BITTE NICHT VERÄNDERN!)
bool bneustartgefordert = false;
bool bspeichertbool = false;

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
                        einstellungsdateiänderung(p, v);
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
    string sname;
    switch (speichernummer) {
    case 1:
        sname = "data1.txt";
        break;
    case 2:
        sname = "data2.txt";
        break;
    case 3:
        sname = "data3.txt";
        break;
    case 4:
        sname = "data4.txt";
        break;
    default:
        fehleranzeige("Programmier-Fehler", "Es gibt nur 4 Speicherdateien!");
    }
    ifstream leser(sname, std::ios::in);
    string x[128];
    
    getline(leser, x[0]);
    ikontostand[0] = stoi(x[0]);
    getline(leser, x[1]);
    ikontostand[1] = stoi(x[1]);
    getline(leser, x[2]);
    ikontostand[2] = stoi(x[2]);

    getline(leser, x[3]);
    iicasinogelder1[0] = stoi(x[3]);
    getline(leser, x[4]);
    iicasinogelder2[0] = stoi(x[4]);
    getline(leser, x[5]);
    iicasinogelder3[0] = stoi(x[5]);
    getline(leser, x[6]);
    iicasinogelder4[0] = stoi(x[6]);
    getline(leser, x[7]);
    iicasinogelder5[0] = stoi(x[7]);
    getline(leser, x[8]);
    iicasinogelder6[0] = stoi(x[8]);

    getline(leser, x[9]);
    iicasinogelder1[1] = stoi(x[9]);
    getline(leser, x[10]);
    iicasinogelder2[1] = stoi(x[10]);
    getline(leser, x[11]);
    iicasinogelder3[1] = stoi(x[11]);
    getline(leser, x[12]);
    iicasinogelder4[1] = stoi(x[12]);
    getline(leser, x[13]);
    iicasinogelder5[1] = stoi(x[13]);
    getline(leser, x[14]);
    iicasinogelder6[1] = stoi(x[14]);

    getline(leser, x[15]);
    iicasinogelder1[2] = stoi(x[15]);
    getline(leser, x[16]);
    iicasinogelder2[2] = stoi(x[16]);
    getline(leser, x[17]);
    iicasinogelder3[2] = stoi(x[17]);
    getline(leser, x[18]);
    iicasinogelder4[2] = stoi(x[18]);
    getline(leser, x[19]);
    iicasinogelder5[2] = stoi(x[19]);
    getline(leser, x[20]);
    iicasinogelder6[2] = stoi(x[20]);

    getline(leser, x[21]);
    iicasinogelder1[3] = stoi(x[21]);
    getline(leser, x[22]);
    iicasinogelder2[3] = stoi(x[22]);
    getline(leser, x[23]);
    iicasinogelder3[3] = stoi(x[23]);
    getline(leser, x[24]);
    iicasinogelder4[3] = stoi(x[24]);
    getline(leser, x[25]);
    iicasinogelder5[3] = stoi(x[25]);
    getline(leser, x[26]);
    iicasinogelder6[3] = stoi(x[26]);

    getline(leser, x[27]);
    iicasinogelder1[4] = stoi(x[27]);
    getline(leser, x[28]);
    iicasinogelder2[4] = stoi(x[28]);
    getline(leser, x[29]);
    iicasinogelder3[4] = stoi(x[29]);
    getline(leser, x[30]);
    iicasinogelder4[4] = stoi(x[30]);
    getline(leser, x[31]);
    iicasinogelder5[4] = stoi(x[31]);
    getline(leser, x[32]);
    iicasinogelder6[4] = stoi(x[32]);

    getline(leser, x[33]);
    icasinoiiwuerfelanzahl1[0] = stoi(x[33]);
    getline(leser, x[34]);
    icasinoiiwuerfelanzahl2[0] = stoi(x[34]);
    getline(leser, x[35]);
    icasinoiiwuerfelanzahl3[0] = stoi(x[35]);
    getline(leser, x[36]);
    icasinoiiwuerfelanzahl4[0] = stoi(x[36]);
    getline(leser, x[37]);
    icasinoiiwuerfelanzahl5[0] = stoi(x[37]);
    getline(leser, x[38]);
    icasinoiiwuerfelanzahl6[0] = stoi(x[38]);

    getline(leser, x[39]);
    icasinoiiwuerfelanzahl1[1] = stoi(x[39]);
    getline(leser, x[40]);
    icasinoiiwuerfelanzahl2[1] = stoi(x[40]);
    getline(leser, x[41]);
    icasinoiiwuerfelanzahl3[1] = stoi(x[41]);
    getline(leser, x[42]);
    icasinoiiwuerfelanzahl4[1] = stoi(x[42]);
    getline(leser, x[43]);
    icasinoiiwuerfelanzahl5[1] = stoi(x[43]);
    getline(leser, x[44]);
    icasinoiiwuerfelanzahl6[1] = stoi(x[44]);

    getline(leser, x[45]);
    icasinoiiwuerfelanzahl1[2] = stoi(x[45]);
    getline(leser, x[46]);
    icasinoiiwuerfelanzahl2[2] = stoi(x[46]);
    getline(leser, x[47]);
    icasinoiiwuerfelanzahl3[2] = stoi(x[47]);
    getline(leser, x[48]);
    icasinoiiwuerfelanzahl4[2] = stoi(x[48]);
    getline(leser, x[49]);
    icasinoiiwuerfelanzahl5[2] = stoi(x[49]);
    getline(leser, x[50]);
    icasinoiiwuerfelanzahl6[2] = stoi(x[50]);

    getline(leser, x[51]);
    iiwuerfelanzahl[0] = stoi(x[51]);
    getline(leser, x[52]);
    iiwuerfelanzahl[1] = stoi(x[52]);
    getline(leser, x[53]);
    iiwuerfelanzahl[2] = stoi(x[53]);

    getline(leser, x[54]);
    iiwuerfelwert[0] = stoi(x[54]);
    getline(leser, x[55]);
    iiwuerfelwert[1] = stoi(x[55]);
    getline(leser, x[56]);
    iiwuerfelwert[2] = stoi(x[56]);
    getline(leser, x[57]);
    iiwuerfelwert[3] = stoi(x[57]);
    getline(leser, x[58]);
    iiwuerfelwert[4] = stoi(x[58]);
    getline(leser, x[59]);
    iiwuerfelwert[5] = stoi(x[59]);
    getline(leser, x[60]);
    iiwuerfelwert[6] = stoi(x[60]);
    getline(leser, x[61]);
    iiwuerfelwert[7] = stoi(x[61]);

    getline(leser, x[62]);
    ispieler = stoi(x[62]);
    iispielernummer = stoi(x[62]);

    getline(leser, x[63]);
    iirundenzahl = stoi(x[63]);

    getline(leser, x[64]);
    ssletzteaktion[0] = x[64];

    getline(leser, x[65]);
    ssletzteaktion[1] = x[65];

    getline(leser, x[66]);
    ssletzteaktion[2] = x[66];

    ikontostandtext1 = "ikontostand: $" + to_string(ikontostand[0]);
    ikontostandtext2 = "ikontostand: $" + to_string(ikontostand[1]);
    ikontostandtext3 = "ikontostand: $" + to_string(ikontostand[2]);

    srundentext = "Runde " + to_string(iirundenzahlfürtext) + " von 4";

    bspielgeladen = true;
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

        //Prüfen, ob die Dateien leer sind!
        ifstream prüfer("data1.txt", std::ios::in);
        string stemp1;
        getline(prüfer, stemp1);
        if (stemp1 == "leer") {
            sspeichersname1 = "Spielspeicher 1 - LEER";
        }
        else {
            sspeichersname1 = "Spielspeicher 1 - VOLL";
        }

        ifstream prüfer2("data2.txt", std::ios::in);
        string stemp2;
        getline(prüfer2, stemp2);
        if (stemp2 == "leer") {
            sspeichersname2 = "Spielspeicher 2 - LEER";
        }
        else {
            sspeichersname2 = "Spielspeicher 2 - VOLL";
        }

        ifstream prüfer3("data3.txt", std::ios::in);
        string stemp3;
        getline(prüfer3, stemp3);
        if (stemp3 == "leer") {
            sspeichersname3 = "Spielspeicher 3 - LEER";
        }
        else {
            sspeichersname3 = "Spielspeicher 3 - VOLL";
        }

        ifstream prüfer4("data4.txt", std::ios::in);
        string stemp4;
        getline(prüfer4, stemp4);
        if (stemp4 == "leer") {
            sspeichersname4 = "Spielspeicher 4 - LEER";
        }
        else {
            sspeichersname4 = "Spielspeicher 4 - VOLL";
        }

        Button Spielspeicher1(sspeichersname1, { 650, 100 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
        Spielspeicher1.setFont(font);
        Spielspeicher1.setPosition({ 25.0f, 50.0f });

        Button Spielspeicher2(sspeichersname2, { 650, 100 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
        Spielspeicher2.setFont(font);
        Spielspeicher2.setPosition({ 25.0f, 200.0f });

        Button Spielspeicher3(sspeichersname3, { 650, 100 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
        Spielspeicher3.setFont(font);
        Spielspeicher3.setPosition({ 25.0f, 350.0f });

        Button Spielspeicher4(sspeichersname4, { 650, 100 }, 30, sf::Color(195, 195, 195, 255), sf::Color::Black);
        Spielspeicher4.setFont(font);
        Spielspeicher4.setPosition({ 25.0f, 500.0f });

        Button Speicher1löschen("Löschen", { 75, 100 }, 20, sf::Color::Red, sf::Color::Black);
        Speicher1löschen.setFont(font);
        Speicher1löschen.setPosition({ 700.0f, 50.0f });

        Button Speicher2löschen("Löschen", { 75, 100 }, 20, sf::Color::Red, sf::Color::Black);
        Speicher2löschen.setFont(font);
        Speicher2löschen.setPosition({ 700.0f, 200.0f });

        Button Speicher3löschen("Löschen", { 75, 100 }, 20, sf::Color::Red, sf::Color::Black);
        Speicher3löschen.setFont(font);
        Speicher3löschen.setPosition({ 700.0f, 350.0f });

        Button Speicher4löschen("Löschen", { 75, 100 }, 20, sf::Color::Red, sf::Color::Black);
        Speicher4löschen.setFont(font);
        Speicher4löschen.setPosition({ 700.0f, 500.0f });

        Button Abbrechenbutton("Abbrechen", { 800.0f, 100.0f },30,  sf::Color::Magenta, sf::Color::Black);
        

        Spielspeicher1.drawTo(dateifenster);
        Spielspeicher2.drawTo(dateifenster);
        Spielspeicher3.drawTo(dateifenster);
        Spielspeicher4.drawTo(dateifenster);
        Speicher1löschen.drawTo(dateifenster);
        Speicher2löschen.drawTo(dateifenster);
        Speicher3löschen.drawTo(dateifenster);
        Speicher4löschen.drawTo(dateifenster);
        dateifenster.display();
        sf::Event event;
        while (dateifenster.pollEvent(event))
        {
            // Schließt das Fenster, falls schließen gedrückt wird
            if (event.type == sf::Event::Closed) {
                dateifenster.close();

            }

            switch (event.type) {
            case sf::Event::MouseButtonPressed:
                if (Spielspeicher1.isMouseOver(dateifenster)) {
                    if (speichern == true) {
                        speicherdateienänderung(1, ikontostand, iicasinogelder1, iicasinogelder2, iicasinogelder3, iicasinogelder4, iicasinogelder5, iicasinogelder6, icasinoiiwuerfelanzahl1, icasinoiiwuerfelanzahl2, icasinoiiwuerfelanzahl3, icasinoiiwuerfelanzahl4, icasinoiiwuerfelanzahl5, icasinoiiwuerfelanzahl6, iiwuerfelanzahl, iiwuerfelwert, iispielernummer, iirundenzahl, ssletzteaktion);
                        dateifenster.close();
                    }
                    else if (speichern == false) {
                        //Prüfen, ob Datei leer ist
                        ifstream prüfer("data1.txt", std::ios::in);
                        string stemp;
                        getline(prüfer, stemp);
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
                        speicherdateienänderung(2, ikontostand, iicasinogelder1, iicasinogelder2, iicasinogelder3, iicasinogelder4, iicasinogelder5, iicasinogelder6, icasinoiiwuerfelanzahl1, icasinoiiwuerfelanzahl2, icasinoiiwuerfelanzahl3, icasinoiiwuerfelanzahl4, icasinoiiwuerfelanzahl5, icasinoiiwuerfelanzahl6, iiwuerfelanzahl, iiwuerfelwert, iispielernummer, iirundenzahl, ssletzteaktion);
                        dateifenster.close();
                    }
                    else if (speichern == false) {
                        //Prüfen, ob Datei leer ist
                        ifstream prüfer("data2.txt", std::ios::in);
                        string stemp;
                        getline(prüfer, stemp);
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
                        speicherdateienänderung(3, ikontostand, iicasinogelder1, iicasinogelder2, iicasinogelder3, iicasinogelder4, iicasinogelder5, iicasinogelder6, icasinoiiwuerfelanzahl1, icasinoiiwuerfelanzahl2, icasinoiiwuerfelanzahl3, icasinoiiwuerfelanzahl4, icasinoiiwuerfelanzahl5, icasinoiiwuerfelanzahl6, iiwuerfelanzahl, iiwuerfelwert, iispielernummer, iirundenzahl, ssletzteaktion);
                        dateifenster.close();
                    }
                    else if (speichern == false) {
                        //Prüfen, ob Datei leer ist
                        ifstream prüfer("data3.txt", std::ios::in);
                        string stemp;
                        getline(prüfer, stemp);
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
                        speicherdateienänderung(4, ikontostand, iicasinogelder1, iicasinogelder2, iicasinogelder3, iicasinogelder4, iicasinogelder5, iicasinogelder6, icasinoiiwuerfelanzahl1, icasinoiiwuerfelanzahl2, icasinoiiwuerfelanzahl3, icasinoiiwuerfelanzahl4, icasinoiiwuerfelanzahl5, icasinoiiwuerfelanzahl6, iiwuerfelanzahl, iiwuerfelwert, iispielernummer, iirundenzahl, ssletzteaktion);
                        dateifenster.close();
                    }
                    else if (speichern == false) {
                        //Prüfen, ob Datei leer ist
                        ifstream prüfer("data4.txt", std::ios::in);
                        string stemp;
                        getline(prüfer, stemp);
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
                if (Speicher1löschen.isMouseOver(dateifenster)) {
                    speicherdateienlöschung(1);
                }
                if (Speicher2löschen.isMouseOver(dateifenster)) {
                    speicherdateienlöschung(2);
                }
                if (Speicher3löschen.isMouseOver(dateifenster)) {
                    speicherdateienlöschung(3);
                }
                if (Speicher4löschen.isMouseOver(dateifenster)) {
                    speicherdateienlöschung(4);
                }
            }
        }
    }
}

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

        sf::Text titeltext("Pausenmenü", font);
        titeltext.setCharacterSize(40);
        titeltext.setFillColor(sf::Color::Black);
        titeltext.setPosition(20.0f, 20.0f);

        Button fortsetzenbutton("Fortsetzen", { 200, 100 }, 30, sf::Color::Magenta, sf::Color::Black);
        fortsetzenbutton.setFont(font);
        fortsetzenbutton.setPosition(sf::Vector2f(20.0f, 100.0f));
        fortsetzenbutton.drawTo(pause);

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
        hilfebutton.setPosition({ 270.0f, 100.0f });
        hilfebutton.drawTo(pause);

        Button beendenbtn("Beenden", { 200, 100 }, 30, sf::Color(255, 0, 0, 255), sf::Color::Black);
        beendenbtn.setFont(font);
        beendenbtn.setPosition({ 155.0f, 350.0f });
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

            if (speichernbutton.isMouseOver(pause)) {
                dateienmanager(true);
            } 

            if (einstellungsbutton.isMouseOver(pause)) {
                Einstellungen();
            }

            if (hilfebutton.isMouseOver(pause)) {
                //Bedienungsanleitung öffnen
                system("start winword \"./Bedienungsanleitung Las Vegas.docx\"");
            }

            if (beendenbtn.isMouseOver(pause)) {
                //Nach Speicherung warnen/fragen vllt.
                bool meldung = bestätigungsmeldung("Möchten Sie den Spielstand speichern?");
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
//Dient zur Aussortierung von Würfel, auf den geklickt wurde
void setzeWürfel(int ispieler, int wert) {
    //Prüfen, ob schon ein Würfel angeklickt wurde!
    if (baufwuerfelgedrueckt == false) {
        //Da auf ein Würfel gedrückt wurde, würde ich auch lieber den Bool dafür auf true setzen XD
        baufwuerfelgedrueckt = true;

        //Variable für das Zählen der Würfel, mit den gleichen Wert
        int anzahl = 0;
        switch (wert) {
        case 1:
            //Schaue nach weiteren Würfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (iiwuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der Würfel verschwindet
                    iiwuerfelwert[i] = 8;
                }
            }
            iausgewählteAuge[0] = wert;
            iausgewählteAuge[1] = anzahl;

            //Würfel in Casinoarray setzen
            icasinoiiwuerfelanzahl1[iispielernummer] += anzahl;

            break;
        case 2:
            //Schaue nach weiteren Würfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (iiwuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der Würfel verschwindet
                    iiwuerfelwert[i] = 8;
                }
            }
            iausgewählteAuge[0] = wert;
            iausgewählteAuge[1] = anzahl;

            icasinoiiwuerfelanzahl2[iispielernummer] += anzahl;

            break;
        case 3:
            //Schaue nach weiteren Würfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (iiwuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der Würfel verschwindet
                    iiwuerfelwert[i] = 8;
                }
            }
            iausgewählteAuge[0] = wert;
            iausgewählteAuge[1] = anzahl;

            icasinoiiwuerfelanzahl3[iispielernummer] += anzahl;

            break;
        case 4:
            //Schaue nach weiteren Würfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (iiwuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der Würfel verschwindet
                    iiwuerfelwert[i] = 8;
                }
            }
            iausgewählteAuge[0] = wert;
            iausgewählteAuge[1] = anzahl;

            icasinoiiwuerfelanzahl4[iispielernummer] += anzahl;

            break;
        case 5:
            //Schaue nach weiteren Würfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (iiwuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der Würfel verschwindet
                    iiwuerfelwert[i] = 8;
                }
            }
            iausgewählteAuge[0] = wert;
            iausgewählteAuge[1] = anzahl;

            icasinoiiwuerfelanzahl5[iispielernummer] += anzahl;

            break;
        case 6:
            //Schaue nach weiteren Würfeln gleiche Wertes
            //Schleife durch jede Zahl durch
            for (int i = 0; i <= 7; i++) {
                if (iiwuerfelwert[i] == wert) {
                    anzahl++;
                    //Setze den Wert auf Acht, damit der Würfel verschwindet
                    iiwuerfelwert[i] = 8;
                }
            }
            iausgewählteAuge[0] = wert;
            iausgewählteAuge[1] = anzahl;

            icasinoiiwuerfelanzahl6[iispielernummer] += anzahl;

            break;
        }
        //Abschließende Sortierung der Würfel starten und anzeigen
        for (int i = 0; i <= 6; i++)
        {
            for (int j = i + 1; j <= 7; j++)
            {
                if (iiwuerfelwert[i] > iiwuerfelwert[j])
                {
                    //-----Tausch-----
                    int h = iiwuerfelwert[i];
                    iiwuerfelwert[i] = iiwuerfelwert[j];
                    iiwuerfelwert[j] = h;
                }
            }
        }
        //Gesamtwürfelzahl des ispielers ändern
        iiwuerfelanzahl[iispielernummer] -= anzahl;

        //String des ispielers ändern
        ssletzteaktion[iispielernummer] = "Zuletzt: " + to_string(anzahl) + " Würfeln auf Casino " + to_string(wert);
    }
}

void setzeSchrittZurück() {
    //Prüfen, ob die Funktion überhaupt was bringen würde
    if(baufwuerfelgedrueckt == true){
        baufwuerfelgedrueckt = false;
        int wert = iausgewählteAuge[0];
        int anzahl = iausgewählteAuge[1];
        int tempanzahl = iausgewählteAuge[1];

        printf("Anzahl: %i", iausgewählteAuge[1]);

        //Switch für den Casinoarrays
        switch (wert) {
        case 1:
            icasinoiiwuerfelanzahl1[iispielernummer] -= anzahl;
            break;
        case 2:
            icasinoiiwuerfelanzahl2[iispielernummer] -= anzahl;
            break;
        case 3:
            icasinoiiwuerfelanzahl3[iispielernummer] -= anzahl;
            break;
        case 4:
            icasinoiiwuerfelanzahl4[iispielernummer] -= anzahl;
            break;
        case 5:
            icasinoiiwuerfelanzahl5[iispielernummer] -= anzahl;
            break;
        case 6:
            icasinoiiwuerfelanzahl6[iispielernummer] -= anzahl;
            break;
        }

        printf("Anzahl Wuerfel auf Casino 1 zurueckgezogen: %i\n", icasinoiiwuerfelanzahl1[0]);


        //Einsetzungsverfahren
        for (int i = 0; i <= 7; i++) {
            if (iiwuerfelwert[i] == 8) {
                if (anzahl != 0) {
                    printf("Wuerfel wird zur%cckgesetzt!\n", (char)129);
                    iiwuerfelwert[i] = wert;
                    anzahl--;
                }
            }
        }

        //Würfel sortieren
        for (int i = 0; i <= 6; i++)
        {
            for (int j = i + 1; j <= 7; j++)
            {
                if (iiwuerfelwert[i] > iiwuerfelwert[j])
                {
                    //-----Tausch-----
                    int h = iiwuerfelwert[i];
                    iiwuerfelwert[i] = iiwuerfelwert[j];
                    iiwuerfelwert[j] = h;
                }
            }
        }

        //Abschließende Sortierung der Würfel starten und anzeigen
        for (int i = 0; i <= 6; i++)
        {
            for (int j = i + 1; j <= 7; j++)
            {
                if (iiwuerfelwert[i] > iiwuerfelwert[j])
                {
                    //-----Tausch-----
                    int h = iiwuerfelwert[i];
                    iiwuerfelwert[i] = iiwuerfelwert[j];
                    iiwuerfelwert[j] = h;
                }
            }
        }
        //Würfelanzahl zurücksetzen
        iiwuerfelanzahl[iispielernummer] += tempanzahl;

        //Array zurücksetzen
        iausgewählteAuge[0] = 0;
        iausgewählteAuge[1] = 0;
    }
}

void para() {
    //Erforderliche Funktionen aufrufen und ausführen und dann alles resetten
    //Geld auf Konten einzahlen
    
    //Casino1
    //Wiederhole solange, bis wir keine Preise oder ispieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(icasinoiiwuerfelanzahl1),
            size(iicasinogelder1));
        cashcounter++)
    {
        //Finde die Position vom Würfel nit dem höchsten Wert
        int* location = max_element(begin(icasinoiiwuerfelanzahl1),
            end(icasinoiiwuerfelanzahl1));
        //Transformiere Position zu einem Index
        ptrdiff_t index = distance(begin(icasinoiiwuerfelanzahl1), location);

        //Steigere den ikontostand dieses ispielers
        ikontostand[index] += iicasinogelder1[cashcounter];

        //Setze den Wert auf 0 , um den ispieler nicht erneut gewinnen zu lassen
        icasinoiiwuerfelanzahl1[index] = 0;
    }

    //Kontostände ausdrucken lassen
    for (const int& val : ikontostand) {
        cout << val << endl;
    }

    //Casino2
    //Wiederhole solange, bis wir keine Preise oder ispieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(icasinoiiwuerfelanzahl2),
            size(iicasinogelder2));
        cashcounter++)
    {
        //Finde die Position vom Würfel nit dem höchsten Wert
        int* location = max_element(begin(icasinoiiwuerfelanzahl2),
            end(icasinoiiwuerfelanzahl2));
        //Transformiere Position zu einem Index
        ptrdiff_t index = distance(begin(icasinoiiwuerfelanzahl2), location);

        //Steigere den ikontostand dieses ispielers
        ikontostand[index] += iicasinogelder2[cashcounter];

        //Setze den Wert auf 0 , um den ispieler nicht erneut gewinnen zu lassen
        icasinoiiwuerfelanzahl2[index] = 0;
    }

    //Kontostände ausdrucken lassen
    for (const int& val : ikontostand) {
        cout << val << endl;
    }

    //Casino3
    //Wiederhole solange, bis wir keine Preise oder ispieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(icasinoiiwuerfelanzahl3),
            size(iicasinogelder3));
        cashcounter++)
    {
        //Finde die Position vom Würfel nit dem höchsten Wert
        int* location = max_element(begin(icasinoiiwuerfelanzahl3),
            end(icasinoiiwuerfelanzahl3));
        //Transformiere Position zu einem Index
        ptrdiff_t index = distance(begin(icasinoiiwuerfelanzahl3), location);

        //Steigere den ikontostand dieses ispielers
        ikontostand[index] += iicasinogelder3[cashcounter];

        //Setze den Wert auf 0 , um den ispieler nicht erneut gewinnen zu lassen
        icasinoiiwuerfelanzahl3[index] = 0;
    }

    //Kontostände ausdrucken lassen
    for (const int& val : ikontostand) {
        cout << val << endl;
    }

    //Casino4
    //Wiederhole solange, bis wir keine Preise oder ispieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(icasinoiiwuerfelanzahl4),
            size(iicasinogelder4));
        cashcounter++)
    {
        //Finde die Position vom Würfel nit dem höchsten Wert
        int* location = max_element(begin(icasinoiiwuerfelanzahl4),
            end(icasinoiiwuerfelanzahl4));
        //Transformiere Position zu einem Index
        ptrdiff_t index = distance(begin(icasinoiiwuerfelanzahl4), location);

        //Steigere den ikontostand dieses ispielers
        ikontostand[index] += iicasinogelder4[cashcounter];

        //Setze den Wert auf 0 , um den ispieler nicht erneut gewinnen zu lassen
        icasinoiiwuerfelanzahl4[index] = 0;
    }

    //Kontostände ausdrucken lassen
    for (const int& val : ikontostand) {
        cout << val << endl;
    }

    //Casino5
    //Wiederhole solange, bis wir keine Preise oder ispieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(icasinoiiwuerfelanzahl5),
            size(iicasinogelder5));
        cashcounter++)
    {
        //Finde die Position vom Würfel nit dem höchsten Wert
        int* location = max_element(begin(icasinoiiwuerfelanzahl5),
            end(icasinoiiwuerfelanzahl5));
        //Transformiere Position zu einem Index
        ptrdiff_t index = distance(begin(icasinoiiwuerfelanzahl1), location);

        //Steigere den ikontostand dieses ispielers
        ikontostand[index] += iicasinogelder5[cashcounter];

        //Setze den Wert auf 0 , um den ispieler nicht erneut gewinnen zu lassen
        icasinoiiwuerfelanzahl5[index] = 0;
    }

    //Kontostände ausdrucken lassen
    for (const int& val : ikontostand) {
        cout << val << endl;
    }

    //Casino6
    //Wiederhole solange, bis wir keine Preise oder ispieler mehr haben, welches zuerst zutrifft
    for (size_t cashcounter = 0;
        cashcounter < min(size(icasinoiiwuerfelanzahl6),
            size(iicasinogelder6));
        cashcounter++)
    {
        //Finde die Position vom Würfel nit dem höchsten Wert
        int* location = max_element(begin(icasinoiiwuerfelanzahl6),
            end(icasinoiiwuerfelanzahl6));
        //Transformiere Position zu einem Index
        ptrdiff_t index = distance(begin(icasinoiiwuerfelanzahl6), location);

        //Steigere den ikontostand dieses ispielers
        ikontostand[index] += iicasinogelder6[cashcounter];

        //Setze den Wert auf 0 , um den ispieler nicht erneut gewinnen zu lassen
        icasinoiiwuerfelanzahl6[index] = 0;
    }

    //Kontostände ausdrucken lassen
    for (const int& val : ikontostand) {
        cout << val << endl;
    }

    //Resetten von alles nötigen und wiederanfangen
    for (int i = 0; i <= 2; i++) {
        icasinoiiwuerfelanzahl1[i] = 0;
        icasinoiiwuerfelanzahl2[i] = 0;
        icasinoiiwuerfelanzahl3[i] = 0;
        icasinoiiwuerfelanzahl4[i] = 0;
        icasinoiiwuerfelanzahl5[i] = 0;
        icasinoiiwuerfelanzahl6[i] = 0;
        iiwuerfelanzahl[i] = 8;
        
    }
    ispielerhabenwürfel = true;
    iispielernummer -= 1;

    //Strings für UI ändern
    ikontostandtext1 = "ikontostand: $" + to_string(ikontostand[0]);
    ikontostandtext2 = "ikontostand: $" + to_string(ikontostand[1]);
    ikontostandtext3 = "ikontostand: $" + to_string(ikontostand[2]);
}

void gewinner(int ispieler) {
    sf::Font font;
    if (!font.loadFromFile("res/Fonts/font.ttf")) {
        printf("FONT-FEHLER: SCHRIFTART KONNTE NICHT GELADEN WERDEN!\nBitte konsultieren Sie die Bedienungsanleitung!\nDr%ccken Sie eine Taste um das Programm zu beenden!", (char)129);
        _getch();
        exit(0);
    }

    sf::RenderWindow gewinnerfenster(sf::VideoMode(1000, 600), "Gewinner", sf::Style::Titlebar);

    string gewinnertext;
    switch (ispieler) {
    case 1:
        gewinnertext = "ispieler 1 hat gewonnen!";
        break;
    case 2:
        gewinnertext = "ispieler 2 hat gewonnen!";
        break;
    case 3:
        gewinnertext = "ispieler 3 hat gewonnen!";
        break;
    }

    sf::Event event;
    while (gewinnerfenster.isOpen()) {

        //Nachricht
        sf::Text nachrichtentext(gewinnertext, font);
        nachrichtentext.setCharacterSize(70);
        nachrichtentext.setFillColor(sf::Color::Black);
        //Setze den Text in die Mitte
        float fxPos = (1000.0f / 2) - (nachrichtentext.getLocalBounds().width / 2);
        float fyPos = (600.0f / 3) - (nachrichtentext.getLocalBounds().height / 2);
        nachrichtentext.setPosition(fxPos, fyPos);

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

        Button menuebutton("Hauptmenü", { 200, 100 }, 30, sf::Color::Yellow, sf::Color::Black);
        menuebutton.setFont(font);
        menuebutton.setPosition(sf::Vector2f(525.0f, 475.0f));

        Button verlassenbutton("Beenden", { 200, 100 }, 30, sf::Color::Red, sf::Color::Black);
        verlassenbutton.setFont(font);
        verlassenbutton.setPosition(sf::Vector2f(775.0f, 475.0f));

        sf::Event event;
        while (gewinnerfenster.pollEvent(event)) {
            // Schließt das Fenster, falls schließen gedrückt wird

            switch (event.type) {
                //Wenn es geklickt hat
            case sf::Event::MouseButtonPressed:
                if (spielladenbutton.isMouseOver(gewinnerfenster)) {
                    gewinnerfenster.close();
                    dateienmanager(false);
                }

                if (neuesspielbutton.isMouseOver(gewinnerfenster)) {
                    gewinnerfenster.close();
                    //Variablen setzen
                    for (int i = 0; i <= 4; i++) {
                        iicasinogelder1[i] = 0;
                        iicasinogelder2[i] = 0;
                        iicasinogelder3[i] = 0;
                        iicasinogelder4[i] = 0;
                        iicasinogelder5[i] = 0;
                        iicasinogelder6[i] = 0;
                    }

                    for (int i = 0; i <= 2; i++) {
                        icasinoiiwuerfelanzahl1[i] = 0;
                        icasinoiiwuerfelanzahl2[i] = 0;
                        icasinoiiwuerfelanzahl3[i] = 0;
                        icasinoiiwuerfelanzahl4[i] = 0;
                        icasinoiiwuerfelanzahl5[i] = 0;
                        icasinoiiwuerfelanzahl6[i] = 0;
                        ikontostand[i] = 0;
                        iiwuerfelanzahl[i] = 8;
                    }
                    iirundenzahl = 0;
                    isumme1 = 0;
                    isumme2 = 0;
                    isumme3 = 0;
                    isumme4 = 0;
                    isumme5 = 0;
                    isumme6 = 0;
                    ispielerhabenwürfel = true;
                    baufwuerfelgedrueckt = false;
                    bspielbeendet = false;
                    bzugbeendet = false;
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

        //Variablen!!
        //Für Casinos
        //Variablen für Casino 1
        int ic1ispieler1 = icasinoiiwuerfelanzahl1[0];
        int ic1ispieler2 = icasinoiiwuerfelanzahl1[1];
        int ic1ispieler3 = icasinoiiwuerfelanzahl1[2];

        //Variablen für Casino 2
        int ic2ispieler1 = icasinoiiwuerfelanzahl2[0];
        int ic2ispieler2 = icasinoiiwuerfelanzahl2[1];
        int ic2ispieler3 = icasinoiiwuerfelanzahl2[2];

        //Variablen für Casino 3
        int ic3ispieler1 = icasinoiiwuerfelanzahl3[0];
        int ic3ispieler2 = icasinoiiwuerfelanzahl3[1];
        int ic3ispieler3 = icasinoiiwuerfelanzahl3[2];

        //Variablen für Casino 4
        int ic4ispieler1 = icasinoiiwuerfelanzahl4[0];
        int ic4ispieler2 = icasinoiiwuerfelanzahl4[1];
        int ic4ispieler3 = icasinoiiwuerfelanzahl4[2];

        //Variablen für Casino 5
        int ic5ispieler1 = icasinoiiwuerfelanzahl5[0];
        int ic5ispieler2 = icasinoiiwuerfelanzahl5[1];
        int ic5ispieler3 = icasinoiiwuerfelanzahl5[2];

        //Variablen für Casino 6
        int ic6ispieler1 = icasinoiiwuerfelanzahl6[0];
        int ic6ispieler2 = icasinoiiwuerfelanzahl6[1];
        int ic6ispieler3 = icasinoiiwuerfelanzahl6[2];

        //
        //Würfel zeichnen
        //
        sf::Texture wuerfel;
        switch (iiwuerfelwert[0]) {
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
        //wuerfelsprite.setScale(0.15, 0.15);

        sf::Texture wuerfel2;
        switch (iiwuerfelwert[1]) {
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
        //wuerfelsprite2.setScale(0.15, 0.15);

        sf::Texture wuerfel3;
        switch (iiwuerfelwert[2]) {
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
        //wuerfelsprite3.setScale(0.15, 0.15);

        sf::Texture wuerfel4;
        switch (iiwuerfelwert[3]) {
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
        //wuerfelsprite4.setScale(0.15, 0.15);

        sf::Texture wuerfel5;
        switch (iiwuerfelwert[4]) {
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
        //wuerfelsprite5.setScale(0.15, 0.15);

        sf::Texture wuerfel6;
        switch (iiwuerfelwert[5]) {
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
        //wuerfelsprite6.setScale(0.15, 0.15);

        sf::Texture wuerfel7;
        switch (iiwuerfelwert[6]) {
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
        //wuerfelsprite7.setScale(0.15, 0.15);

        sf::Texture wuerfel8;
        switch (iiwuerfelwert[7]) {
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
        rundenkasten.setPoint(0, sf::Vector2f(1251.0f, 90.0f));
        rundenkasten.setPoint(1, sf::Vector2f(1251.0f, 150.0f));
        rundenkasten.setPoint(2, sf::Vector2f(1600.0f, 150.0f));
        rundenkasten.setPoint(3, sf::Vector2f(1600.0f, 90.0f));
        rundenkasten.setFillColor(Color(150, 150, 150, 150));
        spiel.draw(rundenkasten);

        sf::Text srundentext(srundentext, font);
        srundentext.setCharacterSize(30);
        srundentext.setFillColor(sf::Color::Black);
        srundentext.setPosition(1350.0f, 100.0f);
        spiel.draw(srundentext);


        //ispieler 1 Kasten
        sf::ConvexShape ispieler1kasten;
        ispieler1kasten.setPointCount(4);
        ispieler1kasten.setPoint(0, sf::Vector2f(1252.0f, 200.0f));
        ispieler1kasten.setPoint(1, sf::Vector2f(1600.0f, 200.0f));
        ispieler1kasten.setPoint(2, sf::Vector2f(1600.0f, 305.0f));
        ispieler1kasten.setPoint(3, sf::Vector2f(1252.0f, 305.0f));
        ispieler1kasten.setFillColor(farben[0] - sf::Color(0, 0, 0, 200));
        spiel.draw(ispieler1kasten);

        sf::Text ispieler1text("ispieler 1", font);
        ispieler1text.setCharacterSize(25);
        ispieler1text.setFillColor(farben[0]);
        ispieler1text.setPosition(1385.0f, 205.0f);
        spiel.draw(ispieler1text);

        sf::Text ispieler1geld(ikontostandtext1, font);
        ispieler1geld.setCharacterSize(20);
        ispieler1geld.setFillColor(farben[0]);
        ispieler1geld.setPosition(1260.0f, 235.0f);
        spiel.draw(ispieler1geld);

        sf::Text ispieler1würfelzahl(swuerfelzahl1, font);
        ispieler1würfelzahl.setCharacterSize(20);
        ispieler1würfelzahl.setFillColor(farben[0]);
        ispieler1würfelzahl.setPosition(1260.0f, 255.0f);
        spiel.draw(ispieler1würfelzahl);

        sf::Text ispieler1zuletztaktion(ssletzteaktion[0], font);
        ispieler1zuletztaktion.setCharacterSize(20);
        ispieler1zuletztaktion.setFillColor(farben[0]);
        ispieler1zuletztaktion.setPosition(1260.0f, 275.0f);
        spiel.draw(ispieler1zuletztaktion);

        //ispieler 2 Kasten
        sf::ConvexShape ispieler2kasten;
        ispieler2kasten.setPointCount(4);
        ispieler2kasten.setPoint(0, sf::Vector2f(1252.0f, 400.0f));
        ispieler2kasten.setPoint(1, sf::Vector2f(1600.0f, 400.0f));
        ispieler2kasten.setPoint(2, sf::Vector2f(1600.0f, 505.0f));
        ispieler2kasten.setPoint(3, sf::Vector2f(1252.0f, 505.0f));
        ispieler2kasten.setFillColor(farben[1] - sf::Color(0, 0, 0, 200));
        spiel.draw(ispieler2kasten);

        sf::Text ispieler2text("ispieler 2", font);
        ispieler2text.setCharacterSize(25);
        ispieler2text.setFillColor(farben[1]);
        ispieler2text.setPosition(1385.0f, 405.0f);
        spiel.draw(ispieler2text);

        sf::Text ispieler2geld(ikontostandtext2, font);
        ispieler2geld.setCharacterSize(20);
        ispieler2geld.setFillColor(farben[1]);
        ispieler2geld.setPosition(1260.0f, 435.0f);
        spiel.draw(ispieler2geld);

        sf::Text ispieler2würfelzahl(swuerfelzahl2, font);
        ispieler2würfelzahl.setCharacterSize(20);
        ispieler2würfelzahl.setFillColor(farben[1]);
        ispieler2würfelzahl.setPosition(1260.0f, 455.0f);
        spiel.draw(ispieler2würfelzahl);

        sf::Text ispieler2zuletztaktion(ssletzteaktion[1], font);
        ispieler2zuletztaktion.setCharacterSize(20);
        ispieler2zuletztaktion.setFillColor(farben[1]);
        ispieler2zuletztaktion.setPosition(1260.0f, 475.0f);
        spiel.draw(ispieler2zuletztaktion);

        //ispieler 3 Kasten
        sf::ConvexShape ispieler3kasten;
        ispieler3kasten.setPointCount(4);
        ispieler3kasten.setPoint(0, sf::Vector2f(1252.0f, 600.0f));
        ispieler3kasten.setPoint(1, sf::Vector2f(1600.0f, 600.0f));
        ispieler3kasten.setPoint(2, sf::Vector2f(1600.0f, 705.0f));
        ispieler3kasten.setPoint(3, sf::Vector2f(1252.0f, 705.0f));
        ispieler3kasten.setFillColor(farben[2] - sf::Color(0, 0, 0, 200));
        spiel.draw(ispieler3kasten);

        sf::Text ispieler3text("ispieler 3", font);
        ispieler3text.setCharacterSize(25);
        ispieler3text.setFillColor(farben[2]);
        ispieler3text.setPosition(1385.0f, 605.0f);
        spiel.draw(ispieler3text);

        sf::Text ispieler3geld(ikontostandtext3, font);
        ispieler3geld.setCharacterSize(20);
        ispieler3geld.setFillColor(farben[2]);
        ispieler3geld.setPosition(1260.0f, 635.0f);
        spiel.draw(ispieler3geld);

        sf::Text ispieler3würfelzahl(swuerfelzahl3, font);
        ispieler3würfelzahl.setCharacterSize(20);
        ispieler3würfelzahl.setFillColor(farben[2]);
        ispieler3würfelzahl.setPosition(1260.0f, 655.0f);
        spiel.draw(ispieler3würfelzahl);

        sf::Text ispieler3zuletztaktion(ssletzteaktion[2], font);
        ispieler3zuletztaktion.setCharacterSize(20);
        ispieler3zuletztaktion.setFillColor(farben[2]);
        ispieler3zuletztaktion.setPosition(1260.0f, 675.0f);
        spiel.draw(ispieler3zuletztaktion);

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

        //Eigentlicher Text
        sf::Text subtext(ssubtextstring, font);
        subtext.setCharacterSize(25);
        //Setze den Text in die Mitte
        float fxPos = (0.0f + 1600.0f / 2) - (subtext.getLocalBounds().width / 2);
        float fyPos = (900.0f + 35.0f / 2.6) - (subtext.getLocalBounds().height / 2);
        subtext.setPosition(fxPos, fyPos);
        //subtext.setPosition(sf::Vector2f(600.0f, 900.0f));
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
        if (baufwuerfelgedrueckt == true) {
            undobutton.drawTo(spiel);
        }
        
        //Zug Beenden Button
        Button donebutton("Zug beenden", { 170, 50 }, 20, sf::Color::Yellow, sf::Color::Black);
        donebutton.setFont(font);
        donebutton.setPosition({ 1410.0f, 942.5f });
        if (baufwuerfelgedrueckt == true) {
            donebutton.drawTo(spiel);
        }

        //
        //Geldscheine
        sf::Texture c1gs1;
        int geldscheinc1gs1 = iicasinogelder1[0];
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
        switch (iicasinogelder1[1]) {
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
        switch (iicasinogelder1[2]) {
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
        switch (iicasinogelder1[3]) {
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
        switch (iicasinogelder1[4]) {
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
        switch (iicasinogelder2[0]) {
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
        switch (iicasinogelder2[1]) {
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
        switch (iicasinogelder2[2]) {
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
        switch (iicasinogelder2[3]) {
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
        switch (iicasinogelder2[4]) {
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
        switch (iicasinogelder3[0]) {
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
        switch (iicasinogelder3[1]) {
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
        switch (iicasinogelder3[2]) {
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
        switch (iicasinogelder3[3]) {
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
        switch (iicasinogelder3[4]) {
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
        switch (iicasinogelder4[0]) {
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
        switch (iicasinogelder4[1]) {
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
        switch (iicasinogelder4[2]) {
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
        switch (iicasinogelder4[3]) {
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
        switch (iicasinogelder4[4]) {
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
        switch (iicasinogelder5[0]) {
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
        switch (iicasinogelder5[1]) {
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
        switch (iicasinogelder5[2]) {
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
        switch (iicasinogelder5[3]) {
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
        switch (iicasinogelder5[4]) {
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
        switch (iicasinogelder6[0]) {
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
        switch (iicasinogelder6[1]) {
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
        switch (iicasinogelder6[2]) {
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
        switch (iicasinogelder6[3]) {
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
        switch (iicasinogelder6[4]) {
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

        //Würfel auf Casinos zeichnen
        //Aufbau der snamen: c = Casino, z = Zeile, s = Spalte
        //Es wird von oben nach unten gezählt sprich von links nach rechts: c1z6s4 ist rechtsunten

        sf::Texture c1z1s1;
        c1z1s1.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z1s1.setSmooth(true);
        sf::Sprite c1z1s1sprite(c1z1s1);
        c1z1s1sprite.setPosition(20.0f, 520.0f);
        c1z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z1s2;
        c1z1s2.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z1s2.setSmooth(true);
        sf::Sprite c1z1s2sprite(c1z1s2);
        c1z1s2sprite.setPosition(65.0f, 520.0f);
        c1z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z1s3;
        c1z1s3.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z1s3.setSmooth(true);
        sf::Sprite c1z1s3sprite(c1z1s3);
        c1z1s3sprite.setPosition(110.0f, 520.0f);
        c1z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z1s4;
        c1z1s4.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z1s4.setSmooth(true);
        sf::Sprite c1z1s4sprite(c1z1s4);
        c1z1s4sprite.setPosition(155.0f, 520.0f);
        c1z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z2s1;
        c1z2s1.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z2s1.setSmooth(true);
        sf::Sprite c1z2s1sprite(c1z2s1);
        c1z2s1sprite.setPosition(20.0f, 565.0f);
        c1z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z2s2;
        c1z2s2.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z2s2.setSmooth(true);
        sf::Sprite c1z2s2sprite(c1z2s2);
        c1z2s2sprite.setPosition(65.0f, 565.0f);
        c1z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z2s3;
        c1z2s3.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z2s3.setSmooth(true);
        sf::Sprite c1z2s3sprite(c1z2s3);
        c1z2s3sprite.setPosition(110.0f, 565.0f);
        c1z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z2s4;
        c1z2s4.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z2s4.setSmooth(true);
        sf::Sprite c1z2s4sprite(c1z2s4);
        c1z2s4sprite.setPosition(155.0f, 565.0f);
        c1z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z3s1;
        c1z3s1.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z3s1.setSmooth(true);
        sf::Sprite c1z3s1sprite(c1z3s1);
        c1z3s1sprite.setPosition(20.0f, 610.0f);
        c1z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z3s2;
        c1z3s2.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z3s2.setSmooth(true);
        sf::Sprite c1z3s2sprite(c1z3s2);
        c1z3s2sprite.setPosition(65.0f, 610.0f);
        c1z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z3s3;
        c1z3s3.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z3s3.setSmooth(true);
        sf::Sprite c1z3s3sprite(c1z3s3);
        c1z3s3sprite.setPosition(110.0f, 610.0f);
        c1z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z3s4;
        c1z3s4.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z3s4.setSmooth(true);
        sf::Sprite c1z3s4sprite(c1z3s4);
        c1z3s4sprite.setPosition(155.0f, 610.0f);
        c1z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z4s1;
        c1z4s1.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z4s1.setSmooth(true);
        sf::Sprite c1z4s1sprite(c1z4s1);
        c1z4s1sprite.setPosition(20.0f, 655.0f);
        c1z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z4s2;
        c1z4s2.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z4s2.setSmooth(true);
        sf::Sprite c1z4s2sprite(c1z4s2);
        c1z4s2sprite.setPosition(65.0f, 655.0f);
        c1z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z4s3;
        c1z4s3.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z4s3.setSmooth(true);
        sf::Sprite c1z4s3sprite(c1z4s3);
        c1z4s3sprite.setPosition(110.0f, 655.0f);
        c1z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z4s4;
        c1z4s4.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z4s4.setSmooth(true);
        sf::Sprite c1z4s4sprite(c1z4s4);
        c1z4s4sprite.setPosition(155.0f, 655.0f);
        c1z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z5s1;
        c1z5s1.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z5s1.setSmooth(true);
        sf::Sprite c1z5s1sprite(c1z5s1);
        c1z5s1sprite.setPosition(20.0f, 700.0f);
        c1z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z5s2;
        c1z5s2.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z5s2.setSmooth(true);
        sf::Sprite c1z5s2sprite(c1z5s2);
        c1z5s2sprite.setPosition(65.0f, 700.0f);
        c1z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z5s3;
        c1z5s3.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z5s3.setSmooth(true);
        sf::Sprite c1z5s3sprite(c1z5s3);
        c1z5s3sprite.setPosition(110.0f, 700.0f);
        c1z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c1z5s4;
        c1z5s4.loadFromFile("res/Bilder/würfel/dice-png-1.png");
        c1z5s4.setSmooth(true);
        sf::Sprite c1z5s4sprite(c1z5s4);
        c1z5s4sprite.setPosition(155.0f, 700.0f);
        c1z5s4sprite.setScale(0.5f, 0.5f);

        //Casino 2
        sf::Texture c2z1s1;
        c2z1s1.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z1s1.setSmooth(true);
        sf::Sprite c2z1s1sprite(c2z1s1);
        c2z1s1sprite.setPosition(220.0f, 520.0f);
        c2z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z1s2;
        c2z1s2.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z1s2.setSmooth(true);
        sf::Sprite c2z1s2sprite(c2z1s2);
        c2z1s2sprite.setPosition(265.0f, 520.0f);
        c2z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z1s3;
        c2z1s3.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z1s3.setSmooth(true);
        sf::Sprite c2z1s3sprite(c2z1s3);
        c2z1s3sprite.setPosition(310.0f, 520.0f);
        c2z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z1s4;
        c2z1s4.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z1s4.setSmooth(true);
        sf::Sprite c2z1s4sprite(c2z1s4);
        c2z1s4sprite.setPosition(355.0f, 520.0f);
        c2z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z2s1;
        c2z2s1.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z2s1.setSmooth(true);
        sf::Sprite c2z2s1sprite(c2z2s1);
        c2z2s1sprite.setPosition(220.0f, 565.0f);
        c2z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z2s2;
        c2z2s2.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z2s2.setSmooth(true);
        sf::Sprite c2z2s2sprite(c2z2s2);
        c2z2s2sprite.setPosition(265.0f, 565.0f);
        c2z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z2s3;
        c2z2s3.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z2s3.setSmooth(true);
        sf::Sprite c2z2s3sprite(c2z2s3);
        c2z2s3sprite.setPosition(310.0f, 565.0f);
        c2z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z2s4;
        c2z2s4.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z2s4.setSmooth(true);
        sf::Sprite c2z2s4sprite(c2z2s4);
        c2z2s4sprite.setPosition(355.0f, 565.0f);
        c2z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z3s1;
        c2z3s1.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z3s1.setSmooth(true);
        sf::Sprite c2z3s1sprite(c2z3s1);
        c2z3s1sprite.setPosition(220.0f, 610.0f);
        c2z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z3s2;
        c2z3s2.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z3s2.setSmooth(true);
        sf::Sprite c2z3s2sprite(c2z3s2);
        c2z3s2sprite.setPosition(265.0f, 610.0f);
        c2z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z3s3;
        c2z3s3.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z3s3.setSmooth(true);
        sf::Sprite c2z3s3sprite(c2z3s3);
        c2z3s3sprite.setPosition(310.0f, 610.0f);
        c2z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z3s4;
        c2z3s4.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z3s4.setSmooth(true);
        sf::Sprite c2z3s4sprite(c2z3s4);
        c2z3s4sprite.setPosition(355.0f, 610.0f);
        c2z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z4s1;
        c2z4s1.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z4s1.setSmooth(true);
        sf::Sprite c2z4s1sprite(c2z4s1);
        c2z4s1sprite.setPosition(220.0f, 655.0f);
        c2z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z4s2;
        c2z4s2.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z4s2.setSmooth(true);
        sf::Sprite c2z4s2sprite(c2z4s2);
        c2z4s2sprite.setPosition(265.0f, 655.0f);
        c2z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z4s3;
        c2z4s3.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z4s3.setSmooth(true);
        sf::Sprite c2z4s3sprite(c2z4s3);
        c2z4s3sprite.setPosition(310.0f, 655.0f);
        c2z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z4s4;
        c2z4s4.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z4s4.setSmooth(true);
        sf::Sprite c2z4s4sprite(c2z4s4);
        c2z4s4sprite.setPosition(355.0f, 655.0f);
        c2z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z5s1;
        c2z5s1.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z5s1.setSmooth(true);
        sf::Sprite c2z5s1sprite(c2z5s1);
        c2z5s1sprite.setPosition(220.0f, 700.0f);
        c2z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z5s2;
        c2z5s2.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z5s2.setSmooth(true);
        sf::Sprite c2z5s2sprite(c2z5s2);
        c2z5s2sprite.setPosition(265.0f, 700.0f);
        c2z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z5s3;
        c2z5s3.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z5s3.setSmooth(true);
        sf::Sprite c2z5s3sprite(c2z5s3);
        c2z5s3sprite.setPosition(310.0f, 700.0f);
        c2z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c2z5s4;
        c2z5s4.loadFromFile("res/Bilder/würfel/dice-png-2.png");
        c2z5s4.setSmooth(true);
        sf::Sprite c2z5s4sprite(c2z5s4);
        c2z5s4sprite.setPosition(355.0f, 700.0f);
        c2z5s4sprite.setScale(0.5f, 0.5f);

        //Casino 3
        sf::Texture c3z1s1;
        c3z1s1.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z1s1.setSmooth(true);
        sf::Sprite c3z1s1sprite(c3z1s1);
        c3z1s1sprite.setPosition(420.0f, 520.0f);
        c3z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z1s2;
        c3z1s2.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z1s2.setSmooth(true);
        sf::Sprite c3z1s2sprite(c3z1s2);
        c3z1s2sprite.setPosition(465.0f, 520.0f);
        c3z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z1s3;
        c3z1s3.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z1s3.setSmooth(true);
        sf::Sprite c3z1s3sprite(c3z1s3);
        c3z1s3sprite.setPosition(510.0f, 520.0f);
        c3z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z1s4;
        c3z1s4.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z1s4.setSmooth(true);
        sf::Sprite c3z1s4sprite(c3z1s4);
        c3z1s4sprite.setPosition(555.0f, 520.0f);
        c3z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z2s1;
        c3z2s1.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z2s1.setSmooth(true);
        sf::Sprite c3z2s1sprite(c3z2s1);
        c3z2s1sprite.setPosition(420.0f, 565.0f);
        c3z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z2s2;
        c3z2s2.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z2s2.setSmooth(true);
        sf::Sprite c3z2s2sprite(c3z2s2);
        c3z2s2sprite.setPosition(465.0f, 565.0f);
        c3z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z2s3;
        c3z2s3.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z2s3.setSmooth(true);
        sf::Sprite c3z2s3sprite(c3z2s3);
        c3z2s3sprite.setPosition(510.0f, 565.0f);
        c3z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z2s4;
        c3z2s4.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z2s4.setSmooth(true);
        sf::Sprite c3z2s4sprite(c3z2s4);
        c3z2s4sprite.setPosition(555.0f, 565.0f);
        c3z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z3s1;
        c3z3s1.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z3s1.setSmooth(true);
        sf::Sprite c3z3s1sprite(c3z3s1);
        c3z3s1sprite.setPosition(420.0f, 610.0f);
        c3z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z3s2;
        c3z3s2.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z3s2.setSmooth(true);
        sf::Sprite c3z3s2sprite(c3z3s2);
        c3z3s2sprite.setPosition(465.0f, 610.0f);
        c3z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z3s3;
        c3z3s3.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z3s3.setSmooth(true);
        sf::Sprite c3z3s3sprite(c3z3s3);
        c3z3s3sprite.setPosition(510.0f, 610.0f);
        c3z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z3s4;
        c3z3s4.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z3s4.setSmooth(true);
        sf::Sprite c3z3s4sprite(c3z3s4);
        c3z3s4sprite.setPosition(555.0f, 610.0f);
        c3z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z4s1;
        c3z4s1.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z4s1.setSmooth(true);
        sf::Sprite c3z4s1sprite(c3z4s1);
        c3z4s1sprite.setPosition(420.0f, 655.0f);
        c3z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z4s2;
        c3z4s2.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z4s2.setSmooth(true);
        sf::Sprite c3z4s2sprite(c3z4s2);
        c3z4s2sprite.setPosition(465.0f, 655.0f);
        c3z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z4s3;
        c3z4s3.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z4s3.setSmooth(true);
        sf::Sprite c3z4s3sprite(c3z4s3);
        c3z4s3sprite.setPosition(510.0f, 655.0f);
        c3z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z4s4;
        c3z4s4.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z4s4.setSmooth(true);
        sf::Sprite c3z4s4sprite(c3z4s4);
        c3z4s4sprite.setPosition(555.0f, 655.0f);
        c3z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z5s1;
        c3z5s1.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z5s1.setSmooth(true);
        sf::Sprite c3z5s1sprite(c3z5s1);
        c3z5s1sprite.setPosition(420.0f, 700.0f);
        c3z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z5s2;
        c3z5s2.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z5s2.setSmooth(true);
        sf::Sprite c3z5s2sprite(c3z5s2);
        c3z5s2sprite.setPosition(465.0f, 700.0f);
        c3z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z5s3;
        c3z5s3.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z5s3.setSmooth(true);
        sf::Sprite c3z5s3sprite(c3z5s3);
        c3z5s3sprite.setPosition(510.0f, 700.0f);
        c3z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c3z5s4;
        c3z5s4.loadFromFile("res/Bilder/würfel/dice-png-3.png");
        c3z5s4.setSmooth(true);
        sf::Sprite c3z5s4sprite(c3z5s4);
        c3z5s4sprite.setPosition(555.0f, 700.0f);
        c3z5s4sprite.setScale(0.5f, 0.5f);

        //Casino 4
        sf::Texture c4z1s1;
        c4z1s1.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z1s1.setSmooth(true);
        sf::Sprite c4z1s1sprite(c4z1s1);
        c4z1s1sprite.setPosition(620.0f, 520.0f);
        c4z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z1s2;
        c4z1s2.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z1s2.setSmooth(true);
        sf::Sprite c4z1s2sprite(c4z1s2);
        c4z1s2sprite.setPosition(665.0f, 520.0f);
        c4z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z1s3;
        c4z1s3.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z1s3.setSmooth(true);
        sf::Sprite c4z1s3sprite(c4z1s3);
        c4z1s3sprite.setPosition(710.0f, 520.0f);
        c4z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z1s4;
        c4z1s4.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z1s4.setSmooth(true);
        sf::Sprite c4z1s4sprite(c4z1s4);
        c4z1s4sprite.setPosition(755.0f, 520.0f);
        c4z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z2s1;
        c4z2s1.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z2s1.setSmooth(true);
        sf::Sprite c4z2s1sprite(c4z2s1);
        c4z2s1sprite.setPosition(620.0f, 565.0f);
        c4z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z2s2;
        c4z2s2.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z2s2.setSmooth(true);
        sf::Sprite c4z2s2sprite(c4z2s2);
        c4z2s2sprite.setPosition(665.0f, 565.0f);
        c4z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z2s3;
        c4z2s3.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z2s3.setSmooth(true);
        sf::Sprite c4z2s3sprite(c4z2s3);
        c4z2s3sprite.setPosition(710.0f, 565.0f);
        c4z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z2s4;
        c4z2s4.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z2s4.setSmooth(true);
        sf::Sprite c4z2s4sprite(c4z2s4);
        c4z2s4sprite.setPosition(755.0f, 565.0f);
        c4z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z3s1;
        c4z3s1.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z3s1.setSmooth(true);
        sf::Sprite c4z3s1sprite(c4z3s1);
        c4z3s1sprite.setPosition(620.0f, 610.0f);
        c4z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z3s2;
        c4z3s2.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z3s2.setSmooth(true);
        sf::Sprite c4z3s2sprite(c4z3s2);
        c4z3s2sprite.setPosition(665.0f, 610.0f);
        c4z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z3s3;
        c4z3s3.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z3s3.setSmooth(true);
        sf::Sprite c4z3s3sprite(c4z3s3);
        c4z3s3sprite.setPosition(710.0f, 610.0f);
        c4z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z3s4;
        c4z3s4.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z3s4.setSmooth(true);
        sf::Sprite c4z3s4sprite(c4z3s4);
        c4z3s4sprite.setPosition(755.0f, 610.0f);
        c4z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z4s1;
        c4z4s1.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z4s1.setSmooth(true);
        sf::Sprite c4z4s1sprite(c4z4s1);
        c4z4s1sprite.setPosition(620.0f, 655.0f);
        c4z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z4s2;
        c4z4s2.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z4s2.setSmooth(true);
        sf::Sprite c4z4s2sprite(c4z4s2);
        c4z4s2sprite.setPosition(665.0f, 655.0f);
        c4z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z4s3;
        c4z4s3.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z4s3.setSmooth(true);
        sf::Sprite c4z4s3sprite(c4z4s3);
        c4z4s3sprite.setPosition(710.0f, 655.0f);
        c4z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z4s4;
        c4z4s4.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z4s4.setSmooth(true);
        sf::Sprite c4z4s4sprite(c4z4s4);
        c4z4s4sprite.setPosition(755.0f, 655.0f);
        c4z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z5s1;
        c4z5s1.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z5s1.setSmooth(true);
        sf::Sprite c4z5s1sprite(c4z5s1);
        c4z5s1sprite.setPosition(620.0f, 700.0f);
        c4z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z5s2;
        c4z5s2.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z5s2.setSmooth(true);
        sf::Sprite c4z5s2sprite(c4z5s2);
        c4z5s2sprite.setPosition(665.0f, 700.0f);
        c4z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z5s3;
        c4z5s3.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z5s3.setSmooth(true);
        sf::Sprite c4z5s3sprite(c4z5s3);
        c4z5s3sprite.setPosition(710.0f, 700.0f);
        c4z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c4z5s4;
        c4z5s4.loadFromFile("res/Bilder/würfel/dice-png-4.png");
        c4z5s4.setSmooth(true);
        sf::Sprite c4z5s4sprite(c4z5s4);
        c4z5s4sprite.setPosition(755.0f, 700.0f);
        c4z5s4sprite.setScale(0.5f, 0.5f);

        //Casino 5
        sf::Texture c5z1s1;
        c5z1s1.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z1s1.setSmooth(true);
        sf::Sprite c5z1s1sprite(c5z1s1);
        c5z1s1sprite.setPosition(820.0f, 520.0f);
        c5z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z1s2;
        c5z1s2.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z1s2.setSmooth(true);
        sf::Sprite c5z1s2sprite(c5z1s2);
        c5z1s2sprite.setPosition(865.0f, 520.0f);
        c5z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z1s3;
        c5z1s3.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z1s3.setSmooth(true);
        sf::Sprite c5z1s3sprite(c5z1s3);
        c5z1s3sprite.setPosition(910.0f, 520.0f);
        c5z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z1s4;
        c5z1s4.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z1s4.setSmooth(true);
        sf::Sprite c5z1s4sprite(c5z1s4);
        c5z1s4sprite.setPosition(955.0f, 520.0f);
        c5z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z2s1;
        c5z2s1.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z2s1.setSmooth(true);
        sf::Sprite c5z2s1sprite(c5z2s1);
        c5z2s1sprite.setPosition(820.0f, 565.0f);
        c5z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z2s2;
        c5z2s2.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z2s2.setSmooth(true);
        sf::Sprite c5z2s2sprite(c5z2s2);
        c5z2s2sprite.setPosition(865.0f, 565.0f);
        c5z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z2s3;
        c5z2s3.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z2s3.setSmooth(true);
        sf::Sprite c5z2s3sprite(c5z2s3);
        c5z2s3sprite.setPosition(910.0f, 565.0f);
        c5z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z2s4;
        c5z2s4.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z2s4.setSmooth(true);
        sf::Sprite c5z2s4sprite(c5z2s4);
        c5z2s4sprite.setPosition(955.0f, 565.0f);
        c5z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z3s1;
        c5z3s1.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z3s1.setSmooth(true);
        sf::Sprite c5z3s1sprite(c5z3s1);
        c5z3s1sprite.setPosition(820.0f, 610.0f);
        c5z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z3s2;
        c5z3s2.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z3s2.setSmooth(true);
        sf::Sprite c5z3s2sprite(c5z3s2);
        c5z3s2sprite.setPosition(865.0f, 610.0f);
        c5z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z3s3;
        c5z3s3.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z3s3.setSmooth(true);
        sf::Sprite c5z3s3sprite(c5z3s3);
        c5z3s3sprite.setPosition(910.0f, 610.0f);
        c5z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z3s4;
        c5z3s4.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z3s4.setSmooth(true);
        sf::Sprite c5z3s4sprite(c5z3s4);
        c5z3s4sprite.setPosition(955.0f, 610.0f);
        c5z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z4s1;
        c5z4s1.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z4s1.setSmooth(true);
        sf::Sprite c5z4s1sprite(c5z4s1);
        c5z4s1sprite.setPosition(820.0f, 655.0f);
        c5z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z4s2;
        c5z4s2.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z4s2.setSmooth(true);
        sf::Sprite c5z4s2sprite(c5z4s2);
        c5z4s2sprite.setPosition(865.0f, 655.0f);
        c5z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z4s3;
        c5z4s3.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z4s3.setSmooth(true);
        sf::Sprite c5z4s3sprite(c5z4s3);
        c5z4s3sprite.setPosition(910.0f, 655.0f);
        c5z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z4s4;
        c5z4s4.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z4s4.setSmooth(true);
        sf::Sprite c5z4s4sprite(c5z4s4);
        c5z4s4sprite.setPosition(955.0f, 655.0f);
        c5z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z5s1;
        c5z5s1.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z5s1.setSmooth(true);
        sf::Sprite c5z5s1sprite(c5z5s1);
        c5z5s1sprite.setPosition(820.0f, 700.0f);
        c5z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z5s2;
        c5z5s2.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z5s2.setSmooth(true);
        sf::Sprite c5z5s2sprite(c5z5s2);
        c5z5s2sprite.setPosition(865.0f, 700.0f);
        c5z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z5s3;
        c5z5s3.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z5s3.setSmooth(true);
        sf::Sprite c5z5s3sprite(c5z5s3);
        c5z5s3sprite.setPosition(910.0f, 700.0f);
        c5z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c5z5s4;
        c5z5s4.loadFromFile("res/Bilder/würfel/dice-png-5.png");
        c5z5s4.setSmooth(true);
        sf::Sprite c5z5s4sprite(c5z5s4);
        c5z5s4sprite.setPosition(955.0f, 700.0f);
        c5z5s4sprite.setScale(0.5f, 0.5f);

        //Casino 6
        sf::Texture c6z1s1;
        c6z1s1.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z1s1.setSmooth(true);
        sf::Sprite c6z1s1sprite(c6z1s1);
        c6z1s1sprite.setPosition(1020.0f, 520.0f);
        c6z1s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z1s2;
        c6z1s2.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z1s2.setSmooth(true);
        sf::Sprite c6z1s2sprite(c6z1s2);
        c6z1s2sprite.setPosition(1065.0f, 520.0f);
        c6z1s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z1s3;
        c6z1s3.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z1s3.setSmooth(true);
        sf::Sprite c6z1s3sprite(c6z1s3);
        c6z1s3sprite.setPosition(1110.0f, 520.0f);
        c6z1s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z1s4;
        c6z1s4.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z1s4.setSmooth(true);
        sf::Sprite c6z1s4sprite(c6z1s4);
        c6z1s4sprite.setPosition(1155.0f, 520.0f);
        c6z1s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z2s1;
        c6z2s1.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z2s1.setSmooth(true);
        sf::Sprite c6z2s1sprite(c6z2s1);
        c6z2s1sprite.setPosition(1020.0f, 565.0f);
        c6z2s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z2s2;
        c6z2s2.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z2s2.setSmooth(true);
        sf::Sprite c6z2s2sprite(c6z2s2);
        c6z2s2sprite.setPosition(1065.0f, 565.0f);
        c6z2s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z2s3;
        c6z2s3.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z2s3.setSmooth(true);
        sf::Sprite c6z2s3sprite(c6z2s3);
        c6z2s3sprite.setPosition(1110.0f, 565.0f);
        c6z2s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z2s4;
        c6z2s4.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z2s4.setSmooth(true);
        sf::Sprite c6z2s4sprite(c6z2s4);
        c6z2s4sprite.setPosition(1155.0f, 565.0f);
        c6z2s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z3s1;
        c6z3s1.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z3s1.setSmooth(true);
        sf::Sprite c6z3s1sprite(c6z3s1);
        c6z3s1sprite.setPosition(1020.0f, 610.0f);
        c6z3s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z3s2;
        c6z3s2.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z3s2.setSmooth(true);
        sf::Sprite c6z3s2sprite(c6z3s2);
        c6z3s2sprite.setPosition(1065.0f, 610.0f);
        c6z3s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z3s3;
        c6z3s3.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z3s3.setSmooth(true);
        sf::Sprite c6z3s3sprite(c6z3s3);
        c6z3s3sprite.setPosition(1110.0f, 610.0f);
        c6z3s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z3s4;
        c6z3s4.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z3s4.setSmooth(true);
        sf::Sprite c6z3s4sprite(c6z3s4);
        c6z3s4sprite.setPosition(1155.0f, 610.0f);
        c6z3s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z4s1;
        c6z4s1.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z4s1.setSmooth(true);
        sf::Sprite c6z4s1sprite(c6z4s1);
        c6z4s1sprite.setPosition(1020.0f, 655.0f);
        c6z4s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z4s2;
        c6z4s2.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z4s2.setSmooth(true);
        sf::Sprite c6z4s2sprite(c6z4s2);
        c6z4s2sprite.setPosition(1065.0f, 655.0f);
        c6z4s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z4s3;
        c6z4s3.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z4s3.setSmooth(true);
        sf::Sprite c6z4s3sprite(c6z4s3);
        c6z4s3sprite.setPosition(1110.0f, 655.0f);
        c6z4s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z4s4;
        c6z4s4.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z4s4.setSmooth(true);
        sf::Sprite c6z4s4sprite(c6z4s4);
        c6z4s4sprite.setPosition(1155.0f, 655.0f);
        c6z4s4sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z5s1;
        c6z5s1.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z5s1.setSmooth(true);
        sf::Sprite c6z5s1sprite(c6z5s1);
        c6z5s1sprite.setPosition(1020.0f, 700.0f);
        c6z5s1sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z5s2;
        c6z5s2.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z5s2.setSmooth(true);
        sf::Sprite c6z5s2sprite(c6z5s2);
        c6z5s2sprite.setPosition(1065.0f, 700.0f);
        c6z5s2sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z5s3;
        c6z5s3.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z5s3.setSmooth(true);
        sf::Sprite c6z5s3sprite(c6z5s3);
        c6z5s3sprite.setPosition(1110.0f, 700.0f);
        c6z5s3sprite.setScale(0.5f, 0.5f);

        sf::Texture c6z5s4;
        c6z5s4.loadFromFile("res/Bilder/würfel/dice-png-6.png");
        c6z5s4.setSmooth(true);
        sf::Sprite c6z5s4sprite(c6z5s4);
        c6z5s4sprite.setPosition(1155.0f, 700.0f);
        c6z5s4sprite.setScale(0.5f, 0.5f);

        //Algorithmus für kleine Würfel bei den Casinos
        //Casino 1
        //c1z1s1
        if (ic1ispieler1 != 0) {
            c1z1s1sprite.setColor(farben[0]);
            spiel.draw(c1z1s1sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z1s1sprite.setColor(farben[1]);
            spiel.draw(c1z1s1sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z1s1sprite.setColor(farben[2]);
            spiel.draw(c1z1s1sprite);
            ic1ispieler3--;   
        }
        else {
            //Mach nichts
        }
        
        //c1z1s2
        if (ic1ispieler1 != 0) {
            c1z1s2sprite.setColor(farben[0]);
            spiel.draw(c1z1s2sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z1s2sprite.setColor(farben[1]);
            spiel.draw(c1z1s2sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z1s2sprite.setColor(farben[2]);
            spiel.draw(c1z1s2sprite);
            ic1ispieler3--;
        }
        
        //c1z1s3
        if (ic1ispieler1 != 0) {
            c1z1s3sprite.setColor(farben[0]);
            spiel.draw(c1z1s3sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z1s3sprite.setColor(farben[1]);
            spiel.draw(c1z1s3sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z1s3sprite.setColor(farben[2]);
            spiel.draw(c1z1s3sprite);
            ic1ispieler3--;
        }

        //c1z1s4
        if (ic1ispieler1 != 0) {
            c1z1s4sprite.setColor(farben[0]);
            spiel.draw(c1z1s4sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z1s4sprite.setColor(farben[1]);
            spiel.draw(c1z1s4sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z1s4sprite.setColor(farben[2]);
            spiel.draw(c1z1s4sprite);
            ic1ispieler3--;
        }

        //c1z2s1
        if (ic1ispieler1 != 0) {
            c1z2s1sprite.setColor(farben[0]);
            spiel.draw(c1z2s1sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z2s1sprite.setColor(farben[1]);
            spiel.draw(c1z2s1sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z2s1sprite.setColor(farben[2]);
            spiel.draw(c1z2s1sprite);
            ic1ispieler3--;
        }

        //c1z2s2
        if (ic1ispieler1 != 0) {
            c1z2s2sprite.setColor(farben[0]);
            spiel.draw(c1z2s2sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z2s2sprite.setColor(farben[1]);
            spiel.draw(c1z2s2sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z2s2sprite.setColor(farben[2]);
            spiel.draw(c1z2s2sprite);
            ic1ispieler3--;
        }

        //c1z2s3
        if (ic1ispieler1 != 0) {
            c1z2s3sprite.setColor(farben[0]);
            spiel.draw(c1z2s3sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z2s3sprite.setColor(farben[1]);
            spiel.draw(c1z2s3sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z2s3sprite.setColor(farben[2]);
            spiel.draw(c1z2s3sprite);
            ic1ispieler3--;
        }

        //c1z2s4
        if (ic1ispieler1 != 0) {
            c1z2s4sprite.setColor(farben[0]);
            spiel.draw(c1z2s4sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z2s4sprite.setColor(farben[1]);
            spiel.draw(c1z2s4sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z2s4sprite.setColor(farben[2]);
            spiel.draw(c1z2s4sprite);
            ic1ispieler3--;
        }

        //c1z3s1
        if (ic1ispieler1 != 0) {
            c1z3s1sprite.setColor(farben[0]);
            spiel.draw(c1z3s1sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z3s1sprite.setColor(farben[1]);
            spiel.draw(c1z3s1sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z3s1sprite.setColor(farben[2]);
            spiel.draw(c1z3s1sprite);
            ic1ispieler3--;
        }

        //c1z3s2
        if (ic1ispieler1 != 0) {
            c1z3s2sprite.setColor(farben[0]);
            spiel.draw(c1z3s2sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z3s2sprite.setColor(farben[1]);
            spiel.draw(c1z3s2sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z3s2sprite.setColor(farben[2]);
            spiel.draw(c1z3s2sprite);
            ic1ispieler3--;
        }

        //c1z3s3
        if (ic1ispieler1 != 0) {
            c1z3s3sprite.setColor(farben[0]);
            spiel.draw(c1z3s3sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z3s3sprite.setColor(farben[1]);
            spiel.draw(c1z3s3sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z3s3sprite.setColor(farben[2]);
            spiel.draw(c1z3s3sprite);
            ic1ispieler3--;
        }

        //c1z3s4
        if (ic1ispieler1 != 0) {
            c1z3s4sprite.setColor(farben[0]);
            spiel.draw(c1z3s4sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z3s4sprite.setColor(farben[1]);
            spiel.draw(c1z3s4sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z3s4sprite.setColor(farben[2]);
            spiel.draw(c1z3s4sprite);
            ic1ispieler3--;
        }

        //c1z4s1
        if (ic1ispieler1 != 0) {
            c1z4s1sprite.setColor(farben[0]);
            spiel.draw(c1z4s1sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z4s1sprite.setColor(farben[1]);
            spiel.draw(c1z4s1sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z4s1sprite.setColor(farben[2]);
            spiel.draw(c1z4s1sprite);
            ic1ispieler3--;
        }

        //c1z4s2
        if (ic1ispieler1 != 0) {
            c1z4s2sprite.setColor(farben[0]);
            spiel.draw(c1z4s2sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z4s2sprite.setColor(farben[1]);
            spiel.draw(c1z4s2sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z4s2sprite.setColor(farben[2]);
            spiel.draw(c1z4s2sprite);
            ic1ispieler3--;
        }

        //c1z4s3
        if (ic1ispieler1 != 0) {
            c1z4s3sprite.setColor(farben[0]);
            spiel.draw(c1z4s3sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z4s3sprite.setColor(farben[1]);
            spiel.draw(c1z4s3sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z4s3sprite.setColor(farben[2]);
            spiel.draw(c1z4s3sprite);
            ic1ispieler3--;
        }

        //c1z4s4
        if (ic1ispieler1 != 0) {
            c1z4s4sprite.setColor(farben[0]);
            spiel.draw(c1z4s4sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z4s4sprite.setColor(farben[1]);
            spiel.draw(c1z4s4sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z4s4sprite.setColor(farben[2]);
            spiel.draw(c1z4s4sprite);
            ic1ispieler3--;
        }

        //c1z5s1
        if (ic1ispieler1 != 0) {
            c1z5s1sprite.setColor(farben[0]);
            spiel.draw(c1z5s1sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z5s1sprite.setColor(farben[1]);
            spiel.draw(c1z5s1sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z5s1sprite.setColor(farben[2]);
            spiel.draw(c1z5s1sprite);
            ic1ispieler3--;
        }

        //c1z5s2
        if (ic1ispieler1 != 0) {
            c1z5s2sprite.setColor(farben[0]);
            spiel.draw(c1z5s2sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z5s2sprite.setColor(farben[1]);
            spiel.draw(c1z5s2sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z5s2sprite.setColor(farben[2]);
            spiel.draw(c1z5s2sprite);
            ic1ispieler3--;
        }

        //c1z5s3
        if (ic1ispieler1 != 0) {
            c1z5s3sprite.setColor(farben[0]);
            spiel.draw(c1z5s3sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z5s3sprite.setColor(farben[1]);
            spiel.draw(c1z5s3sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z5s3sprite.setColor(farben[2]);
            spiel.draw(c1z5s3sprite);
            ic1ispieler3--;
        }

        //c1z5s4
        if (ic1ispieler1 != 0) {
            c1z5s4sprite.setColor(farben[0]);
            spiel.draw(c1z5s4sprite);
            ic1ispieler1--;
        }
        else if (ic1ispieler2 != 0) {
            c1z5s4sprite.setColor(farben[1]);
            spiel.draw(c1z5s4sprite);
            ic1ispieler2--;
        }
        else if (ic1ispieler3 != 0) {
            c1z5s4sprite.setColor(farben[2]);
            spiel.draw(c1z5s4sprite);
            ic1ispieler3--;
        }

        //Casino 2
        //c2z1s1
        if (ic2ispieler1 != 0) {
            c2z1s1sprite.setColor(farben[0]);
            spiel.draw(c2z1s1sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z1s1sprite.setColor(farben[1]);
            spiel.draw(c2z1s1sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z1s1sprite.setColor(farben[2]);
            spiel.draw(c2z1s1sprite);
            ic2ispieler3--;
        }

        //c2z1s2
        if (ic2ispieler1 != 0) {
            c2z1s2sprite.setColor(farben[0]);
            spiel.draw(c2z1s2sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z1s2sprite.setColor(farben[1]);
            spiel.draw(c2z1s2sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z1s2sprite.setColor(farben[2]);
            spiel.draw(c2z1s2sprite);
            ic2ispieler3--;
        }

        //c2z1s3
        if (ic2ispieler1 != 0) {
            c2z1s3sprite.setColor(farben[0]);
            spiel.draw(c2z1s3sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z1s3sprite.setColor(farben[1]);
            spiel.draw(c2z1s3sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z1s3sprite.setColor(farben[2]);
            spiel.draw(c2z1s3sprite);
            ic2ispieler3--;
        }

        //c2z1s4
        if (ic2ispieler1 != 0) {
            c2z1s4sprite.setColor(farben[0]);
            spiel.draw(c2z1s4sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z1s4sprite.setColor(farben[1]);
            spiel.draw(c2z1s4sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z1s4sprite.setColor(farben[2]);
            spiel.draw(c2z1s4sprite);
            ic2ispieler3--;
        }

        //c2z2s1
        if (ic2ispieler1 != 0) {
            c2z2s1sprite.setColor(farben[0]);
            spiel.draw(c2z2s1sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z2s1sprite.setColor(farben[1]);
            spiel.draw(c2z2s1sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z2s1sprite.setColor(farben[2]);
            spiel.draw(c2z2s1sprite);
            ic2ispieler3--;
        }

        //c2z2s2
        if (ic2ispieler1 != 0) {
            c2z2s2sprite.setColor(farben[0]);
            spiel.draw(c2z2s2sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z2s2sprite.setColor(farben[1]);
            spiel.draw(c2z2s2sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z2s2sprite.setColor(farben[2]);
            spiel.draw(c2z2s2sprite);
            ic2ispieler3--;
        }

        //c2z2s3
        if (ic2ispieler1 != 0) {
            c2z2s3sprite.setColor(farben[0]);
            spiel.draw(c2z2s3sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z2s3sprite.setColor(farben[1]);
            spiel.draw(c2z2s3sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z2s3sprite.setColor(farben[2]);
            spiel.draw(c2z2s3sprite);
            ic2ispieler3--;
        }

        //c2z2s4
        if (ic2ispieler1 != 0) {
            c2z2s4sprite.setColor(farben[0]);
            spiel.draw(c2z2s4sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z2s4sprite.setColor(farben[1]);
            spiel.draw(c2z2s4sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z2s4sprite.setColor(farben[2]);
            spiel.draw(c2z2s4sprite);
            ic2ispieler3--;
        }

        //c2z3s1
        if (ic2ispieler1 != 0) {
            c2z3s1sprite.setColor(farben[0]);
            spiel.draw(c2z3s1sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z3s1sprite.setColor(farben[1]);
            spiel.draw(c2z3s1sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z3s1sprite.setColor(farben[2]);
            spiel.draw(c2z3s1sprite);
            ic2ispieler3--;
        }

        //c2z3s2
        if (ic2ispieler1 != 0) {
            c2z3s2sprite.setColor(farben[0]);
            spiel.draw(c2z3s2sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z3s2sprite.setColor(farben[1]);
            spiel.draw(c2z3s2sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z3s2sprite.setColor(farben[2]);
            spiel.draw(c2z3s2sprite);
            ic2ispieler3--;
        }

        //c2z3s3
        if (ic2ispieler1 != 0) {
            c2z3s3sprite.setColor(farben[0]);
            spiel.draw(c2z3s3sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z3s3sprite.setColor(farben[1]);
            spiel.draw(c2z3s3sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z3s3sprite.setColor(farben[2]);
            spiel.draw(c2z3s3sprite);
            ic2ispieler3--;
        }

        //c2z3s4
        if (ic2ispieler1 != 0) {
            c2z3s4sprite.setColor(farben[0]);
            spiel.draw(c2z3s4sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z3s4sprite.setColor(farben[1]);
            spiel.draw(c2z3s4sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z3s4sprite.setColor(farben[2]);
            spiel.draw(c2z3s4sprite);
            ic2ispieler3--;
        }

        //c2z4s1
        if (ic2ispieler1 != 0) {
            c2z4s1sprite.setColor(farben[0]);
            spiel.draw(c2z4s1sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z4s1sprite.setColor(farben[1]);
            spiel.draw(c2z4s1sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z4s1sprite.setColor(farben[2]);
            spiel.draw(c2z4s1sprite);
            ic2ispieler3--;
        }

        //c2z4s2
        if (ic2ispieler1 != 0) {
            c2z4s2sprite.setColor(farben[0]);
            spiel.draw(c2z4s2sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z4s2sprite.setColor(farben[1]);
            spiel.draw(c2z4s2sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z4s2sprite.setColor(farben[2]);
            spiel.draw(c2z4s2sprite);
            ic2ispieler3--;
        }

        //c2z4s3
        if (ic2ispieler1 != 0) {
            c2z4s3sprite.setColor(farben[0]);
            spiel.draw(c2z4s3sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z4s3sprite.setColor(farben[1]);
            spiel.draw(c2z4s3sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z4s3sprite.setColor(farben[2]);
            spiel.draw(c2z4s3sprite);
            ic2ispieler3--;
        }

        //c2z4s4
        if (ic2ispieler1 != 0) {
            c2z4s4sprite.setColor(farben[0]);
            spiel.draw(c2z4s4sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z4s4sprite.setColor(farben[1]);
            spiel.draw(c2z4s4sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z4s4sprite.setColor(farben[2]);
            spiel.draw(c2z4s4sprite);
            ic2ispieler3--;
        }

        //c2z5s1
        if (ic2ispieler1 != 0) {
            c2z5s1sprite.setColor(farben[0]);
            spiel.draw(c2z5s1sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z5s1sprite.setColor(farben[1]);
            spiel.draw(c2z5s1sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z5s1sprite.setColor(farben[2]);
            spiel.draw(c2z5s1sprite);
            ic2ispieler3--;
        }

        //c2z5s2
        if (ic2ispieler1 != 0) {
            c2z5s2sprite.setColor(farben[0]);
            spiel.draw(c2z5s2sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z5s2sprite.setColor(farben[1]);
            spiel.draw(c2z5s2sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z5s2sprite.setColor(farben[2]);
            spiel.draw(c2z5s2sprite);
            ic2ispieler3--;
        }

        //c2z5s3
        if (ic2ispieler1 != 0) {
            c2z5s3sprite.setColor(farben[0]);
            spiel.draw(c2z5s3sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z5s3sprite.setColor(farben[1]);
            spiel.draw(c2z5s3sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z5s3sprite.setColor(farben[2]);
            spiel.draw(c2z5s3sprite);
            ic2ispieler3--;
        }

        //c2z5s4
        if (ic2ispieler1 != 0) {
            c2z5s4sprite.setColor(farben[0]);
            spiel.draw(c2z5s4sprite);
            ic2ispieler1--;
        }
        else if (ic2ispieler2 != 0) {
            c2z5s4sprite.setColor(farben[1]);
            spiel.draw(c2z5s4sprite);
            ic2ispieler2--;
        }
        else if (ic2ispieler3 != 0) {
            c2z5s4sprite.setColor(farben[2]);
            spiel.draw(c2z5s4sprite);
            ic2ispieler3--;
        }

        //Casino 3
        //c3z1s1
        if (ic3ispieler1 != 0) {
            c3z1s1sprite.setColor(farben[0]);
            spiel.draw(c3z1s1sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z1s1sprite.setColor(farben[1]);
            spiel.draw(c3z1s1sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z1s1sprite.setColor(farben[2]);
            spiel.draw(c3z1s1sprite);
            ic3ispieler3--;
        }

        //c3z1s2
        if (ic3ispieler1 != 0) {
            c3z1s2sprite.setColor(farben[0]);
            spiel.draw(c3z1s2sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z1s2sprite.setColor(farben[1]);
            spiel.draw(c3z1s2sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z1s2sprite.setColor(farben[2]);
            spiel.draw(c3z1s2sprite);
            ic3ispieler3--;
        }

        //c3z1s3
        if (ic3ispieler1 != 0) {
            c3z1s3sprite.setColor(farben[0]);
            spiel.draw(c3z1s3sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z1s3sprite.setColor(farben[1]);
            spiel.draw(c3z1s3sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z1s3sprite.setColor(farben[2]);
            spiel.draw(c3z1s3sprite);
            ic3ispieler3--;
        }

        //c3z1s4
        if (ic3ispieler1 != 0) {
            c3z1s4sprite.setColor(farben[0]);
            spiel.draw(c3z1s4sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z1s4sprite.setColor(farben[1]);
            spiel.draw(c3z1s4sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z1s4sprite.setColor(farben[2]);
            spiel.draw(c3z1s4sprite);
            ic3ispieler3--;
        }

        //c3z2s1
        if (ic3ispieler1 != 0) {
            c3z2s1sprite.setColor(farben[0]);
            spiel.draw(c3z2s1sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z2s1sprite.setColor(farben[1]);
            spiel.draw(c3z2s1sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z2s1sprite.setColor(farben[2]);
            spiel.draw(c3z2s1sprite);
            ic3ispieler3--;
        }

        //c3z2s2
        if (ic3ispieler1 != 0) {
            c3z2s2sprite.setColor(farben[0]);
            spiel.draw(c3z2s2sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z2s2sprite.setColor(farben[1]);
            spiel.draw(c3z2s2sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z2s2sprite.setColor(farben[2]);
            spiel.draw(c3z2s2sprite);
            ic3ispieler3--;
        }

        //c3z2s3
        if (ic3ispieler1 != 0) {
            c3z2s3sprite.setColor(farben[0]);
            spiel.draw(c3z2s3sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z2s3sprite.setColor(farben[1]);
            spiel.draw(c3z2s3sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z2s3sprite.setColor(farben[2]);
            spiel.draw(c3z2s3sprite);
            ic3ispieler3--;
        }

        //c3z2s4
        if (ic3ispieler1 != 0) {
            c3z2s4sprite.setColor(farben[0]);
            spiel.draw(c3z2s4sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z2s4sprite.setColor(farben[1]);
            spiel.draw(c3z2s4sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z2s4sprite.setColor(farben[2]);
            spiel.draw(c3z2s4sprite);
            ic3ispieler3--;
        }

        //c3z3s1
        if (ic3ispieler1 != 0) {
            c3z3s1sprite.setColor(farben[0]);
            spiel.draw(c3z3s1sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z3s1sprite.setColor(farben[1]);
            spiel.draw(c3z3s1sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z3s1sprite.setColor(farben[2]);
            spiel.draw(c3z3s1sprite);
            ic3ispieler3--;
        }

        //c3z3s2
        if (ic3ispieler1 != 0) {
            c3z3s2sprite.setColor(farben[0]);
            spiel.draw(c3z3s2sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z3s2sprite.setColor(farben[1]);
            spiel.draw(c3z3s2sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z3s2sprite.setColor(farben[2]);
            spiel.draw(c3z3s2sprite);
            ic3ispieler3--;
        }

        //c3z3s3
        if (ic3ispieler1 != 0) {
            c3z3s3sprite.setColor(farben[0]);
            spiel.draw(c3z3s3sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z3s3sprite.setColor(farben[1]);
            spiel.draw(c3z3s3sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z3s3sprite.setColor(farben[2]);
            spiel.draw(c3z3s3sprite);
            ic3ispieler3--;
        }

        //c3z3s4
        if (ic3ispieler1 != 0) {
            c3z3s4sprite.setColor(farben[0]);
            spiel.draw(c3z3s4sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z3s4sprite.setColor(farben[1]);
            spiel.draw(c3z3s4sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z3s4sprite.setColor(farben[2]);
            spiel.draw(c3z3s4sprite);
            ic3ispieler3--;
        }

        //c3z4s1
        if (ic3ispieler1 != 0) {
            c3z4s1sprite.setColor(farben[0]);
            spiel.draw(c3z4s1sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z4s1sprite.setColor(farben[1]);
            spiel.draw(c3z4s1sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z4s1sprite.setColor(farben[2]);
            spiel.draw(c3z4s1sprite);
            ic3ispieler3--;
        }

        //c3z4s2
        if (ic3ispieler1 != 0) {
            c3z4s2sprite.setColor(farben[0]);
            spiel.draw(c3z4s2sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z4s2sprite.setColor(farben[1]);
            spiel.draw(c3z4s2sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z4s2sprite.setColor(farben[2]);
            spiel.draw(c3z4s2sprite);
            ic3ispieler3--;
        }

        //c3z4s3
        if (ic3ispieler1 != 0) {
            c3z4s3sprite.setColor(farben[0]);
            spiel.draw(c3z4s3sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z4s3sprite.setColor(farben[1]);
            spiel.draw(c3z4s3sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z4s3sprite.setColor(farben[2]);
            spiel.draw(c3z4s3sprite);
            ic3ispieler3--;
        }

        //c3z4s4
        if (ic3ispieler1 != 0) {
            c3z4s4sprite.setColor(farben[0]);
            spiel.draw(c3z4s4sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z4s4sprite.setColor(farben[1]);
            spiel.draw(c3z4s4sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z4s4sprite.setColor(farben[2]);
            spiel.draw(c3z4s4sprite);
            ic3ispieler3--;
        }

        //c3z5s1
        if (ic3ispieler1 != 0) {
            c3z5s1sprite.setColor(farben[0]);
            spiel.draw(c3z5s1sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z5s1sprite.setColor(farben[1]);
            spiel.draw(c3z5s1sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z5s1sprite.setColor(farben[2]);
            spiel.draw(c3z5s1sprite);
            ic3ispieler3--;
        }

        //c3z5s2
        if (ic3ispieler1 != 0) {
            c3z5s2sprite.setColor(farben[0]);
            spiel.draw(c3z5s2sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z5s2sprite.setColor(farben[1]);
            spiel.draw(c3z5s2sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z5s2sprite.setColor(farben[2]);
            spiel.draw(c3z5s2sprite);
            ic3ispieler3--;
        }

        //c3z5s3
        if (ic3ispieler1 != 0) {
            c3z5s3sprite.setColor(farben[0]);
            spiel.draw(c3z5s3sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z5s3sprite.setColor(farben[1]);
            spiel.draw(c3z5s3sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z5s3sprite.setColor(farben[2]);
            spiel.draw(c3z5s3sprite);
            ic3ispieler3--;
        }

        //c3z5s4
        if (ic3ispieler1 != 0) {
            c3z5s4sprite.setColor(farben[0]);
            spiel.draw(c3z5s4sprite);
            ic3ispieler1--;
        }
        else if (ic3ispieler2 != 0) {
            c3z5s4sprite.setColor(farben[1]);
            spiel.draw(c3z5s4sprite);
            ic3ispieler2--;
        }
        else if (ic3ispieler3 != 0) {
            c3z5s4sprite.setColor(farben[2]);
            spiel.draw(c3z5s4sprite);
            ic3ispieler3--;
        }

        //Casino 4
        //c4z1s1
        if (ic4ispieler1 != 0) {
            c4z1s1sprite.setColor(farben[0]);
            spiel.draw(c4z1s1sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z1s1sprite.setColor(farben[1]);
            spiel.draw(c4z1s1sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z1s1sprite.setColor(farben[2]);
            spiel.draw(c4z1s1sprite);
            ic4ispieler3--;
        }

        //c4z1s2
        if (ic4ispieler1 != 0) {
            c4z1s2sprite.setColor(farben[0]);
            spiel.draw(c4z1s2sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z1s2sprite.setColor(farben[1]);
            spiel.draw(c4z1s2sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z1s2sprite.setColor(farben[2]);
            spiel.draw(c4z1s2sprite);
            ic4ispieler3--;
        }

        //c4z1s3
        if (ic4ispieler1 != 0) {
            c4z1s3sprite.setColor(farben[0]);
            spiel.draw(c4z1s3sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z1s3sprite.setColor(farben[1]);
            spiel.draw(c4z1s3sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z1s3sprite.setColor(farben[2]);
            spiel.draw(c4z1s3sprite);
            ic4ispieler3--;
        }

        //c4z1s4
        if (ic4ispieler1 != 0) {
            c4z1s4sprite.setColor(farben[0]);
            spiel.draw(c4z1s4sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z1s4sprite.setColor(farben[1]);
            spiel.draw(c4z1s4sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z1s4sprite.setColor(farben[2]);
            spiel.draw(c4z1s4sprite);
            ic4ispieler3--;
        }

        //c4z2s1
        if (ic4ispieler1 != 0) {
            c4z2s1sprite.setColor(farben[0]);
            spiel.draw(c4z2s1sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z2s1sprite.setColor(farben[1]);
            spiel.draw(c4z2s1sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z2s1sprite.setColor(farben[2]);
            spiel.draw(c4z2s1sprite);
            ic4ispieler3--;
        }

        //c4z2s2
        if (ic4ispieler1 != 0) {
            c4z2s2sprite.setColor(farben[0]);
            spiel.draw(c4z2s2sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z2s2sprite.setColor(farben[1]);
            spiel.draw(c4z2s2sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z2s2sprite.setColor(farben[2]);
            spiel.draw(c4z2s2sprite);
            ic4ispieler3--;
        }

        //c4z2s3
        if (ic4ispieler1 != 0) {
            c4z2s3sprite.setColor(farben[0]);
            spiel.draw(c4z2s3sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z2s3sprite.setColor(farben[1]);
            spiel.draw(c4z2s3sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z2s3sprite.setColor(farben[2]);
            spiel.draw(c4z2s3sprite);
            ic4ispieler3--;
        }

        //c4z2s4
        if (ic4ispieler1 != 0) {
            c4z2s4sprite.setColor(farben[0]);
            spiel.draw(c4z2s4sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z2s4sprite.setColor(farben[1]);
            spiel.draw(c4z2s4sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z2s4sprite.setColor(farben[2]);
            spiel.draw(c4z2s4sprite);
            ic4ispieler3--;
        }

        //c4z3s1
        if (ic4ispieler1 != 0) {
            c4z3s1sprite.setColor(farben[0]);
            spiel.draw(c4z3s1sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z3s1sprite.setColor(farben[1]);
            spiel.draw(c4z3s1sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z3s1sprite.setColor(farben[2]);
            spiel.draw(c4z3s1sprite);
            ic4ispieler3--;
        }

        //c4z3s2
        if (ic4ispieler1 != 0) {
            c4z3s2sprite.setColor(farben[0]);
            spiel.draw(c4z3s2sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z3s2sprite.setColor(farben[1]);
            spiel.draw(c4z3s2sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z3s2sprite.setColor(farben[2]);
            spiel.draw(c4z3s2sprite);
            ic4ispieler3--;
        }

        //c4z3s3
        if (ic4ispieler1 != 0) {
            c4z3s3sprite.setColor(farben[0]);
            spiel.draw(c4z3s3sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z3s3sprite.setColor(farben[1]);
            spiel.draw(c4z3s3sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z3s3sprite.setColor(farben[2]);
            spiel.draw(c4z3s3sprite);
            ic4ispieler3--;
        }

        //c4z3s4
        if (ic4ispieler1 != 0) {
            c4z3s4sprite.setColor(farben[0]);
            spiel.draw(c4z3s4sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z3s4sprite.setColor(farben[1]);
            spiel.draw(c4z3s4sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z3s4sprite.setColor(farben[2]);
            spiel.draw(c4z3s4sprite);
            ic4ispieler3--;
        }

        //c4z4s1
        if (ic4ispieler1 != 0) {
            c4z4s1sprite.setColor(farben[0]);
            spiel.draw(c4z4s1sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z4s1sprite.setColor(farben[1]);
            spiel.draw(c4z4s1sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z4s1sprite.setColor(farben[2]);
            spiel.draw(c4z4s1sprite);
            ic4ispieler3--;
        }

        //c4z4s2
        if (ic4ispieler1 != 0) {
            c4z4s2sprite.setColor(farben[0]);
            spiel.draw(c4z4s2sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z4s2sprite.setColor(farben[1]);
            spiel.draw(c4z4s2sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z4s2sprite.setColor(farben[2]);
            spiel.draw(c4z4s2sprite);
            ic4ispieler3--;
        }

        //c4z4s3
        if (ic4ispieler1 != 0) {
            c4z4s3sprite.setColor(farben[0]);
            spiel.draw(c4z4s3sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z4s3sprite.setColor(farben[1]);
            spiel.draw(c4z4s3sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z4s3sprite.setColor(farben[2]);
            spiel.draw(c4z4s3sprite);
            ic4ispieler3--;
        }

        //c4z4s4
        if (ic4ispieler1 != 0) {
            c4z4s4sprite.setColor(farben[0]);
            spiel.draw(c4z4s4sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z4s4sprite.setColor(farben[1]);
            spiel.draw(c4z4s4sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z4s4sprite.setColor(farben[2]);
            spiel.draw(c4z4s4sprite);
            ic4ispieler3--;
        }

        //c4z5s1
        if (ic4ispieler1 != 0) {
            c4z5s1sprite.setColor(farben[0]);
            spiel.draw(c4z5s1sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z5s1sprite.setColor(farben[1]);
            spiel.draw(c4z5s1sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z5s1sprite.setColor(farben[2]);
            spiel.draw(c4z5s1sprite);
            ic4ispieler3--;
        }

        //c4z5s2
        if (ic4ispieler1 != 0) {
            c4z5s2sprite.setColor(farben[0]);
            spiel.draw(c4z5s2sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z5s2sprite.setColor(farben[1]);
            spiel.draw(c4z5s2sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z5s2sprite.setColor(farben[2]);
            spiel.draw(c4z5s2sprite);
            ic4ispieler3--;
        }

        //c4z5s3
        if (ic4ispieler1 != 0) {
            c4z5s3sprite.setColor(farben[0]);
            spiel.draw(c4z5s3sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z5s3sprite.setColor(farben[1]);
            spiel.draw(c4z5s3sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z5s3sprite.setColor(farben[2]);
            spiel.draw(c4z5s3sprite);
            ic4ispieler3--;
        }

        //c4z5s4
        if (ic4ispieler1 != 0) {
            c4z5s4sprite.setColor(farben[0]);
            spiel.draw(c4z5s4sprite);
            ic4ispieler1--;
        }
        else if (ic4ispieler2 != 0) {
            c4z5s4sprite.setColor(farben[1]);
            spiel.draw(c4z5s4sprite);
            ic4ispieler2--;
        }
        else if (ic4ispieler3 != 0) {
            c4z5s4sprite.setColor(farben[2]);
            spiel.draw(c4z5s4sprite);
            ic4ispieler3--;
        }

        //Casino 5
        //c5z1s1
        if (ic5ispieler1 != 0) {
            c5z1s1sprite.setColor(farben[0]);
            spiel.draw(c5z1s1sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z1s1sprite.setColor(farben[1]);
            spiel.draw(c5z1s1sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z1s1sprite.setColor(farben[2]);
            spiel.draw(c5z1s1sprite);
            ic5ispieler3--;
        }

        //c5z1s2
        if (ic5ispieler1 != 0) {
            c5z1s2sprite.setColor(farben[0]);
            spiel.draw(c5z1s2sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z1s2sprite.setColor(farben[1]);
            spiel.draw(c5z1s2sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z1s2sprite.setColor(farben[2]);
            spiel.draw(c5z1s2sprite);
            ic5ispieler3--;
        }

        //c5z1s3
        if (ic5ispieler1 != 0) {
            c5z1s3sprite.setColor(farben[0]);
            spiel.draw(c5z1s3sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z1s3sprite.setColor(farben[1]);
            spiel.draw(c5z1s3sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z1s3sprite.setColor(farben[2]);
            spiel.draw(c5z1s3sprite);
            ic5ispieler3--;
        }

        //c5z1s4
        if (ic5ispieler1 != 0) {
            c5z1s4sprite.setColor(farben[0]);
            spiel.draw(c5z1s4sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z1s4sprite.setColor(farben[1]);
            spiel.draw(c5z1s4sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z1s4sprite.setColor(farben[2]);
            spiel.draw(c5z1s4sprite);
            ic5ispieler3--;
        }

        //c5z2s1
        if (ic5ispieler1 != 0) {
            c5z2s1sprite.setColor(farben[0]);
            spiel.draw(c5z2s1sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z2s1sprite.setColor(farben[1]);
            spiel.draw(c5z2s1sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z2s1sprite.setColor(farben[2]);
            spiel.draw(c5z2s1sprite);
            ic5ispieler3--;
        }

        //c5z2s2
        if (ic5ispieler1 != 0) {
            c5z2s2sprite.setColor(farben[0]);
            spiel.draw(c5z2s2sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z2s2sprite.setColor(farben[1]);
            spiel.draw(c5z2s2sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z2s2sprite.setColor(farben[2]);
            spiel.draw(c5z2s2sprite);
            ic5ispieler3--;
        }

        //c5z2s3
        if (ic5ispieler1 != 0) {
            c5z2s3sprite.setColor(farben[0]);
            spiel.draw(c5z2s3sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z2s3sprite.setColor(farben[1]);
            spiel.draw(c5z2s3sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z2s3sprite.setColor(farben[2]);
            spiel.draw(c5z2s3sprite);
            ic5ispieler3--;
        }

        //c5z2s4
        if (ic5ispieler1 != 0) {
            c5z2s4sprite.setColor(farben[0]);
            spiel.draw(c5z2s4sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z2s4sprite.setColor(farben[1]);
            spiel.draw(c5z2s4sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z2s4sprite.setColor(farben[2]);
            spiel.draw(c5z2s4sprite);
            ic5ispieler3--;
        }

        //c5z3s1
        if (ic5ispieler1 != 0) {
            c5z3s1sprite.setColor(farben[0]);
            spiel.draw(c5z3s1sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z3s1sprite.setColor(farben[1]);
            spiel.draw(c5z3s1sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z3s1sprite.setColor(farben[2]);
            spiel.draw(c5z3s1sprite);
            ic5ispieler3--;
        }

        //c5z3s2
        if (ic5ispieler1 != 0) {
            c5z3s2sprite.setColor(farben[0]);
            spiel.draw(c5z3s2sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z3s2sprite.setColor(farben[1]);
            spiel.draw(c5z3s2sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z3s2sprite.setColor(farben[2]);
            spiel.draw(c5z3s2sprite);
            ic5ispieler3--;
        }

        //c5z3s3
        if (ic5ispieler1 != 0) {
            c5z3s3sprite.setColor(farben[0]);
            spiel.draw(c5z3s3sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z3s3sprite.setColor(farben[1]);
            spiel.draw(c5z3s3sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z3s3sprite.setColor(farben[2]);
            spiel.draw(c5z3s3sprite);
            ic5ispieler3--;
        }

        //c5z3s4
        if (ic5ispieler1 != 0) {
            c5z3s4sprite.setColor(farben[0]);
            spiel.draw(c5z3s4sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z3s4sprite.setColor(farben[1]);
            spiel.draw(c5z3s4sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z3s4sprite.setColor(farben[2]);
            spiel.draw(c5z3s4sprite);
            ic5ispieler3--;
        }

        //c5z4s1
        if (ic5ispieler1 != 0) {
            c5z4s1sprite.setColor(farben[0]);
            spiel.draw(c5z4s1sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z4s1sprite.setColor(farben[1]);
            spiel.draw(c5z4s1sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z4s1sprite.setColor(farben[2]);
            spiel.draw(c5z4s1sprite);
            ic5ispieler3--;
        }

        //c5z4s2
        if (ic5ispieler1 != 0) {
            c5z4s2sprite.setColor(farben[0]);
            spiel.draw(c5z4s2sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z4s2sprite.setColor(farben[1]);
            spiel.draw(c5z4s2sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z4s2sprite.setColor(farben[2]);
            spiel.draw(c5z4s2sprite);
            ic5ispieler3--;
        }

        //c5z4s3
        if (ic5ispieler1 != 0) {
            c5z4s3sprite.setColor(farben[0]);
            spiel.draw(c5z4s3sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z4s3sprite.setColor(farben[1]);
            spiel.draw(c5z4s3sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z4s3sprite.setColor(farben[2]);
            spiel.draw(c5z4s3sprite);
            ic5ispieler3--;
        }

        //c5z4s4
        if (ic5ispieler1 != 0) {
            c5z4s4sprite.setColor(farben[0]);
            spiel.draw(c5z4s4sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z4s4sprite.setColor(farben[1]);
            spiel.draw(c5z4s4sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z4s4sprite.setColor(farben[2]);
            spiel.draw(c5z4s4sprite);
            ic5ispieler3--;
        }

        //c5z5s1
        if (ic5ispieler1 != 0) {
            c5z5s1sprite.setColor(farben[0]);
            spiel.draw(c5z5s1sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z5s1sprite.setColor(farben[1]);
            spiel.draw(c5z5s1sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z5s1sprite.setColor(farben[2]);
            spiel.draw(c5z5s1sprite);
            ic5ispieler3--;
        }

        //c5z5s2
        if (ic5ispieler1 != 0) {
            c5z5s2sprite.setColor(farben[0]);
            spiel.draw(c5z5s2sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z5s2sprite.setColor(farben[1]);
            spiel.draw(c5z5s2sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z5s2sprite.setColor(farben[2]);
            spiel.draw(c5z5s2sprite);
            ic5ispieler3--;
        }

        //c5z5s3
        if (ic5ispieler1 != 0) {
            c5z5s3sprite.setColor(farben[0]);
            spiel.draw(c5z5s3sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z5s3sprite.setColor(farben[1]);
            spiel.draw(c5z5s3sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z5s3sprite.setColor(farben[2]);
            spiel.draw(c5z5s3sprite);
            ic5ispieler3--;
        }

        //c5z5s4
        if (ic5ispieler1 != 0) {
            c5z5s4sprite.setColor(farben[0]);
            spiel.draw(c5z5s4sprite);
            ic5ispieler1--;
        }
        else if (ic5ispieler2 != 0) {
            c5z5s4sprite.setColor(farben[1]);
            spiel.draw(c5z5s4sprite);
            ic5ispieler2--;
        }
        else if (ic5ispieler3 != 0) {
            c5z5s4sprite.setColor(farben[2]);
            spiel.draw(c5z5s4sprite);
            ic5ispieler3--;
        }

        //Casino 6
        //c6z1s1
        if (ic6ispieler1 != 0) {
            c6z1s1sprite.setColor(farben[0]);
            spiel.draw(c6z1s1sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z1s1sprite.setColor(farben[1]);
            spiel.draw(c6z1s1sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z1s1sprite.setColor(farben[2]);
            spiel.draw(c6z1s1sprite);
            ic6ispieler3--;
        }

        //c6z1s2
        if (ic6ispieler1 != 0) {
            c6z1s2sprite.setColor(farben[0]);
            spiel.draw(c6z1s2sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z1s2sprite.setColor(farben[1]);
            spiel.draw(c6z1s2sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z1s2sprite.setColor(farben[2]);
            spiel.draw(c6z1s2sprite);
            ic6ispieler3--;
        }

        //c6z1s3
        if (ic6ispieler1 != 0) {
            c6z1s3sprite.setColor(farben[0]);
            spiel.draw(c6z1s3sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z1s3sprite.setColor(farben[1]);
            spiel.draw(c6z1s3sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z1s3sprite.setColor(farben[2]);
            spiel.draw(c6z1s3sprite);
            ic6ispieler3--;
        }

        //c6z1s4
        if (ic6ispieler1 != 0) {
            c6z1s4sprite.setColor(farben[0]);
            spiel.draw(c6z1s4sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z1s4sprite.setColor(farben[1]);
            spiel.draw(c6z1s4sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z1s4sprite.setColor(farben[2]);
            spiel.draw(c6z1s4sprite);
            ic6ispieler3--;
        }

        //c6z2s1
        if (ic6ispieler1 != 0) {
            c6z2s1sprite.setColor(farben[0]);
            spiel.draw(c6z2s1sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z2s1sprite.setColor(farben[1]);
            spiel.draw(c6z2s1sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z2s1sprite.setColor(farben[2]);
            spiel.draw(c6z2s1sprite);
            ic6ispieler3--;
        }

        //c6z2s2
        if (ic6ispieler1 != 0) {
            c6z2s2sprite.setColor(farben[0]);
            spiel.draw(c6z2s2sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z2s2sprite.setColor(farben[1]);
            spiel.draw(c6z2s2sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z2s2sprite.setColor(farben[2]);
            spiel.draw(c6z2s2sprite);
            ic6ispieler3--;
        }

        //c6z2s3
        if (ic6ispieler1 != 0) {
            c6z2s3sprite.setColor(farben[0]);
            spiel.draw(c6z2s3sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z2s3sprite.setColor(farben[1]);
            spiel.draw(c6z2s3sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z2s3sprite.setColor(farben[2]);
            spiel.draw(c6z2s3sprite);
            ic6ispieler3--;
        }

        //c6z2s4
        if (ic6ispieler1 != 0) {
            c6z2s4sprite.setColor(farben[0]);
            spiel.draw(c6z2s4sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z2s4sprite.setColor(farben[1]);
            spiel.draw(c6z2s4sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z2s4sprite.setColor(farben[2]);
            spiel.draw(c6z2s4sprite);
            ic6ispieler3--;
        }

        //c6z3s1
        if (ic6ispieler1 != 0) {
            c6z3s1sprite.setColor(farben[0]);
            spiel.draw(c6z3s1sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z3s1sprite.setColor(farben[1]);
            spiel.draw(c6z3s1sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z3s1sprite.setColor(farben[2]);
            spiel.draw(c6z3s1sprite);
            ic6ispieler3--;
        }

        //c6z3s2
        if (ic6ispieler1 != 0) {
            c6z3s2sprite.setColor(farben[0]);
            spiel.draw(c6z3s2sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z3s2sprite.setColor(farben[1]);
            spiel.draw(c6z3s2sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z3s2sprite.setColor(farben[2]);
            spiel.draw(c6z3s2sprite);
            ic6ispieler3--;
        }

        //c6z3s3
        if (ic6ispieler1 != 0) {
            c6z3s3sprite.setColor(farben[0]);
            spiel.draw(c6z3s3sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z3s3sprite.setColor(farben[1]);
            spiel.draw(c6z3s3sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z3s3sprite.setColor(farben[2]);
            spiel.draw(c6z3s3sprite);
            ic6ispieler3--;
        }

        //c6z3s4
        if (ic6ispieler1 != 0) {
            c6z3s4sprite.setColor(farben[0]);
            spiel.draw(c6z3s4sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z3s4sprite.setColor(farben[1]);
            spiel.draw(c6z3s4sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z3s4sprite.setColor(farben[2]);
            spiel.draw(c6z3s4sprite);
            ic6ispieler3--;
        }

        //c6z4s1
        if (ic6ispieler1 != 0) {
            c6z4s1sprite.setColor(farben[0]);
            spiel.draw(c6z4s1sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z4s1sprite.setColor(farben[1]);
            spiel.draw(c6z4s1sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z4s1sprite.setColor(farben[2]);
            spiel.draw(c6z4s1sprite);
            ic6ispieler3--;
        }

        //c6z4s2
        if (ic6ispieler1 != 0) {
            c6z4s2sprite.setColor(farben[0]);
            spiel.draw(c6z4s2sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z4s2sprite.setColor(farben[1]);
            spiel.draw(c6z4s2sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z4s2sprite.setColor(farben[2]);
            spiel.draw(c6z4s2sprite);
            ic6ispieler3--;
        }

        //c6z4s3
        if (ic6ispieler1 != 0) {
            c6z4s3sprite.setColor(farben[0]);
            spiel.draw(c6z4s3sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z4s3sprite.setColor(farben[1]);
            spiel.draw(c6z4s3sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z4s3sprite.setColor(farben[2]);
            spiel.draw(c6z4s3sprite);
            ic6ispieler3--;
        }

        //c6z4s4
        if (ic6ispieler1 != 0) {
            c6z4s4sprite.setColor(farben[0]);
            spiel.draw(c6z4s4sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z4s4sprite.setColor(farben[1]);
            spiel.draw(c6z4s4sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z4s4sprite.setColor(farben[2]);
            spiel.draw(c6z4s4sprite);
            ic6ispieler3--;
        }

        //c6z5s1
        if (ic6ispieler1 != 0) {
            c6z5s1sprite.setColor(farben[0]);
            spiel.draw(c6z5s1sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z5s1sprite.setColor(farben[1]);
            spiel.draw(c6z5s1sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z5s1sprite.setColor(farben[2]);
            spiel.draw(c6z5s1sprite);
            ic6ispieler3--;
        }

        //c6z5s2
        if (ic6ispieler1 != 0) {
            c6z5s2sprite.setColor(farben[0]);
            spiel.draw(c6z5s2sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z5s2sprite.setColor(farben[1]);
            spiel.draw(c6z5s2sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z5s2sprite.setColor(farben[2]);
            spiel.draw(c6z5s2sprite);
            ic6ispieler3--;
        }

        //c6z5s3
        if (ic6ispieler1 != 0) {
            c6z5s3sprite.setColor(farben[0]);
            spiel.draw(c6z5s3sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z5s3sprite.setColor(farben[1]);
            spiel.draw(c6z5s3sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z5s3sprite.setColor(farben[2]);
            spiel.draw(c6z5s3sprite);
            ic6ispieler3--;
        }

        //c6z5s4
        if (ic6ispieler1 != 0) {
            c6z5s4sprite.setColor(farben[0]);
            spiel.draw(c6z5s4sprite);
            ic6ispieler1--;
        }
        else if (ic6ispieler2 != 0) {
            c6z5s4sprite.setColor(farben[1]);
            spiel.draw(c6z5s4sprite);
            ic6ispieler2--;
        }
        else if (ic6ispieler3 != 0) {
            c6z5s4sprite.setColor(farben[2]);
            spiel.draw(c6z5s4sprite);
            ic6ispieler3--;
        }

        //Würfelsprites zeichnen
        wuerfelsprite.setColor(farben[iispielernummer]);
        wuerfelsprite2.setColor(farben[iispielernummer]);
        wuerfelsprite3.setColor(farben[iispielernummer]);
        wuerfelsprite4.setColor(farben[iispielernummer]);
        wuerfelsprite5.setColor(farben[iispielernummer]);
        wuerfelsprite6.setColor(farben[iispielernummer]);
        wuerfelsprite7.setColor(farben[iispielernummer]);
        wuerfelsprite8.setColor(farben[iispielernummer]);
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
                bool meldung = bestätigungsmeldung("Möchten Sie den Spielstand speichern?");
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
                    if (baufwuerfelgedrueckt == true) {
                        bzugbeendet = true;
                        baufwuerfelgedrueckt = false;
                    }
                }

                if (undobutton.isMouseOver(spiel)) { 
                    setzeSchrittZurück();
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
                    int wuerfelplatzwert = iiwuerfelwert[0];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeWürfel(iispielernummer, 1);
                        break;
                    case 2:
                        setzeWürfel(iispielernummer, 2);
                        break;
                    case 3:
                        setzeWürfel(iispielernummer, 3);
                        break;
                    case 4:
                        setzeWürfel(iispielernummer, 4);
                        break;
                    case 5:
                        setzeWürfel(iispielernummer, 5);
                        break;
                    case 6:
                        setzeWürfel(iispielernummer, 6);
                        break;
                    }
                }
                if (wuerfelcoords2.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = iiwuerfelwert[1];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeWürfel(iispielernummer, 1);
                        break;
                    case 2:
                        setzeWürfel(iispielernummer, 2);
                        break;
                    case 3:
                        setzeWürfel(iispielernummer, 3);
                        break;
                    case 4:
                        setzeWürfel(iispielernummer, 4);
                        break;
                    case 5:
                        setzeWürfel(iispielernummer, 5);
                        break;
                    case 6:
                        setzeWürfel(iispielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords3.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = iiwuerfelwert[2];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeWürfel(iispielernummer, 1);
                        break;
                    case 2:
                        setzeWürfel(iispielernummer, 2);
                        break;
                    case 3:
                        setzeWürfel(iispielernummer, 3);
                        break;
                    case 4:
                        setzeWürfel(iispielernummer, 4);
                        break;
                    case 5:
                        setzeWürfel(iispielernummer, 5);
                        break;
                    case 6:
                        setzeWürfel(iispielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords4.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = iiwuerfelwert[3];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeWürfel(iispielernummer, 1);
                        break;
                    case 2:
                        setzeWürfel(iispielernummer, 2);
                        break;
                    case 3:
                        setzeWürfel(iispielernummer, 3);
                        break;
                    case 4:
                        setzeWürfel(iispielernummer, 4);
                        break;
                    case 5:
                        setzeWürfel(iispielernummer, 5);
                        break;
                    case 6:
                        setzeWürfel(iispielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords5.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = iiwuerfelwert[4];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeWürfel(iispielernummer, 1);
                        break;
                    case 2:
                        setzeWürfel(iispielernummer, 2);
                        break;
                    case 3:
                        setzeWürfel(iispielernummer, 3);
                        break;
                    case 4:
                        setzeWürfel(iispielernummer, 4);
                        break;
                    case 5:
                        setzeWürfel(iispielernummer, 5);
                        break;
                    case 6:
                        setzeWürfel(iispielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords6.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = iiwuerfelwert[5];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeWürfel(iispielernummer, 1);
                        break;
                    case 2:
                        setzeWürfel(iispielernummer, 2);
                        break;
                    case 3:
                        setzeWürfel(iispielernummer, 3);
                        break;
                    case 4:
                        setzeWürfel(iispielernummer, 4);
                        break;
                    case 5:
                        setzeWürfel(iispielernummer, 5);
                        break;
                    case 6:
                        setzeWürfel(iispielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords7.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = iiwuerfelwert[6];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeWürfel(iispielernummer, 1);
                        break;
                    case 2:
                        setzeWürfel(iispielernummer, 2);
                        break;
                    case 3:
                        setzeWürfel(iispielernummer, 3);
                        break;
                    case 4:
                        setzeWürfel(iispielernummer, 4);
                        break;
                    case 5:
                        setzeWürfel(iispielernummer, 5);
                        break;
                    case 6:
                        setzeWürfel(iispielernummer, 6);
                        break;
                    }

                }
                if (wuerfelcoords8.contains(mouse))
                {
                    // mouse is on sprite!
                    int wuerfelplatzwert = iiwuerfelwert[7];
                    switch (wuerfelplatzwert) {
                    case 1:
                        setzeWürfel(iispielernummer, 1);
                        break;
                    case 2:
                        setzeWürfel(iispielernummer, 2);
                        break;
                    case 3:
                        setzeWürfel(iispielernummer, 3);
                        break;
                    case 4:
                        setzeWürfel(iispielernummer, 4);
                        break;
                    case 5:
                        setzeWürfel(iispielernummer, 5);
                        break;
                    case 6:
                        setzeWürfel(iispielernummer, 6);
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
    while(bspielbeendet == false) {
        //srundentext verändern

        //Geldscheine zufällig auswählen und anzeigen (i <=5 machen nicht vergessen)
        srand(time(NULL));
        if (bspielgeladen == false) {
            for (int i = 0; i <= 5; i++) {
                switch (i) {
                case 0:
                    for (int k = 0; ; k++) {
                        //int k = 0;
                        int j = rand() % 6;
                        iicasinogelder1[k] = igeldwerte[j];
                        printf("Gerade gezogen: %i\n", iicasinogelder1[k]);
                        isumme1 += iicasinogelder1[k];
                        if (isumme1 >= 50000) {
                            printf("Summe Casino 1: %i\n", isumme1);
                            break;
                        }

                    }
                    break;
                case 1:
                    for (int k = 0; ; k++) {
                        //int k = 0;
                        int j = rand() % 6;
                        iicasinogelder2[k] = igeldwerte[j];
                        printf("Gerade gezogen: %i\n", iicasinogelder2[k]);
                        isumme2 += iicasinogelder2[k];
                        if (isumme2 >= 50000) {
                            printf("Summe Casino 2: %i\n", isumme2);
                            break;
                        }

                    }
                    break;
                case 2:
                    for (int k = 0; ; k++) {
                        //int k = 0;
                        int j = rand() % 6;
                        iicasinogelder3[k] = igeldwerte[j];
                        printf("Gerade gezogen: %i\n", iicasinogelder3[k]);
                        isumme3 += iicasinogelder3[k];
                        if (isumme3 >= 50000) {
                            printf("Summe Casino 3: %i\n", isumme3);
                            break;
                        }
                    }
                    break;
                case 3:
                    for (int k = 0; ; k++) {
                        //int k = 0;
                        int j = rand() % 6;
                        iicasinogelder4[k] = igeldwerte[j];
                        printf("Gerade gezogen: %i\n", iicasinogelder4[k]);
                        isumme4 += iicasinogelder4[k];
                        if (isumme4 >= 50000) {
                            printf("Summe Casino 4: %i\n", isumme4);
                            break;
                        }
                    }
                    break;
                case 4:
                    for (int k = 0; ; k++) {
                        //int k = 0;
                        int j = rand() % 6;
                        iicasinogelder5[k] = igeldwerte[j];
                        printf("Gerade gezogen: %i\n", iicasinogelder5[k]);
                        isumme5 += iicasinogelder5[k];
                        if (isumme5 >= 50000) {
                            printf("Summe Casino 5: %i\n", isumme5);
                            break;
                        }
                    }
                    break;
                case 5:
                    for (int k = 0; ; k++) {
                        //int k = 0;
                        int j = rand() % 6;
                        iicasinogelder6[k] = igeldwerte[j];
                        printf("Gerade gezogen: %i\n", iicasinogelder6[k]);
                        isumme6 += iicasinogelder6[k];
                        if (isumme6 >= 50000) {
                            printf("Summe Casino 6: %i\n", isumme6);
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
                if (iicasinogelder1[i] < iicasinogelder1[j])
                {
                    //-----Tausch-----
                    int h = iicasinogelder1[i];
                    iicasinogelder1[i] = iicasinogelder1[j];
                    iicasinogelder1[j] = h;
                }
            }
        }

        //Casino 2
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
            {
                if (iicasinogelder2[i] < iicasinogelder2[j])
                {
                    //-----Tausch-----
                    int h = iicasinogelder2[i];
                    iicasinogelder2[i] = iicasinogelder2[j];
                    iicasinogelder2[j] = h;
                }
            }
        }

        //Casino 3
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
            {
                if (iicasinogelder3[i] < iicasinogelder3[j])
                {
                    //-----Tausch-----
                    int h = iicasinogelder3[i];
                    iicasinogelder3[i] = iicasinogelder3[j];
                    iicasinogelder3[j] = h;
                }
            }
        }

        //Casino 4
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
            {
                if (iicasinogelder4[i] < iicasinogelder4[j])
                {
                    //-----Tausch-----
                    int h = iicasinogelder4[i];
                    iicasinogelder4[i] = iicasinogelder4[j];
                    iicasinogelder4[j] = h;
                }
            }
        }

        //Casino 5
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
            {
                if (iicasinogelder5[i] < iicasinogelder5[j])
                {
                    //-----Tausch-----
                    int h = iicasinogelder5[i];
                    iicasinogelder5[i] = iicasinogelder5[j];
                    iicasinogelder5[j] = h;
                }
            }
        }

        //Casino 6
        for (int i = 0; i <= 3; i++)
        {
            for (int j = i + 1; j <= 4; j++)
            {
                if (iicasinogelder6[i] < iicasinogelder6[j])
                {
                    //-----Tausch----- 
                    int h = iicasinogelder6[i];
                    iicasinogelder6[i] = iicasinogelder6[j];
                    iicasinogelder6[j] = h;
                }
            }
        }

        while (ispielerhabenwürfel == true) {
            srundentext = "Runde " + to_string(iirundenzahlfürtext) + " von 4";
            //Würfeln, bis keiner mehr würfeln mehr hat
            if (bspielgeladen == false) {
                ispieler = 0;
            }
            for (ispieler; ispieler <= 2; ispieler++) {
                //Schreiben von Arrays
                swuerfelzahl1 = "ispieler 1 hat " + to_string(iiwuerfelanzahl[0]) + " Würfeln übrig";
                swuerfelzahl2 = "ispieler 2 hat " + to_string(iiwuerfelanzahl[1]) + " Würfeln übrig";
                swuerfelzahl3 = "ispieler 3 hat " + to_string(iiwuerfelanzahl[2]) + " Würfeln übrig";
                iirundenzahlfürtext = iirundenzahl + 1;
                srundentext = "Runde " + to_string(iirundenzahlfürtext) + " von 4";
                //Temporäre Variable fürs Überprüfen, ob ein ispieler kein Würfel hat
                int tempanzwuerfel = iiwuerfelanzahl[ispieler];
                if (tempanzwuerfel == 0) {
                    continue;
                }
                iispielernummer = ispieler;
                int anzahlwuerfel = iiwuerfelanzahl[ispieler];

                if(bspielgeladen == false){
                    //Würfel würfeln
                    for (int i = 0; i <= 7; i++) {
                        if (i > anzahlwuerfel - 1) {
                            iiwuerfelwert[i] = 8;
                        }
                        else {
                            iiwuerfelwert[i] = gen();
                            cout << iiwuerfelwert[i];
                        }
                    }
                    printf("\n");
                }
                bspielgeladen = false;
                //Würfeln sortieren
                for (int i = 0; i <= 6; i++)
                {
                    for (int j = i + 1; j <= 7; j++)
                    {
                        if (iiwuerfelwert[i] > iiwuerfelwert[j])
                        {
                            //-----Tausch-----
                            int h = iiwuerfelwert[i];
                            iiwuerfelwert[i] = iiwuerfelwert[j];
                            iiwuerfelwert[j] = h;
                        }
                    }
                }

                //Warten bis auf Würfel gedrückt wird
                while (bzugbeendet == false) {
                    //Unten den Nutzer drauf hinweisen, auf einen Würfel zu klicken!
                    switch (iispielernummer) {
                    case 0:
                        if (baufwuerfelgedrueckt == false) {
                            ssubtextstring = "ispieler 1: Bitte wählen Sie einen Würfel!";
                        }
                        else {
                            ssubtextstring = "ispieler 1: Beenden Sie den Zug oder setzen Sie den Schritt zurück!";
                        }
                        break;
                    case 1:
                        if (baufwuerfelgedrueckt == false) {
                            ssubtextstring = "ispieler 2: Bitte wählen Sie einen Würfel!";
                        }
                        else {
                            ssubtextstring = "ispieler 2: Beenden Sie den Zug oder setzen Sie den Schritt zurück!";
                        }
                        break;
                    case 2:
                        if (baufwuerfelgedrueckt == false) {
                            ssubtextstring = "ispieler 3: Bitte wählen Sie einen Würfel!";
                        }
                        else {
                            ssubtextstring = "ispieler 3: Beenden Sie den Zug oder setzen Sie den Schritt zurück!";
                        }
                        break;
                    default:
                        break;
                    }
                }
                //Resetten
                baufwuerfelgedrueckt = false;
                bzugbeendet = false;

                if (iiwuerfelanzahl[0] == 0 && iiwuerfelanzahl[1] == 0 && iiwuerfelanzahl[2] == 0) {
                    para();
                    if (iirundenzahl == 3) {
                        //Gewinner zeigen, dass er gewonnen hat!
                        printf("Spiel zu Ende!\n");
                        bspielbeendet = true;
                        if (ikontostand[0] > ikontostand[1] && ikontostand[0] > ikontostand[2]) {
                            zeichenthread.terminate();
                            gewinner(1);
                        }
                        else if (ikontostand[0]< ikontostand[1] && ikontostand[1] > ikontostand[2]) {
                            zeichenthread.terminate();
                            gewinner(2);
                        }
                        else if (ikontostand[0] < ikontostand[2] && ikontostand[1] < ikontostand[2]) {
                            zeichenthread.terminate();
                            gewinner(3);
                        }
                    }
                    else {
                        printf("iirundenzahl ist unter 3!\n");
                        iirundenzahl++;
                        iirundenzahlfürtext = iirundenzahl + 1;
                    }
                }
            }

            /*
            if (iiwuerfelanzahl[0] == 0 && iiwuerfelanzahl[1] == 0 && iiwuerfelanzahl[2] == 0) {
                printf("ispieler haben keine Würfel mehr!\n");
                para();
                if (iirundenzahl <= 3) {
                    //Funktion fürs Resetten und Geldzuteilen
                    
                    iirundenzahl++;
                }
                else {
                    //Spiel vorbei!
                    bspielbeendet = true;
                }

            }*/
        }

    }
    //Ende vom Spiel
    //Gewinner anzeigen und sterben oder so und Verlierer wird ein Kopfgeld gesetzt


}

//Dieses Thread arbeitet im Hintergrund und sorgt für einen problemlosen Lauf des Spiels
//Er überprüft auf Probleme und Fehler und korrigiert diese. In diesem Thread werden auch wichtige Funktionen aufgerufen.
void Systemthread() {
    
    //Check als erstes, ob wichtige Dateien vorhanden sind!
    if (einstellungsdateiprüfer() == false) {
        printf("[WARNUNG] Einstellungsdatei nicht gefunden! Wird erstellt!\n");
        einstellungsdateierstellung();
        fehleranzeige("Datei-Fehler", "Einstellungsdateifehler! Bitte neustarten");

    }
    else {
        //Gähnix
        printf("[INFO] Einstellungsdatei vorhanden!\n");
    }

    if (speicherdatenprüfung() == false) {
        printf("[WARNUNG]Speicherdateien nicht gefunden! Dateien werden erstellt!");
        speicherdateienerstellung();
    }
    else {
        printf("[INFO] Speicherdateien vorhanden!\n");
    }
    
    //Variablenprüfung
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
    sf::Music music;
    music.openFromFile("res/Audio/titelmusik.wav");
    music.setVolume(50);
    music.setLoop(true);
    //Musik spielen, falls Einstellung erlaubt
    if (e == 1) {
        music.play();
    }
    
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
  
    // Eine Schleife, dass das Programm solange laufen lässt, bis es geschlossen wird
    while (window.isOpen())
    {
        // Überprüft alle Eventtrigger nach dem letzten Frame
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Schließt das Fenster, falls schließen gedrückt wird
            if (event.type == sf::Event::Closed) {
                  window.close();
                  exit(0);
            }
              

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
                    //Variablen setzen
                    for (int i = 0; i <= 4; i++) {
                        iicasinogelder1[i] = 0;
                        iicasinogelder2[i] = 0;
                        iicasinogelder3[i] = 0;
                        iicasinogelder4[i] = 0;
                        iicasinogelder5[i] = 0;
                        iicasinogelder6[i] = 0;
                    }

                    for (int i = 0; i <= 2; i++) {
                        icasinoiiwuerfelanzahl1[i] = 0;
                        icasinoiiwuerfelanzahl2[i] = 0;
                        icasinoiiwuerfelanzahl3[i] = 0;
                        icasinoiiwuerfelanzahl4[i] = 0;
                        icasinoiiwuerfelanzahl5[i] = 0;
                        icasinoiiwuerfelanzahl6[i] = 0;
                        iiwuerfelanzahl[i] = 8;
                    }
                    iirundenzahl = 0;
                    ispielerhabenwürfel = true;
                    baufwuerfelgedrueckt = false;
                    bspielbeendet = false; 
                    bzugbeendet = false;
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
                    dateienmanager(false);
                    
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