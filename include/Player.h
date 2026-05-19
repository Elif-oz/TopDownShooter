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
    float speed;
    int score;

    bool isInvincible;
    sf::Clock damageTimer;
    bool isVisible;

    float bulletSpeed;
    sf::Clock shootTimer;


  public:
    Player(sf::Texture& playerTex, sf::Texture& weaponTex, sf::Vector2f startPos);

    void update(float dt, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void takeDamage();
    void shoot(std::vector<Bullet>& bullets, sf::Texture& bulletTexture, sf::RenderWindow& window, sf::Sound& shootSound);
    void addScore(int points);

    sf::FloatRect getBounds();
    sf::Vector2f getPosition();
    int getHp();
    bool checkInvincible();
    int getScore();
};

#endif // PLAYER_H
