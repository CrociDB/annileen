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
    public:
        glm::vec3 scale;
        glm::vec3 position;
        glm::quat rotation;

        glm::mat4 getModelMatrix();
        float* getModelMatrixFloatArray();
        
        void translate(const glm::vec3& pos, bool local = true);

        void rotate(const glm::vec3& axis);
        void rotate(const glm::quat& quat);

        void rotateYaw(float angle);
        void rotatePitch(float angle);
        void rotateRoll(float angle);

        void setEulerAngles(const glm::vec3& euler);
        glm::vec3 getEuler() const;

        glm::vec3 getForward() const;
        glm::vec3 getRight() const;
        glm::vec3 getUp() const;

        void setForward(glm::vec3 forward);

        void lookAt(Transform transform);
        void lookAt(glm::vec3 pos);

        Transform();
        ~Transform();
    };
}

#endif
