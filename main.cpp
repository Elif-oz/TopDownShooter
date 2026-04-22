#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Top-Down Shooter - Ilk Deneme");

    sf::CircleShape oyuncu(50.f);
    oyuncu.setFillColor(sf::Color::Yellow);
    oyuncu.setPosition(640.f, 360.f);

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

        sf::Vector2f pozisyon = oyuncu.getPosition();
        sf::Vector2u pencereBoyutu = window.getSize();

        if (pozisyon.x < 0.f) {
            pozisyon.x = 0.f;
        }
        if (pozisyon.y < 0.f) {
            pozisyon.y = 0.f;
        }
        if (pozisyon.x > pencereBoyutu.x - 100.f) {
            pozisyon.x = pencereBoyutu.x - 100.f;
        }
        if (pozisyon.y > pencereBoyutu.y - 100.f) {
            pozisyon.y = pencereBoyutu.y - 100.f;
        }

        oyuncu.setPosition(pozisyon);


        window.clear();
        window.draw(oyuncu);
        window.display();
    }

    return 0;
}
