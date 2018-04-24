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
    
    void translate(const glm::vec3& pos, bool local = true);

    void rotate(const glm::vec3& axis);
    void rotate(const glm::quat& quat);

    void setEulerAngles(const glm::vec3& euler);
    glm::vec3 getEuler() const;

    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;

    Transform();
    ~Transform();
};

#endif
