#version 430

// Input and output topologies
// TODO(student): First, generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(lines) in;
layout(triangle_strip, max_vertices = 256) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform int no_of_instances;
uniform int no_of_generated_points;
// TODO(student): Declare any other uniforms here

// Input
in int instance[2];

float t = 0;

//  0 - DRAW CURVE | 1 - DRAW LINES | 2 - DRAW HELIX
int drawn_object = 1;

vec3 rotateY(vec3 point, float u)
{
    float x = point.x * cos(u) - point.z *sin(u);
    float z = point.x * sin(u) + point.z *cos(u);
    return vec3(x, point.y, z);
}

vec3 translateX(vec3 point, float t)
{
    return vec3(point.x + t, point.y, point.z);
}

vec3 B2() {
	return (1 - t) * control_p0 + t * control_p1;
}

vec3 B3() {

	return (1 - t) * B2() + t * B2();
}

vec3 B4() {

	return (1 - t) * B3() + t * B3();
}

void main()
{
    const int SURFACE_TYPE_TRANSLATION = 0;
    const int SURFACE_TYPE_ROTATION = 1;

    // TODO(student): Rather than emitting vertices for the control
    // points, you must emit vertices that approximate the curve itself.

	switch (drawn_object) {
		case 0: {
			t = 0.0;
			for (int i = 0; i <  no_of_generated_points; i++)
			{
				t = float(i) / float(no_of_generated_points);
				vec3 position = B4();
				gl_Position = Projection * View * vec4(position, 1); EmitVertex();
			}

			EndPrimitive();
			break;
		}
		case 1: {
			t = 0.0;
			float offset = no_of_generated_points / no_of_instances;

			for (int i = 0; i < no_of_generated_points; i++)
			{
				vec3 position = B4();
				vec3 moved_position = translateX(position, instance[0]);
			
				gl_Position = Projection * View * vec4(moved_position, 1.0); EmitVertex();

				position = translateX(moved_position, offset);			
				gl_Position = Projection * View * vec4(position, 1.0); EmitVertex();

				t += 1.0f / no_of_generated_points;
			}

			EndPrimitive();
			break;
		}
		case 2: {

		break;
		}
	}
}
