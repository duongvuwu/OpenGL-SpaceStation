#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // ID của Shader Program sau khi link thành công
    unsigned int ID;

    // Constructor: Đọc và xây dựng Shader từ đường dẫn file
    Shader(const char* vertexPath, const char* fragmentPath);

    // Kích hoạt Shader Program
    void use();

    // --- CÁC HÀM TIỆN ÍCH UNIFORM (Phục vụ Mathematical Control) ---
    
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    
    // Tiện ích cho Directional Light & Phong Model (Vector 3 chiều)
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    
    // Tiện ích truyền Ma trận (Model, View, Projection, Normal Matrix)
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;

private:
    // Hàm kiểm tra lỗi biên dịch/link (Bảo đảm tính ổn định hệ thống)
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif