#include "Loot.h"

Loot::Loot(sf::Texture& texture, sf::Vector2f pos, LootType lType) {
    type = lType;
    sprite.setTexture(texture);
    sprite.setScale(2.f, 2.f);
    sprite.setOrigin(16.f, 16.f);
    sprite.setPosition(pos);
}

void Loot::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Loot::getBounds() {
    return sprite.getGlobalBounds();
}

LootType Loot::getType() {
    return type;
}
