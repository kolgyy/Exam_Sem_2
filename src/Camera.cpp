#include <Camera.hpp>

Camera::Camera(sf::RenderWindow& renderWindow, Map& map) : m_renderWindow(renderWindow), m_map(map) {
    m_radius = 5; // ������ ������-����������
    m_camera.setRadius(m_radius);
    m_camera.setFillColor(sf::Color(0, 255, 0)); // ��������� �������� �����
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

void Camera::draw() { // ��������� ������
    for (auto& vvec : m_vecRays) {
        m_renderWindow.draw(vvec);
    }
    m_renderWindow.draw(m_camera);
}

void Camera::update() {
    sf::Time dt = m_clock.restart();
    checkKeyBoardHit(dt);
    projection();
}

void Camera::projection() {
    for (unsigned int i = 0; i < m_vecRays.size(); i++) {
        m_vecRays[i][0].position = m_position; // ������ ������� ����

        if (Intersect(i)) { // ���� ��� ������������ � ������������, �� �� ���������
            m_vecRays[i][1].position = m_intersection;
        }
        else { // ����� ������������ ����� ����
            m_vecRays[i][1].position = { m_position.x + m_rayLength * dCos((m_angle + m_fov / 2) - i * (m_fov / WIDTHSCREEN)), // ����� ���� �� X
                                        m_position.y + m_rayLength * dSin((m_angle + m_fov / 2) - i * (m_fov / WIDTHSCREEN)) }; // ����� ���� �� Y
        }
    }
}

void Camera::checkKeyBoardHit(sf::Time dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        float dx = m_speedMove * cos(m_angle * PI / 180) * dt.asSeconds();
        float dy = m_speedMove * sin(m_angle * PI / 180) * dt.asSeconds();

        // �������� �������� ��� ����������� ������
        if (!checkCollision(m_position.x + dx, m_position.y)) {
            m_position.x += dx;
        }
        if (!checkCollision(m_position.x, m_position.y + dy)) {
            m_position.y += dy;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        float dx = -m_speedMove * cos(m_angle * PI / 180) * dt.asSeconds();
        float dy = -m_speedMove * sin(m_angle * PI / 180) * dt.asSeconds();

        // �������� �������� ��� ����������� �����
        if (!checkCollision(m_position.x + dx, m_position.y)) {
            m_position.x += dx;
        }
        if (!checkCollision(m_position.x, m_position.y + dy)) {
            m_position.y += dy;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        float dx = -m_speedMove * sin(m_angle * PI / 180) * dt.asSeconds();
        float dy = m_speedMove * cos(m_angle * PI / 180) * dt.asSeconds();

        const float newAngle = m_angle - m_speedAngle * dt.asSeconds();
        if (!checkCollision(m_position.x + dx * cos(newAngle * PI / 180), m_position.y + dy * sin(newAngle * PI / 180))) {
            m_angle = newAngle;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        float dx = m_speedMove * sin(m_angle * PI / 180) * dt.asSeconds();
        float dy = -m_speedMove * cos(m_angle * PI / 180) * dt.asSeconds();

        const float newAngle = m_angle + m_speedAngle * dt.asSeconds();
        if (!checkCollision(m_position.x + dx * cos(newAngle * PI / 180), m_position.y + dy * sin(newAngle * PI / 180))) {
            m_angle = newAngle;
        }
    }

    if (m_angle > 360) m_angle = 0;
    if (m_angle < 0) m_angle = 360;

    m_camera.setPosition(m_position);
    m_camera.setRotation(m_angle);
}

bool Camera::Intersect(unsigned int it) {
    float fAngle = (m_angle + m_fov / 2 - it * (m_fov / WIDTHSCREEN)); // ���� ����������� ���� �� ������ 
    sf::Vector2f direction = { dCos(fAngle), dSin(fAngle) }; // ����������� ����

    for (unsigned int i = 0; i < m_rayLength; i++) { // �������� �� ����� �����
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
bool Camera::checkCollision(float x, float y) { // ���� �� ���� � �����
    unsigned int dx = static_cast<unsigned int>(x / BlockSize);
    unsigned int dy = static_cast<unsigned int>(y / BlockSize);

    return m_map.checkMapCase(dx, dy);
}
