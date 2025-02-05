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
    void generateCircleOutline(float radius, int segments);
    vector<VertexAttrib> getCircleVertices();

private:
    vector<util::PolygonMesh<VertexAttrib> > meshes;
    vector<VertexAttrib> circleVertices;
};

#endif