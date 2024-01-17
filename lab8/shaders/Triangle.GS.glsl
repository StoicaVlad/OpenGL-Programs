#version 430

// Input and output topologies
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here

// Output
layout(location = 0) out vec3 color;

void main()
{
    vec3 center;
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;
    vec3 p3 = gl_in[2].gl_Position.xyz;
    // TODO(student): Emit the vertices of the triangle for which
    // information is received in the input attributes. Use EmitPoint()
    // and set a color information associated with each vertex.
    
    // YELLOW TRIANGLE
    color =  vec3(1, 1, 0);
    gl_Position = Projection * View * vec4(p1, 1);
    EmitVertex();
    color =  vec3(1, 1, 0);
    gl_Position = Projection * View * vec4(p2, 1);
    EmitVertex();
    color =  vec3(1, 1, 0);
    gl_Position = Projection * View * vec4(p3, 1);
    EmitVertex();

    EndPrimitive();

    // BOTTOM RED TRIANGLE
    center = vec3((p1.x + p2.x) / 2, (p1.y + p2.y) / 2, p1.z);

    vec3 mirror_direction = center - p3;
    vec3 new_vertex_position = center + mirror_direction;
    color =  vec3(1.0f, 0.f, 0.f);
    gl_Position = Projection * View * vec4(new_vertex_position, 1);
    EmitVertex();
    color =  vec3(1.0f, 0.75f, 0.f);
    gl_Position = Projection * View * vec4(p1, 1);
    EmitVertex();
    color =  vec3(1.0f, 0.75f, 0.f);
    gl_Position = Projection * View * vec4(p2, 1);
    EmitVertex();

    EndPrimitive();

    //RIGHT RED TRIANGLE
    center = vec3((p3.x + p2.x) / 2, (p3.y + p2.y) / 2, p3.z);
    mirror_direction = center - p1;
    new_vertex_position = center + mirror_direction;
    color =  vec3(1.0f, 0.f, 0.f);
    gl_Position = Projection * View * vec4(new_vertex_position, 1);
    EmitVertex();
    color =  vec3(1.0f, 0.75f, 0.f);
    gl_Position = Projection * View * vec4(p3, 1);
    EmitVertex();
    color =  vec3(1.0f, 0.75f, 0.f);
    gl_Position = Projection * View * vec4(p2, 1);
    EmitVertex();

    EndPrimitive();

    //LEFT RED TRIANGLE
    center = vec3(
        (p1.x + p3.x) / 2,
        (p1.y + p3.y) / 2,
        p1.z
    );
    mirror_direction = center - p2;
    new_vertex_position = center + mirror_direction;
    color =  vec3(1.0f, 0.f, 0.f);
    gl_Position = Projection * View * vec4(new_vertex_position, 1);
    EmitVertex();
    color =  vec3(1.0f, 0.75f, 0.f);
    gl_Position = Projection * View * vec4(p1, 1);
    EmitVertex();
    color =  vec3(1.0f, 0.75f, 0.f);
    gl_Position = Projection * View * vec4(p3, 1);
    EmitVertex();

    EndPrimitive();
}
