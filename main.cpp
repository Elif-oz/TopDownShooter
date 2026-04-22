#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Top-Down Shooter - Ilk Deneme");

    sf::CircleShape oyuncu(50.f);
    oyuncu.setFillColor(sf::Color::Blue);
    oyuncu.setPosition(350.f, 250.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float hiz = 0.5f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            oyuncu.move(0.f, -hiz);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            oyuncu.move(0.f, hiz);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            oyuncu.move(-hiz, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            oyuncu.move(hiz, 0.f);
        }


        window.clear();
        window.draw(oyuncu);
        window.display();
    }

    return 0;
}
