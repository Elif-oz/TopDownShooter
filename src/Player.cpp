#include "Player.h"
#include <iostream>

Player::Player(sf::Texture& playerTex, sf::Texture& weaponTex, sf::Vector2f startPos)
{
    bodySprite.setTexture(playerTex);

    currentFrame = 0;
    bodySprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

    weaponSprite.setTexture(weaponTex);

    bodySprite.setScale(2.f, 2.f);
    weaponSprite.setScale(2.f, 2.f);

    sf::FloatRect pBounds = bodySprite.getLocalBounds();
    bodySprite.setOrigin(pBounds.width / 2.f, pBounds.height / 2.f);

    sf::FloatRect wBounds = weaponSprite.getLocalBounds();
    // Silahin origin'i sol orta kisma alindi ki oradan donsun
    weaponSprite.setOrigin(0.f, wBounds.height / 2.f);

    bodySprite.setPosition(startPos);

    hp = 3;
    maxHp = 3;
    baseSpeed = 300.f;
    speed = baseSpeed;
    currentFireCooldown = 0.5f;

    isSpeedBoosted = false;
    isFireRateBoosted = false;
    isInvincible = false;
    isVisible = true;

    score = 0;
    bulletSpeed = 800.f;

}

void Player::update(float dt, sf::RenderWindow& window) { //Hareket ve yon

    prevPos = bodySprite.getPosition();
    //Player hizi kontrolu
    if (isSpeedBoosted && speedBoostTimer.getElapsedTime().asSeconds() > 5.0f) {
        isSpeedBoosted = false;
        speed = baseSpeed;
    }
    // Ates hizi kontrolu
    if (isFireRateBoosted && fireRateTimer.getElapsedTime().asSeconds() > 5.0f) {
        isFireRateBoosted = false;
        currentFireCooldown = 0.5f;
    }
    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        bodySprite.move(0.f, -speed * dt);
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        bodySprite.move(0.f, speed * dt);
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        bodySprite.move(-speed * dt, 0.f);
        bodySprite.setScale(-2.f, 2.f);  // Sprite'in sola bakmasi icin
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        bodySprite.move(speed * dt, 0.f);
        isMoving = true;
        bodySprite.setScale(2.f, 2.f);  // Sprite'in saga bakmasi icin
    }

    if (isMoving) {
        if (animTimer.getElapsedTime().asSeconds() > 0.15f) {
            currentFrame++;

            if (currentFrame >= 4) {
                currentFrame = 0;
            }

            bodySprite.setTextureRect(sf::IntRect(currentFrame * 32, 0, 32, 32));
            animTimer.restart();
        }
    } else {
        currentFrame = 0;
        bodySprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    }


    sf::FloatRect bounds = bodySprite.getGlobalBounds(); // Ekran siniri kontrolleri
    sf::Vector2f pos = bodySprite.getPosition();

    if (bounds.left < 0.f) pos.x -= bounds.left;
    if (bounds.top < 0.f) pos.y -= bounds.top;
    if (bounds.left + bounds.width > 1280.f) pos.x -= (bounds.left + bounds.width - 1280.f);
    if (bounds.top + bounds.height > 768.f) pos.y -= (bounds.top + bounds.height - 768.f);

    bodySprite.setPosition(pos);


    weaponSprite.setPosition(bodySprite.getPosition()); // Silah pozisyonu ve fareye donmesi

    sf::Vector2f playerCenter = bodySprite.getPosition();
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosWindow);

    float dx = mousePosWorld.x - playerCenter.x;
    float dy = mousePosWorld.y - playerCenter.y;

    float angleRad = std::atan2(dy, dx);
    float angleDeg = angleRad * 180.f / 3.14159265f;

    float orbitRadius = 35.f; // Silahin karaktere uzakligi
    float weaponX = playerCenter.x + (std::cos(angleRad) * orbitRadius);
    float weaponY = playerCenter.y + (std::sin(angleRad) * orbitRadius);

    weaponSprite.setPosition(weaponX, weaponY);
    weaponSprite.setRotation(angleDeg);

    // Silah sola bakarken ters donmesini engellemek icin
    if (angleDeg > 90.f || angleDeg < -90.f) {
        weaponSprite.setScale(2.f, -2.f);
    } else {
        weaponSprite.setScale(2.f, 2.f);
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
        float bobOffset = 0.f;

        if (currentFrame == 1 || currentFrame == 3) {
            bobOffset = -4.f;
        }
        bodySprite.move(0.f, bobOffset);

        window.draw(bodySprite);
        window.draw(weaponSprite);

        bodySprite.move(0.f, -bobOffset);
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

int Player::getMaxHp() {
    return maxHp;
}

bool Player::checkInvincible() {
    return isInvincible;
}

void Player::shoot(std::vector<Bullet>& bullets, sf::Texture& bulletTexture, sf::RenderWindow& window, sf::Sound& shootSound) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootTimer.getElapsedTime().asSeconds() > currentFireCooldown) {

        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosWindow);

        sf::Vector2f wPos = weaponSprite.getPosition();
        float dx = mousePosWorld.x - wPos.x;
        float dy = mousePosWorld.y - wPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 0.1f) {
            float dirX = dx / distance;
            float dirY = dy / distance;
            float perpX = -dirY;
            float perpY = dirX;

            float barrelLength = 20.f; // Silahin uzunlugu
            float barrelOffset = 3.f;

            float angleDeg = std::atan2(dy, dx) * 180.f / 3.14159265f;
            if (angleDeg > 90.f || angleDeg < -90.f) {
                barrelOffset = -barrelOffset;
            }

            sf::Vector2f spawnPos(
              wPos.x + (dirX * barrelLength)+ (perpX * barrelOffset),
              wPos.y + (dirY * barrelLength)+ (perpY * barrelOffset)
            );

            sf::Vector2f vel(dirX * bulletSpeed, dirY * bulletSpeed);

            Bullet newBullet(spawnPos, vel, bulletTexture);
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
    maxHp = 3;
    score = 0;
    bodySprite.setPosition(startPos);
    isInvincible = false;
    isVisible = true;

    isSpeedBoosted = false;
    isFireRateBoosted = false;
    speed = baseSpeed;
    currentFireCooldown = 0.5f;
}

void Player::revertPosition() {
    bodySprite.setPosition(prevPos);
    weaponSprite.setPosition(prevPos);
}

void Player::heal(int amount) {

    if (hp < maxHp){
      hp += amount;
    }
     else if (maxHp < 5) {
        maxHp += amount;
        hp = maxHp;
     }
    // can siniri
    if (hp > maxHp) hp = maxHp;
    if (maxHp > 5) maxHp = 5;
}

void Player::activateSpeedBoost() {
    isSpeedBoosted = true;
    speed = baseSpeed * 1.5f;
    speedBoostTimer.restart();
}

void Player::activateFireRateBoost() {
    isFireRateBoosted = true;
    currentFireCooldown = 0.15f;
    fireRateTimer.restart();
}

float Player::getScaleX() const {
    return bodySprite.getScale().x;
}

