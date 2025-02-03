#include "Model.h"
#include "RoundedCorner.h"

Model::Model() {
    int side = 250;         // Side length of the rounded corner
    int radius = 125;       // Radius of the rounded corner

    // Four rounded corners to represent a rounded square
    meshes.push_back(RoundedCorner(0, 0, side, radius));
    meshes.push_back(RoundedCorner(0, 0, side, radius));
    meshes.push_back(RoundedCorner(0, 0, side, radius));
    meshes.push_back(RoundedCorner(0, 0, side, radius));
}

vector<util::PolygonMesh<VertexAttrib>> Model::getMeshes() {
    return meshes;
}

Model::~Model() {

}