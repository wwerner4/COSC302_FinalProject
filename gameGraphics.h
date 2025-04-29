#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <SFML/Graphics.hpp>
#include <vector>

class GameGraphics {
    public:
        GameGraphics();
        ~GameGraphics();

        sf::RenderWindow window;

        void titleScreen();
        void playScreen();
        void testScreen();

        void loopDraw();
        void resizeWindow(sf::Event);
        void closeWindow();
        void onClick();
        void eventHandler(sf::Event);

    private:

        void clearWindow();
        sf::Vector2u currentWindowSize;
        sf::Font font;

        // I'm storing texts, shapes, sprites, and textures as well as drawnElements. This is double storage, but sf::Drawable is a virtual class so I can't call all of an object's functions from drawnElements
        std::vector<sf::Text*> texts;
        std::vector<sf::Shape*> shapes;
        std::vector<sf::Sprite*> sprites;
        std::vector<sf::Texture*> textures;
        std::map<sf::Shape*,char> interactables;
        std::vector<std::vector<sf::Drawable*>> drawnElements;   // 2D to allow element grouping (i.e., player's hand might be elements[2][0 through 5]). The order of elements in drawnElements is the order they are drawn in, which is important
};

#endif