#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"

int main()
{
    srand(static_cast<unsigned>(time(NULL)));
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Top-Down Shooter");
    sf::Clock dtClock;

    //Yuklemeler
    sf::Font font1;
        if (!font1.loadFromFile("pixel1.ttf")) std::cout << "ERROR: pixel1.ttf\n";

    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("bullet.png")) std::cout << "ERROR: bullet.png\n";

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("player.png")) std::cout << "ERROR: player.png\n";

    sf::Texture weaponTexture;
    if (!weaponTexture.loadFromFile("weapon.png")) std::cout << "ERROR: weapon.png\n";

    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("enemy.png")) std::cout << "ERROR: enemy.png\n";


    sf::SoundBuffer shootBuffer;
    if (!shootBuffer.loadFromFile("Shoot.wav")) std::cout << "ERROR: Shoot.wav\n";
    sf::Sound shootSound;
    shootSound.setBuffer(shootBuffer);

    //Objeler
    Player myPlayer(playerTexture, weaponTexture, sf::Vector2f(640.f, 360.f));

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    sf::Clock enemySpawnTimer;

     //UI ayarlari
    sf::Text hpText;
    hpText.setFont(font1);
    hpText.setCharacterSize(30);
    hpText.setFillColor(sf::Color::White);
    hpText.setStyle(sf::Text::Bold);
    hpText.setPosition(20.f, 20.f);

    sf::Text scoreText;
    scoreText.setFont(font1);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(20.f, 60.f);

    //Oyun dongusu
    while (window.isOpen())
    {
        float dt = dtClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        myPlayer.update(dt, window); // Oyuncuyu ve silahi guncelleme

        myPlayer.shoot(bullets, bulletTexture, window, shootSound); // Ates etme kontrolu

        // Dusman Spawner
        if (enemySpawnTimer.getElapsedTime().asSeconds() > 1.0f) {

            int spawnEdge = rand() % 4;
            float x = 0.f, y = 0.f;

            if (spawnEdge == 0)      { x = rand() % 1280; y = -50.f; }  // Üstten
            else if (spawnEdge == 1) { x = rand() % 1280; y = 770.f; }  // Alttan
            else if (spawnEdge == 2) { x = -50.f;       y = rand() % 720; } // Soldan
            else if (spawnEdge == 3) { x = 1330.f;      y = rand() % 720; } // Sağdan

            sf::Vector2f spawnPos(x, y);

            Enemy newEnemy(enemyTexture, spawnPos);

            enemies.push_back(newEnemy); // Orduya (listeye) ekle
            enemySpawnTimer.restart();
        }


        for (size_t i = 0; i < bullets.size(); i++) {
            bullets[i].update(dt);

            sf::Vector2f bulletPos = bullets[i].getPosition();

            if (bulletPos.x < 0.f || bulletPos.x > 1280.f || bulletPos.y < 0.f || bulletPos.y > 720.f) {

                bullets.erase(bullets.begin() + i);

                //Liste sola kaydiği icin indeksi geri alma
                i--;
            }
        }

        // Dusman hareketi ve oyuncuya carpmasi
        sf::Vector2f targetPlayerPos = myPlayer.getPosition();

        for (size_t i = 0; i < enemies.size(); i++) {

            enemies[i].update(dt, targetPlayerPos);

                if (enemies[i].getBounds().intersects(myPlayer.getBounds())) {

                 if (!myPlayer.checkInvincible()) {

                    myPlayer.takeDamage();

                    if (myPlayer.getHp() <= 0) {
                        std::cout << "Game Over!" << std::endl;
                        window.close();
                    }
                 }

                 enemies.erase(enemies.begin() + i);
                 i--;

               }
            }



        // Mermi-Dusman carpismasi
        for (size_t i = 0; i < bullets.size(); i++) {
            bool bulletDestroyed = false;

            for (size_t j = 0; j < enemies.size(); j++) {

                if (bullets[i].getBounds().intersects(enemies[j].getBounds())) {

                    enemies.erase(enemies.begin() + j);
                    bullets.erase(bullets.begin() + i);
                    myPlayer.addScore(10);
                    std::cout << "Enemy destroyed! Score: " << myPlayer.getScore() << std::endl;
                    bulletDestroyed = true;

                    break;
                }
            }
            if (bulletDestroyed) i--;
        }

        // Arayuzu guncelleme
        hpText.setString("HP:"+ std::to_string(myPlayer.getHp()));

        scoreText.setString("SCORE:" + std::to_string(myPlayer.getScore()));

        window.clear();

        myPlayer.draw(window);

        for (size_t i = 0; i < bullets.size(); i++) {
            bullets[i].draw(window);
        }

        for (size_t i = 0; i < enemies.size(); i++) {
            enemies[i].draw(window);
        }

        window.draw(hpText);
        window.draw(scoreText);

        window.display();
    }

    return 0;
}
