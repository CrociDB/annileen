#include "camera.h"
#include "engine.h"


void Camera::updateMatrices()
{
    m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(m_ViewMatrix, -transform.position);
    glm::mat4 rotate = glm::transpose(glm::mat4_cast(transform.rotation));
    m_ViewMatrix = rotate * translate;

    m_ProjectionMatrix = glm::mat4(1.0f);
    m_ProjectionMatrix = glm::perspective(
        glm::radians(fieldOfView),
        (float)Engine::getInstance()->getWidth() / (float)Engine::getInstance()->getHeight(),
        nearClip,
        farClip);

    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
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
