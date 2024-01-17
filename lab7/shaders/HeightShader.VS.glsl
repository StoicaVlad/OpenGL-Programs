#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float Offset;
uniform sampler2D texture_2;

// Output
out vec2 texture_coord;


void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    float height = texture(texture_2, v_texture_coord).r;
    texture_coord = v_texture_coord;
    vec3 final_position = v_position;
    final_position.y += height * 10;

    gl_Position = Projection * View * Model * vec4(final_position, 1.0f);
}
