#ifndef _GYMSCENE_H_
#define _GYMSCENE_H_

#include <vector>

#include "scene.h"

class GymScene : public Scene
{
private:
public:
    void start() override;
    void update() override;

    GymScene();
    ~GymScene();
};

#endif