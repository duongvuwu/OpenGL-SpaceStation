import os

def convert_obj_to_header(obj_file, h_file):
    vertices = []
    normals = []
    
    output_floats = []
    vertex_count = 0

    print(f"Dang doc file OBJ: {obj_file}...")
    
    try:
        with open(obj_file, 'r', encoding='utf-8', errors='ignore') as f:
            for line in f:
                parts = line.strip().split()
                if not parts: continue
                
                if parts[0] == 'v':
                    vertices.append([float(x) for x in parts[1:4]])
                elif parts[0] == 'vn':
                    normals.append([float(x) for x in parts[1:4]])
                
                elif parts[0] == 'f':
                    for i in range(1, 4):
                        if i >= len(parts): break
                        
                        indices = parts[i].split('/')
                        v_idx = int(indices[0]) - 1
                        vn_idx = int(indices[2]) - 1 if len(indices) > 2 and indices[2] != '' else -1
                        
                        px, py, pz = vertices[v_idx]
                        nx, ny, nz = normals[vn_idx] if vn_idx != -1 else (0.0, 1.0, 0.0)
                        
                        # --- PHÉP THUẬT NẰM Ở ĐÂY: AUTO UV MAPPING ---
                        uv_scale = 5.0 # Chia cho 5 để vân kim loại không bị phóng quá to
                        if abs(nx) > abs(ny) and abs(nx) > abs(nz):
                            u, v = pz / uv_scale, py / uv_scale
                        elif abs(nz) > abs(nx) and abs(nz) > abs(ny):
                            u, v = px / uv_scale, py / uv_scale
                        else:
                            u, v = px / uv_scale, pz / uv_scale
                        # ---------------------------------------------

                        output_floats.extend([px, py, pz, nx, ny, nz, u, v])
                        vertex_count += 1

        print(f"Da trich xuat va tao UV tu dong cho {vertex_count} dinh!")
        
        with open(h_file, 'w', encoding='utf-8') as f:
            f.write("#ifndef LAYOUT_DATA_H\n#define LAYOUT_DATA_H\n\n")
            f.write(f"const unsigned int roomVertexCount = {vertex_count};\n\n")
            f.write("// Format: Position X, Y, Z | Normal Nx, Ny, Nz | Auto UV U, V\n")
            f.write("float roomVertices[] = {\n")
            
            for i in range(0, len(output_floats), 8):
                row = output_floats[i:i+8]
                formatted_row = ", ".join([f"{val:.6f}f" for val in row])
                f.write(f"    {formatted_row},\n")
                
            f.write("};\n\n#endif // LAYOUT_DATA_H\n")
            
        print("XONG! Da boc thep cho tram vu tru. Di ngu thoi!")

    except FileNotFoundError:
        print(f"LOI: Khong tim thay file {obj_file}")

if __name__ == "__main__":
    convert_obj_to_header("C:/SpaceStation1/FreeCAD/Project.obj", "C:/SpaceStation1/FreeCAD/layout_data.h")