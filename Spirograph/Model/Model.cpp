#include "Model.h"
#include "CircleOutline.h"
#include "CurveOutline.h"
#include <iostream>

/*
Represents the four meshes for a Spirograph.
Consists of the outer circle, inner circle, seed circle,
and the outline of the seed curve.
*/
Model::Model() {
    meshes.push_back(CircleOutline(0, 0, outerRadius));         // Outer circle
    meshes.push_back(CircleOutline(0, 0, innerRadius));         // Inner circle
    meshes.push_back(CircleOutline(0, 0, seedRadius));          // Seed
    meshes.push_back(CurveOutline(outerRadius, innerRadius));   // Seed Curve
}

// Get the meshes of the circle objects
vector<util::PolygonMesh<VertexAttrib>> Model::getMeshes() {
    return meshes;
}

// Given a bool, either increases or decreases the radius of the inner circle
void Model::updateInnerRadius(bool increase) {
    if (increase) {
        if (innerRadius < 200) {
            innerRadius += 5;
        } 
    } else {
        if (innerRadius > 5) {
            innerRadius -= 5;
        }
    }
    meshes[1] = CircleOutline(0, 0, innerRadius);           // Update inner circle
    meshes[3] = CurveOutline(outerRadius, innerRadius);     // Update curve outline
}

// Destructor
Model::~Model() {
    meshes.clear();
}