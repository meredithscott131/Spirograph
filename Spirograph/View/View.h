#ifndef __VIEW_H__
#define __VIEW_H__

#include <glad/glad.h>
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>
#include <cstdio>
#include <ShaderProgram.h>
#include <ObjectInstance.h>
#include "PolygonMesh.h"
#include "VertexAttrib.h"
#include "../Controller/Callbacks.h"
#include "../Model/Model.h"


class View
{
public:
    View();
    ~View();
    void init(Callbacks* callbacks, Model &m); 
    void display();
    bool shouldWindowClose();
    void closeWindow();
    glm::mat4 projection;
    void drawOuterCircle();
    void drawInnerCircle();
    void drawSeed();
    void drawCurve();
    void updateInnerCircle(util::PolygonMesh<VertexAttrib> innerCircleMesh);
    void updateCurve(const util::PolygonMesh<VertexAttrib>& newCurveMesh);
    void setCurveVisibility(bool visible) { curveVisible = visible; } 

private:   
    GLFWwindow* window;
    util::ShaderProgram program;
    util::ShaderLocationsVault shaderLocations;
    vector<util::ObjectInstance *> objects;
    glm::mat4 modelview;
    int frames;
    double time;
    Model* model;
    float speed = 0.025f;       // speed of the inner circle and seed
    bool curveVisible = true;   // visibility of the seed curve
};

#endif