#ifndef __CURVE_OUTLINE_H__
#define __CURVE_OUTLINE_H__

#include "VertexAttrib.h"
#include <PolygonMesh.h>
#include <GLFW/glfw3.h>
#include <numeric> // For std::gcd

/*
Represents the outline of the seed's movement, which is a hypotrochoid curve.
*/
class CurveOutline : public util::PolygonMesh<VertexAttrib> {
public:
    CurveOutline(float outerRadius, float innerRadius);
    ~CurveOutline() {}
    int gcd(int a, int b);
};

/*
Constructor for the seed's movement outline.
Takes in the outer and inner radii of the rolling circle.
*/
CurveOutline::CurveOutline(float R, float r) {
    // Create the vertex data
    vector<glm::vec4> positions;

    int SLICES = 300; // Higher resolution for a smoother curve
    float PI = 3.141592653589793f;
    float seedDistance = r / 2.0f; // Offset for the seed inside the inner circle

    // Find the correct full period to complete the hypotrochoid loop
    float loopPeriod = 2.0f * PI * (r / gcd((int)R, (int)r)); // Full path closure

    // Generate points along the hypotrochoid path
    for (int i = 0; i <= SLICES; ++i) {
        float theta = (float)i / SLICES * loopPeriod; // Cover the full period
        float phi = ((R - r) / r) * theta; // Correct internal rotation

        // Compute seed's spirograph path
        float S_x = (R - r) * cos(theta) + seedDistance * cos(phi);
        float S_y = (R - r) * sin(theta) - seedDistance * sin(phi);

        positions.push_back(glm::vec4(S_x, S_y, 0.0, 1.0));
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

    // Create indices for drawing the curve
    vector<unsigned int> indices;
    for (int i = 0; i < SLICES; ++i) {
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    indices.push_back(SLICES); // Close the loop

    this->setVertexData(vertexData);
    this->setPrimitives(indices);
    this->setPrimitiveType(GL_LINE_STRIP); // Use a line strip to connect the points smoothly
    this->setPrimitiveSize(2); // 2 vertices per line
}

int CurveOutline::gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

#endif