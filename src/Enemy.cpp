#include "Enemy.h"
#include <cmath>

Enemy::Enemy(sf::Texture& texture, sf::Vector2f startPos)
{
    sprite.setTexture(texture);
    currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

    sprite.setScale(2.f, 2.f);

    sprite.setOrigin(16.f, 16.f);

    sprite.setPosition(startPos);
    speed = 200.f;

}

void Enemy::update(float dt, sf::Vector2f targetPos) {
    sf::Vector2f currentPos = sprite.getPosition();

    float dx = targetPos.x - currentPos.x;
    float dy = targetPos.y - currentPos.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance != 0) {
        float moveX = (dx / distance) * speed * dt;
        float moveY = (dy / distance) * speed * dt;
        sprite.move(moveX, moveY);

        if (moveX < 0) {
            sprite.setScale(-2.f, 2.f);
        } else {
            sprite.setScale(2.f, 2.f);
        }

        if (animTimer.getElapsedTime().asSeconds() > 0.15f) {
            currentFrame++;

            if (currentFrame >= 5) {
                currentFrame = 0;
            }

            sprite.setTextureRect(sf::IntRect(currentFrame * 32, 0, 32, 32));
            animTimer.restart();
        }
    }


}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Enemy::getBounds() {
    return sprite.getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() {
    return sprite.getPosition();
}
