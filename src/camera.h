#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "transform.h"

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

class Camera
{
private:
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_CameraTarget;

public:
    Transform transform;
    float fieldOfView;
    float nearClip;
    float farClip;

    void updateMatrices();

    void setForward(const glm::vec3& forward);

    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewProjectionMatrix();

    Camera(float fov, float near, float far);
    ~Camera();
};

#endif
