#include "camera.h"
#include "engine.h"


void Camera::updateMatrices()
{
    m_ViewMatrix = glm::mat4(1.0f);
    //glm::mat4 translate = glm::translate(m_ViewMatrix, -transform.position * 0.0f);
    //glm::mat4 rotate = glm::transpose(transform.getRotationMatrix());
    glm::mat4 rotate = glm::lookAt(
        transform.position,
        transform.position + m_CameraTarget,
        getUp());

    m_ViewMatrix *= rotate;

    m_ProjectionMatrix = glm::mat4(1.0f);
    m_ProjectionMatrix = glm::perspective(
        glm::radians(fieldOfView),
        (float)Engine::getInstance()->getWidth() / (float)Engine::getInstance()->getHeight(),
        nearClip,
        farClip);

    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
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

glm::mat4 Camera::getProjectionMatrix()
{
    return m_ProjectionMatrix;
}

glm::mat4 Camera::getViewProjectionMatrix()
{
    return m_ViewProjectionMatrix;
}

Camera::Camera(float fov, float near, float far) : fieldOfView(fov), nearClip(near), farClip(far)
{
}

Camera::~Camera()
{
}
