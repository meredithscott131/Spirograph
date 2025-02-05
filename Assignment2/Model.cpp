#include "Model.h"
#include <GL/gl.h>
#include <cmath>

#define PI 3.14159265358979323846

Model::Model() {}

vector<util::PolygonMesh<VertexAttrib>> Model::getMeshes() {
    return meshes;
}

Model::~Model() {}

void Model::generateCircleOutline(float radius, int segments) {
    circleVertices.clear();
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * PI * float(i) / float(segments);
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        VertexAttrib vertex;
        vector<float> position = {x, y, 0.0f, 1.0f};  // Ensure 4D vector
        vertex.setData("position", position);
        
        circleVertices.push_back(vertex);
    }
}

vector<VertexAttrib> Model::getCircleVertices() {
    return circleVertices;
}