#include "WaveManager.h"
#include <cstdlib>
#include <algorithm>

WaveManager::WaveManager()
{
    reset();
}

void WaveManager::reset() {
    currentWave = 1;
    enemiesToSpawn = 0;
    inWaveBreak = true;
    waveBreakTimer.restart();
    enemySpawnTimer.restart();
}

void WaveManager::update(std::vector<Enemy>& enemies, sf::Texture& enemyTexture) {
    if (inWaveBreak) {

        if (waveBreakTimer.getElapsedTime().asSeconds() > 3.0f) {
            inWaveBreak = false;
            enemiesToSpawn = 5 + (currentWave * 2);
        }
    }
    else {

        float spawnDelay = std::max(0.3f, 1.2f - (currentWave * 0.1f));

        if (enemiesToSpawn > 0 && enemySpawnTimer.getElapsedTime().asSeconds() > spawnDelay) {

            int spawnEdge = rand() % 4;
            float x = 0.f, y = 0.f;

            if (spawnEdge == 0)      { x = rand() % 1280; y = -100.f; }
            else if (spawnEdge == 1) { x = rand() % 1280; y = 868.f; }
            else if (spawnEdge == 2) { x = -100.f;        y = rand() % 768; }
            else if (spawnEdge == 3) { x = 1380.f;        y = rand() % 768; }

            sf::Vector2f spawnPos(x, y);
            Enemy newEnemy(enemyTexture, spawnPos);
            enemies.push_back(newEnemy);

            enemiesToSpawn--;
            enemySpawnTimer.restart();
        }

        if (enemiesToSpawn <= 0 && enemies.empty()) {
            currentWave++;
            inWaveBreak = true;
            waveBreakTimer.restart();
        }
    }
}

  int WaveManager::getCurrentWave() const { return currentWave; }
  bool WaveManager::isInBreak() const { return inWaveBreak; }
  std::string WaveManager::getBreakText() const {
    return "WAVE " + std::to_string(currentWave);
  }
