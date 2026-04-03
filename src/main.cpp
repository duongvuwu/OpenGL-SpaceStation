#include "Model.h"

#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // Hoặc đường dẫn chính xác tới file bạn vừa lưu

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/Shader.h"
#include "Camera.h"


// Giả định Student A đã cung cấp file này chứa mảng roomVertices, roomVertexCount 
// và actorVertices, actorVertexCount (cho quả cầu)
#include "../layout_data.h" 

#include <iostream>

// --- KHAI BÁO HÀM (Function Prototypes) ---
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);
unsigned int loadCubemap(std::vector<std::string> faces);

// --- THÔNG SỐ CỬA SỔ ---
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// --- KHỞI TẠO CAMERA (Student B) ---
Camera camera(glm::vec3(0.0f, 0.0f, 0.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// --- TIMING ---
float deltaTime = 0.0f;	// Thời gian giữa frame hiện tại và frame trước
float lastFrame = 0.0f;

int main()
{
    printf("\n\n🚀 TRẠM VŨ TRỤ ĐÃ KHỞI ĐỘNG!\n\n");
    // =========================================================================
    // 1. THIẾT LẬP GLFW & CỬA SỔ
    // =========================================================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "The Space Station - Team Project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Lỗi: Không thể tạo cửa sổ GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Đăng ký các hàm callback xử lý sự kiện
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback); // Xử lý phím bấm 1 lần (như nút TAB)

    // Khóa con trỏ chuột trong màn hình
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Nạp GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Lỗi: Không thể khởi tạo GLAD" << std::endl;
        return -1;
    }

    // =========================================================================
    // 2. CẤU HÌNH RENDER LOGIC (DEPTH TEST & ALPHA BLENDING)
    // =========================================================================
    // Bật kiểm tra chiều sâu (Z-Buffer)
    glEnable(GL_DEPTH_TEST);
    
    // Bật hiệu ứng trong suốt (Alpha Blending) cho cửa sổ trạm vũ trụ
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // =========================================================================
    // 3. BIÊN DỊCH SHADER (Student C)
    // =========================================================================
    Shader spaceStationShader("../assets/space_station.vs", "../assets/space_station.fs");

    // =========================================================================
    // 4. NẠP DỮ LIỆU HÌNH HỌC (Student A)
    // =========================================================================
    unsigned int roomVAO, roomVBO;
    glGenVertexArrays(1, &roomVAO);
    glGenBuffers(1, &roomVBO);

    glBindVertexArray(roomVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roomVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roomVertices), roomVertices, GL_STATIC_DRAW);

    // Cấu trúc Interleaved: Position(3) | Normal(3) | UV(2)
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture Coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // (Tùy chọn) Khởi tạo actorVAO, actorVBO cho quả cầu nhân vật tương tự như trên...
    
    // Nạp Texture (Đảm bảo bạn có file wall_texture.png có kênh Alpha)
    unsigned int wallTexture = loadTexture("C:/SpaceStation1/assets/textures/wall_texture.png");
    spaceStationShader.use();
    spaceStationShader.setInt("diffuseTexture", 0);
    // [THÊM MỚI] KHỞI TẠO DỮ LIỆU SKYBOX
