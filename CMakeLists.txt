cmake_minimum_required(VERSION 3.15)
project(SpaceStation1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Tìm OpenGL
find_package(OpenGL REQUIRED)

# 2. Khai báo file thực thi
add_executable(SpaceStation1 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/src/glad.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shader/Shader.cpp"
)

# 3. Header (.h)
target_include_directories(SpaceStation1 PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/assimp/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/stb"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# 4. LIÊN KẾT THƯ VIỆN 
target_link_libraries(SpaceStation1 PRIVATE
    # Dùng thẳng 2 file có sẵn trong thư mục của bạn
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/assimp/libassimp.dll.a"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/assimp/libunit.dll.a"
    
    # Đường dẫn GLFW (Kiểm tra lại xem máy bạn là .a hay .dll.a)
    "C:/msys64/ucrt64/lib/libglfw3.a" 
    
    OpenGL::GL
    gdi32
)

# 5. Copy Assets
if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    add_custom_command(TARGET SpaceStation1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:SpaceStation1>/assets"
    )
endif()