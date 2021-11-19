#include "transform.h"
#include "core/logger.h"

#include <gtx/matrix_decompose.hpp>

namespace annileen
{
    glm::mat4 Transform::getModelMatrix()
    {
        glm::mat4 matrix(1.0f);

        computeQuaternion();

        matrix = glm::translate(matrix, m_Position);
        glm::mat4 mrot = glm::mat4_cast(m_RotationQuat);
        matrix *= mrot;
        matrix = glm::scale(matrix, m_Scale);

        return matrix;
    }

    void Transform::setModelMatrix(const glm::mat4& matrix)
    {
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(matrix, scale, rotation, position, skew, perspective);

        this->position(position);
        this->scale(scale);
        this->rotation(rotation);
    }

    void Transform::applyModelMatrix(const glm::mat4& matrix)
    {
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(matrix, scale, rotation, translation, skew, perspective);

        //this->scaleBy(scale);
        this->translate(translation);
        this->rotate(rotation);
    }

    glm::quat Transform::computeQuaternion()
    {
        m_RotationQuat = glm::quat(glm::radians(m_RotationEuler));
        return m_RotationQuat;
    }

    void Transform::translate(const glm::vec3& pos, bool local)
    {
        if (local)
        {
            m_Position += rotation() * pos;
        }
        else
        {
            m_Position += pos;
        }
    }

    void Transform::rotate(const glm::vec3& axis)
    {
        m_RotationEuler += axis;
    }

    void Transform::rotate(const glm::quat& quat)
    {
        m_RotationEuler += glm::degrees(glm::eulerAngles(quat));
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

    glm::vec3 Transform::getForward()
    {
        return rotation() * glm::vec3(0.0f, 0.0f, 1.0f);
    }

    glm::vec3 Transform::getRight()
    {
        return rotation() * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glm::vec3 Transform::getUp()
    {
        return rotation() * glm::vec3(0.0f, 1.0f, 0.0f);
    }

    void Transform::setForward(glm::vec3 forward)
    {
        lookAt(m_Position + forward);
    }

    void Transform::lookAt(Transform transform)
    {
        lookAt(transform.position());
    }

    void Transform::lookAt(glm::vec3 pos)
    {
        glm::vec3 direction = glm::normalize(m_Position - pos);
        float dot = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), direction);
        float angle = (float)acos(dot);
        glm::vec3 rotationAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), direction));
        m_RotationEuler = glm::normalize(rotationAxis * angle);
    }


    Transform::Transform()
    {
        m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_RotationEuler = glm::vec3(0.0f, 0.0f, 0.0f);
        computeQuaternion();
    }


    Transform::~Transform()
    {
    }
}
