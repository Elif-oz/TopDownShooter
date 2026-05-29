#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>


class Enemy
{
  private:
    sf::Sprite sprite;
    float speed;

    sf::Clock animTimer;
    int currentFrame;

  public:
    Enemy(sf::Texture& texture, sf::Vector2f startPos);

    // Dusman hareketinin hesaplanmasi icin oyuncunun anlýk pozisyonunu alma
    void update(float dt, sf::Vector2f targetPos);

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds();
    sf::Vector2f getPosition();
};

#endif // ENEMY_H
