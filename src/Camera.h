#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Các hướng di chuyển của "The Head"
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,   // Bay lên theo trục Z
    DOWN  // Bay xuống theo trục Z
};

// Hệ thống Camera kép
enum Camera_Mode {
    POV_MODE, // Góc nhìn thứ nhất (di chuyển cùng nhân vật)
    CCTV_MODE // Góc nhìn cố định (CCTV ở góc phòng)
};

// Các hằng số mặc định
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  3.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
public:
    // Thuộc tính Vector của Camera
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Thuộc tính riêng cho CCTV Mode
    glm::vec3 CCTV_Position;
    glm::vec3 CCTV_LookAt;

    // Góc Euler để xoay bằng chuột
    float Yaw;
    float Pitch;

    // Thông số điều khiển
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Trạng thái hiện tại
    Camera_Mode Mode;

    // Khởi tạo Camera (Thiết lập WorldUp là trục Z: 0, 0, 1)
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f), float yaw = YAW, float pitch = PITCH) 
        : Front(glm::vec3(1.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), Mode(POV_MODE)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        
        // Setup vị trí CCTV mặc định (Student A có thể chỉnh lại cho khớp góc phòng)
        CCTV_Position = glm::vec3(10.0f, 10.0f, 8.0f);
        CCTV_LookAt = glm::vec3(0.0f, 0.0f, 0.0f);

        updateCameraVectors();
    }

    // Trả về Ma trận View (World -> View Space)
    glm::mat4 GetViewMatrix()
    {
        if (Mode == CCTV_MODE) {
            // CCTV nhìn cố định từ góc phòng về tâm
            return glm::lookAt(CCTV_Position, CCTV_LookAt, WorldUp);
        } else {
            // POV di chuyển linh hoạt
            return glm::lookAt(Position, Position + Front, Up);
        }
    }

    // Trả về Ma trận Projection (View -> Projection Space)
    glm::mat4 GetProjectionMatrix(float screenWidth, float screenHeight)
    {
        return glm::perspective(glm::radians(Zoom), screenWidth / screenHeight, 0.1f, 100.0f);
    }

    // Chuyển đổi giữa 2 chế độ (Sẽ gán vào phím Tab ở Main)
    void ToggleMode()
    {
        Mode = (Mode == POV_MODE) ? CCTV_MODE : POV_MODE;
    }

    // Xử lý Input từ bàn phím (WASD + Space/Ctrl)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        // Khóa di chuyển nếu đang ở chế độ CCTV
        if (Mode == CCTV_MODE) return;

        float velocity = MovementSpeed * deltaTime;
        
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
            
        // Bay lên/xuống theo trục Z (Môi trường không trọng lực)
        if (direction == UP)
            Position += WorldUp * velocity;
        if (direction == DOWN)
            Position -= WorldUp * velocity;
    }

    // Xử lý Input từ chuột (Tính toán Pitch và Yaw)
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        // Khóa xoay chuột nếu đang ở CCTV Mode
        if (Mode == CCTV_MODE) return;

        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Giới hạn góc Pitch để không bị lật ngược cổ (Gimbal lock)
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Cập nhật lại các vector hướng nhìn
        updateCameraVectors();
    }

private:
    // Cập nhật Front, Right và Up Vectors dựa trên Euler Angles
    void updateCameraVectors()
    {
        // Tính toán vector Front mới
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.z = sin(glm::radians(Pitch));
        Front = glm::normalize(front);
        
        // Tính toán lại Right và Up vector (Sử dụng Cross Product)
        Right = glm::normalize(glm::cross(Front, WorldUp));  
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif