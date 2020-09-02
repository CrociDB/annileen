#pragma once

#include <engine/transform.h>
#include <engine/scenenodemodule.h>

namespace annileen
{
    class Transform;

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

        void updateMatrices();

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
        ~Camera();
    };

    typedef Camera* CameraPtr;
}
