#include "Player.h"
#include <iostream>

Player::Player(sf::Texture& playerTex, sf::Texture& weaponTex, sf::Vector2f startPos)
{
    bodySprite.setTexture(playerTex);
    weaponSprite.setTexture(weaponTex);

    sf::FloatRect pBounds = bodySprite.getLocalBounds();
    bodySprite.setOrigin(pBounds.width / 2.f, pBounds.height / 2.f);

    sf::FloatRect wBounds = weaponSprite.getLocalBounds();
    // Silahin origin'i sol orta kisma alindi ki oradan donsun
    weaponSprite.setOrigin(0.f, wBounds.height / 2.f);

    bodySprite.setPosition(startPos);

    hp = 3;
    speed = 500.f;
    isInvincible = false;
    isVisible = true;

    score = 0;
    bulletSpeed = 800.f;

}

void Player::update(float dt, sf::RenderWindow& window) { //Hareket ve yon

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        bodySprite.move(0.f, -speed * dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        bodySprite.move(0.f, speed * dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        bodySprite.move(-speed * dt, 0.f);
        bodySprite.setScale(-1.f, 1.f); // Sprite'in sola bakmasi icin
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        bodySprite.move(speed * dt, 0.f);
        bodySprite.setScale(1.f, 1.f);  // Sprite'in saga bakmasi icin
    }


    sf::FloatRect bounds = bodySprite.getGlobalBounds(); // Ekran siniri kontrolleri
    sf::Vector2f pos = bodySprite.getPosition();

    if (bounds.left < 0.f) pos.x -= bounds.left;
    if (bounds.top < 0.f) pos.y -= bounds.top;
    if (bounds.left + bounds.width > 1280.f) pos.x -= (bounds.left + bounds.width - 1280.f);
    if (bounds.top + bounds.height > 720.f) pos.y -= (bounds.top + bounds.height - 720.f);

    bodySprite.setPosition(pos);


    weaponSprite.setPosition(bodySprite.getPosition()); // Silah pozisyonu ve fareye donmesi

    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosWindow);

    float dx = mousePosWorld.x - weaponSprite.getPosition().x;
    float dy = mousePosWorld.y - weaponSprite.getPosition().y;

    float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;
    weaponSprite.setRotation(angle);

    // Silah sola bakarken ters dönmesini engellemek icin
    if (angle > 90.f || angle < -90.f) {
        weaponSprite.setScale(1.f, -1.f);
    } else {
        weaponSprite.setScale(1.f, 1.f);
    }


    if (isInvincible) {
        float timeSinceHit = damageTimer.getElapsedTime().asSeconds(); // Hasar alinca yanip sonmesi icin
        if (timeSinceHit > 2.0f) {
            isInvincible = false;
            isVisible = true;
        } else {
            // Saniyede birkac kez yanip sonme efekti
            isVisible = (((int)(timeSinceHit * 10.f)) % 2 == 0);
        }
    }
}

void Player::draw(sf::RenderWindow& window) {
    if (isVisible) {
        window.draw(bodySprite);
        window.draw(weaponSprite);
    }
}

void Player::takeDamage() {
    if (!isInvincible) {
        hp--;
        isInvincible = true;
        damageTimer.restart();
        std::cout << "Player hit! Remaining HP: " << hp << std::endl;
    }
}

sf::FloatRect Player::getBounds() {
    return bodySprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() {
    return bodySprite.getPosition();
}

int Player::getHp() {
    return hp;
}

bool Player::checkInvincible() {
    return isInvincible;
}

void Player::shoot(std::vector<Bullet>& bullets, sf::Texture& bulletTexture, sf::RenderWindow& window, sf::Sound& shootSound) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootTimer.getElapsedTime().asSeconds() > 0.5f) {

        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosWindow);

        sf::Vector2f wPos = weaponSprite.getPosition();
        float dx = mousePosWorld.x - wPos.x;
        float dy = mousePosWorld.y - wPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 0.1f) {
            sf::Vector2f vel((dx / distance) * bulletSpeed, (dy / distance) * bulletSpeed);
            Bullet newBullet(wPos, vel, bulletTexture);
            bullets.push_back(newBullet);
        }

        shootTimer.restart();
        shootSound.play();
    }
}

void Player::addScore(int points) {
    score += points;
}

int Player::getScore() {
    return score;
}

void Player::reset(sf::Vector2f startPos) {
    hp = 3;
    score = 0;
    bodySprite.setPosition(startPos);
    isInvincible = false;
    isVisible = true;
}

