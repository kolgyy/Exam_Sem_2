#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Logger.hpp>
#include <Map.hpp>
#include <Camera.hpp>
#include <Variables.hpp>

int main()
{
    sf::Music music;

    try {
        if (!music.openFromFile("../assets/Music.wav")) {
            throw std::runtime_error("Failed to open music file");
        }

        music.setVolume(35);
        music.play();
        music.setLoop(true);

    }
    catch (const std::exception& ex) {
        std::cerr << "An exception occurred: " << ex.what() << std::endl;
    }

    sf::RenderWindow window(sf::VideoMode(WIDTHSCREEN, HEIGHTSCREEN), "ray-cast scene");
    window.setFramerateLimit(60); // Установка 60 кадров в сек.
    
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
