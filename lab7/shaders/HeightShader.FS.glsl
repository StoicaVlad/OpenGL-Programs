#version 330

// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;
uniform sampler2D texture_4;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture() function.
    vec4 height_map = texture(texture_2, texture_coord);
    vec4 base_color = texture(texture_1, texture_coord);
    vec4 snow_color = texture(texture_3, texture_coord);
    vec4 water_color = texture(texture_4, texture_coord);

    // TODO(student): Use the "discard" directive to terminate execution
    // based on the value of the alpha channel
    if(base_color.a < 0.5) {
        discard;
    }
    // snow level > 0.65
    if (height_map.r > 0.65f) 
    {
        out_color = snow_color;
    } 
    else if (height_map.r <= 0.65f && height_map.r > 0.5f) 
    {
        float alpha_blend = (height_map.r - 0.5f) / (0.65f - 0.5f);
        out_color = mix(base_color, snow_color, alpha_blend);
    } 
    // water level < 0.3
    else if (height_map.r < 0.3f) 
    {
        out_color = water_color;
    } 
    else if (height_map.r >= 0.3f && height_map.r < 0.4f) 
    {
        float alpha_blend = (height_map.r - 0.3f) / (0.4f - 0.3f);
        out_color = mix(base_color, water_color, height_map.r);
    } 
    else
    {
        out_color = mix(base_color, height_map, 0.5f);
    }
}
