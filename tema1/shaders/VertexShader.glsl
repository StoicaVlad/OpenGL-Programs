#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 player_position;

// Output value to fragment shader
out vec3 world_position;
out vec3 world_normal;

float curveFactor = 0.02;

void main()
{

	world_position = vec3(Model * vec4(v_position, 1.0));
	world_normal = v_normal;


	vec4 curvedPosition = Model * vec4(v_position, 1.0);
	curvedPosition.y = curvedPosition.y - pow(distance(player_position, vec3(curvedPosition)), 2) * curveFactor;
	curvedPosition = Projection * View * curvedPosition;

	vec4 notCurved = Projection * View * Model * vec4(v_position, 1.0);

	gl_Position = curvedPosition;
}
