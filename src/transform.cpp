#include "transform.h"



glm::mat4 Transform::getModelMatrix()
{
    glm::mat4 matrix(1.0f);

    matrix = glm::translate(matrix, position);
    glm::mat4 mrot = glm::mat4_cast(rotation);
    matrix *= mrot;
    matrix = glm::scale(matrix, scale);

    return matrix;
}

void Transform::Rotate(glm::vec3 axis)
{
    setEulerAngles(axis + getEuler());
}

void Transform::Rotate(float angle, glm::vec3 axis)
{
    setEulerAngles((angle * axis) + getEuler());
}

void Transform::Rotate(glm::quat quat)
{
    rotation = rotation * quat;
}

void Transform::setEulerAngles(const glm::vec3& euler)
{
    rotation = glm::quat(glm::radians(euler));
}

glm::vec3 Transform::getEuler()
{
    return glm::degrees(glm::eulerAngles(rotation));
}

glm::vec3 Transform::getForward()
{
    return rotation * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 Transform::getRight()
{
    return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Transform::getUp()
{
    return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

Transform::Transform()
{
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::quat();
}


Transform::~Transform()
{
}
