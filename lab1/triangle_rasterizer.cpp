#include "lab/lab1/triangle_rasterizer.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace lab;

void TriangleRasterizer::Rasterize(
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3,
    Image *image,
    Image *depthImage)
{
    float xMin = min(v1.position.x, min(v2.position.x, v3.position.x));
    float yMin = min(v1.position.y, min(v2.position.y, v3.position.y));
    float xMax = max(v1.position.x, max(v2.position.x, v3.position.x));
    float yMax = max(v1.position.y, max(v2.position.y, v3.position.y));

    for (int row = yMin; row < yMax; row++) {
        for (int column = xMin; column < xMax; column++) {

            if (row < 0 || row >= image->GetHeight() ||
                column < 0 || column >= image->GetWidth()) {
                continue;
            }

            glm::vec2 p(column + 0.5f, row + 0.5f);

            if (CheckPointInsideTriangle(p, v1, v2, v3)) {

                float depth = ComputePixelDepth(p, v1, v2, v3);
                float pixelDepth = depthImage->Get(row, column).x;

                if (pixelDepth > depth) {
                    image->Set(row, column, ComputePixelColor (p, v1, v2, v3));
                    depthImage->Set(row, column, glm::vec3(depth, 0, 0));
                }
            }
        }
    }
}

float TriangleRasterizer::ComputeTriangleArea(
    const glm::vec2 &v1,
    const glm::vec2 &v2,
    const glm::vec2 &v3)
{
    // TODO(student): Calculate and return the area of
    // the triangle determined by vertices v1, v2 and v3
    //
    // Use Heron's formula
    float a = glm::length(v1 - v2);
    float b = glm::length(v2 - v3);
    float c = glm::length(v1 - v3);

    float s = (a + b + c) / 2;
    float area = sqrt(s * (s - a) * (s - b) * (s - c));

    return area;
}

bool TriangleRasterizer::CheckPointInsideTriangle(
    const glm::vec2 &p,
    const VertexFormat& v1,
    const VertexFormat& v2,
    const VertexFormat& v3)
{
    const float EPSILON = 5.0f;

    // TODO(student): Check if the point p, received as a
    // parameter, is found in the triangle determined by
    // the vertices v1, v2 and v3
    //
    // Use the areas for this process, along with the EPSILON
    // value to compare the areas to account for possible
    // precision errors that may occur
    float s_v123 = ComputeTriangleArea(v1.position, v2.position, v3.position);
    float s_v12p = ComputeTriangleArea(v1.position, v2.position, p);
    float s_v23p = ComputeTriangleArea(v2.position, v3.position, p);
    float s_v13p = ComputeTriangleArea(v1.position, v3.position, p);

    return abs(s_v123 - (s_v12p + s_v23p + s_v13p)) < EPSILON;

    return false;
}

glm::vec3 TriangleRasterizer::ComputePixelColor(
    const glm::vec2& p,
    const VertexFormat& v1,
    const VertexFormat& v2,
    const VertexFormat& v3)
{
    // TODO(student): Calculate and return the color at point p,
    // received as a parameter, by interpolating between the
    // colors of vertices v1, v2 and v3
    //
    // Use the barycentric coordinates of point p
    float v_area = ComputeTriangleArea(v1.position, v2.position, v3.position);

    float u = ComputeTriangleArea(p, v1.position, v2.position) / v_area;
    float v = ComputeTriangleArea(p, v1.position, v3.position) / v_area;
    float w = ComputeTriangleArea(p, v2.position, v3.position) / v_area;
    glm::vec3 p_color = u * v3.color + v * v2.color + w * v1.color;

    return p_color;
}

float TriangleRasterizer::ComputePixelDepth(
    const glm::vec2& p,
    const VertexFormat& v1,
    const VertexFormat& v2,
    const VertexFormat& v3)
{
    // TODO(student): Calculate and return the depth value of
    // point p, received as a parameter, by interpolation between
    // the depth values (position.z) of vertices v1, v2 and v3
    //
    // Use the barycentric coordinates of point p
    float v_area = ComputeTriangleArea(v1.position, v2.position, v3.position);
    float u = ComputeTriangleArea(p, v1.position, v2.position) / v_area;
    float v = ComputeTriangleArea(p, v1.position, v3.position) / v_area;
    float w = ComputeTriangleArea(p, v2.position, v3.position) / v_area;
    float depth = u * v3.position.z + v * v2.position.z + w * v1.position.z;

    return depth;
}
