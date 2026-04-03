#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoords;
uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aPos;
    // Loại bỏ phần tịnh tiến (Translation) của ma trận View 
    // để Skybox luôn bao quanh Camera dù bạn bay đi đâu
    mat4 viewNoTransform = mat4(mat3(view)); 
    vec4 pos = projection * viewNoTransform * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // Thủ thuật để Skybox luôn có độ sâu tối đa (Z = 1.0)
}