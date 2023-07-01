#pragma once
#include <Variables.hpp>
#include <Map.hpp>
#include <math.h>

class Camera {
protected:
    sf::RenderWindow& m_renderWindow;
    sf::CircleShape m_camera;
    sf::Vector2f m_position, m_intersection;
    sf::Clock m_clock;
    Map& m_map;

    float m_angle, m_fov, m_rayLength;
    float m_radius;
    float m_speedMove, m_speedAngle;

    std::vector<sf::VertexArray> m_vecRays;
public:
	Camera(sf::RenderWindow& renderWindow, Map& map);
	void draw();
    void update();
    void projection();
    void checkKeyBoardHit(sf::Time dt);
    bool Intersect(unsigned int it);
    bool checkCollision(float x, float y);

    inline float toRadian(float degree) { return (PI / 180) * degree; } // Перевод градусов в радианы
    inline float dCos(float degree) { return cos(toRadian(degree)); } // Получение косинуса от радиан
    inline float dSin(float degree) { return sin(toRadian(degree)); }// Получение синуса от радиан
};
