#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <SFML/Graphics.hpp>
#include <vector>

class GameGraphics {
    public:
        GameGraphics();
        ~GameGraphics();

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
        std::vector<std::vector<sf::Drawable*>> drawnElements;   // 2D to allow element grouping (i.e., player's hand might be elements[2][0 through 5]). This allows addition and removal of elements without doing index math
};

#endif