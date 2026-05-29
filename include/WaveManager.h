#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Enemy.h"


class WaveManager
{
  private:
    int currentWave;
    int enemiesToSpawn;
    bool inWaveBreak;
    sf::Clock waveBreakTimer;
    sf::Clock enemySpawnTimer;

  public:
      WaveManager();

    void update(std::vector<Enemy>& enemies, sf::Texture& enemyTexture);

    void reset();

    int getCurrentWave() const;
    bool isInBreak() const;
    std::string getBreakText() const;

};

#endif // WAVEMANAGER_H
