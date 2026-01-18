#include "Cylinder.h"
#include <cmath>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

Cylinder::Cylinder(glm::vec3 center, float radius, float height, glm::vec4 color, int segments)
{
    this->center = center;
    this->radius = radius;
    this->height = height;
    this->color = color;
    this->segments = segments;
    this->model = glm::mat4(1.0f);
    this->faces = {};

    // Temporary storage for generated vertices
    std::vector<Vertex> topCircleVerts;
    std::vector<Vertex> bottomCircleVerts;

    float halfHeight = height / 2.0f;
    float angleStep = 2.0f * PI / (float)segments;

    // 1. Generate Vertices for the Rings
    // We loop i from 0 to segments to ensure the texture wraps fully back to 1.0
    for (int i = 0; i <= segments; i++)
    {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Calculate Texture U coordinate (0.0 to 1.0 around the circle)
        float u = (float)i / (float)segments;

        // -- Top Vertex --
        Vertex topV;
        topV.Position = glm::vec3(center.x + x, center.y + halfHeight, center.z + z);
        topV.TexCoords = glm::vec2(u, 1.0f); // Top of texture
        topCircleVerts.push_back(topV);

        // -- Bottom Vertex --
        Vertex botV;
        botV.Position = glm::vec3(center.x + x, center.y - halfHeight, center.z + z);
        botV.TexCoords = glm::vec2(u, 0.0f); // Bottom of texture
        bottomCircleVerts.push_back(botV);
    }

    // 2. Create Top Cap (Circle)
    std::vector<Vertex> topCapPoly;
    // Center point for the Triangle Fan (essential for correct UV mapping of a cap)
    Vertex topCenter;
    topCenter.Position = glm::vec3(center.x, center.y + halfHeight, center.z);
    topCenter.TexCoords = glm::vec2(0.5f, 0.5f); // Center of texture
    topCapPoly.push_back(topCenter);

    for (int i = 0; i <= segments; i++)
    {
        // Planar mapping for cap: Remap -1..1 (Cos/Sin) to 0..1
        float angle = i * angleStep;
        float capU = (cos(angle) * 0.5f) + 0.5f;
        float capV = (sin(angle) * 0.5f) + 0.5f;

        Vertex v = topCircleVerts[i];
        v.TexCoords = glm::vec2(capU, capV); // Overwrite UVs for the cap specifically
        topCapPoly.push_back(v);
    }
    faces.push_back(Polygon(topCapPoly, color));

    // 3. Create Bottom Cap (Circle)
    std::vector<Vertex> botCapPoly;
    Vertex botCenter;
    botCenter.Position = glm::vec3(center.x, center.y - halfHeight, center.z);
    botCenter.TexCoords = glm::vec2(0.5f, 0.5f);
    botCapPoly.push_back(botCenter);

    for (int i = 0; i <= segments; i++)
    {
        float angle = i * angleStep;
        // Flip V for bottom to mirror it? Or keep standard. Let's keep standard.
        float capU = (cos(angle) * 0.5f) + 0.5f;
        float capV = (sin(angle) * 0.5f) + 0.5f;

        Vertex v = bottomCircleVerts[i];
        v.TexCoords = glm::vec2(capU, capV);
        botCapPoly.push_back(v);
    }
    faces.push_back(Polygon(botCapPoly, color));

    // 4. Create Side Walls (Quads)
    for (int i = 0; i < segments; i++)
    {
        std::vector<Vertex> sideQuad;

        // Define the quad (2 triangles) using the computed UVs from step 1
        // Top Right
        sideQuad.push_back(topCircleVerts[i + 1]);
        // Top Left
        sideQuad.push_back(topCircleVerts[i]);
        // Bottom Left
        sideQuad.push_back(bottomCircleVerts[i]);
        // Bottom Right
        sideQuad.push_back(bottomCircleVerts[i + 1]);

        faces.push_back(Polygon(sideQuad, color));
    }
}

void Cylinder::setTexture(GLuint texID)
{
    for (Polygon &face : faces)
    {
        face.setTexture(texID);
    }
}

void Cylinder::transformation(glm::mat4 t)
{
    model = t;
}

void Cylinder::draw(Shader &shader)
{
    for (Polygon &face : faces)
    {
        face.transformation(model);
        face.draw(shader);
    }
}
