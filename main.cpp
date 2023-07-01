#include <Logger.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

#define PI 3.1415f
#define xCase 20 // ���-�� ������ �� Y
#define yCase 15 // ���-�� ������ �� X
#define BlockSize 32 // ������ �����

const unsigned int WIDTHSCREEN = xCase * BlockSize; // ������ ������
const unsigned int HEIGHTSCREEN = yCase * BlockSize; // ������ ������


class Map {
protected:
    sf::RenderWindow& m_renderWindow;
    std::vector<std::vector<unsigned int>>m_vecMap;
    std::vector<sf::RectangleShape>m_vecBox;

public:
    Map(sf::RenderWindow& renderWindow) : m_renderWindow(renderWindow) {
        m_vecMap =
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // ��������� �����
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
            {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1},
            {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
            {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}

        };
        for (unsigned int y = 0; y < yCase; y++) {
            for (unsigned int x = 0; x < xCase; x++) {
                if (m_vecMap[y][x]) {
                    sf::RectangleShape box(sf::Vector2f(BlockSize, BlockSize)); // �������������� ���� � �����
                    box.setPosition(sf::Vector2f(x * BlockSize, y * BlockSize)); // ���������� ������ �� ������
                    m_vecBox.push_back(box);
                }
            }
        }
    }

    void draw() {
        for (auto& vvec : m_vecBox) {
            m_renderWindow.draw(vvec);
        }
    }

    bool checkMapCase(unsigned int dx, unsigned int dy) {
        if (m_vecMap[dy][dx]) {
            return true;
        }
        else {
            return false;
        }
    }

};


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
    Camera(sf::RenderWindow& renderWindow, Map& map) : m_renderWindow(renderWindow), m_map(map) {
        m_radius = 5; // ������������� ������ ������-����������
        m_camera.setRadius(m_radius);
        m_camera.setFillColor(sf::Color(0, 255, 0)); // ������������ ������� ����
        m_camera.setOrigin(sf::Vector2f(m_radius, m_radius));

        m_speedMove = 200.0f; // �������� ��������
        m_speedAngle = 100.0f; // �������� ��������

        m_position = { 70, 70 }; // ����������� ������� 

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
        checkKeyBoardHit(dt);
        projection();

    }
    void projection() {
        for (unsigned int i = 0; i < m_vecRays.size(); i++) {
            m_vecRays[i][0].position = m_position; // ������ ������� ����

            if (Intersect(i)) { // ���� ���������� ������, �� ������ ����� ����
                m_vecRays[i][1].position = m_intersection;
            }
            else { // ���� ���, �� ������������ ������
                m_vecRays[i][1].position = { m_position.x + m_rayLength * dCos((m_angle + m_fov / 2) - i * (m_fov / WIDTHSCREEN)), // ����� ���� �� X
                                            m_position.y + m_rayLength * dSin((m_angle + m_fov / 2) - i * (m_fov / WIDTHSCREEN)) }; // ����� ����� �� Y
            }
        }
    }

    void checkKeyBoardHit(sf::Time dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { // �������� ������
            m_position.x += dCos(m_angle) * m_speedMove * dt.asSeconds();
            m_position.y += dSin(m_angle) * m_speedMove * dt.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { // �������� �����
            m_position.x -= dCos(m_angle) * m_speedMove * dt.asSeconds();
            m_position.y -= dSin(m_angle) * m_speedMove * dt.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { // ������� ������
            m_angle -= m_speedAngle * dt.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { // ������� �������
            m_angle += m_speedAngle * dt.asSeconds();
        }

        if (m_angle > 360) m_angle = 0;
        if (m_angle < 0) m_angle = 360;

        m_camera.setPosition(m_position);
        m_camera.setRotation(m_angle);
    }

    bool Intersect(unsigned int it) {

        float fAngle = (m_angle + m_fov / 2 - it * (m_fov / WIDTHSCREEN));
        sf::Vector2f direction = { dCos(fAngle), dSin(fAngle) };

        for (unsigned int i = 0; i < m_rayLength; i++) {
            int dx = static_cast<int>(m_position.x + i * direction.x);
            int dy = static_cast<int>(m_position.y + i * direction.y);

            if (m_map.checkMapCase(dx / BlockSize, dy / BlockSize)) {
                m_intersection.x = dx;
                m_intersection.y = dy;
                return true;
            }
        }
        return false;
    }



    inline float toRadian(float degree) { return (PI / 180) * degree; } // ������� �������� � �������
    inline float dCos(float degree) { return cos(toRadian(degree)); } // ��������� �������� �� ������
    inline float dSin(float degree) { return sin(toRadian(degree)); }// ��������� ������ �� ������
};





int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTHSCREEN, HEIGHTSCREEN), "ray-cast scene");
    window.setFramerateLimit(60); // ������������� 60 ������
    
    Map map(window);
    Camera camera(window, map);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        camera.update();

        window.clear();

        camera.draw();
        map.draw();

        window.display();
    }

    return 0;
}