# 🌌 The Space Station (The Interactive Observer)

[![OpenGL](https://img.shields.io/badge/OpenGL-3.3%2B-blue.svg)](https://www.opengl.org/)
[![CMake](https://img.shields.io/badge/CMake-Supported-green.svg)](https://cmake.org/)
[![C++](https://img.shields.io/badge/Language-C++-00599C.svg)](https://isocpp.org/)

A 3D interactive computer graphics project simulating a zero-gravity space station environment using OpenGL. This project focuses on real-time rendering, interactive navigation, and advanced lighting models to create an immersive viewing experience.

## ✨ Features

* **Dual Camera System:** Switch seamlessly between two viewing modes:
    * *POV Mode (Observer):* First-person perspective allowing free exploration of the space station.
    * *CCTV Mode:* A fixed-corner surveillance perspective observing the environment.
* **Zero-Gravity Navigation:** Full 3D spatial movement, including Z-axis vertical flight (moving up and down) to simulate a realistic space environment.
* **Advanced Illumination:** Implementation of the **Phong Reflection Model** (Ambient, Diffuse, Specular) illuminated by a Sunlight source (**Directional Light**) instead of a standard point light.
* **Transparent Materials:** The environment features a distinct "window" utilizing **Alpha Blending** to render transparent textures, providing a view into the void of space.

## 🎮 Controls

| Action | Key / Input |
| :--- | :--- |
| **Move Forward / Backward** | `W` / `S` |
| **Move Left / Right** | `A` / `D` |
| **Fly Up / Down (Z-axis)** | `Space` / `Left-Ctrl` |
| **Look Around** | `Mouse Movement` |
| **Toggle Camera (POV / CCTV)** | `Tab` |

## 🛠️ Installation & Build Instructions

This project uses **CMake** as the build system.

### Prerequisites
Make sure you have the following libraries configured in your environment:
* [GLFW](https://www.glfw.org/) (Window creation and input handling)
* [GLAD](https://glad.dav1d.de/) (OpenGL function loader)
* [GLM](https://github.com/g-truc/glm) (OpenGL Mathematics)
* [stb_image](https://github.com/nothings/stb) (Texture loading)

### Build Steps
1. Clone the repository to your local machine.
2. Open a terminal and navigate to the project directory.
3. Run the following commands:
```bash
mkdir build
cd build
cmake ..
make
4.	Execute the compiled application.
👥 Team Responsibilities
This project was collaboratively developed by a team of 3 students:
•Student A (The Architect): Responsible for 3D Geometry generation, UV Mapping, environment layout, and Alpha Blending logic for the transparent window.
•Student B (The Engineer): Responsible for Transformation Matrices (Model, View, Projection), Input Handling, and the Dual Camera Logic (POV & CCTV).
•Student C (The Visual Scientist): Responsible for GLSL Shader programming, Lighting Math (Phong model), Material Properties, and Directional Light implementation.
________________________________________
🌌 Trạm Vũ Trụ (The Interactive Observer)
Dự án đồ họa máy tính 3D mô phỏng không gian trạm vũ trụ không trọng lực, được phát triển bằng OpenGL. Dự án tập trung vào kỹ thuật kết xuất đồ họa thời gian thực, hệ thống điều hướng tương tác và các mô hình ánh sáng nâng cao.
✨ Tính Năng Chính
•	Hệ thống Camera Kép: Chuyển đổi mượt mà giữa 2 chế độ góc nhìn:
o	Chế độ POV (Góc nhìn thứ nhất): Cho phép người quan sát tự do di chuyển và khám phá trạm vũ trụ.
o	Chế độ CCTV: Góc nhìn camera an ninh được đặt cố định ở góc phòng.
•	Di chuyển Không Trọng Lực: Khả năng di chuyển tự do trong không gian 3D, bao gồm cả việc bay lên/xuống theo trục Z đặc thù của môi trường vũ trụ.
•	Chiếu sáng Nâng cao: Áp dụng mô hình chiếu sáng Phong (Phong Reflection Model) bao gồm Ambient, Diffuse và Specular. Nguồn sáng chính được sử dụng là Ánh sáng Mặt Trời (Directional Light).
•	Vật liệu Trong suốt: Tường của trạm vũ trụ có một "cửa sổ" sử dụng kỹ thuật Alpha Blending để xử lý texture trong suốt một cách chân thực.
🎮 Hệ thống Điều khiển
Thao tác	Phím / Chuột
Tiến / Lùi	W / S
Sang trái / Sang phải	A / D
Bay Lên / Xuống (Trục Z)	Space / Left-Ctrl
Xoay góc nhìn	Di chuyển Chuột
Đổi chế độ Camera (POV / CCTV)	Tab
🛠️ Hướng dẫn Cài đặt & Build
Dự án sử dụng CMake để quản lý tiến trình build.
Yêu cầu Thư viện (Dependencies)
Cần đảm bảo môi trường C++ của bạn đã được liên kết với các thư viện sau:
•	GLFW: Quản lý cửa sổ và xử lý sự kiện đầu vào.
•	GLAD: Nạp các hàm OpenGL.
•	GLM: Thư viện toán học cho ma trận và vector.
•	stb_image: Đọc file hình ảnh làm texture.
Các bước Build
1.	Clone mã nguồn về máy tính.
2.	Mở terminal tại thư mục chứa dự án và chạy lần lượt các lệnh sau:
Bash
mkdir build
cd build
cmake ..
make
3.	Chạy file thực thi vừa được tạo ra.
👥 Phân công Công việc
Dự án được thực hiện bởi nhóm gồm 3 sinh viên:
•Student A (The Architect - Kiến trúc sư): Phụ trách xây dựng Hình học 3D, trải UV Mapping, bố cục môi trường và xử lý logic hòa trộn màu (Alpha Blending) cho cửa sổ trong suốt.
•Student B (The Engineer - Kỹ sư): Phụ trách thiết lập Ma trận không gian (Model, View, Projection), xử lý Input từ người dùng và xây dựng logic cho hệ thống Camera Kép.
•Student C (The Visual Scientist - Chuyên gia Hình ảnh): Phụ trách viết mã GLSL Shader, tính toán toán học cho mô hình ánh sáng Phong, xử lý thuộc tính Vật liệu và cấu hình ánh sáng Directional Light.

