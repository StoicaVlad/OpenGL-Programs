#version 430

// Input and output topologies
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

// Input
layout(location = 0) in vec3 v_normal[];
layout(location = 1) in vec2 v_texture_coord[];

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here
uniform float distance_time;

// Output
layout(location = 0) out vec2 texture_coord;
layout(location = 1) out vec3 normal;

void main()
{
	// vertex 0
	vec3 normal_direction = v_normal[0];
	vec4 pos = gl_in[0].gl_Position;
	pos += vec4(normal_direction, 0) * distance_time;
	texture_coord = v_texture_coord[0];
	gl_Position = Projection * View * pos;
	EmitVertex();

	// vertex 1
	pos = gl_in[1].gl_Position;
	pos += vec4(normal_direction, 0) * distance_time;
	texture_coord = v_texture_coord[1];
	gl_Position = Projection * View * pos;
	EmitVertex();

	// vertex 2
	pos = gl_in[2].gl_Position;
	pos += vec4(normal_direction, 0) * distance_time;
	texture_coord = v_texture_coord[2];
	gl_Position = Projection * View * pos;
	EmitVertex();

	EndPrimitive();
}
