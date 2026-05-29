#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include "Bullet.h"


class Player
{
  private:
    sf::Sprite bodySprite;
    sf::Sprite weaponSprite;

    int hp;
    int maxHp;
    float speed;
    int score;

    float baseSpeed;
    float currentFireCooldown;

    bool isInvincible;
    sf::Clock damageTimer;
    bool isVisible;
    bool isSpeedBoosted;
    bool isFireRateBoosted;

    float bulletSpeed;
    sf::Clock shootTimer;
    sf::Clock speedBoostTimer;
    sf::Clock fireRateTimer;

    sf::Vector2f prevPos;

    sf::Clock animTimer;
    int currentFrame;


  public:
    Player(sf::Texture& playerTex, sf::Texture& weaponTex, sf::Vector2f startPos);

    void update(float dt, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void takeDamage();
    void shoot(std::vector<Bullet>& bullets, sf::Texture& bulletTexture, sf::RenderWindow& window, sf::Sound& shootSound);
    void addScore(int points);
    void reset(sf::Vector2f startPos);

    sf::FloatRect getBounds();
    sf::Vector2f getPosition();
    int getHp();
    int getMaxHp();
    int getScore();
    bool checkInvincible();

    void revertPosition();

    void heal(int amount);

    void activateSpeedBoost();
    void activateFireRateBoost();
};

#endif // PLAYER_H
