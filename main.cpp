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

    //Oyun icin Yuklemeler
    sf::Font font1;
        if (!font1.loadFromFile("assets/fonts/pixel1.ttf")) std::cout << "ERROR: pixel1.ttf\n";//fontlar

    sf::Font font2;
        if (!font2.loadFromFile("assets/fonts/pixel.ttf")) std::cout << "ERROR: pixel.ttf\n";

    sf::SoundBuffer shootBuffer;
    if (!shootBuffer.loadFromFile("assets/sounds/Shoot.wav")) std::cout << "ERROR: Shoot.wav\n";//sesler
    sf::Sound shootSound;
    shootSound.setBuffer(shootBuffer);

    sf::SoundBuffer coinBuffer;
    if (!coinBuffer.loadFromFile("assets/sounds/coin.wav")) std::cout << "ERROR: coin.wav\n";
    sf::Sound coinSound;
    coinSound.setBuffer(coinBuffer);

    sf::SoundBuffer lifeBuffer;
    if (!lifeBuffer.loadFromFile("assets/sounds/life.wav")) std::cout << "ERROR: life.wav\n";
    sf::Sound lifeSound;
    lifeSound.setBuffer(lifeBuffer);

    sf::SoundBuffer speedBuffer;
    if (!speedBuffer.loadFromFile("assets/sounds/speed.wav")) std::cout << "ERROR: speed.wav\n";
    sf::Sound speedSound;
    speedSound.setBuffer(speedBuffer);

    sf::SoundBuffer fireBuffer;
    if (!fireBuffer.loadFromFile("assets/sounds/fire.wav")) std::cout << "ERROR: fire.wav\n";
    sf::Sound fireSound;
    fireSound.setBuffer(fireBuffer);

    sf::SoundBuffer bombBuffer;
    if (!bombBuffer.loadFromFile("assets/sounds/bomb.wav")) std::cout << "ERROR: bomb.wav\n";
    sf::Sound bombSound;
    bombSound.setBuffer(bombBuffer);

    sf::SoundBuffer hitBuffer;
    if (!hitBuffer.loadFromFile("assets/sounds/hitHurt.wav")) std::cout << "ERROR: hitHurt.wav\n";
    sf::Sound hitSound;
    hitSound.setBuffer(hitBuffer);

    sf::Music menuMusic;
    if (!menuMusic.openFromFile("assets/sounds/menu_music.mp3")) std::cout << "ERROR: menu_music.mp3\n";
    menuMusic.setLoop(true);
    menuMusic.play();

    sf::SoundBuffer gameOverBuffer;
    if (!gameOverBuffer.loadFromFile("assets/sounds/gameover.mp3")) std::cout << "ERROR: gameover.mp3\n";
    sf::Sound gameOverSound;
    gameOverSound.setBuffer(gameOverBuffer);

    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/images/bullet.png")) std::cout << "ERROR: bullet.png\n";
    bulletTexture.setSmooth(false);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/images/player.png")) std::cout << "ERROR: player.png\n";
    playerTexture.setSmooth(false);

    sf::Texture tDamagedHat;
    if (!tDamagedHat.loadFromFile("assets/images/damaged_hat.png")) std::cout << "ERROR: damaged_hat.png\n";
    tDamagedHat.setSmooth(false);

    sf::Texture tDeathAnim;
    if (!tDeathAnim.loadFromFile("assets/images/player_hurt.png")) std::cout << "ERROR: player_hurt\n";
    tDeathAnim.setSmooth(false);

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
    if (!bootsTexture.loadFromFile("assets/images/speed.png")) std::cout << "ERROR: speed.png\n";
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
    titleText.setFillColor(sf::Color::Black);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
    titleText.setPosition(1280.f / 2.0f, 100.f);

    //Menu icin yuklemeler
    sf::Texture tSky, tMountain, tGround, tCloud, tCactus, tTumbleweed, tGrass1, tGrass2;
    sf::Texture tBtnStart, tBtnStartHover, tBtnExit, tBtnExitHover;

    if (!tSky.loadFromFile("assets/images/menu/bg-sky.png")) std::cout << "ERROR: bg-sky.png\n";
    tSky.setSmooth(false);
    if (!tMountain.loadFromFile("assets/images/menu/bg-back.png")) std::cout << "ERROR: bg-back.png\n";
    tMountain.setSmooth(false);
    if (!tGround.loadFromFile("assets/images/menu/bg-front.png")) std::cout << "ERROR: bg-front.png\n";
    tGround.setSmooth(false);
    if (!tBtnStart.loadFromFile("assets/images/menu/start-btn.png")) std::cout << "ERROR: start-btn.png\n";
    tBtnStart.setSmooth(false);
    if (!tBtnStartHover.loadFromFile("assets/images/menu/start-btn-hover.png")) std::cout << "ERROR: start-btn-hover.png\n";
    tBtnStartHover.setSmooth(false);
    if (!tBtnExit.loadFromFile("assets/images/menu/exit-btn.png")) std::cout << "ERROR: exit-btn.png\n";
    tBtnExit.setSmooth(false);
    if (!tBtnExitHover.loadFromFile("assets/images/menu/exit-btn-hover.png")) std::cout << "ERROR: exit-btn-hover.png\n";
    tBtnExitHover.setSmooth(false);
    if (!tCloud.loadFromFile("assets/images/menu/cloud.png")) std::cout << "ERROR: cloud.png\n";
    tCloud.setSmooth(false);
    if (!tCactus.loadFromFile("assets/images/menu/cactus.png")) std::cout << "ERROR: cactus.png\n";
    tCactus.setSmooth(false);
    if (!tTumbleweed.loadFromFile("assets/images/menu/tumbleweed.png")) std::cout << "ERROR: tumbleweed.png\n";
    tTumbleweed.setSmooth(false);
    if (!tGrass1.loadFromFile("assets/images/menu/grass1.png")) std::cout << "ERROR: grass1.png\n";
    tGrass1.setSmooth(false);
    if (!tGrass2.loadFromFile("assets/images/menu/grass2.png")) std::cout << "ERROR: grass2.png\n";
    tGrass2.setSmooth(false);

    //Kedi animasyonu icin
    int catFrameWidth = playerTexture.getSize().x / 4;
    int catFrameHeight = playerTexture.getSize().y;
    int catCurrentFrame = 0;
    float catAnimTimer = 0.f;

    sf::Sprite menuPlayer(playerTexture);
    menuPlayer.setScale(4.f, 4.f);
    menuPlayer.setPosition(150.f, 468.f);

    sf::Sprite sMenuStone(tileTextures[8]);
    sMenuStone.setScale(2.f, 2.f);

    sf::Sprite sSky(tSky), sMountain(tMountain), sGround(tGround), sCactus(tCactus), sTumbleweed(tTumbleweed), sGrass1(tGrass1), sGrass2(tGrass2);
    sf::Sprite sBtnStart(tBtnStart), sBtnStartHover(tBtnStartHover), sBtnExit(tBtnExit), sBtnExitHover(tBtnExitHover);

    sf::FloatRect twBounds = sTumbleweed.getLocalBounds();
    sTumbleweed.setOrigin(twBounds.width / 2.f, twBounds.height / 2.f);

    //Bulutlari kesme
    int cloudHeight = tCloud.getSize().y;

    sf::Sprite sCloud1(tCloud), sCloud2(tCloud), sCloud3(tCloud);

    //sf::IntRect(Baslangic_X, Baslangic_Y, Genislik, Yukseklik)
    sCloud1.setTextureRect(sf::IntRect(0, 0, 80, cloudHeight));
    sCloud2.setTextureRect(sf::IntRect(80, 0, 100, cloudHeight));
    sCloud3.setTextureRect(sf::IntRect(160, 0, 200, cloudHeight));

    sSky.setScale(2.f, 2.f);
    sMountain.setScale(2.f, 2.f);
    sGround.setScale(2.f, 2.f);
    sCactus.setScale(2.f, 2.f);
    sCloud1.setScale(2.f, 2.f);
    sCloud2.setScale(2.f, 2.f);
    sCloud3.setScale(2.f, 2.f);
    sTumbleweed.setScale(2.f, 2.f);
    sGrass1.setScale(2.f, 2.f);
    sGrass2.setScale(2.f, 2.f);

    sBtnStart.setPosition(500.f, 200.f);
    sBtnStartHover.setPosition(494.f, 196.f);

    sBtnExit.setPosition(500.f, 300.f);
    sBtnExitHover.setPosition(496.f, 296.f);

    float mountainX = 0.f;
    float groundX = 0.f;
    float cactusX = 1280.f;

    float cloud1X = 100.f;   float cloud1Y = 20.f; //bulutlarin yeri
    float cloud2X = 550.f;   float cloud2Y = 70.f;
    float cloud3X = 1000.f;  float cloud3Y = 130.f;
    float cloud4X = 1450.f;  float cloud4Y = 30.f;
    float cloud5X = 1900.f;  float cloud5Y = 85.f;
    float cloud6X = 2400.f;  float cloud6Y = 145.f;

    float grass1_1X = 200.f;  float grass1_1Y = 550.f; //otlarin yeri
    float grass1_2X = 900.f;  float grass1_2Y = 640.f;
    float grass2_1X = 550.f;  float grass2_1Y = 675.f;
    float grass2_2X = 1350.f; float grass2_2Y = 680.f;

    float stoneX = 2500.f; //tasin yeri
    float stoneY = 580.f;

    float tumbleweedX = 1280.f + 1000.f;
    float tumbleweedY = 600.f;


    // Game Over yazisi
    sf::Text gameOverText;
    gameOverText.setFont(font2);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect goBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(goBounds.left + goBounds.width / 2.0f, goBounds.top + goBounds.height / 2.0f);
    gameOverText.setPosition(1280.f / 2.0f, 230.f);

    sf::Sprite sDamagedHat(tDamagedHat); //Game over ekrani icin
    sDamagedHat.setScale(4.f, 4.f);
    sf::FloatRect hatBounds = sDamagedHat.getLocalBounds();
    sDamagedHat.setOrigin(hatBounds.width / 2.0f, hatBounds.height / 2.0f);
    sDamagedHat.setPosition(1280.f / 2.0f, 220.f);

    sf::Sprite sDeathAnim(tDeathAnim);
    sDeathAnim.setScale(2.f, 2.f);
    int deathFrameWidth = tDeathAnim.getSize().x / 5;
    int deathFrameHeight = tDeathAnim.getSize().y;
    sDeathAnim.setOrigin(deathFrameWidth / 2.f, deathFrameHeight / 2.f);
    sDeathAnim.setTextureRect(sf::IntRect(0, 0, deathFrameWidth, deathFrameHeight));

    int deathCurrentFrame = 0;
    float deathAnimClock = 0.f;
    bool isDeathAnimFinished = false;

    sf::Texture tEmptyBtn, tEmptyBtnHover; //buton gorsel yuklemesi
    if (!tEmptyBtn.loadFromFile("assets/images/empty_btn.png")) std::cout << "ERROR: empty_btn.png\n";
    if (!tEmptyBtnHover.loadFromFile("assets/images/empty_btn_hover.png")) std::cout << "ERROR: empty_btn_hover.png\n";
    tEmptyBtn.setSmooth(false);
    tEmptyBtnHover.setSmooth(false);

    //tekrar oyna butonu
    sf::Sprite sRestartBg(tEmptyBtn);
    sf::Sprite sRestartBgHover(tEmptyBtnHover);
    sRestartBg.setScale(2.f, 2.f);
    sRestartBgHover.setScale(2.f, 2.f);
    sf::FloatRect btnBounds = sRestartBg.getLocalBounds();
    sf::FloatRect hoverBounds = sRestartBgHover.getLocalBounds();
    sRestartBg.setOrigin(btnBounds.width / 2.f, btnBounds.height / 2.f);
    sRestartBgHover.setOrigin(hoverBounds.width / 2.f, hoverBounds.height / 2.f);

    // Exit butonu
    sf::Sprite sExitBg(tEmptyBtn);
    sf::Sprite sExitBgHover(tEmptyBtnHover);
    sExitBg.setScale(2.f, 2.f);
    sExitBgHover.setScale(2.f, 2.f);
    sExitBg.setOrigin(btnBounds.width / 2.f, btnBounds.height / 2.f);
    sExitBgHover.setOrigin(hoverBounds.width / 2.f, hoverBounds.height / 2.f);

    sf::Text goExitText; //exit yazisi
    goExitText.setFont(font2);
    goExitText.setString("Exit");
    goExitText.setCharacterSize(25);
    sf::FloatRect eTextBounds = goExitText.getLocalBounds();
    goExitText.setOrigin(std::round(eTextBounds.left + eTextBounds.width / 2.0f), std::round(eTextBounds.top + eTextBounds.height / 2.0f));

    sf::Text restartText; //tekrar oyna yazisi
    restartText.setFont(font2);
    restartText.setString("Play Again");
    restartText.setCharacterSize(25);
    sf::FloatRect rTextBounds = restartText.getLocalBounds();
    restartText.setOrigin(std::round(rTextBounds.left + rTextBounds.width / 2.0f), std::round(rTextBounds.top + rTextBounds.height / 2.0f));
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

    sf::Clock gameOverTimer;
    float fadeAlpha = 0.f;
    bool isDeadSequence = false;

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
      case GameState::MENU: {

            catAnimTimer += dt;
            if (catAnimTimer >= 0.1f) {
                catCurrentFrame++;
                if (catCurrentFrame >= 4) {
                    catCurrentFrame = 0;
                }
                catAnimTimer = 0.f;
            }

            menuPlayer.setTextureRect(sf::IntRect(catCurrentFrame * catFrameWidth, 0, catFrameWidth, catFrameHeight));

            float currentCatY = 468.f;

        if (catCurrentFrame == 1 || catCurrentFrame == 3) { //adim attiginda yukarı cikmasi
            currentCatY = 464.f;
        }

        menuPlayer.setPosition(150.f, currentCatY);

            mountainX -= 30.f * dt; //ekrandakilerin hizlari
            groundX -= 120.f * dt;
            cactusX -= 120.f * dt;
            cloud1X -= 14.f * dt;
            cloud2X -= 11.f * dt;
            cloud3X -= 18.f * dt;
            cloud4X -= 15.f * dt;
            cloud5X -= 12.f * dt;
            cloud6X -= 17.f * dt;
            grass1_1X -= 120.f * dt;
            grass1_2X -= 120.f * dt;
            grass2_1X -= 120.f * dt;
            grass2_2X -= 120.f * dt;
            stoneX -= 120.f * dt;
            tumbleweedX -= 200.f * dt;

            // Ekrani gecince basa sarma
            if (mountainX <= -1280.f) mountainX = 0.f; //daglar

            if (groundX <= -1280.f) groundX = 0.f; //zemin

            if (cactusX <= -200.f) { //kaktus
                cactusX = 1280.f + (rand() % 1000);
            }

            if (cloud1X <= -200.f) {  //bulutlar
                cloud1X = 1280.f + (rand() % 300); cloud1Y = 20.f + (rand() % 25);
            }

            if (cloud2X <= -200.f) {
                cloud2X = 1280.f + (rand() % 300); cloud2Y = 60.f + (rand() % 30);
            }

            if (cloud3X <= -200.f) {
                cloud3X = 1280.f + (rand() % 300); cloud3Y = 110.f + (rand() % 40);
            }

            if (cloud4X <= -200.f) {
                cloud4X = 1280.f + (rand() % 400); cloud4Y = 20.f + (rand() % 25);
            }

            if (cloud5X <= -200.f) {
                cloud5X = 1280.f + (rand() % 400); cloud5Y = 60.f + (rand() % 30);
            }

            if (cloud6X <= -200.f) {
                cloud6X = 1280.f + (rand() % 400); cloud6Y = 110.f + (rand() % 40);
            }

            if (grass1_1X <= -100.f) { //otlar
                grass1_1X = 1280.f + (rand() % 400);
                grass1_1Y = 550.f + (rand() % 50);
            }
            if (grass1_2X <= -100.f) {
                grass1_2X = 1280.f + (rand() % 500);
                grass1_2Y = 650.f + (rand() % 50);
            }
            if (grass2_1X <= -100.f) {
                grass2_1X = 1280.f + (rand() % 450);
                grass2_1Y = 590.f + (rand() % 50);
            }
            if (grass2_2X <= -100.f) {
                grass2_2X = 1280.f + (rand() % 600);
                grass2_2Y = 680.f + (rand() % 50);
            }

            if (stoneX <= -100.f) {
                stoneX = 1280.f + 500.f + (rand() % 1000);
                stoneY = 600.f + (rand() % 40);
            }

            sTumbleweed.rotate(-270.f * dt); //tumbleweed
            if (tumbleweedX <= -100.f) {
                tumbleweedX = 1280.f + 1500.f + (rand() % 3000);
                tumbleweedY = 600.f + (rand() % 100);
            }

            // Start buton kontrolu
            if (sBtnStart.getGlobalBounds().contains(mousePosWorld)) {
                if (isMouseClicked && !mouseWasPressed) {
                    menuMusic.stop();
                    currentState = GameState::PLAYING;
                }
            }

            // Exit buton kontrolu
            if (sBtnExit.getGlobalBounds().contains(mousePosWorld)) {
                if (isMouseClicked && !mouseWasPressed) {
                    window.close();
                }
            }

            break;
      }

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

        // Loot guncelleme
        for (size_t i = 0; i < loots.size(); i++) {
            loots[i].update(dt);
        }

        // Dusman hareketi ve oyuncuya carpmasi
        sf::Vector2f targetPlayerPos = myPlayer.getPosition();

        for (size_t i = 0; i < enemies.size(); i++) {

            enemies[i].update(dt, targetPlayerPos);

                if (enemies[i].getBounds().intersects(myPlayer.getBounds())) {

                 if (!myPlayer.checkInvincible()) {

                    myPlayer.takeDamage();
                    hitSound.play();

                    if (myPlayer.getHp() <= 0) {
                        std::cout << "Game Over!" << std::endl;
                        gameOverSound.play();
                        currentState = GameState::GAMEOVER;
                        isDeadSequence = true;
                        gameOverTimer.restart();
                        fadeAlpha = 0.f;
                        isDeathAnimFinished = false;
                        deathCurrentFrame = 0;
                        deathAnimClock = 0.f;
                        sDeathAnim.setPosition(myPlayer.getPosition());

                        if (myPlayer.getScaleX() < 0) {
                            sDeathAnim.setScale(-2.f, 2.f);
                        } else {
                            sDeathAnim.setScale(2.f, 2.f);
                        }
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
                        loots.push_back(Loot(heartTexture, enemies[j].getPosition(), LootType::HEALTH, 9));
                    }
                    else if (dropChance >= 10 && dropChance < 30) { //%20 skor
                        loots.push_back(Loot(coinTexture, enemies[j].getPosition(), LootType::SCORE, 8));
                    }
                    else if (dropChance >= 30 && dropChance < 35) { //%5 hiz
                        loots.push_back(Loot(bootsTexture, enemies[j].getPosition(), LootType::SPEED_BOOST, 8));
                    }
                    else if (dropChance >= 35 && dropChance < 40) {//%5 ates
                        loots.push_back(Loot(fireTexture, enemies[j].getPosition(), LootType::FIRE_RATE, 8));
                    }
                    else if (dropChance >= 40 && dropChance < 42) {//%2 bomba
                        loots.push_back(Loot(bombTexture, enemies[j].getPosition(), LootType::BOMB, 8));
                    }

                    enemies.erase(enemies.begin() + j);
                    bullets.erase(bullets.begin() + i);
                    myPlayer.addScore(10);
                    std::cout << "Enemy destroyed! Score: " << myPlayer.getScore() << std::endl;
                    hitSound.play();
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
                    lifeSound.play();
                }
                else if (loots[i].getType() == LootType::SCORE) {
                    myPlayer.addScore(50);
                    coinSound.play();
                }
                else if (loots[i].getType() == LootType::SPEED_BOOST) {
                    myPlayer.activateSpeedBoost();
                    speedSound.play();
                }
                else if (loots[i].getType() == LootType::FIRE_RATE) {
                    myPlayer.activateFireRateBoost();
                    fireSound.play();
                }
                else if (loots[i].getType() == LootType::BOMB) {
                    myPlayer.addScore(enemies.size() * 10);
                    enemies.clear();
                    bombSound.play();
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
         if (!isDeathAnimFinished) {
             deathAnimClock += dt;

             if (deathAnimClock >= 0.15f) {
                 deathCurrentFrame++;
                 deathAnimClock = 0.f;

                 if (deathCurrentFrame >= 5) {
                     deathCurrentFrame = 4;
                     isDeathAnimFinished = true;

                     gameOverTimer.restart();
                    }
                }

             if (deathCurrentFrame == 1) {
                        sDeathAnim.setPosition(myPlayer.getPosition().x, myPlayer.getPosition().y - 10.f);
                    } else {
                        sDeathAnim.setPosition(myPlayer.getPosition().x, myPlayer.getPosition().y);
                    }

                    sDeathAnim.setTextureRect(sf::IntRect(deathCurrentFrame * deathFrameWidth, 0, deathFrameWidth, deathFrameHeight));
                }

           else {

            float goTime = gameOverTimer.getElapsedTime().asSeconds();

            // 3.5 saniye sonra butonlar aktif
            if (goTime > 3.5f) {

                sRestartBg.setPosition(1280.f / 2.0f, 550.f); //tekrar oyna butonu yeri
                sRestartBgHover.setPosition(1280.f / 2.0f, 550.f);

                sExitBg.setPosition(1280.f / 2.0f, 630.f); //exit butonu yeri
                sExitBgHover.setPosition(1280.f / 2.0f, 630.f);

                // Play Again Butonu
                if (sRestartBg.getGlobalBounds().contains(mousePosWorld)) {

                    if (isMouseClicked && !mouseWasPressed) {
                        bullets.clear();
                        enemies.clear();
                        loots.clear();
                        myPlayer.reset(sf::Vector2f(640.f, 384.f));
                        waveManager.reset();
                        previousHp = myPlayer.getHp();

                        isDeadSequence = false;
                        currentState = GameState::PLAYING;
                    }
                }
                //exit butonu
                if (sExitBg.getGlobalBounds().contains(mousePosWorld)) {
                    if (isMouseClicked && !mouseWasPressed) {
                        window.close();
                    }
                }
            }
           }
           break;
      }
    }

        mouseWasPressed = isMouseClicked;

        window.clear();

        switch (currentState)
        {
            case GameState::MENU: {
                window.draw(sSky);

                //Daglar
                sMountain.setPosition(mountainX, 0.f);
                window.draw(sMountain);
                sMountain.setPosition(mountainX + 1280.f, 0.f);
                window.draw(sMountain);

                //Bulutlar
                sCloud1.setPosition(cloud1X, cloud1Y); //tip1
                window.draw(sCloud1);
                sCloud1.setPosition(cloud4X, cloud4Y);
                window.draw(sCloud1);

                sCloud2.setPosition(cloud2X, cloud2Y); //tip2
                window.draw(sCloud2);
                sCloud2.setPosition(cloud5X, cloud5Y);
                window.draw(sCloud2);

                sCloud3.setPosition(cloud3X, cloud3Y); //tip3
                window.draw(sCloud3);
                sCloud3.setPosition(cloud6X, cloud6Y);
                window.draw(sCloud3);

                //Zemin
                sGround.setPosition(groundX, 0.f);
                window.draw(sGround);
                sGround.setPosition(groundX + 1280.f, 0.f);
                window.draw(sGround);

                //Otlar
                sGrass1.setPosition(std::round(grass1_1X), std::round(grass1_1Y));
                window.draw(sGrass1);

                sGrass1.setPosition(std::round(grass1_2X), std::round(grass1_2Y));
                window.draw(sGrass1);

                sGrass2.setPosition(std::round(grass2_1X), std::round(grass2_1Y));
                window.draw(sGrass2);

                sGrass2.setPosition(std::round(grass2_2X), std::round(grass2_2Y));
                window.draw(sGrass2);

                //tas
                sMenuStone.setPosition(stoneX, stoneY);
                window.draw(sMenuStone);

                //kaktus
                sCactus.setPosition(cactusX, 385.f);
                window.draw(sCactus);


                //Start butonu ve hover
                window.draw(sBtnStart);
                if (sBtnStart.getGlobalBounds().contains(mousePosWorld)) {
                    window.draw(sBtnStartHover);
                }

                window.draw(sBtnExit);
                if (sBtnExit.getGlobalBounds().contains(mousePosWorld)) {
                    window.draw(sBtnExitHover);
                }

                window.draw(menuPlayer);

                //tumbleweed
                sTumbleweed.setPosition(tumbleweedX, tumbleweedY);
                window.draw(sTumbleweed);

                window.draw(titleText);
                break;

            }

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

            case GameState::GAMEOVER:{

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
                window.draw(sDeathAnim);
                for (size_t i = 0; i < bullets.size(); i++) bullets[i].draw(window);
                for (size_t i = 0; i < enemies.size(); i++) enemies[i].draw(window);
                for (size_t i = 0; i < loots.size(); i++) loots[i].draw(window);

                if (isDeathAnimFinished) {
                    float goTime = gameOverTimer.getElapsedTime().asSeconds();

                    if (goTime < 2.0f) {
                        fadeAlpha = (goTime / 2.0f) * 220.f;
                    } else {
                        fadeAlpha = 220.f;
                    }

                sf::RectangleShape darkOverlay(sf::Vector2f(1280.f, 768.f));
                darkOverlay.setFillColor(sf::Color(0, 0, 0, (sf::Uint8)fadeAlpha));
                window.draw(darkOverlay);

                if (goTime > 2.0f) {

                    sf::FloatRect goBounds = gameOverText.getLocalBounds();
                    gameOverText.setOrigin(goBounds.left + goBounds.width / 2.0f, goBounds.top + goBounds.height / 2.0f);
                    gameOverText.setPosition(1280.f / 2.0f, 400.f);

                    window.draw(sDamagedHat);
                    window.draw(gameOverText);
                }

                if (goTime > 3.5f) {
                    //tekrar oyna butonunu cizme
                    if (sRestartBg.getGlobalBounds().contains(mousePosWorld)) {
                        window.draw(sRestartBgHover);
                    } else {
                        window.draw(sRestartBg);
                    }
                    restartText.setPosition(std::round(1280.f / 2.0f), std::round(550.f));
                    window.draw(restartText);

                    // Exit Butonunu cizme
                    if (sExitBg.getGlobalBounds().contains(mousePosWorld)) {
                        window.draw(sExitBgHover);
                    } else {
                        window.draw(sExitBg);
                    }

                    goExitText.setPosition(std::round(1280.f / 2.0f), std::round(630.f));
                    window.draw(goExitText);

                    // Skor merkezde
                    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
                    scoreText.setOrigin(scoreBounds.left + scoreBounds.width / 2.0f, scoreBounds.top + scoreBounds.height / 2.0f);
                    scoreText.setPosition(1280.f / 2.0f, 480.f);
                    window.draw(scoreText);

                    // eski yerine sifirla
                    scoreText.setOrigin(0.f, 0.f);
                    scoreText.setPosition(20.f, 100.f);
                }
              }

                break;
            }
        }

       window.display();
    }

    return 0;
}
