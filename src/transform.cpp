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

void Transform::translate(const glm::vec3& pos, bool local)
{
    if (local)
    {
        position += rotation * pos;
    }
    else
    {
        position += pos;
    }
}

void Transform::rotate(const glm::vec3& axis)
{
    rotate(glm::quat(glm::radians(axis)));
}

void Transform::rotate(const glm::quat& quat)
{
	rotation *= quat;
}

void Transform::rotateYaw(float angle)
{
	rotate(angle * getRight());
}

void Transform::rotatePitch(float angle)
{
	rotate(angle * getUp());
}

void Transform::rotateRoll(float angle)
{
	rotate(angle * getForward());
}

void Transform::setEulerAngles(const glm::vec3& euler)
{
    rotation = glm::quat(glm::radians(euler));
}

glm::vec3 Transform::getEuler() const
{
    return glm::degrees(glm::eulerAngles(rotation));
}

glm::vec3 Transform::getForward() const
{
    return rotation * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 Transform::getRight() const
{
    return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Transform::getUp() const
{
    return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}


void Transform::setForward(glm::vec3 forward)
{

}

void Transform::lookAt(Transform transform)
{
    lookAt(transform.position);
}

void Transform::lookAt(glm::vec3 pos)
{
    glm::vec3 direction = glm::normalize(position - pos);
    float dot = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), direction);
    float angle = (float)acos(dot);
    glm::vec3 rotationAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), direction));
    rotation = glm::normalize(glm::quat(rotationAxis * angle));
}


Transform::Transform()
{
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
}


Transform::~Transform()
{
}