float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
    };

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Nạp Shader và Texture cho Skybox
    Shader skyboxShader("../assets/skybox.vs", "../assets/skybox.fs");
    std::vector<std::string> faces = {
        "../assets/skybox/right.jpg", "../assets/skybox/left.jpg",
        "../assets/skybox/top.jpg",   "../assets/skybox/bottom.jpg",
        "../assets/skybox/front.jpg", "../assets/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);
    
    skyboxShader.use();
    skyboxShader.setInt("skybox", 1);
    Model milano("assets/GotMillanoDespiece.obj");
    // =========================================================================
    // 5. VÒNG LẶP RENDER CHÍNH (MAIN LOOP)
    // =========================================================================
    while (!glfwWindowShouldClose(window))
    {
        // Tính toán Delta Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Xử lý Input liên tục (Giữ phím di chuyển)
        processInput(window);

        // Làm sạch màn hình (Bầu trời vũ trụ màu đen)
        glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Kích hoạt Shader
        spaceStationShader.use();

        // --- MATHEMATICAL CONTROL: TRUYỀN ÁNH SÁNG MẶT TRỜI ---
        // Thiết lập nguồn sáng Directional Light
        spaceStationShader.setVec3("lightDir", -0.2f, -1.0f, -0.3f);
        spaceStationShader.setVec3("lightColor", 1.0f, 0.95f, 0.9f); // Vàng nhạt ấm áp
        spaceStationShader.setFloat("ambientStrength", 0.1f);
        spaceStationShader.setFloat("diffuseStrength", 0.8f);
        spaceStationShader.setFloat("specularStrength", 1.0f);
        spaceStationShader.setFloat("shininess", 64.0f);

        // --- TRUYỀN MA TRẬN CAMERA ---
        glm::mat4 projection = camera.GetProjectionMatrix((float)SCR_WIDTH, (float)SCR_HEIGHT);
        glm::mat4 view = camera.GetViewMatrix();
        spaceStationShader.setMat4("projection", projection);
        spaceStationShader.setMat4("view", view);
        
        // Vị trí Camera phục vụ cho Specular Lighting
        spaceStationShader.setVec3("viewPos", camera.Position);

        // --- VẼ CĂN PHÒNG ---
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glm::mat4 roomModel = glm::mat4(1.0f);
        // Thu nhỏ căn phòng 100 lần (từ 200mm -> 2 đơn vị OpenGL)
        roomModel = glm::scale(roomModel, glm::vec3(0.01f, 0.01f, 0.01f));
        spaceStationShader.setMat4("model", roomModel);
        
        // Tính toán ma trận Normal để ánh sáng không bị méo khi scale
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(roomModel)));
        spaceStationShader.setMat3("normalMatrix", normalMatrix);

        // Gọi hàm vẽ toàn bộ đỉnh
        glBindVertexArray(roomVAO);
        glDrawArrays(GL_TRIANGLES, 0, roomVertexCount);

        // --- VẼ THE ACTOR (QUẢ CẦU) ---
        // Nếu ở chế độ CCTV, ta sẽ thấy quả cầu đại diện cho nhân vật đang di chuyển
        if (camera.Mode == CCTV_MODE) 
        {
            glm::mat4 actorModel = glm::mat4(1.0f);
            actorModel = glm::translate(actorModel, camera.Position); // Gắn quả cầu vào vị trí Camera
            actorModel = glm::scale(actorModel, glm::vec3(0.5f));     // Thu nhỏ quả cầu lại
            
            spaceStationShader.setMat4("model", actorModel);
            glm::mat3 actorNormalMatrix = glm::transpose(glm::inverse(glm::mat3(actorModel)));
            spaceStationShader.setMat3("normalMatrix", actorNormalMatrix);
            
            // glBindVertexArray(actorVAO);
            // glDrawArrays(GL_TRIANGLES, 0, actorVertexCount);
        }
        // --- VẼ SKYBOX ---
        glDepthFunc(GL_LEQUAL); // Đổi hàm test chiều sâu: Cho phép vẽ nếu chiều sâu <= 1.0 (vô cực)
        skyboxShader.use();
        
        // Truyền ma trận View và Projection
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        // Vẽ khối lập phương khổng lồ
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        glDepthFunc(GL_LESS); // Trả lại hàm test chiều sâu về mặc định cho khung hình sau
   
        spaceStationShader.use();
        
        glm::mat4 modelMillano = glm::mat4(1.0f);
        // 1. Lùi ra xa 10 mét để có góc nhìn rộng
        modelMillano = glm::translate(modelMillano, glm::vec3(0.0f, 0.0f, 7.0f)); 
        
        // 2. RADAR QUÉT: Bắt nó tự động xoay tròn liên tục! 
        // Dù nó có bị lệch tâm tít thò lò ở đâu, nó cũng sẽ vung vẩy quét ngang qua màn hình!
        // modelMillano = glm::rotate(modelMillano, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f)); 
        
        // 3. THU NHỎ (Thử mức 0.01 trước, nếu vẫn không thấy thì đổi thành 0.001)
        modelMillano = glm::scale(modelMillano, glm::vec3(1.0f, 1.0f, 1.0f)); 

        spaceStationShader.setMat4("model", modelMillano);
        
        // 4. KÍNH X-QUANG: Ép vẽ bằng lưới thép màu trắng sáng rực, bất chấp ánh sáng!
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        
        milano.Draw(); // VẼ!!!
        
        // Trả lại chế độ vẽ đặc cho các hộp gỗ cũ của bạn
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // Đổi buffers và lấy các sự kiện I/O
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // =========================================================================
    // 6. GIẢI PHÓNG BỘ NHỚ
    // =========================================================================
    glDeleteVertexArrays(1, &roomVAO);
    glDeleteBuffers(1, &roomVBO);
    glfwTerminate();
    return 0;
}

// =========================================================================
// HÀM XỬ LÝ INPUT VÀ TIỆN ÍCH
// =========================================================================

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Di chuyển WASD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
        
    // Bay lên/xuống trong môi trường không trọng lực
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Bấm phím Tab 1 lần để đổi góc nhìn POV <-> CCTV
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        camera.ToggleMode();
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = lastX - xpos;
    float yoffset = lastY - ypos; // Đảo ngược y vì trục y của chuột đi từ trên xuống dưới

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Hàm giả định để nạp Texture (Tích hợp thư viện stb_image.h)
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    // OpenGL đọc ảnh từ dưới lên, nên cần lật ảnh lại cho đúng chiều
    stbi_set_flip_vertically_on_load(true); 

    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;

        // BẮT ĐẦU CẤU HÌNH TRÊN GPU
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        // Chống lỗi lệch byte khi nạp ảnh (quan trọng cho ảnh JPG lẻ pixel)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Các thông số lặp lại và lọc ảnh
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "Successfully loaded texture at: " << path << std::endl;
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
// Hàm để nạp CubeMap
unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false); // CubeMap thường không cần lật ảnh

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        // Số 3 ở cuối: ÉP CỨNG đọc ảnh dưới dạng RGB (3 kênh màu)
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 3); 
        if (data)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Chống lỗi méo ảnh
            // Vì đã ép 3 kênh màu, ta mạnh dạn hardcode GL_RGB luôn
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            std::cout << "Thanh cong nạp mat Skybox: " << faces[i] << " (" << width << "x" << height << ")" << std::endl;
        }
        else
        {
            std::cout << "LOI: Khong the nap Skybox tai: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}