#version 330 core

in vec3 v_position;
in vec3 normal;
in vec2 tex_coord_in;

uniform mat4 mvp_matrix;
uniform mat4 model;

out vec3 pos;
out vec3 worldPos;
out vec3 worldNormal;
out vec2 tex_coord;

void main()
{
    pos = v_position;
    worldPos = vec3(model * vec4(v_position, 1.0));
    gl_Position = mvp_matrix * vec4(v_position, 1.0);

    // transform normal to world space coordinates
    // https://learnopengl.com/#!Lighting/Basic-Lighting
    worldNormal = mat3(transpose(inverse(model))) * normal;
    tex_coord = tex_coord_in;
}
