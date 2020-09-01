#include <engine/camera.h>
#include <engine/engine.h>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

namespace annileen
{
    void Camera::updateMatrices()
    {
        m_ViewMatrix = glm::mat4(1.0f);
        glm::mat4 rotate = glm::lookAt(
            getTransform().position,
            getTransform().position + m_CameraTarget,
            getUp());

        m_ViewMatrix *= rotate;

        m_ProjectionMatrix = glm::mat4(1.0f);
        m_ProjectionMatrix = glm::perspective(
            glm::radians(m_FieldOfView),
            (float)Engine::getInstance()->getWidth() / (float)Engine::getInstance()->getHeight(),
            m_NearClip,
            m_FarClip);

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        m_ViewRotationMatrix = glm::mat4(glm::mat3(m_ViewMatrix));
    }

    void Camera::setForward(const glm::vec3 & forward)
    {
        m_CameraTarget = forward;
    }

    glm::vec3 Camera::getForward() const
    {
        return m_CameraTarget;
    }

    glm::vec3 Camera::getRight() const
    {
        return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_CameraTarget));
    }

    glm::vec3 Camera::getUp() const
    {
        return glm::normalize(glm::cross(m_CameraTarget, getRight()));
    }

    glm::mat4 Camera::getViewMatrix()
    {
        return m_ViewMatrix;
    }

    glm::mat4 Camera::getViewRotationMatrix()
    {
        return m_ViewRotationMatrix;
    }

    glm::mat4 Camera::getProjectionMatrix()
    {
        return m_ProjectionMatrix;
    }

    glm::mat4 Camera::getViewProjectionMatrix()
    {
        return m_ViewProjectionMatrix;
    }

    float* Camera::getViewRotationMatrixFloatArray()
    {
        return glm::value_ptr(m_ViewRotationMatrix);
    }

    float* Camera::getViewMatrixFloatArray()
    {
        return glm::value_ptr(m_ViewMatrix);
    }

    float* Camera::getProjectionMatrixFloatArray()
    {
        return glm::value_ptr(m_ProjectionMatrix);
    }

    float* Camera::getViewProjectionMatrixFloatArray()
    {
        return glm::value_ptr(m_ViewProjectionMatrix);
    }

    Camera::Camera(float fov, float nearc, float farc) : m_FieldOfView(fov), m_NearClip(nearc), m_FarClip(farc), 
        enabled(true), isStatic(false)
    {
        clearType = CameraClearType::CameraClearColor;
        clearColor = glm::vec3(0.156, 0.337, 0.360);
    }

    Camera::~Camera()
    {
    }
}
