#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <iostream>
#include <memory>

#include "shader.h"

class Material
{
private:
    std::shared_ptr<Shader> m_Shader;

public:
    void init(Shader* shader);

    Material();
    ~Material();
};

#endif

