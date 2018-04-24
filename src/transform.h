#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

class Transform
{
public:
    glm::vec3 scale;
    glm::vec3 position;
    glm::quat rotation;

    glm::mat4 getModelMatrix();

    void Rotate(glm::vec3 axis);
    void Rotate(float angle, glm::vec3 axis);
    glm::quat FromEuler(glm::vec3 euler);

    glm::vec3 getEuler();

    glm::vec3 getForward();
    glm::vec3 getRight();
    glm::vec3 getUp();

    Transform();
    ~Transform();
};

#endif
