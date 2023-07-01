#include <Logger.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

#define PI 3.1415f
#define xCase 20 // Кол-во блоков по Y
#define yCase 15 // Кол-во блоков по X
#define BlockSize 32 // Размер блока

const unsigned int WIDTHSCREEN = xCase * BlockSize; // Ширина экрана
const unsigned int HEIGHTSCREEN = yCase * BlockSize; // Высота экрана

class Camera {
protected:
    sf::RenderWindow& m_renderWindow;
    sf::CircleShape m_camera;
    sf::Vector2f m_position, m_intersection;
    sf::Clock m_clock;

    float m_angle, m_fov, m_rayLength;
    float m_radius;
    float m_speedMove, m_speedAngle;

    std::vector<sf::VertexArray> m_vecRays;

public:
    Camera(sf::RenderWindow& renderWindow) : m_renderWindow(renderWindow) {
        m_radius = 5; // Устанавливаем радиус камеры-окружности
        m_camera.setRadius(m_radius);
        m_camera.setFillColor(sf::Color(0, 255, 0)); // Устаналиваем зеленый цвет
        m_camera.setOrigin(sf::Vector2f(m_radius, m_radius));

        m_speedMove = 200.0f; // Скорость движения
        m_speedAngle = 100.0f; // Скорость поворота

        m_position = { 128, 128 }; // Изначальная позиция 

        m_fov = 60; // Угол обзора

        m_rayLength = 200; // Длина луча

        for (unsigned int i = 0; i < WIDTHSCREEN; i++) { // Установка цвета для лучей
            sf::VertexArray ray(sf::Lines, 2);
            ray[0].color = sf::Color::Red;
            ray[1].color = sf::Color::Red;

            m_vecRays.push_back(ray);
        }


    }
    void draw() { // Отрисовка камеры и лучей от камеры
        for (auto& vvec : m_vecRays) {
            m_renderWindow.draw(vvec);
        }
        m_renderWindow.draw(m_camera);
    }
    void update() {
        sf::Time dt = m_clock.restart(); 

    }
    void projection() {
        for (unsigned int i = 0; i < m_vecRays.size(); i++) {
            m_vecRays[i][0].position = m_position; // Начало каждого луча

            m_vecRays[i][1].position = { m_position.x + m_rayLength * dCos((m_angle + m_fov / 2) - i * (m_fov / WIDTHSCREEN)), // Конец луча по X
                                        m_position.y + m_rayLength * dSin((m_angle + m_fov / 2) - i * (m_fov / WIDTHSCREEN)) }; // Конец лучка по Y
        }
    }

    void checkKeyBoardHit() {

    }

    inline float toRadian(float degree) { return (PI / 180) * degree; } // Перевод градусов в радианы
    inline float dCos(float degree) { return cos(toRadian(degree)); } // Получение косинуса от радиан
    inline float dSin(float degree) { return sin(toRadian(degree)); }// Получение синуса от радиан
};



int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTHSCREEN, HEIGHTSCREEN), "ray-cast scene");
    window.setFramerateLimit(60); // Устанавливаем 60 кадров
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}