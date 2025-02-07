#ifndef __MODEL_H__
#define __MODEL_H__

#include "PolygonMesh.h"
#include "VertexAttrib.h"
#include <vector>
using namespace std;

class Model 
{
public:
    Model();
    ~Model();
    vector<util::PolygonMesh<VertexAttrib> > getMeshes();
    int getOuterRadius() { return outerRadius; }
    int getInnerRadius() { return innerRadius; }
    int getSeedRadius() { return seedRadius; }
    void decreaseInnerRadius();
    void increaseInnerRadius();

private:
    int outerRadius = 300;      // Radius of the rounded corner
    int innerRadius = 200;      // Radius of the inner circle
    int seedRadius = 10;        // Radius of the seed
    vector<util::PolygonMesh<VertexAttrib> > meshes;
};
#endif