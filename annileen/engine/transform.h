#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

namespace annileen
{
    class Transform
    {
    private:
        glm::vec3 m_Scale;
        glm::vec3 m_Position;
        glm::vec3 m_RotationEuler;
        glm::quat m_RotationQuat;

        glm::quat computeQuaternion();

    public:
        inline glm::vec3 scale() const { return m_Scale; }
        inline glm::vec3 position() const { return m_Position; }
        inline glm::quat rotation() { return computeQuaternion(); }
        inline glm::vec3 euler() const { return m_RotationEuler; }

        inline void scale(const glm::vec3& scale) { m_Scale = scale; }
        inline void position(const glm::vec3& position) { m_Position = position; }
        inline void euler(const glm::vec3& euler) { m_RotationEuler = euler; }
        inline void rotation(const glm::quat& rotation) { euler(glm::degrees(glm::eulerAngles(rotation))); }

        glm::mat4 getModelMatrix();
        void setModelMatrix(const glm::mat4& matrix);
        void applyModelMatrix(const glm::mat4& matrix);
        
        void translate(const glm::vec3& pos, bool local = true);

        void rotate(const glm::vec3& axis);
        void rotate(const glm::quat& quat);

        void rotateYaw(float angle);
        void rotatePitch(float angle);
        void rotateRoll(float angle);

        glm::vec3 getForward();
        glm::vec3 getRight();
        glm::vec3 getUp();

        void setForward(glm::vec3 forward);

        void lookAt(Transform transform);
        void lookAt(glm::vec3 pos);

        Transform();
        ~Transform();
    };
}

#endif
