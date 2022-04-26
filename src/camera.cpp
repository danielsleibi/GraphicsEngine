#include <GraphicsEngine/camera.hpp>

GE::Camera::Camera() : Camera(cameraPos = glm::vec3(0.0f, 0.0f, 3.0f),
                              cameraFront = glm::vec3(0.0f, 0.0f, -1.0f),
                              cameraUp = glm::vec3(0.0f, 1.0f, 0.0f))
{
}
GE::Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
{
    cameraPos = pos;
    cameraFront = front;
    cameraUp = up;
    yaw = -90.0f;
    pitch = 0;
    movementSpeed = 2.5f;
    firstMovement = true;
}
void GE::Camera::move(CameraDirection dir, float deltaTime)
{
    const float velocity = movementSpeed * deltaTime;
    if (dir == FORWARD)
        cameraPos += velocity * cameraFront;
    if (dir == BACKWARD)
        cameraPos -= velocity * cameraFront;
    if (dir == LEFT)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
                     velocity;
    if (dir == RIGHT)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
                     velocity;

    if (dir == UP)
        cameraPos += velocity * cameraUp;
    if (dir == DOWN)
        cameraPos -= velocity * cameraUp;
}

void GE::Camera::lookTo(float xpos, float ypos)
{
    if (firstMovement)
    {
        lastX = xpos;
        lastY = ypos;
        firstMovement = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: y ranges bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}
void GE::Camera::setUp(glm::vec3 up)
{
    cameraUp = up;
}
glm::vec3 GE::Camera::getUp()
{
    return cameraUp;
}
void GE::Camera::setFront(glm::vec3 front)
{
    cameraFront = front;
}
glm::vec3 GE::Camera::getFront()
{
    return cameraFront;
}
void GE::Camera::setPosition(glm::vec3 position)
{
    cameraPos = position;
}
glm::vec3 GE::Camera::getPosition()
{
    return cameraPos;
}
void GE::Camera::setYaw(float yaw)
{
    GE::Camera::yaw = yaw;
}
float GE::Camera::getYaw()
{
    return yaw;
}
void GE::Camera::setPitch(float pitch)
{
    GE::Camera::pitch = pitch;
}
float GE::Camera::getPitch()
{
    return pitch;
}
void GE::Camera::setMovementSpeed(float speed)
{
    movementSpeed = speed;
}
float GE::Camera::getMovementSpeed()
{
    return movementSpeed;
}

void GE::Camera::setZoom(float zoom)
{
    GE::Camera::zoom = zoom;
}

float GE::Camera::getZoom()
{
    return zoom;
}

glm::mat4 GE::Camera::getMatrix()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
