#include <SFML/Graphics.hpp>
#include <cmath>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Top-Down Shooter");

    sf::CircleShape oyuncu(50.f, 3);
    oyuncu.setFillColor(sf::Color::Green);
    oyuncu.setPosition(640.f, 360.f);
    sf::FloatRect bounds = oyuncu.getLocalBounds();
    oyuncu.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float hiz = 0.2f;

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



        sf::FloatRect sinirlar = oyuncu.getGlobalBounds();
        sf::Vector2f pozisyon = oyuncu.getPosition();

        if (sinirlar.left < 0.f) {
            pozisyon.x -= sinirlar.left; // Ne kadar eksiye düþtüyse, o kadar saða it
        }

        if (sinirlar.top < 0.f) {
            pozisyon.y -= sinirlar.top;
        }
        // Sað Duvar (Sol kenar + Geniþlik = Sað kenar)
        if (sinirlar.left + sinirlar.width > 1280.f) {
            pozisyon.x -= (sinirlar.left + sinirlar.width - 1280.f); // Taþtýðý miktar kadar sola it
        }
        // Alt Duvar (Üst kenar + Yükseklik = Alt kenar)
        if (sinirlar.top + sinirlar.height > 720.f) {
            pozisyon.y -= (sinirlar.top + sinirlar.height - 720.f); // Taþtýðý miktar kadar yukarý it
        }

        oyuncu.setPosition(pozisyon);



        sf::Vector2i farePozisyonuInt = sf::Mouse::getPosition(window);
        sf::Vector2f farePozisyonu(farePozisyonuInt.x, farePozisyonuInt.y); // Int'ten Float'a çevirme

        //Oyuncu ile fare arasýndaki uzaklýk
        float dx = farePozisyonu.x - pozisyon.x;
        float dy = farePozisyonu.y - pozisyon.y;

        // Radyan cinsinden bulunup dereceye çevirme
        float aci = std::atan2(dy, dx) * 180.f / 3.14159265f;


        //SFML'de üçgenin ucu varsayýlan olarak yukarý baktýðý için +90 derece
        oyuncu.setRotation(aci + 90.f);

        window.clear();
        window.draw(oyuncu);
        window.display();
    }

    return 0;
}
