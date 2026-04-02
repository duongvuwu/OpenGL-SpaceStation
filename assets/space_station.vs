#version 330 core

// Dữ liệu đầu vào từ Student A (Interleaved Data)
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// Dữ liệu truyền sang Fragment Shader
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

// Ma trận điều hướng từ Student B
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Ma trận pháp tuyến (Normal Matrix)
uniform mat3 normalMatrix;

void main()
{
    // 1. Tính toán vị trí Fragment trong World Space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // 2. Chuyển đổi Normal sang World Space một cách an toàn
    Normal = normalMatrix * aNormal;  
    
    // 3. Truyền tọa độ UV
    TexCoords = aTexCoords;
    
    // 4. Vị trí cuối cùng trên màn hình
    gl_Position = projection * view * vec4(FragPos, 1.0);
}