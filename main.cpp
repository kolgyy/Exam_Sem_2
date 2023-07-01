#include <Logger.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

#define PI 3.1415f
#define xCase 20 // ���-�� ������ �� Y
#define yCase 15 // ���-�� ������ �� X
#define BlockSize 32 // ������ �����

const unsigned int WIDTHSCREEN = xCase * BlockSize; // ������ ������
const unsigned int HEIGHTSCREEN = yCase * BlockSize; // ������ ������

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
        m_radius = 5; // ������������� ������ ������-����������
        m_camera.setRadius(m_radius);
        m_camera.setFillColor(sf::Color(0, 255, 0)); // ������������ ������� ����
        m_camera.setOrigin(sf::Vector2f(m_radius, m_radius));

        m_speedMove = 200.0f; // �������� ��������
        m_speedAngle = 100.0f; // �������� ��������

        m_position = { 128, 128 }; // ����������� ������� 

        m_fov = 60; // ���� ������

        m_rayLength = 200; // ����� ����

        for (unsigned int i = 0; i < WIDTHSCREEN; i++) { // ��������� ����� ��� �����
            sf::VertexArray ray(sf::Lines, 2);
            ray[0].color = sf::Color::Red;
            ray[1].color = sf::Color::Red;

            m_vecRays.push_back(ray);
        }


    }
    void draw() { // ��������� ������ � ����� �� ������
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
            m_vecRays[i][0].position = m_position; // ������ ������� ����

            m_vecRays[i][1].position = { m_position.x + m_rayLength * dCos((m_angle + m_fov / 2) - i * (m_fov / WIDTHSCREEN)), // ����� ���� �� X
                                        m_position.y + m_rayLength * dSin((m_angle + m_fov / 2) - i * (m_fov / WIDTHSCREEN)) }; // ����� ����� �� Y
        }
    }

    void checkKeyBoardHit() {

    }

    inline float toRadian(float degree) { return (PI / 180) * degree; } // ������� �������� � �������
    inline float dCos(float degree) { return cos(toRadian(degree)); } // ��������� �������� �� ������
    inline float dSin(float degree) { return sin(toRadian(degree)); }// ��������� ������ �� ������
};



int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTHSCREEN, HEIGHTSCREEN), "ray-cast scene");
    window.setFramerateLimit(60); // ������������� 60 ������
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