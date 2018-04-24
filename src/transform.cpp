#include "transform.h"



glm::mat4 Transform::getModelMatrix()
{
    glm::mat4 matrix(1.0f);
    glm::mat4 rot = glm::mat4_cast(rotation);

    matrix = glm::translate(matrix, position);
    matrix *= rot;
    matrix = glm::scale(matrix, scale);

    return matrix;
}

void Transform::Rotate(glm::vec3 axis)
{
    rotation = FromEuler(axis + getEuler());
}

void Transform::Rotate(float angle, glm::vec3 axis)
{
    rotation = FromEuler((angle * axis) + getEuler());
}

glm::quat Transform::FromEuler(glm::vec3 euler)
{
    glm::quat rx = glm::angleAxis(glm::radians(euler.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat ry = glm::angleAxis(glm::radians(euler.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rz = glm::angleAxis(glm::radians(euler.z), glm::vec3(0.0f, 0.0f, 1.0f));

    return rx * ry * rz;
}

glm::vec3 Transform::getEuler()
{
    return glm::eulerAngles(rotation) * 180.0f / glm::pi<float>();
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
