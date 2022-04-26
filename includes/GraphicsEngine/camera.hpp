#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace GE
{
    /**
     * @brief Define a camera direction, use to move the camera
     *
     */
    enum CameraDirection
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    /**
     * @brief Represent a Fly Camera in a 3D coordinate system
     *
     */

    class Camera
    {
    public:
        Camera();
        Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
        void move(CameraDirection dir, float deltaTime);
        void lookTo(float xpos, float ypos);
        void setUp(glm::vec3 up);
        glm::vec3 getUp();
        void setFront(glm::vec3 front);
        glm::vec3 getFront();
        void setPosition(glm::vec3 position);
        glm::vec3 getPosition();
        void setYaw(float yaw);
        float getYaw();
        void setPitch(float pitch);
        float getPitch();
        void setMovementSpeed(float speed);
        float getMovementSpeed();
        void setZoom(float zoom);
        float getZoom();
        glm::mat4 getMatrix();

    private:
        float lastX, lastY;
        bool firstMovement;
        float yaw, pitch;
        float movementSpeed;
        float zoom;

        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
    };
}