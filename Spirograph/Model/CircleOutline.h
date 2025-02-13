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

    int slices = 50;                    // Resolution
    float PI = 3.141592653589793f; 

    // Generating vertices of the circle
    for (int i = 0; i <= slices; ++i) {
        float theta = (float)i / slices * 2.0f * PI;
        float px = x + radius * cos(theta);
        float py = y + radius * sin(theta);
        positions.push_back(glm::vec4(px, py, 0.0, 1.0));
    }

    // Creating vertex attributes
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

    // Creating indices
    vector<unsigned int> indices;
    for (int i = 0; i < slices; ++i) {
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    indices.push_back(slices);
    indices.push_back(0);

    this->setVertexData(vertexData);
    this->setPrimitives(indices);
    this->setPrimitiveType(GL_LINE_LOOP);
    this->setPrimitiveSize(2);
}

#endif