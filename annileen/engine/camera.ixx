module;

#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <glm.hpp>

export module camera;

import scenenode;

export namespace annileen
{
    enum class CameraClearType
    {
        CameraClearColor,
        CameraClearSkybox,
        CameraClearNone
    };

    class Camera final : public SceneNodeModule
    {
    private:
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewRotationMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_CameraTarget;
    public:
        // Should this be moved to a parent component class? Will model be a "component"?
        bool isStatic;
        bool enabled;

        float fieldOfView;
        float nearClip;
        float farClip;

        // Clear types
        CameraClearType clearType;
        glm::vec3 clearColor;

        void updateMatrices(int screenWidth, int screenHeight);

        void setForward(const glm::vec3& forward);

        glm::vec3 getForward() const;
        glm::vec3 getRight() const;
        glm::vec3 getUp() const;

        glm::mat4 getViewMatrix();
        glm::mat4 getViewRotationMatrix();
        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewProjectionMatrix();

        float* getViewRotationMatrixFloatArray();
        float* getViewMatrixFloatArray();
        float* getProjectionMatrixFloatArray();
        float* getViewProjectionMatrixFloatArray();

        Camera();
        virtual ~Camera();
    };

}

namespace annileen
{
    void Camera::updateMatrices(int screenWidth, int screenHeight)
    {
        m_ViewMatrix = glm::mat4(1.0f);
        glm::mat4 rotate = glm::lookAt(
            getTransform().position(),
            getTransform().position() + m_CameraTarget,
            getUp());

        m_ViewMatrix *= rotate;

        m_ProjectionMatrix = glm::mat4(1.0f);
        m_ProjectionMatrix = glm::perspective(
            glm::radians(fieldOfView),
            static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
            nearClip,
            farClip);

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        m_ViewRotationMatrix = glm::mat4(glm::mat3(m_ViewMatrix));
    }

    void Camera::setForward(const glm::vec3& forward)
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

    Camera::Camera() : fieldOfView(60.0f), nearClip(0.1f), farClip(300.0f),
        enabled(true), isStatic(false)
    {
        clearType = CameraClearType::CameraClearColor;
        clearColor = glm::vec3(0.156, 0.337, 0.360);
    }

    Camera::~Camera()
    {
    }
}
