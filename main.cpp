#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

struct Mermi {
sf::CircleShape sekil;
sf::Vector2f yonVeHiz;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Top-Down Shooter");

    std::vector<Mermi> mermiler;
    sf::Clock atesZamanlayici;

    sf::Clock dtSaati;
    float oyuncuHizi = 500.f;

    float mermiHizi = 800.f;

    sf::CircleShape oyuncu(50.f, 3);
    oyuncu.setFillColor(sf::Color::Green);
    oyuncu.setPosition(640.f, 360.f);
    sf::FloatRect bounds = oyuncu.getLocalBounds();
    oyuncu.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.f);

    while (window.isOpen())
    {
        float dt = dtSaati.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            oyuncu.move(0.f, -oyuncuHizi * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            oyuncu.move(0.f, oyuncuHizi * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            oyuncu.move(-oyuncuHizi * dt, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            oyuncu.move(oyuncuHizi * dt, 0.f);
        }



        sf::FloatRect sinirlar = oyuncu.getGlobalBounds();
        sf::Vector2f pozisyon = oyuncu.getPosition();

        if (sinirlar.left < 0.f) {
            pozisyon.x -= sinirlar.left; // Ne kadar eksiye dustuyse, o kadar saga it
        }

        if (sinirlar.top < 0.f) {
            pozisyon.y -= sinirlar.top;
        }
        // Sag Duvar (Sol kenar + Genislik = Sag kenar)
        if (sinirlar.left + sinirlar.width > 1280.f) {
            pozisyon.x -= (sinirlar.left + sinirlar.width - 1280.f); // Taþtýðý miktar kadar sola it
        }
        // Alt Duvar (Ust kenar + Yukseklik = Alt kenar)
        if (sinirlar.top + sinirlar.height > 720.f) {
            pozisyon.y -= (sinirlar.top + sinirlar.height - 720.f); // Taþtýðý miktar kadar yukarý it
        }

        oyuncu.setPosition(pozisyon);



        sf::Vector2i fareFiziksel = sf::Mouse::getPosition(window);
        sf::Vector2f farePozisyonu = window.mapPixelToCoords(fareFiziksel);

        //Oyuncu ile fare arasindaki uzaklik
        float dx = farePozisyonu.x - pozisyon.x;
        float dy = farePozisyonu.y - pozisyon.y;

        // Radyan cinsinden bulunup dereceye cevirme
        float aci = std::atan2(dy, dx) * 180.f / 3.14159265f;


        //SFML'de ucgenin ucu yukari baktigi icin +90 derece
        oyuncu.setRotation(aci + 90.f);



        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && atesZamanlayici.getElapsedTime().asSeconds() > 0.2f) {

            Mermi yeniMermi;
            yeniMermi.sekil.setRadius(5.f);
            yeniMermi.sekil.setFillColor(sf::Color::Yellow);
            yeniMermi.sekil.setOrigin(5.f, 5.f);
            yeniMermi.sekil.setPosition(oyuncu.getPosition());


            float uzaklik = std::sqrt(dx * dx + dy * dy);
            if (uzaklik != 0) {
                yeniMermi.yonVeHiz = sf::Vector2f((dx / uzaklik) * mermiHizi, (dy / uzaklik) * mermiHizi);
            }

            mermiler.push_back(yeniMermi);
            atesZamanlayici.restart();
        }

        for (size_t i = 0; i < mermiler.size(); i++) {
            mermiler[i].sekil.move(mermiler[i].yonVeHiz * dt);

            sf::Vector2f mermiPoz = mermiler[i].sekil.getPosition();

            if (mermiPoz.x < 0.f || mermiPoz.x > 1280.f || mermiPoz.y < 0.f || mermiPoz.y > 720.f) {

                mermiler.erase(mermiler.begin() + i);

                //Liste sola kaydiği icin indeksi geri alma
                i--;
            }
        }

        window.clear();
        window.draw(oyuncu);

        for (size_t i = 0; i < mermiler.size(); i++) {
            window.draw(mermiler[i].sekil);
        }
        window.display();
    }

    return 0;
}
