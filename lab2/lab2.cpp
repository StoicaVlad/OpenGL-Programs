#include "lab/Lab2/Lab2.h"

#include <vector>

using namespace std;
using namespace lab;

Lab2::Lab2()
{
    window->SetSize(1280, 720);
}

Lab2::~Lab2()
{
}

void Lab2::Initialize()
{
    image->Init(1280, 720, 3 /* channels */);
    depthImage->Init(1280, 720, 1 /* channels */, glm::vec3 (1) /* initial value */);

    logic_space = { 0, 0, 16.0f, 9.0f };
    viewport_space = { 0, 0, 1280, 720 };

    // TODO(student): Divide the screen into 4 quadrants and
    // draw the geometric shapes 4 times, once in each quadrant
    viewport_space = { 0, 0, 640, 360 };
    DrawShapes();
    viewport_space = { 640, 0, 640, 360 };
    DrawShapes();
    viewport_space = { 0, 360, 640, 360 };
    DrawShapes();
    viewport_space = { 640, 360, 640, 360 };
    DrawShapes();
}

void Lab2::DrawShapes()
{
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(0, 0,  0.5), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(0, 1,  0.5), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(1, 0,  0.5), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(1, 1,  0.5), glm::vec3(0, 1, 1)),
    };

    vector<unsigned int> indices
    {
        0, 1, 2,    // indices for first triangle
        1, 2, 3,    // indices for second triangle
    };

    vector<VertexFormat> vertices_triangle
    {
        VertexFormat(glm::vec3(-1, 0,  0.5), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(0, 3,  0.5), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(1, 0,  0.5), glm::vec3(0, 0, 1)),
    };

    float ox = 0.66f;
    float oy = 1.f;

    vector<unsigned int> indices_triangle
    {
        0, 1, 2,    // indices for first triangle
    };

    glm::mat3 viewPortTransformation = transform2D::Viewport(logic_space, viewport_space);

    {
        glm::mat3 transformation = glm::mat3(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(1, 6);

        //Rasterize(vertices, indices, transformation);
        Rasterize(vertices_triangle, indices_triangle, transformation);
    }

    {
        glm::mat3 transformation = glm::mat3(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(1, 6);
        //transformation *= transform2D::Translate(ox, oy);
        transformation *= transform2D::Rotate(30);
        //transformation *= transform2D::Translate(-ox, -oy);

        //Rasterize(vertices, indices, transformation);
        Rasterize(vertices_triangle, indices_triangle, transformation);
    }

    {
        glm::mat3 transformation = glm::mat3(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(1, 6);
        //transformation *= transform2D::Translate(ox, oy);
        transformation *= transform2D::Rotate(-30);
        //transformation *= transform2D::Translate(-ox, -oy);

        //Rasterize(vertices, indices, transformation);
        Rasterize(vertices_triangle, indices_triangle, transformation);
    }

    {
        glm::mat3 transformation = glm::mat3(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(1, 6);
        //transformation *= transform2D::Translate(ox, oy);
        transformation *= transform2D::Rotate(-60);
        //transformation *= transform2D::Translate(-ox, -oy);

        //Rasterize(vertices, indices, transformation);
        Rasterize(vertices_triangle, indices_triangle, transformation);
    }

    {
        glm::mat3 transformation = glm::mat3(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(1, 6);
        //transformation *= transform2D::Translate(ox, oy);
        transformation *= transform2D::Rotate(60);
        //transformation *= transform2D::Translate(-ox, -oy);

        //Rasterize(vertices, indices, transformation);
        Rasterize(vertices_triangle, indices_triangle, transformation);
    }


    // TODO(student): Apply a uniform scaling transformation,
    // which halves the scale of the square. Apply the
    // transformation from the bottom-left corner of the square
    {
        glm::mat3 transformation = glm::mat4(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(4, 6);
        transformation *= transform2D::Scale(0.5f, 0.5f);

        Rasterize(vertices, indices, transformation);
    }

    // TODO(student): Apply a uniform scaling transformation,
    // which doubles the scale of the square. Apply the
    // transformation from the bottom-left corner of the square
    {
        glm::mat3 transformation = glm::mat4(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(7, 6);
        transformation *= transform2D::Scale(2, 2);

        Rasterize(vertices, indices, transformation);
    }

    // TODO(student): Apply a 45 degree rotation transformation
    // to the lower-left corner of the square
    {
        glm::mat3 transformation = glm::mat4(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(10, 6);
        transformation *= transform2D::Rotate(45);

        Rasterize(vertices, indices, transformation);
    }

    // TODO(student): Apply two transformations together, one
    // of non-uniform scaling with the scaling vector (1, 2)
    // and a rotation transformation of 45 degrees. Apply both
    // transformations to the lower left corner of the square.
    {
        glm::mat3 transformation = glm::mat4(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(13, 6);
        transformation *= transform2D::Rotate(45) * transform2D::Scale(1.0f, 2.0f);

        Rasterize(vertices, indices, transformation);
    }

    {
        glm::mat3 transformation = glm::mat3(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(1, 2);

        Rasterize(vertices, indices, transformation);
    }

    // TODO(student): Apply a uniform scaling transformation,
    // which halves the scale of the square. Apply the
    // transformation from the center of the square
    {
        glm::mat3 transformation = glm::mat4(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(4, 2);
        transformation *= transform2D::Translate(0.5, 0.5);
        transformation *= transform2D::Scale(0.5f, 0.5f);
        transformation *= transform2D::Translate(-0.5, -0.5);

        Rasterize(vertices, indices, transformation);
    }

    // TODO(student): Apply a uniform scaling transformation,
    // which doubles the scale of the square. Apply the
    // transformation from the center of the square
    {
        glm::mat3 transformation = glm::mat4(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(7, 2);
        transformation *= transform2D::Translate(0.5, 0.5);
        transformation *= transform2D::Scale(2.0f, 2.0f);
        transformation *= transform2D::Translate(-0.5, -0.5);

        Rasterize(vertices, indices, transformation);
    }

    // TODO(student): Apply a 45 degree rotation transformation
    // to the center of the square
    {
        glm::mat3 transformation = glm::mat4(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(10, 2);
        transformation *= transform2D::Translate(0.5, 0.5);
        transformation *= transform2D::Rotate(45);
        transformation *= transform2D::Translate(-0.5, -0.5);

        Rasterize(vertices, indices, transformation);
    }

    // TODO(student): Apply two transformations together, one
    // of non-uniform scaling with the scaling vector (1, 2)
    // and a rotation transformation of 45 degrees. Apply both
    // transformations to the center of the square
    {
        glm::mat3 transformation = glm::mat4(1.0f);
        transformation *= viewPortTransformation;
        transformation *= transform2D::Translate(13, 2);
        transformation *= transform2D::Translate(0.5, 0.5);
        transformation *= transform2D::Rotate(45);
        transformation *= transform2D::Scale(1.0f, 2.0f);
        transformation *= transform2D::Translate(-0.5, -0.5);

        Rasterize(vertices, indices, transformation);
    }
}

void Lab2::Rasterize(
    const vector<VertexFormat> &vertices,
    const vector<unsigned int> &indices,
    const glm::mat3 transformation)
{
    for (int i = 0; i < indices.size(); i += 3) {
        auto v1 = vertices[indices[i]];
        auto v2 = vertices[indices[i+1]];
        auto v3 = vertices[indices[i+2]];

        glm::vec3 pos1 = transformation * glm::vec3(v1.position.x, v1.position.y, 1);
        v1.position = glm::vec3(pos1.x, pos1.y, v1.position.z);
        
        glm::vec3 pos2 = transformation * glm::vec3(v2.position.x, v2.position.y, 1);
        v2.position = glm::vec3(pos2.x, pos2.y, v2.position.z);
        
        glm::vec3 pos3 = transformation * glm::vec3(v3.position.x, v3.position.y, 1);
        v3.position = glm::vec3(pos3.x, pos3.y, v3.position.z);

        TriangleRasterizer::Rasterize(
            v1, v2, v3, image, depthImage
        );
    }
}

void Lab2::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    bool need_refresh = false;

    if (window->KeyHold(GLFW_KEY_UP)) {
        logic_space.y += 9 * deltaTime;
        need_refresh = true;
    }

    if (window->KeyHold(GLFW_KEY_DOWN)) {
        logic_space.y -= 9 * deltaTime;
        need_refresh = true;
    }

    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        logic_space.x += 16 * deltaTime;
        need_refresh = true;
    }

    if (window->KeyHold(GLFW_KEY_LEFT)) {
        logic_space.x -= 16 * deltaTime;
        need_refresh = true;
    }

    if (need_refresh) {
        image->Clear(glm::vec3(0));
        depthImage->Clear(glm::vec3(1));

        DrawShapes();

        image->UpdateInternalData();
        depthImage->UpdateInternalData();
    }
}
