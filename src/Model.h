#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Thư viện Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <iostream>

#include <cstddef>

using namespace std;

// Cấu trúc một điểm (Vertex) của mô hình 3D
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

// Lớp Model quản lý việc nạp và vẽ
class Model {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;

    // Khi gọi Model("đường_dẫn.obj"), nó sẽ tự động chạy hàm này
    Model(string const &path) {
        loadModel(path);
    }

    // Hàm gọi lệnh vẽ phi thuyền ra màn hình
    void Draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    // Dùng Assimp đọc file .obj
    void loadModel(string const &path) {
        Assimp::Importer importer;
        // Triangulate: Chuyển đa giác thành tam giác. GenNormals: Tự tính toán ánh sáng.
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            cout << "❌ LỖI ASSIMP NẠP MÔ HÌNH: " << importer.GetErrorString() << endl;
            return;
        }

        // Đọc Mesh đầu tiên trong file (Thường file xuất từ Blender gom chung thành 1 mesh)
        for(unsigned int i = 0; i < scene->mNumMeshes; i++) {
            processMesh(scene->mMeshes[i]);
            unsigned int offset = vertices.size();
        }
        setupMesh();
        cout << "✅ SO DINH CUA MILANO LÀ: " << vertices.size() << endl;
        printf("✅ SO MAT CUA MILANO LÀ: %d\n", indices.size() / 3);
    }

    // Chuyển dữ liệu của Assimp thành mảng số liệu cho OpenGL
    // Chuyển đổi dữ liệu Assimp thành dữ liệu OpenGL
    void processMesh(aiMesh *mesh) {
        // Ghi nhớ vị trí bắt đầu để các mảnh ghép không đè lên nhau
        unsigned int offset = vertices.size(); 

        // 1. Quét các đỉnh (Vertices)
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            
            if (mesh->HasNormals()) {
                vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            }
            
            if(mesh->mTextureCoords[0]) {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            } else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);
        }
        
        // 2. Quét các mặt (Indices) và cộng thêm offset
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j] + offset);
            }
        }
    }

    // Gửi mảng số liệu lên Card đồ họa (GPU)
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Tọa độ đỉnh (Position)
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        
        // Pháp tuyến (Normal) - Dùng cho ánh sáng
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        
        // Tọa độ dán ảnh (Texture)
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};
#endif