#ifndef __CIRCLE_OUTLINE_H__
#define __CIRCLE_OUTLINE_H__

#include "VertexAttrib.h"
#include <PolygonMesh.h>
#include <GLFW/glfw3.h>

/*
Represents a circle outline.
*/
class CircleOutline : public util::PolygonMesh<VertexAttrib> {
public:
    CircleOutline(int x, int y, int radius);
    ~CircleOutline() {}
};

/*
Constructor for a circle outline.
Takes in the x and y coordinates of the center and the radius of the circle.
*/
CircleOutline::CircleOutline(int x, int y, int radius) {
    // Create the vertex data
    vector<glm::vec4> positions;

    int SLICES = 50;  // More slices for a smoother circle
    float PI = 3.14159;
    float cx = x;
    float cy = y;

    // Generate vertices along the circle's circumference
    for (int i = 0; i <= SLICES; ++i) {
        float theta = (float)i / SLICES * 2.0f * PI;  // Full circle (0 to 2*pi)
        float px = cx + radius * cos(theta);
        float py = cy + radius * sin(theta);
        positions.push_back(glm::vec4(px, py, 0.0, 1.0));
    }

    // Create vertex attributes
    vector<VertexAttrib> vertexData;
    for (int i = 0; i < positions.size(); i++) {
        vector<float> data;
        VertexAttrib v;
        for (int j = 0; j < 4; j++) {
            data.push_back(positions[i][j]);
        }
        v.setData("position", data);
        vertexData.push_back(v);
    }

    // Create indices for drawing the circle using lines
    vector<unsigned int> indices;
    for (int i = 0; i < SLICES; ++i) {
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    
    // Closing the last line for the circle (looping back to the first point)
    indices.push_back(SLICES);
    indices.push_back(0);

    this->setVertexData(vertexData);
    this->setPrimitives(indices);           // Give it the index data that forms the outline
    this->setPrimitiveType(GL_LINE_LOOP);   // Use GL_LINE_LOOP to connect the points in a circle
    this->setPrimitiveSize(2);              // 2 vertices per line (to form a line between points)
}

#endif