#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Top-Down Shooter - Ilk Deneme");

    sf::CircleShape oyuncu(50.f);
    oyuncu.setFillColor(sf::Color::Green);
    oyuncu.setPosition(350.f, 250.f); // Daireyi ekranýn ortasýna koyar

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(oyuncu);
        window.display();
    }

    return 0;
}
