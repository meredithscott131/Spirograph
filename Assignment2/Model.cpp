#include "Model.h"
#include "CircleOutline.h"

Model::Model() {
    int outerRadius = 300;       // Radius of the rounded corner
    int innerRadius = 200;      // Radius of the inner circle
    int seedRadius = 10;        // Radius of the seed

    // Four rounded corners to represent a rounded square
    meshes.push_back(CircleOutline(0, 0, outerRadius));
    meshes.push_back(CircleOutline(0, 0, innerRadius));
    meshes.push_back(CircleOutline(0, 0, seedRadius));
}

vector<util::PolygonMesh<VertexAttrib>> Model::getMeshes() {
    return meshes;
}

Model::~Model() {

}