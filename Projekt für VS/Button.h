//Autor: Kerem Okumus
//geändert am 25.01.2021 (Angefangen mit Konzept und einfache Programmierung mit Darstellung ohne Funktion); 26.01.2021 (finalisiert und hoffentlich ohne Fehler ;) )
//Ich bin sehr sehr objektorientiert :(

#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Button {

	//Eine öffentliche Klasse, um das Button zu erzeugen! 
public:
	Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor) {
		button.setSize(buttonSize);
		button.setFillColor(bgColor);

		// Längen und Breiten als Variable speichern
		btnWidth = buttonSize.x;
		btnHeight = buttonSize.y;

		//Design
		shadow.setSize({ btnWidth + 5.0f, btnHeight + 5.0f });
		shadow.setFillColor(bgColor - sf::Color(100, 100, 100, 100));

		text.setString(btnText);
		text.setCharacterSize(charSize);
		text.setFillColor(textColor);
	}

	// Setze die Schriftart als Eingabe
	void setFont(sf::Font& fonts) {
		text.setFont(fonts);
	}

	// Setzt die Hintergrundfarbe
	void setBackColor(sf::Color color) {
		button.setFillColor(color);
	}

	//Setzt die Textfarbe
	void setTextColor(sf::Color color) {
		text.setFillColor(color);
	}

	//Setzt die Position
	void setPosition(sf::Vector2f point) {
		button.setPosition(point);
		shadow.setPosition(point);
		//Setze den Text automatisch in die Mitte
		float xPos = (point.x + btnWidth / 2) - (text.getLocalBounds().width / 2);
		float yPos = (point.y + btnHeight / 2.6) - (text.getLocalBounds().height / 2);
		text.setPosition(xPos, yPos);
	}

	//Befehl fürs Rendering auf das Fenster
	void drawTo(sf::RenderWindow& window) {
		window.draw(button);
		window.draw(shadow);
		window.draw(text);
	}

	// Prüfung, ob die Maus über einem der Objekte ist.
	bool isMouseOver(sf::RenderWindow& window) {
		int imouseX = sf::Mouse::getPosition(window).x;
		int imouseY = sf::Mouse::getPosition(window).y;

		int btnPosX = button.getPosition().x;
		int btnPosY = button.getPosition().y;

		int btnxPosWidth = button.getPosition().x + btnWidth;
		int btnyPosHeight = button.getPosition().y + btnHeight;

		if (imouseX < btnxPosWidth && imouseX > btnPosX && imouseY < btnyPosHeight && imouseY > btnPosY) {
			return true;
		}
		return false;
	}

	// Variablen für den Button
private:
	sf::RectangleShape button;
	sf::RectangleShape shadow;
	sf::Text text;

	int btnWidth;
	int btnHeight;
};