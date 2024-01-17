#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_tex_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float Offset;
uniform float Random;

// Output
// TODO(student): Output values to fragment shader
out vec3 color;

void main()
{
    float vertex_position = v_position.x + v_position.y + v_position.z;
	float noiseIntensity = 5.0f;
	color = v_position;

	vec3 world_position =  v_position;
	//world_position = v_normal / 2.0f * (rand * sin(v_position * Offset * noiseIntensity));
	world_position.x += (v_normal.x / noiseIntensity) * (vertex_position * cos(v_position.x * Offset * noiseIntensity));
	world_position.y += (v_normal.y / noiseIntensity) * (vertex_position * cos(v_position.y * Offset * noiseIntensity));
	world_position.z += (v_normal.z / noiseIntensity) * (vertex_position * cos(v_position.z * Offset * noiseIntensity));

	 // TODO(student): Compute gl_Position
	gl_Position = Projection * View * Model * vec4(world_position, 1);

}