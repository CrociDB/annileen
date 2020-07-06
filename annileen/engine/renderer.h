#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "engine.h"
//#include "skybox.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Scene;

class Renderer
{
private:
    Engine* m_Engine;

    const bgfx::ViewId m_ViewId = 0;

public:
    void init(Engine* engine);

    void clear(const glm::vec3& color) const;
    void clear() const;
    void initFrame(Scene* scene);
    //void renderSkybox(Camera* camera, Skybox* skybox);
    //void renderMesh(Scene* scene, Mesh* mesh);

    Renderer();
    ~Renderer();
};

#endif
