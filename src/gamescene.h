#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include <vector>

#include "scene.h"
#include "chunk.h"

class GameScene : public Scene
{
private:
    Material* m_BlockMaterial;
    std::vector<Chunk*> m_Chunks;


public:
    void buildMap();

    GameScene();
    ~GameScene();

};

#endif