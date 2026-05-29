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
#include "WaveManager.h"
#include "Loot.h"

enum class GameState {
    MENU,
    PLAYING,
    GAMEOVER
};

int main()
{
    srand(static_cast<unsigned>(time(NULL)));
    sf::RenderWindow window(sf::VideoMode(1280, 768), "Top-Down Shooter");
    window.setFramerateLimit(60);
    sf::Clock dtClock;

    //Yuklemeler
    sf::Font font1;
        if (!font1.loadFromFile("assets/fonts/pixel1.ttf")) std::cout << "ERROR: pixel1.ttf\n";

    sf::Font font2;
        if (!font2.loadFromFile("assets/fonts/pixel.ttf")) std::cout << "ERROR: pixel.ttf\n";

    sf::SoundBuffer shootBuffer;
    if (!shootBuffer.loadFromFile("assets/sounds/Shoot.wav")) std::cout << "ERROR: Shoot.wav\n";
    sf::Sound shootSound;
    shootSound.setBuffer(shootBuffer);

    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/images/bullet.png")) std::cout << "ERROR: bullet.png\n";
    bulletTexture.setSmooth(false);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/images/player.png")) std::cout << "ERROR: player.png\n";
    playerTexture.setSmooth(false);

    sf::Texture weaponTexture;
    if (!weaponTexture.loadFromFile("assets/images/weapon.png")) std::cout << "ERROR: weapon.png\n";
    weaponTexture.setSmooth(false);

    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("assets/images/enemy.png")) std::cout << "ERROR: enemy.png\n";
    enemyTexture.setSmooth(false);

    sf::Texture heartTexture;
    if (!heartTexture.loadFromFile("assets/images/heart.png")) std::cout << "ERROR: heart.png\n";
    heartTexture.setSmooth(false);

    sf::Texture coinTexture;
    if (!coinTexture.loadFromFile("assets/images/coin.png")) std::cout << "ERROR: coin.png\n";
    coinTexture.setSmooth(false);

    sf::Texture bootsTexture;
    if (!bootsTexture.loadFromFile("assets/images/boots.png")) std::cout << "ERROR: boots.png\n";
    bootsTexture.setSmooth(false);

    sf::Texture fireTexture;
    if (!fireTexture.loadFromFile("assets/images/fire.png")) std::cout << "ERROR: fire.png\n";
    fireTexture.setSmooth(false);

    sf::Texture bombTexture;
    if (!bombTexture.loadFromFile("assets/images/bomb.png")) std::cout << "ERROR: bomb.png\n";
    bombTexture.setSmooth(false);

    sf::Texture hpUI_Texture;
    if (!hpUI_Texture.loadFromFile("assets/images/hp_bar.png")) std::cout << "ERROR: hp_bar.png\n";
    hpUI_Texture.setSmooth(false);

    sf::Sprite hpUISprite(hpUI_Texture); //HP gorseli icin
    hpUISprite.setPosition(20.f, 20.f);
    hpUISprite.setScale(2.f, 2.f);

    sf::Texture tileTextures[11];
    std::string fileNames[11] = {
        "assets/images/tiles/PNG0000.PNG", "assets/images/tiles/PNG0001.PNG",
        "assets/images/tiles/PNG0002.PNG", "assets/images/tiles/PNG0003.PNG",
        "assets/images/tiles/PNG0004.PNG", "assets/images/tiles/PNG0005.PNG",
        "assets/images/tiles/PNG0006.PNG", "assets/images/tiles/PNG0007.PNG",
        "assets/images/tiles/PNG0008.PNG", "assets/images/tiles/PNG0009.PNG",
        "assets/images/tiles/PNG0010.PNG"
    };

    for(int i = 0; i < 11; i++) {
        if (!tileTextures[i].loadFromFile(fileNames[i])) std::cout << "ERROR: " << fileNames[i] << "\n";
        tileTextures[i].setSmooth(false);
    }

    sf::Sprite tileSprite;
    tileSprite.setScale(2.f, 2.f);

    const int MAP_ROWS = 12;
    const int MAP_COLS = 20;

    // 0,1,2,3: Kum | 4,5: Kaktus | 6,7: Ot | 8,9: Tas | 10: Iskelet
    int tileMap[MAP_ROWS][MAP_COLS] = {
        {4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5},
        {5, 0, 1, 1, 0, 0, 0, 7, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 1, 1, 0, 6, 0, 0, 0, 2, 0, 1, 1, 0, 0, 8, 0, 0, 0, 5},
        {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 4},
        {4, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
        {5, 0, 0, 8, 0, 0, 0, 1, 1, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 5},
        {5, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 2, 0, 5},
        {5, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 5},
        {5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4}
    };

    //Objeler
    Player myPlayer(playerTexture, weaponTexture, sf::Vector2f(640.f, 384.f));

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<Loot> loots;

     //UI ayarlari

    int previousHp = myPlayer.getHp(); //HP animasyonu icin
    bool isHpAnimating = false;
    sf::Clock hpAnimTimer;
    int hpAnimFrame = 0;

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
    scoreText.setPosition(20.f, 100.f);

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

    WaveManager waveManager;   //Wave yazilari
    sf::Text waveUI;
    waveUI.setFont(font1);
    waveUI.setCharacterSize(30);
    waveUI.setFillColor(sf::Color::Cyan);
    waveUI.setStyle(sf::Text::Bold);
    waveUI.setPosition(20.f, 140.f);

    sf::Text waveCenterText;
    waveCenterText.setFont(font1);
    waveCenterText.setCharacterSize(50);
    waveCenterText.setFillColor(sf::Color::White);

    // Oyunun baslangic durumu
    GameState currentState = GameState::MENU;
    bool mouseWasPressed = false;

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

                if (isMouseClicked && !mouseWasPressed) {
                    currentState = GameState::PLAYING;
                }
            } else {
                startBtn.setFillColor(sf::Color::Blue);
            }
            break;

      case GameState::PLAYING: {

        myPlayer.update(dt, window); // Oyuncuyu ve silahi guncelleme
        waveManager.update(enemies, enemyTexture);

        waveUI.setString("WAVE: " + std::to_string(waveManager.getCurrentWave()));

        if (waveManager.isInBreak()) {
            waveCenterText.setString(waveManager.getBreakText());
            sf::FloatRect bounds = waveCenterText.getLocalBounds();
            waveCenterText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            waveCenterText.setPosition(1280.f / 2.f, 768.f / 2.f);
        }

        sf::FloatRect pBounds = myPlayer.getBounds();

        float shrinkX = 10.f; //Hitbox daraltma
        pBounds.left += shrinkX;
        pBounds.width -= (shrinkX * 2);

        pBounds.top += 30.f;
        pBounds.height -= 30.f;

        bool hitWall = false;

        // Haritadaki tüm kareleri tarama
        for (int i = 0; i < MAP_ROWS; i++) {
            for (int j = 0; j < MAP_COLS; j++) {
                int tileID = tileMap[i][j];

                // 4-5 : Kaktus
                if (tileID == 4 || tileID == 5 ) {

                    sf::FloatRect tileBounds(j * 64.f, i * 64.f, 64.f, 64.f);

                    if (pBounds.intersects(tileBounds)) {
                        hitWall = true;
                        break;
                    }
                }
            }
            if (hitWall) break;
        }

        if (hitWall) {
            myPlayer.revertPosition();
        }

        myPlayer.shoot(bullets, bulletTexture, window, shootSound); // Ates etme kontrolu

        //Mermi guncelleme
        for (size_t i = 0; i < bullets.size(); i++) {
            bullets[i].update(dt);

            sf::Vector2f bulletPos = bullets[i].getPosition();

            if (bulletPos.x < 0.f || bulletPos.x > 1280.f || bulletPos.y < 0.f || bulletPos.y > 768.f) {

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
                        currentState = GameState::GAMEOVER;
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

                    int dropChance = rand() % 100;
                    if (dropChance < 10) {                          //%10 can
                        loots.push_back(Loot(heartTexture, enemies[j].getPosition(), LootType::HEALTH));
                    }
                    else if (dropChance >= 10 && dropChance < 30) { //%30 skor
                        loots.push_back(Loot(coinTexture, enemies[j].getPosition(), LootType::SCORE));
                    }
                    else if (dropChance >= 30 && dropChance < 35) { //%5 hiz
                        loots.push_back(Loot(bootsTexture, enemies[j].getPosition(), LootType::SPEED_BOOST));
                    }
                    else if (dropChance >= 35 && dropChance < 40) {//%5 ates hizi
                        loots.push_back(Loot(fireTexture, enemies[j].getPosition(), LootType::FIRE_RATE));
                    }
                    else if (dropChance >= 40 && dropChance < 42) {//%2 bomba
                        loots.push_back(Loot(bombTexture, enemies[j].getPosition(), LootType::BOMB));
                    }

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

        // Oyuncu-Loot carpismasi
        for (size_t i = 0; i < loots.size(); i++) {
            if (myPlayer.getBounds().intersects(loots[i].getBounds())) {

                if (loots[i].getType() == LootType::HEALTH) {
                    myPlayer.heal(1);
                }
                else if (loots[i].getType() == LootType::SCORE) {
                    myPlayer.addScore(50);
                }
                else if (loots[i].getType() == LootType::SPEED_BOOST) {
                    myPlayer.activateSpeedBoost();
                }
                else if (loots[i].getType() == LootType::FIRE_RATE) {
                    myPlayer.activateFireRateBoost();
                }
                else if (loots[i].getType() == LootType::BOMB) {
                    myPlayer.addScore(enemies.size() * 10);
                    enemies.clear();
                }

                loots.erase(loots.begin() + i);
                i--;
            }
        }

        //HP kontrolu ve animasyonu
        int currentHP = myPlayer.getHp();
        int maxHP = myPlayer.getMaxHp();

        if (currentHP < previousHp) {
            isHpAnimating = true;
            hpAnimTimer.restart();
            hpAnimFrame = 1;
        }
        previousHp = currentHP;

        if (isHpAnimating) {
            if (hpAnimTimer.getElapsedTime().asSeconds() > 0.15f) {
                hpAnimFrame++;
                hpAnimTimer.restart();
                if (hpAnimFrame >= 3) {
                    hpAnimFrame = 3;
                    isHpAnimating = false;
                }
            }
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

                if (isMouseClicked && !mouseWasPressed) {
                    bullets.clear();
                    enemies.clear();
                    loots.clear();
                    myPlayer.reset(sf::Vector2f(640.f, 384.f));
                    waveManager.reset();
                    previousHp = myPlayer.getHp();
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

            case GameState::PLAYING: {

                for (int i = 0; i < MAP_ROWS; i++) {
                    for (int j = 0; j < MAP_COLS; j++) {
                        int tileID = tileMap[i][j];

                        if (tileID >= 4) {
                            tileSprite.setTexture(tileTextures[0]);
                            tileSprite.setPosition(j * 64.f, i * 64.f);
                            window.draw(tileSprite);
                        }

                        tileSprite.setTexture(tileTextures[tileID]);
                        tileSprite.setPosition(j * 64.f, i * 64.f);
                        window.draw(tileSprite);
                    }
                }

                myPlayer.draw(window);

                for (size_t i = 0; i < bullets.size(); i++) bullets[i].draw(window);
                for (size_t i = 0; i < enemies.size(); i++) enemies[i].draw(window);
                for (size_t i = 0; i < loots.size(); i++) loots[i].draw(window);

                int currentHP = myPlayer.getHp();
                int maxHP = myPlayer.getMaxHp();

                int frameWidth = 32;
                int frameHeight = 32;
                float spacing = 10.f;

                for (int i = 0; i < maxHP; i++) {

                 float xPos = 20.f + (i * ((frameWidth * 2.f) + spacing));
                 hpUISprite.setPosition(xPos, 20.f);

                 int currentFrameToDraw = 0;
                 if (i < currentHP) {
                   currentFrameToDraw = 0;
                 }
                   else if (i == currentHP && isHpAnimating) {
                     currentFrameToDraw = hpAnimFrame;
                   }
                     else {
                      currentFrameToDraw = 3;
                     }

                 hpUISprite.setTextureRect(sf::IntRect(currentFrameToDraw * frameWidth, 0, frameWidth, frameHeight));
                 window.draw(hpUISprite);
                }

                window.draw(scoreText);
                window.draw(waveUI);
                if (waveManager.isInBreak()) {
                    window.draw(waveCenterText);
                }
                break;
            }

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
