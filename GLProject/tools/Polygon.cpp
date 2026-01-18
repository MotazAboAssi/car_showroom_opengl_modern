#include "learnopengl/shader.h"
#include "Polygon.h"

Polygon::Polygon(std::vector<Vertex> v, glm::vec4 c)
{
    vertices = v;
    if (!v.empty())
    {
        head = v.at(0).Position;
    }
    color = c;
    model = glm::mat4(1.0f);
    textureID = 0; // Default: No texture

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 1. Upload the Struct Data directly
    // This works because the struct memory layout matches the C-array layout
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // 2. Position Attribute (Location 0)
    // Stride is now sizeof(Vertex)
    // 1. Position Attribute (Location 0)
    glEnableVertexAttribArray(0);
    // CRITICAL: The 5th parameter MUST be sizeof(Vertex), not sizeof(glm::vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // 3. Texture Coordinate Attribute (Location 1)
    // Offset is where 'TexCoords' starts inside the struct
    // 2. Texture Coordinate Attribute (Location 1)
    glEnableVertexAttribArray(1);
    // CRITICAL: The 5th parameter MUST be sizeof(Vertex)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Polygon::transformation(glm::mat4 t)
{
    model = t;
}

void Polygon::setTexture(GLuint texID)
{
    this->textureID = texID;
}

void Polygon::draw(Shader &shader)
{
    shader.setVec4("objectColor", color);
    shader.setMat4("model", model);

    // Bind Texture if it exists
    if (textureID != 0)
    {
        glActiveTexture(GL_TEXTURE0); // Activate Texture Unit 0
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("textureSample", 0); // Tell shader to use Unit 0
    }

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

    // Good practice: unbind vertex array
    glBindVertexArray(0);
}

void Polygon::deleteBuffers()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

Polygon::Polygon()
{
    vertices = {};
    head = glm::vec3(0.0f);
    color = glm::vec4(1.0f);
    model = glm::mat4(1.0f);
    VAO = 0;
    VBO = 0;
    textureID = 0;
}
