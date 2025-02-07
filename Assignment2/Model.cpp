#include "Model.h"
#include "CircleOutline.h"
#include <iostream>

Model::Model() {
    meshes.push_back(CircleOutline(0, 0, outerRadius));     // Outer circle
    meshes.push_back(CircleOutline(0, 0, innerRadius));     // Inner circle
    meshes.push_back(CircleOutline(0, 0, seedRadius));      // Seed
}

// Get the meshes of the circle objects
vector<util::PolygonMesh<VertexAttrib>> Model::getMeshes() {
    return meshes;
}

// Increase the inner circle radius by 5
void Model::increaseInnerRadius() {
    if (innerRadius < 200) {
        innerRadius += 5;
    }
    meshes[1] = CircleOutline(0, 0, innerRadius);
}

// Decrease the inner circle radius by 5
void Model::decreaseInnerRadius() {
    if (innerRadius > 5) {
        innerRadius -= 5;
    }
    meshes[1] = CircleOutline(0, 0, innerRadius);
}

// Destructor
Model::~Model() {
    meshes.clear();
}