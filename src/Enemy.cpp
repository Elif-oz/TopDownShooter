#include "Enemy.h"
#include <cmath>

Enemy::Enemy(sf::Texture& texture, sf::Vector2f startPos)
{
    sprite.setTexture(texture);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    sprite.setPosition(startPos);
    speed = 250.f;
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
