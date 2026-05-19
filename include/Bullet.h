#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>


class Bullet
{
  private:
    sf::Sprite sprite;
    sf::Vector2f velocity;

  public:
    Bullet(sf::Vector2f startPos, sf::Vector2f vel, sf::Texture& texture);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds();
    sf::Vector2f getPosition();
};

#endif // BULLET_H
