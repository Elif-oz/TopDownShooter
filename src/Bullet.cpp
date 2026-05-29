#include "Bullet.h"
#include <cmath>

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f vel, sf::Texture& texture)
{
    sprite.setTexture(texture);
    sprite.setScale(2.f, 2.f);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    sprite.setPosition(startPos);
    velocity = vel;

    float angle = std::atan2(velocity.y, velocity.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle);
}

void Bullet::update(float dt) {
    sprite.move(velocity * dt);
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Bullet::getBounds() {
    return sprite.getGlobalBounds();
}

sf::Vector2f Bullet::getPosition() {
    return sprite.getPosition();
}
