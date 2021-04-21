//Autor: Kerem Okumus, Kiriakos Avramidis, Moritz Biedenbacher
//Las Vegas Spiel als grafische Anwendung
//Dieses Programm nutzt die SFML-Bibliothek, dass zur Programmierung von grafischen Benutzeroberflächen erleichtern soll!
//geändert am: 25.01.2021(Datei erstellt und ersten Code geschrieben); 26.01.2021 (Code verändert und Menü hinzugefügt)


#include<SFML/Graphics.hpp>
#include<SFML/Graphics/Text.hpp>
#include<SFML/Audio.hpp>

#include<iostream>
#include<Windows.h>
#include<fstream>
#include<stdlib.h>
#include<conio.h>
 
using namespace std;
using namespace sf;

//Selbstgeschriebene Header-Dateien
#include "Button.h"

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
    std::ofstream schreiber("einstellungen.einstellungen", std::ios::out | std::ios::binary);

    //Nenne alles, was geschrieben werden muss
    //Siehe oben, um zu verstehen, für was was steht!
    einstellung writer[1];
    writer[0].stellung = 1;
    writer[1].stellung = 50;

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
    std::ifstream leser("einstellungen.einstellungen", std::ios::out | std::ios::binary);
    if (!leser) {
        fehleranzeige("Datei-Fehler", "Einstellungsdatei konnte nicht geöffnet werden.");
        return;
    }

    //Array für gelesene Daten erstellen
    einstellung items[1];

    //Daten von Datei einlesen
    for(int i = 0; i < 2; i++) {
        leser.read((char*) &items[i], sizeof(einstellung));
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
                        std::ofstream schreiber("einstellungen.einstellungen", std::ios::out | std::ios::trunc);
                        //Array für Daten erstellen und beschriften
                        einstellung items[1];
                        //p = Ob Sound laufen soll
                        //v = Lautstärke
                        items[0].stellung = p;
                        items[1].stellung = v;

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
//MAIN SPIEL
//
//Eine Function fürs neue Spiel erstellen (nicht vollständig)
void neuesSpiel() {
    sf::RenderWindow spiel(sf::VideoMode(1920, 1080), "Las Vegas", sf::Style::Fullscreen);

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
        spiel.clear();

        //TestKarte
        //Dies wird mit Punkten gezeichnet sprich an jeder Ecke ein Punkt
        sf::ConvexShape testkarte;
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
        spiel.display();
    }
}

//
//Main-Funktion - Hier befinden sich alle Hauptfunktionen wie Erzeugung des Bildschirms etc.
//
int main()
{
    //Erzeuge das Fenster-Objekt
    sf::RenderWindow window(sf::VideoMode(1220, 800), "Las Vegas");

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
    std::ifstream leser("einstellungen.einstellungen", std::ios::out | std::ios::binary);
    if (!leser) {
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
        
        // Switch, um zu prüfen, welches Event passiert ist und was getan werden soll
        switch (event.type) {
            //Wenn es geklickt hat
            case sf::Event::MouseButtonPressed:
                if (playbtn1.isMouseOver(window)) {
                    //Wenn Spielen-Knopf gedrückt wird
                    //debug printf("Knopf wurde gedrueckt!");
                    //test fehleranzeige("Kritischer Fehler", "Fehler! Bitte starten Sie das Spiel neu!");
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