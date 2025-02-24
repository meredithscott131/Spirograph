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
    void updateInnerRadius(bool increase);

private:
    vector<util::PolygonMesh<VertexAttrib> > meshes;
    int outerRadius = 400;      // Radius of the rounded corner
    int innerRadius = 200;      // Radius of the inner circle
    int seedRadius = 10;        // Radius of the seed
};
#endif