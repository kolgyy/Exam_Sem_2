#pragma once
#include <SFML/Graphics.hpp>
#include <Variables.hpp>


class Map {
protected:
    sf::RenderWindow& m_renderWindow;
    std::vector<std::vector<unsigned int>>m_vecMap;
    std::vector<sf::RectangleShape>m_vecBox;

public:
    Map(sf::RenderWindow& renderWindow);
    void draw();
    bool checkMapCase(unsigned int dx, unsigned int dy);
};