#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include <vector>
#include <PerlinNoise.hpp>
#include <cstdlib>
#include <ctime>

#include "scene.h"
#include "chunk.h"

class GameScene : public Scene
{
private:
    Material* m_BlockMaterial;
    std::vector<Chunk*> m_Chunks;

    siv::PerlinNoise* m_Noise;
public:
    void buildMap();

    GameScene();
    ~GameScene();

};

#endif