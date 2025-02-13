#ifndef __CURVE_OUTLINE_H__
#define __CURVE_OUTLINE_H__

#include "VertexAttrib.h"
#include <PolygonMesh.h>
#include <GLFW/glfw3.h>

/*
Represents the outline of the seed's rotation.
*/
class CurveOutline : public util::PolygonMesh<VertexAttrib> {
public:
    CurveOutline(float outerRadius, float innerRadius);
    ~CurveOutline() {}
    int gcd(int a, int b);
};

/*
Constructor for the seed's rotation outline.
Takes in the outer and inner radii of the rolling circle.
*/
CurveOutline::CurveOutline(float outerRadius, float innerRadius) {
    // Create the vertex data
    vector<glm::vec4> positions;

    int slices = 1000;                      // Resolution  
    float distance = innerRadius / 2.0f;    // Offset for the seed (halfway between inner circle center and outer circle edge)
    float PI = 3.141592653589793f;

    float loop = 2.0f * PI * (innerRadius / gcd((int)outerRadius, (int)innerRadius));

    // Generating the points of the seed path
    for (int i = 0; i <= slices; ++i) {
        float theta = (float) i / slices * loop;
        float phi = ((outerRadius - innerRadius) / innerRadius) * theta;

        float x = (outerRadius - innerRadius) * cos(theta) + distance * cos(phi);
        float y = (outerRadius - innerRadius) * sin(theta) - distance * sin(phi);

        positions.push_back(glm::vec4(x, y, 0.0, 1.0));
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

    // Creating indices for drawing the curve
    vector<unsigned int> indices;
    for (int i = 0; i < slices; ++i) {
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    indices.push_back(slices);

    this->setVertexData(vertexData);
    this->setPrimitives(indices);
    this->setPrimitiveType(GL_LINE_STRIP);
    this->setPrimitiveSize(2);
}

// Returns the greatest common divisor between a and b
int CurveOutline::gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

#endif