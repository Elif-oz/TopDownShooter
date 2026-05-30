#ifndef LOOT_H
#define LOOT_H
#include <SFML/Graphics.hpp>

enum class LootType {
    HEALTH,
    SCORE,
    SPEED_BOOST,
    FIRE_RATE,
    BOMB
};

class Loot {

  private:
    sf::Sprite sprite;
    LootType type;

    int currentFrame; //loot animasyonlarý icin
    int maxFrames;
    float animTimer;
    int frameWidth;
    int frameHeight;

  public:
    Loot(sf::Texture& texture, sf::Vector2f pos, LootType lType, int frames = 1);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds();
    LootType getType();
};

#endif // LOOT_H
