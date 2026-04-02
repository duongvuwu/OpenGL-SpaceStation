#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Cấu hình Texture
uniform sampler2D diffuseTexture;

// Mathematical Control: Thông số Ánh sáng Mặt trời
uniform vec3 lightDir;       // Hướng ánh sáng (ví dụ: -0.2, -1.0, -0.3)
uniform vec3 lightColor;     // Màu ánh sáng (ví dụ: 1.0, 0.95, 0.9 - Vàng nhẹ)
uniform float ambientStrength; // Cường độ sáng nền
uniform float diffuseStrength; // Cường độ khuyếch tán
uniform float specularStrength;// Cường độ phản quang
uniform float shininess;     // Độ bóng của bề mặt (vd: 32.0 hoặc 64.0)

// Thông số Camera (từ Student B)
uniform vec3 viewPos;

void main()
{
    // Lấy màu (RGB) và độ trong suốt (Alpha) từ Texture
    // vec4 texel = texture(diffuseTexture, TexCoords);
    vec4 texel = vec4(1.0, 1.0, 1.0, 1.0);
    // --- MÔ HÌNH CHIẾU SÁNG PHONG ---
    
    // 1. AMBIENT (Ánh sáng môi trường)
    vec3 ambient = ambientStrength * lightColor;
  	
    // 2. DIFFUSE (Ánh sáng khuếch tán)
    vec3 norm = normalize(-Normal);
    // Ánh sáng Directional cần được đảo ngược hướng để trỏ TỪ vật thể VỀ nguồn sáng
    vec3 lightDirNormalized = normalize(-lightDir); 
    float diff = max(dot(norm, lightDirNormalized), 0.0);
    vec3 diffuse = diff * diffuseStrength * lightColor;
    
    // 3. SPECULAR (Ánh sáng phản xạ gương)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirNormalized, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;  
        
    // Tổng hợp ánh sáng Phong
    vec3 phongResult = (ambient + diffuse + specular);
    
    // Áp dụng ánh sáng lên màu Texture
    vec3 finalColor = phongResult * texel.rgb;
    //vec3 finalColor = phongResult * (vec3(1.0) - texel.rgb)
    
    // --- HIỆU ỨNG CHIỀU SÂU (Thay thế Attenuation) ---
    float distance = length(viewPos - FragPos);
    // Mật độ sương mù vũ trụ (0.03). Có thể đưa thành uniform để tinh chỉnh
    float fogFactor = exp(-pow(distance * 0.03, 2.0)); 
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3 spaceFogColor = vec3(0.02, 0.02, 0.05); // Màu đen tuyền của vũ trụ
    
    // Trộn màu vật thể với màu vũ trụ dựa trên khoảng cách
    finalColor = mix(spaceFogColor, finalColor, fogFactor);

    // Xuất kết quả. Kênh Alpha được giữ nguyên để OpenGL xử lý Blending
    FragColor = vec4(finalColor, 1.0);
}