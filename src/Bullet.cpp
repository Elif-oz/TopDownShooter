#include "Bullet.h"

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f vel, sf::Texture& texture)
{
    sprite.setTexture(texture);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    sprite.setPosition(startPos);
    velocity = vel;
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
