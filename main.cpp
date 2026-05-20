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

enum class GameState {
    MENU,
    PLAYING,
    GAMEOVER
};

int main()
{
    srand(static_cast<unsigned>(time(NULL)));
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Top-Down Shooter");
    window.setFramerateLimit(60);
    sf::Clock dtClock;

    //Yuklemeler
    sf::Font font1;
        if (!font1.loadFromFile("pixel1.ttf")) std::cout << "ERROR: pixel1.ttf\n";

    sf::Font font2;
        if (!font2.loadFromFile("pixel.ttf")) std::cout << "ERROR: pixel.ttf\n";

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

    // Menu basliği
    sf::Text titleText;
    titleText.setFont(font2);
    titleText.setString("Top-Down Shooter");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
    titleText.setPosition(1280.f / 2.0f, 200.f);

    // Menu basla butonu
    sf::RectangleShape startBtn(sf::Vector2f(200.f, 60.f));
    startBtn.setFillColor(sf::Color::Blue);
    sf::FloatRect btnBounds = startBtn.getLocalBounds();
    startBtn.setOrigin(btnBounds.width / 2.0f, btnBounds.height / 2.0f);
    startBtn.setPosition(1280.f / 2.0f, 350.f);

    sf::Text startText; //basla butonundaki yazi
    startText.setFont(font2);
    startText.setString("Start");
    startText.setCharacterSize(30);
    sf::FloatRect startTextBounds = startText.getLocalBounds();
    startText.setOrigin(startTextBounds.left + startTextBounds.width / 2.0f, startTextBounds.top + startTextBounds.height / 2.0f);
    startText.setPosition(1280.f / 2.0f, 350.f);

    // Game Over yazisi
    sf::Text gameOverText;
    gameOverText.setFont(font2);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect goBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(goBounds.left + goBounds.width / 2.0f, goBounds.top + goBounds.height / 2.0f);
    gameOverText.setPosition(1280.f / 2.0f, 230.f);

    // Tekrar oyna butonu
    sf::RectangleShape restartBtn(sf::Vector2f(250.f, 60.f));
    restartBtn.setFillColor(sf::Color::Blue);
    sf::FloatRect rBtnBounds = restartBtn.getLocalBounds();
    restartBtn.setOrigin(rBtnBounds.width / 2.0f, rBtnBounds.height / 2.0f);
    restartBtn.setPosition(1280.f / 2.0f, 450.f);

    sf::Text restartText; //tekrar oyna yazisi
    restartText.setFont(font2);
    restartText.setString("Play Again");
    restartText.setCharacterSize(25);
    sf::FloatRect rTextBounds = restartText.getLocalBounds();
    restartText.setOrigin(rTextBounds.left + rTextBounds.width / 2.0f, rTextBounds.top + rTextBounds.height / 2.0f);
    restartText.setPosition(1280.f / 2.0f, 450.f);

    // Oyunun baslangic durumu
    GameState currentState = GameState::MENU;
    bool mouseWasPressed = false; // Tıklama bug'larını önlemek için

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

     // Buton etkilesimleri icin fare pozisyonunu alma
     sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
     sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosWindow);
     bool isMouseClicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);

     switch (currentState) {
      case GameState::MENU:
            // Basla butonunun üzerine gelince
            if (startBtn.getGlobalBounds().contains(mousePosWorld)) {
                startBtn.setFillColor(sf::Color::Cyan);
                // Tıklanırsa oyuna geç
                if (isMouseClicked && !mouseWasPressed) {
                    currentState = GameState::PLAYING;
                }
            } else {
                startBtn.setFillColor(sf::Color::Blue);
            }
            break;

      case GameState::PLAYING: {

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

        //Mermi guncelleme
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
        break;
     }

     case GameState::GAMEOVER: {
            // Tekrar oyna butonunun uzerine gelince
            if (restartBtn.getGlobalBounds().contains(mousePosWorld)) {
                restartBtn.setFillColor(sf::Color::Cyan);
                // Tıklanırsa oyunu sıfırlama ve baslatma
                if (isMouseClicked && !mouseWasPressed) {
                    bullets.clear();
                    enemies.clear();
                    myPlayer.reset(sf::Vector2f(640.f, 360.f));
                    currentState = GameState::PLAYING;
                }
            } else {
                restartBtn.setFillColor(sf::Color::Blue);
            }
            break;
      }
    }

        mouseWasPressed = isMouseClicked;

        window.clear();

        switch (currentState)
        {
            case GameState::MENU:
                window.draw(titleText);
                window.draw(startBtn);
                window.draw(startText);
                break;

            case GameState::PLAYING:
                myPlayer.draw(window);

                for (size_t i = 0; i < bullets.size(); i++) bullets[i].draw(window);
                for (size_t i = 0; i < enemies.size(); i++) enemies[i].draw(window);

                window.draw(hpText);
                window.draw(scoreText);
                break;

            case GameState::GAMEOVER:
                window.draw(gameOverText);
                window.draw(restartBtn);
                window.draw(restartText);

                sf::FloatRect scoreBounds = scoreText.getLocalBounds();
                scoreText.setOrigin(scoreBounds.left + scoreBounds.width / 2.0f, scoreBounds.top + scoreBounds.height / 2.0f);
                scoreText.setPosition(1280.f / 2.0f, 340.f);
                window.draw(scoreText);

                scoreText.setOrigin(0.f, 0.f);
                scoreText.setPosition(20.f, 60.f);
                break;
        }

       window.display();
    }

    return 0;
}
