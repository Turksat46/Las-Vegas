//Autor: Kerem Okumus
//Beschreibung: Diese Datei dient zur Ressourcenerstellung und Verwaltung für das Spiel
//Datum: 25.06.2021

#pragma once

#include "infosystem.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

struct spieldaten {
    int kontostand[3];
    int casinogelder1[5], casinogelder2[5], casinogelder3[5], casinogelder4[5], casinogelder5[5], casinogelder6[5];
    int casinowuerfelanzahl1[3], casinowuerfelanzahl2[3], casinowuerfelanzahl3[3], casinowuerfelanzahl4[3], casinowuerfelanzahl5[3], casinowuerfelanzahl6[3];
    int wuerfelanzahl[3];
    int wuerfelwert[8];
    int spielernummer;
    int rundenzahl;
};
spieldaten Speicher;
void speicherdateienerstellung() {
    printf("Schreiben wird gestartet!\n");

    ofstream schreiber("data1.txt", std::ios::out );
    ofstream schreiber2("data2.txt", std::ios::out );
    ofstream schreiber3("data3.txt", std::ios::out );
    ofstream schreiber4("data4.txt", std::ios::out );

    schreiber << "leer" << endl;
    schreiber2 << "leer" << endl;
    schreiber3 << "leer" << endl;
    schreiber4 << "leer" << endl;

    schreiber.close();
    schreiber2.close();
    schreiber3.close();
    schreiber4.close();

}

void speicherdateienänderung(int speicherplatznummer, int* kontostandarray, int* casinogelder1, int* casinogelder2, int* casinogelder3, int* casinogelder4, int* casinogelder5, int* casinogelder6, int* casinowuerfelanzahl1, int* casinowuerfelanzahl2, int* casinowuerfelanzahl3, int* casinowuerfelanzahl4, int* casinowuerfelanzahl5, int* casinowuerfelanzahl6, int* wuerfelanzahl, int* wuerfelwert, int spielernummer, int rundenzahl, string* letzteaktion) {

    if (speicherplatznummer == 1) {
        ofstream schreiber("data1.txt", std::ios::out);
        for (int i = 0; i <= 2; i++) {
            schreiber << kontostandarray[i] << endl;
        }
        for(int i = 0; i <= 4; i++){
            schreiber << casinogelder1[i] <<endl;
            schreiber << casinogelder2[i] << endl;
            schreiber << casinogelder3[i] << endl;
            schreiber << casinogelder4[i] << endl;
            schreiber << casinogelder5[i] << endl;
            schreiber << casinogelder6[i] << endl;
        }

        for(int i = 0; i <= 2; i++){
            schreiber << casinowuerfelanzahl1[i] << endl;
            schreiber << casinowuerfelanzahl2[i] << endl;
            schreiber << casinowuerfelanzahl3[i] << endl;
            schreiber << casinowuerfelanzahl4[i] << endl;
            schreiber << casinowuerfelanzahl5[i] << endl;
            schreiber << casinowuerfelanzahl6[i] << endl;
        }

        for (int i = 0; i <= 2; i++) {
            schreiber << wuerfelanzahl[i] << endl;
        }

        for (int i = 0; i <= 7; i++) {
            schreiber << wuerfelwert[i] << endl;
        }
        
        schreiber << spielernummer << endl;
        schreiber << rundenzahl << endl;

        for (int i = 0; i <= 2; i++) {
            schreiber << letzteaktion[i] << endl;
        }

        schreiber.close();
    }
    if (speicherplatznummer == 2) {
        ofstream schreiber("data2.txt", std::ios::out);
        for (int i = 0; i < 2; i++) {
            schreiber << kontostandarray[i] << endl;
        }

        for (int i = 0; i < 4; i++) {
            schreiber << casinogelder1[i] << endl;
            schreiber << casinogelder2[i] << endl;
            schreiber << casinogelder3[i] << endl;
            schreiber << casinogelder4[i] << endl;
            schreiber << casinogelder5[i] << endl;
            schreiber << casinogelder6[i] << endl;

        }

        for (int i = 0; i < 2; i++) {
            schreiber << casinowuerfelanzahl1[i] << endl;
            schreiber << casinowuerfelanzahl2[i] << endl;
            schreiber << casinowuerfelanzahl3[i] << endl;
            schreiber << casinowuerfelanzahl4[i] << endl;
            schreiber << casinowuerfelanzahl5[i] << endl;
            schreiber << casinowuerfelanzahl6[i] << endl;
        }

        for (int i = 0; i < 2; i++) {
            schreiber << wuerfelanzahl[i] << endl;
        }

        for (int i = 0; i < 7; i++) {
            schreiber << wuerfelwert[i] << endl;
        }

        schreiber << spielernummer << endl;
        schreiber << rundenzahl << endl;

        for (int i = 0; i <= 2; i++) {
            schreiber << letzteaktion[i] << endl;
        }
        
        schreiber.close();
    }
    if (speicherplatznummer == 3) {
        ofstream schreiber("data3.txt", std::ios::out);
        for (int i = 0; i < 2; i++) {
            schreiber << kontostandarray[i] << endl;
        }
        for (int i = 0; i < 4; i++) {
            schreiber << casinogelder1[i] << endl;
            schreiber << casinogelder2[i] << endl;
            schreiber << casinogelder3[i] << endl;
            schreiber << casinogelder4[i] << endl;
            schreiber << casinogelder5[i] << endl;
            schreiber << casinogelder6[i] << endl;

        }

        for (int i = 0; i < 2; i++) {
            schreiber << casinowuerfelanzahl1[i] << endl;
            schreiber << casinowuerfelanzahl2[i] << endl;
            schreiber << casinowuerfelanzahl3[i] << endl;
            schreiber << casinowuerfelanzahl4[i] << endl;
            schreiber << casinowuerfelanzahl5[i] << endl;
            schreiber << casinowuerfelanzahl6[i] << endl;
        }

        for (int i = 0; i < 2; i++) {
            schreiber << wuerfelanzahl[i] << endl;
        }

        for (int i = 0; i < 7; i++) {
            schreiber << wuerfelwert[i] << endl;
        }

        for (int i = 0; i <= 2; i++) {
            schreiber << letzteaktion[i] << endl;
        }

        schreiber << spielernummer << endl;
        schreiber << rundenzahl << endl;
        schreiber.close();
    }
    if (speicherplatznummer == 4) {
        ofstream schreiber("data4.txt", std::ios::out);
        for (int i = 0; i < 2; i++) {
            schreiber << kontostandarray[i] << endl;
        }
        for (int i = 0; i < 4; i++) {
            schreiber << casinogelder1[i] << endl;
            schreiber << casinogelder2[i] << endl;
            schreiber << casinogelder3[i] << endl;
            schreiber << casinogelder4[i] << endl;
            schreiber << casinogelder5[i] << endl;
            schreiber << casinogelder6[i] << endl;

        }

        for (int i = 0; i < 2; i++) {
            schreiber << casinowuerfelanzahl1[i] << endl;
            schreiber << casinowuerfelanzahl2[i] << endl;
            schreiber << casinowuerfelanzahl3[i] << endl;
            schreiber << casinowuerfelanzahl4[i] << endl;
            schreiber << casinowuerfelanzahl5[i] << endl;
            schreiber << casinowuerfelanzahl6[i] << endl;
        }

        for (int i = 0; i < 2; i++) {
            schreiber << wuerfelanzahl[i] << endl;
        }

        for (int i = 0; i < 7; i++) {
            schreiber << wuerfelwert[i] << endl;
        }

        for (int i = 0; i <= 2; i++) {
            schreiber << letzteaktion[i] << endl;
        }

        schreiber << spielernummer << endl;
        schreiber << rundenzahl << endl;
        schreiber.close();
    }
}

