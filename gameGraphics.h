#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <SFML/Graphics.hpp>
#include <vector>

class GameGraphics {
    public:
        GameGraphics();

        sf::RenderWindow window;

        void loopDraw();

        void titleScreen();
        void testScreen();
        void resizeWindow(sf::Event);
        void closeWindow();

    private:

        void clearWindow();
        sf::Vector2u currentWindowSize;
        sf::Font font;

        std::vector<sf::Text*> texts;
        std::vector<sf::Shape*> shapes;
        std::vector<std::vector<sf::Drawable*>> elements;
};

#endif