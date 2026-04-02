# 1. Yêu cầu phiên bản CMake tối thiểu
cmake_minimum_required(VERSION 3.15)

# 2. Đặt tên dự án và khai báo ngôn ngữ (C cho GLAD, CXX cho C++)
project(SpaceStation LANGUAGES C CXX)

# 3. Thiết lập chuẩn C++ (Sử dụng C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Tắt các extension không chuẩn để code đa nền tảng tốt hơn

# 4. Tìm kiếm các thư viện hệ thống (GLFW và OpenGL)
# (Giả định bạn đã cài GLFW qua trình quản lý gói như vcpkg, MSYS2, hoặc biến môi trường)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# 5. Thu thập các file mã nguồn
# Lấy tất cả các file .cpp trong thư mục gốc
file(GLOB_RECURSE SOURCES "src/*.cpp") 
# Khai báo file mã nguồn của GLAD
set(GLAD_SOURCE "extern/glad/src/glad.c")

# 6. Khai báo file thực thi (Executable)
# Tạo file chạy tên là 'SpaceStation' từ các file nguồn C++ và file glad.c
add_executable(${PROJECT_NAME} ${SOURCES} ${GLAD_SOURCE})

# 7. Khai báo các thư mục chứa file Header (.h, .hpp)
# target_include_directories tốt hơn include_directories vì nó chỉ áp dụng riêng cho target SpaceStation
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/stb
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 8. Liên kết (Link) các thư viện vào dự án
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
)

# 9. Xử lý thư mục Assets (Chứa Shaders, Textures)
# Lệnh này yêu cầu CMake: Sau khi build xong thành công (POST_BUILD),
# hãy copy toàn bộ thư mục 'assets' từ mã nguồn sang thư mục chứa file chạy (.exe)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Đang copy thư mục assets sang thư mục build..."
)