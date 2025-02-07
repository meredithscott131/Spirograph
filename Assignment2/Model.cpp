#include "Model.h"
#include "CircleOutline.h"

Model::Model() {
    meshes.push_back(CircleOutline(0, 0, outerRadius));
    meshes.push_back(CircleOutline(0, 0, innerRadius));
    meshes.push_back(CircleOutline(0, 0, seedRadius));
}

vector<util::PolygonMesh<VertexAttrib>> Model::getMeshes() {
    return meshes;
}

void Model::decreaseInnerRadius() {
    if (innerRadius <= 5) {
        innerRadius = 200;
    }
    innerRadius -= 5;
    meshes[1] = CircleOutline(0, 0, innerRadius);
}

void Model::increaseInnerRadius() {
    if (innerRadius >= 200) {
        innerRadius = 5;
    }
    innerRadius += 5;
    meshes[1] = CircleOutline(0, 0, innerRadius);
}


Model::~Model() {

}