void speicherdateienlöschung(int speicherdatei) {
    string dateiname;
    switch (speicherdatei) {
    case 1:
        dateiname = "data1.txt";
        break;
    case 2:
        dateiname = "data2.txt";
        break;
    case 3:
        dateiname = "data3.txt";
        break;
    case 4:
        dateiname = "data4.txt";
        break;
    }

    ofstream schreiber(dateiname, std::ios::out);

    schreiber << "leer" << endl;
    schreiber.close();

}

void einstellungsdateierstellung() {
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
    ofstream schreiber("einstellungen.einstellungen", std::ios::out );

    //Nenne alles, was geschrieben werden muss
    //Siehe oben, um zu verstehen, für was was steht!
    int writer[3];
    writer[0] = 1;
    writer[1] = 50;

    //schreiber.write((char*)&writer[0].stellung, sizeof(einstellung));

    //Schreibe alle Daten ein
    printf("Schreiben wird gestartet!\n");
    for (int i = 0; i < 2; i++) {
        schreiber.write((char*)&writer[i], sizeof(einstellung));
    }

    //Datei schließen
    schreiber.close();

    //Prüfe, ob alles erfolgreich war!
    if (!schreiber.good()) {
        fehleranzeige("Datei-Fehler", "Einstellungsdatei konnte nicht geöffnet werden.");
        return;
    }

}

//Erstelle eine Struktur für Daten
struct einstellung {
    int stellung;
};

void einstellungsdateiänderung(int p, int v) {
    //Übernehm die Einstellungen
    ofstream schreiber("einstellungen.einstellungen", std::ios::out | std::ios::trunc | ios::binary);
    //Array für Daten erstellen und beschriften
    int items[2];
    //einstellung items[1];
    //p = Ob Sound laufen soll
    //v = Lautstärke
    items[0] = p;
    items[1] = v;

    //Indizen in die Datei schreiben
    for (int i = 0; i < 2; i++) {
        schreiber.write((char*)&items[i], sizeof(einstellung));
    }

    //Datei schließen
    schreiber.close();
    printf("Einstellungen wurden uebernommen! \n");
}

//Für das Systemthread!

bool speicherdatenprüfung() {
    string name1 = "data1.txt";
    string name2 = "data2.txt";
    string name3 = "data3.txt";
    string name4 = "data4.txt";

    ifstream prüfer(name1, std::ios::in );
    if (prüfer.is_open()) {
        prüfer.close();
        return true;
    }
    else {
        prüfer.close();
        return false;
    }
    ifstream prüfer2(name2, std::ios::in );
    if (prüfer2.is_open()) {
        prüfer2.close();
        return true;
    }
    else {
        prüfer2.close();
        return false;
    }
    ifstream prüfer3(name3, std::ios::in );
    if (prüfer3.is_open()) {
        prüfer3.close();
        return true;
    }
    else {
        prüfer3.close();
        return false;
    }
    ifstream prüfer4(name4, std::ios::in );
    if (prüfer4.is_open()) {
        prüfer4.close();
        return true;
    }
    else {
        prüfer4.close();
        return false;
    }

}

bool einstellungsdateiprüfer() {
    string name = "Einstellungen.einstellungen";
    ifstream prüfer(name, std::ios::in );
    if (prüfer.is_open()) {
        prüfer.close();
        return true;
    }
    else {
        prüfer.close();
        return false;
    }
}