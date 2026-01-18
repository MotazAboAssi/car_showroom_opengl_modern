#include "Cube.h"

// Helper to Create a Vertex easily
Vertex CreateVertex(float x, float y, float z, float u, float v)
{
    Vertex vert;
    vert.Position = glm::vec3(x, y, z);
    vert.TexCoords = glm::vec2(u, v);
    return vert;
}

Cube::Cube(glm::vec3 center, float edge, glm::vec4 color)
{
    this->center = center;
    this->edge = edge;
    this->color = color;
    faces = {};
    model = glm::mat4(1.0f);

    float halfEdge = edge / 2.0f;

    // --- GROUP 1: Z-Axis Faces (Front/Back) ---
    // We define a flat square on the Z plane
    std::vector<Vertex> vertices1;
    // Bottom-Left (0,0)
    vertices1.push_back(CreateVertex(center.x - halfEdge, center.y - halfEdge, center.z - halfEdge, 0.0f, 0.0f));
    // Top-Left (0,1)
    vertices1.push_back(CreateVertex(center.x - halfEdge, center.y - halfEdge + edge, center.z - halfEdge, 0.0f, 1.0f));
    // Top-Right (1,1)
    vertices1.push_back(CreateVertex(center.x - halfEdge + edge, center.y - halfEdge + edge, center.z - halfEdge, 1.0f, 1.0f));
    // Bottom-Right (1,0)
    vertices1.push_back(CreateVertex(center.x - halfEdge + edge, center.y - halfEdge, center.z - halfEdge, 1.0f, 0.0f));

    // Create the two faces using this geometry
    Polygon Face1(vertices1, color);
    Polygon Face2(vertices1, color);

    // --- GROUP 2: Y-Axis Faces (Top/Bottom) ---
    std::vector<Vertex> vertices3;
    // Bottom-Left (0,0)
    vertices3.push_back(CreateVertex(center.x - halfEdge, center.y - halfEdge, center.z - halfEdge, 0.0f, 0.0f));
    // Top-Left (0,1)
    vertices3.push_back(CreateVertex(center.x - halfEdge, center.y - halfEdge, center.z - halfEdge + edge, 0.0f, 1.0f));
    // Top-Right (1,1)
    vertices3.push_back(CreateVertex(center.x - halfEdge + edge, center.y - halfEdge, center.z - halfEdge + edge, 1.0f, 1.0f));
    // Bottom-Right (1,0)
    vertices3.push_back(CreateVertex(center.x - halfEdge + edge, center.y - halfEdge, center.z - halfEdge, 1.0f, 0.0f));

    Polygon Face3(vertices3, color);
    Polygon Face4(vertices3, color);

    // --- GROUP 3: X-Axis Faces (Left/Right) ---
    std::vector<Vertex> vertices5;
    // Bottom-Left (0,0)
    vertices5.push_back(CreateVertex(center.x - halfEdge, center.y - halfEdge, center.z - halfEdge + edge, 0.0f, 0.0f));
    // Top-Left (0,1)
    vertices5.push_back(CreateVertex(center.x - halfEdge, center.y - halfEdge + edge, center.z - halfEdge + edge, 0.0f, 1.0f));
    // Top-Right (1,1)
    vertices5.push_back(CreateVertex(center.x - halfEdge, center.y - halfEdge + edge, center.z - halfEdge, 1.0f, 1.0f));
    // Bottom-Right (1,0)
    vertices5.push_back(CreateVertex(center.x - halfEdge, center.y - halfEdge, center.z - halfEdge, 1.0f, 0.0f));

    Polygon Face5(vertices5, color);
    Polygon Face6(vertices5, color);

    faces.push_back(Face1);
    faces.push_back(Face2);
    faces.push_back(Face3);
    faces.push_back(Face4);
    faces.push_back(Face5);
    faces.push_back(Face6);
}

void Cube::transformation(glm::mat4 t)
{
    model = t;
}

void Cube::setTexture(GLuint texID)
{
    for (Polygon &face : faces)
    {
        face.setTexture(texID);
    }
}

void Cube::draw(Shader &shader)
{
    // 1. Back Face
    faces[0].transformation(model);
    // 2. Front Face (Translated forward by 'edge')
    faces[1].transformation(glm::translate(model, glm::vec3(0.0f, 0.0f, edge)));

    // 3. Bottom Face
    faces[2].transformation(model);
    // 4. Top Face (Translated Up by 'edge')
    faces[3].transformation(glm::translate(model, glm::vec3(0.0f, edge, 0.0f)));

    // 5. Left Face
    faces[4].transformation(model);
    // 6. Right Face (Translated Right by 'edge')
    faces[5].transformation(glm::translate(model, glm::vec3(edge, 0.0f, 0.0f)));

    for (auto &face : faces)
    {
        face.draw(shader);
    }
}
