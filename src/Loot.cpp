#include "Loot.h"

Loot::Loot(sf::Texture& texture, sf::Vector2f pos, LootType lType, int frames) {
    type = lType;
    sprite.setTexture(texture);

    maxFrames = frames;
    currentFrame = 0;
    animTimer = 0.f;

    frameWidth = texture.getSize().x / maxFrames;
    frameHeight = texture.getSize().y;
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setPosition(pos);

    sprite.setScale(2.f, 2.f);
}

void Loot::update(float dt) {

    if (maxFrames > 1) {
        animTimer += dt;

        // animasyon hizi
        if (animTimer > 0.1f) {
            currentFrame++;
            if (currentFrame >= maxFrames) {
                currentFrame = 0;
            }

            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            animTimer = 0.f;
        }
    }
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
