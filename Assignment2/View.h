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
#include "Callbacks.h"
#include "Model.h"


class View
{
public:
    View();
    ~View();
    void init(Callbacks* callbacks, Model &model);
    void display();
    bool shouldWindowClose();
    void closeWindow();
    glm::mat4 projection;
    void drawOuterCircle();
    void drawInnerCircle();
    void drawSeed();
    void updateInnerCircle(util::PolygonMesh<VertexAttrib> newMesh);

private:   

    GLFWwindow* window;
    util::ShaderProgram program;
    util::ShaderLocationsVault shaderLocations;
    vector<util::ObjectInstance *> objects;
    glm::mat4 modelview;
    int frames;
    double time;
    Model model;
    float speed = 0.025f;
};

#endif