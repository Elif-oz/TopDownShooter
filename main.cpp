#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

struct Mermi {
 sf::CircleShape sekil;
 sf::Vector2f yonVeHiz;
};

struct Dusman {
 sf::RectangleShape sekil;
};

int main()
{
    srand(static_cast<unsigned>(time(NULL)));

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Top-Down Shooter");

    sf::Clock dtSaati;

    sf::Clock hasarZamanlayici;

    bool dokunulmazMi = false;

    int oyuncuCani = 3;
    float oyuncuHizi = 500.f;
    sf::CircleShape oyuncu(50.f, 3);
    oyuncu.setFillColor(sf::Color::Green);
    oyuncu.setPosition(640.f, 360.f);
    sf::FloatRect bounds = oyuncu.getLocalBounds();
    oyuncu.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.f);

    float mermiHizi = 800.f;
    std::vector<Mermi> mermiler;
    sf::Clock atesZamanlayici;

    float dusmanHizi = 250.f;
    std::vector<Dusman> dusmanlar;
    sf::Clock dusmanZamanlayici;


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



        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && atesZamanlayici.getElapsedTime().asSeconds() > 0.5f) {

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


        if (dusmanZamanlayici.getElapsedTime().asSeconds() > 1.0f) {
            Dusman yeniDusman;
            yeniDusman.sekil.setSize(sf::Vector2f(40.f, 40.f));
            yeniDusman.sekil.setFillColor(sf::Color::Red);
            yeniDusman.sekil.setOrigin(20.f, 20.f);

            int kenar = rand() % 4;
            float x = 0.f, y = 0.f;

            if (kenar == 0)      { x = rand() % 1280; y = -50.f; }  // Üstten
            else if (kenar == 1) { x = rand() % 1280; y = 770.f; }  // Alttan
            else if (kenar == 2) { x = -50.f;       y = rand() % 720; } // Soldan
            else if (kenar == 3) { x = 1330.f;      y = rand() % 720; } // Sağdan

            yeniDusman.sekil.setPosition(x, y);
            dusmanlar.push_back(yeniDusman); // Orduya (listeye) ekle
            dusmanZamanlayici.restart();
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


        for (size_t i = 0; i < dusmanlar.size(); i++) {

            sf::Vector2f oyuncuPoz = oyuncu.getPosition();
            sf::Vector2f dusmanPoz = dusmanlar[i].sekil.getPosition();

            float dx = oyuncuPoz.x - dusmanPoz.x;
            float dy = oyuncuPoz.y - dusmanPoz.y;

            float uzaklik = std::sqrt(dx * dx + dy * dy);

            if (uzaklik != 0) {
                float gitX = (dx / uzaklik) * dusmanHizi * dt;
                float gitY = (dy / uzaklik) * dusmanHizi * dt;
                dusmanlar[i].sekil.move(gitX, gitY);

                if (dusmanlar[i].sekil.getGlobalBounds().intersects(oyuncu.getGlobalBounds())) {

                 if (!dokunulmazMi) {

                    oyuncuCani--;
                    std::cout << "Vuruldun! Kalan Can: " << oyuncuCani << std::endl;
                    dokunulmazMi = true;
                    hasarZamanlayici.restart();

                    if (oyuncuCani <= 0) {
                        std::cout << "Game Over!" << std::endl;
                        window.close();
                    }
                 }
               }
            }
        }


        //Mermi ile dusman carpismasi
        for (size_t i = 0; i < mermiler.size(); i++) {

            for (size_t j = 0; j < dusmanlar.size(); j++) {

                if (mermiler[i].sekil.getGlobalBounds().intersects(dusmanlar[j].sekil.getGlobalBounds())) {

                    dusmanlar.erase(dusmanlar.begin() + j);
                    mermiler.erase(mermiler.begin() + i);

                    break;
                }
            }
        }


        float gecenHasarSuresi = hasarZamanlayici.getElapsedTime().asSeconds();
        bool karakterGozukecekMi = true;

        if (dokunulmazMi) {
          if(gecenHasarSuresi > 2.0f)
          {
              dokunulmazMi = false;
          }
           else
           {
              if (((int)(gecenHasarSuresi * 10.f)) % 2 != 0) {
                karakterGozukecekMi = false;
            }
           }


        }

        window.clear();
        if (karakterGozukecekMi) {
            window.draw(oyuncu);
        }

        for (size_t i = 0; i < mermiler.size(); i++) {
            window.draw(mermiler[i].sekil);
        }

        for (size_t i = 0; i < dusmanlar.size(); i++) {
            window.draw(dusmanlar[i].sekil);
        }

        window.display();
    }

    return 0;
}
