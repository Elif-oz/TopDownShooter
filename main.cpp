#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <SFML/Audio.hpp>

struct Bullet {
 sf::CircleShape bulletShape;
 sf::Vector2f velocity;
};

struct Enemy {
 sf::RectangleShape enemyShape;
};

int main()
{
    srand(static_cast<unsigned>(time(NULL)));

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Top-Down Shooter");

    sf::Clock dtClock;

    sf::Clock damageTimer;

    bool isInvincible = false;

    sf::Font font1;
        if (!font1.loadFromFile("pixel1.ttf")) {
            std::cout << "ERROR: Failed to load font 'pixel1.ttf'!" << std::endl;
        }

    sf::Text hpText;
    hpText.setFont(font1);
    hpText.setCharacterSize(30);
    hpText.setFillColor(sf::Color::White);
    hpText.setStyle(sf::Text::Bold);
    hpText.setPosition(20.f, 20.f);

    int playerScore = 0;
    sf::Text scoreText;
    scoreText.setFont(font1);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(20.f, 60.f);

    sf::SoundBuffer shootBuffer;
        if (!shootBuffer.loadFromFile("Shoot.wav")) {
            std::cout << "ERROR: Failed to load audio 'Shoot.wav'!" << std::endl;
        }
        sf::Sound shootSound;
        shootSound.setBuffer(shootBuffer);

    int playerHp = 3;
    float playerSpeed = 500.f;
    sf::CircleShape player(50.f, 3);
    player.setFillColor(sf::Color::Green);
    player.setPosition(640.f, 360.f);
    sf::FloatRect bounds = player.getLocalBounds();
    player.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.f);

    float bulletSpeed = 800.f;
    std::vector<Bullet> bullets;
    sf::Clock shootTimer;

    float enemySpeed = 250.f;
    std::vector<Enemy> enemies;
    sf::Clock enemySpawnTimer;


    while (window.isOpen())
    {
        float dt = dtClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            player.move(0.f, -playerSpeed * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            player.move(0.f, playerSpeed * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player.move(-playerSpeed * dt, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.move(playerSpeed * dt, 0.f);
        }



        sf::FloatRect bounds = player.getGlobalBounds();
        sf::Vector2f playerPos = player.getPosition();

        if (bounds.left < 0.f) {
            playerPos.x -= bounds.left; // Ne kadar eksiye dustuyse, o kadar saga it
        }

        if (bounds.top < 0.f) {
            playerPos.y -= bounds.top;
        }
        // Sag Duvar (Sol kenar + Genislik = Sag kenar)
        if (bounds.left + bounds.width > 1280.f) {
            playerPos.x -= (bounds.left + bounds.width - 1280.f); // Taþtýðý miktar kadar sola it
        }
        // Alt Duvar (Ust kenar + Yukseklik = Alt kenar)
        if (bounds.top + bounds.height > 720.f) {
            playerPos.y -= (bounds.top + bounds.height - 720.f); // Taþtýðý miktar kadar yukarý it
        }

        player.setPosition(playerPos);



        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosWindow);

        //Distance between player and mouse
        float dx = mousePosWorld.x - playerPos.x;
        float dy = mousePosWorld.y - playerPos.y;

        // Radyan cinsinden bulunup dereceye cevirme
        float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;


        //SFML'de ucgenin ucu yukari baktigi icin +90 derece, player eklenince degisilecek!
        player.setRotation(angle + 90.f);



        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootTimer.getElapsedTime().asSeconds() > 0.5f) {

            Bullet newBullet;
            newBullet.bulletShape.setRadius(5.f);
            newBullet.bulletShape.setFillColor(sf::Color::Yellow);
            newBullet.bulletShape.setOrigin(5.f, 5.f);
            newBullet.bulletShape.setPosition(player.getPosition());

            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance != 0) {
                newBullet.velocity = sf::Vector2f((dx / distance) * bulletSpeed, (dy / distance) * bulletSpeed);
            }

            bullets.push_back(newBullet);
            shootTimer.restart();
            shootSound.play();
        }


        if (enemySpawnTimer.getElapsedTime().asSeconds() > 1.0f) {
            Enemy newEnemy;
            newEnemy.enemyShape.setSize(sf::Vector2f(40.f, 40.f));
            newEnemy.enemyShape.setFillColor(sf::Color::Red);
            newEnemy.enemyShape.setOrigin(20.f, 20.f);

            int spawnEdge = rand() % 4;
            float x = 0.f, y = 0.f;

            if (spawnEdge == 0)      { x = rand() % 1280; y = -50.f; }  // Üstten
            else if (spawnEdge == 1) { x = rand() % 1280; y = 770.f; }  // Alttan
            else if (spawnEdge == 2) { x = -50.f;       y = rand() % 720; } // Soldan
            else if (spawnEdge == 3) { x = 1330.f;      y = rand() % 720; } // Sağdan

            newEnemy.enemyShape.setPosition(x, y);
            enemies.push_back(newEnemy); // Orduya (listeye) ekle
            enemySpawnTimer.restart();
        }


        for (size_t i = 0; i < bullets.size(); i++) {
            bullets[i].bulletShape.move(bullets[i].velocity * dt);

            sf::Vector2f bulletPos = bullets[i].bulletShape.getPosition();

            if (bulletPos.x < 0.f || bulletPos.x > 1280.f || bulletPos.y < 0.f || bulletPos.y > 720.f) {

                bullets.erase(bullets.begin() + i);

                //Liste sola kaydiği icin indeksi geri alma
                i--;
            }
        }


        sf::Vector2f targetPlayerPos = player.getPosition();

        for (size_t i = 0; i < enemies.size(); i++) {

            sf::Vector2f currentEnemyPos = enemies[i].enemyShape.getPosition();

            float dx = targetPlayerPos.x - currentEnemyPos.x;
            float dy = targetPlayerPos.y - currentEnemyPos.y;

            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance != 0) {
                float moveX = (dx / distance) * enemySpeed * dt;
                float moveY = (dy / distance) * enemySpeed * dt;
                enemies[i].enemyShape.move(moveX, moveY);

                if (enemies[i].enemyShape.getGlobalBounds().intersects(player.getGlobalBounds())) {

                 if (!isInvincible) {

                    playerHp--;
                    std::cout << "Player hit! Remaining HP: " << playerHp << std::endl;
                    isInvincible = true;
                    damageTimer.restart();

                    if (playerHp <= 0) {
                        std::cout << "Game Over!" << std::endl;
                        window.close();
                    }
                 }

                 enemies.erase(enemies.begin() + i);
                 i--;

               }
            }
        }


        //Mermi ile Enemy carpismasi
        for (size_t i = 0; i < bullets.size(); i++) {

            for (size_t j = 0; j < enemies.size(); j++) {

                if (bullets[i].bulletShape.getGlobalBounds().intersects(enemies[j].enemyShape.getGlobalBounds())) {     //neden i ve j yi bir geri almadık??

                    enemies.erase(enemies.begin() + j);
                    bullets.erase(bullets.begin() + i);
                    playerScore += 10;
                    std::cout << "Enemy destroyed! Score: " << playerScore << std::endl;

                    break;
                }
            }
        }


        float timeSinceHit = damageTimer.getElapsedTime().asSeconds();
        bool isPlayerVisible = true;

        if (isInvincible) {
          if(timeSinceHit > 2.0f)
          {
              isInvincible = false;
          }
           else
           {
              if (((int)(timeSinceHit * 10.f)) % 2 != 0) {
                isPlayerVisible = false;
            }
           }


        }

        hpText.setString("HP:"+ std::to_string(playerHp));

        scoreText.setString("SCORE:" + std::to_string(playerScore));

        window.clear();
        if (isPlayerVisible) {
            window.draw(player);
        }

        for (size_t i = 0; i < bullets.size(); i++) {
            window.draw(bullets[i].bulletShape);
        }

        for (size_t i = 0; i < enemies.size(); i++) {
            window.draw(enemies[i].enemyShape);
        }

        window.draw(hpText);
        window.draw(scoreText);

        window.display();
    }

    return 0;
}
