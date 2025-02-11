#include "View.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexAttribWithColor.h"
#include <iostream>


View::View() {

}

View::~View(){

}

void View::init(Callbacks *callbacks, Model &m) {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Set the model
    model = &m;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(900, 900, "Spirograph", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

     glfwSetWindowUserPointer(window, (void *)callbacks);

    //using C++ functions as callbacks to a C-style library
    glfwSetKeyCallback(window, 
    [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        reinterpret_cast<Callbacks*>(glfwGetWindowUserPointer(window))->onkey(key,scancode,action,mods);
    });

    glfwSetWindowSizeCallback(window, 
    [](GLFWwindow* window, int width,int height)
    {
        reinterpret_cast<Callbacks*>(glfwGetWindowUserPointer(window))->reshape(width,height);
    });

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // create the shader program
    program.createProgram(string("shaders/default.vert"),
                          string("shaders/default.frag"));
    // enable the shader program
    program.enable();
    shaderLocations = program.getAllShaderVariables();

    
    // create an object that will be used to render this mesh in opengl
    map<string, string> shaderVarsToVertexAttribs;
    shaderVarsToVertexAttribs["vPosition"] = "position";

    for (int i=0;i<model->getMeshes().size();i++) {
        util::ObjectInstance *obj = new util::ObjectInstance("triangles");
        obj->initPolygonMesh<VertexAttrib>(
            program,                        // the shader program
            shaderLocations,                // the shader locations
            shaderVarsToVertexAttribs,      // the shader variable -> attrib map
            model->getMeshes()[i]);         // the mesh object

        objects.push_back(obj);
    }

    
	int width = 800, height = 800; // Fixed initial dimensions
    glfwGetFramebufferSize(window,&width,&height);

    // Fixed orthographic projection based on the initial window size
    projection = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f);
    glViewport(0, 0, width, height);

    frames = 0;
    time = glfwGetTime();
}

void View::display() {
    program.enable();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (curveVisible) drawCurve();  // skip if curve visibility is off
    drawOuterCircle();
    drawInnerCircle();
    drawSeed();

    glFlush();
    program.disable();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// Draws the outer circle
void View::drawOuterCircle() {
    modelview = glm::mat4(1.0f);

    // Send matrices to GPU
    glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Set color to red
    glUniform4fv(shaderLocations.getLocation("vColor"), 1, glm::value_ptr(glm::vec4(1, 0, 0, 1)));

    objects[0]->draw();
}

// Draws the inner circle
void View::drawInnerCircle() {
    static float theta = 0.0f;
    float R = model->getOuterRadius();
    float r = model->getInnerRadius();

    // Update the angle
    theta += speed;

    // Calculate the center of the rolling inner circle
    float C_x = (R - r) * cos(theta);
    float C_y = (R - r) * sin(theta);

    // Calculate the rotation angle of the inner circle to maintain rolling
    float phi = -((R - r) / r) * theta;

    // Apply transformations
    modelview = glm::mat4(1.0f);
    modelview = glm::translate(modelview, glm::vec3(C_x, C_y, 0.0f));
    modelview = glm::rotate(modelview, phi, glm::vec3(0.0f, 0.0f, 1.0f));

    // Send matrices to GPU
    glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Set color to orange
    glUniform4fv(shaderLocations.getLocation("vColor"), 1, glm::value_ptr(glm::vec4(1, 0.5, 0, 1)));

    objects[1]->draw();
}

// Draws the seed
void View::drawSeed() {
    static float theta = 0.0f; // Angle for the inner circle's center movement

    float R = model->getOuterRadius();
    float r = model->getInnerRadius();
    float d = r / 2.0f; // Distance from inner circle center to seed

    // Update angle
    theta += speed;

    // Compute seed's spirograph path with updated inner radius
    float S_x = (R - r) * cos(theta) + d * cos((R - r) / r * theta);
    float S_y = (R - r) * sin(theta) - d * sin((R - r) / r * theta);

    modelview = glm::mat4(1.0f);
    modelview = glm::translate(modelview, glm::vec3(S_x, S_y, 0.0f));

    // Send transformations to GPU
    glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Set color to green
    glUniform4fv(shaderLocations.getLocation("vColor"), 1, glm::value_ptr(glm::vec4(0, 1, 0, 1)));

    objects[2]->draw();
}

// Draws the curve of the seed
void View::drawCurve() {
    modelview = glm::mat4(1.0f);

    // Send matrices to GPU
    glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Set color to green
    glUniform4fv(shaderLocations.getLocation("vColor"), 1, glm::value_ptr(glm::vec4(0, 1, 0, 1)));

    objects[3]->draw();
}

// Updates the inner circle object with the new radius
void View::updateInnerCircle(util::PolygonMesh<VertexAttrib> innerCircleMesh) {
    // Remove old inner circle object and create a new one with the updated mesh
    delete objects[1];
    objects[1] = new util::ObjectInstance("triangles");
    objects[1]->initPolygonMesh<VertexAttrib>(program, shaderLocations, {{"vPosition", "position"}}, model->getMeshes()[1]);

    map<string, string> shaderVarsToVertexAttribs;
    shaderVarsToVertexAttribs["vPosition"] = "position";

    objects[1]->initPolygonMesh<VertexAttrib>(
        program,
        shaderLocations,
        shaderVarsToVertexAttribs,
        innerCircleMesh);
}

// Updates the curve of the seed with the new radius
void View::updateCurve(const util::PolygonMesh<VertexAttrib>& newCurveMesh) {
    // Remove old curve object and create a new one with the updated mesh
    delete objects[3];
    objects[3] = new util::ObjectInstance("triangles");
    objects[3]->initPolygonMesh<VertexAttrib>(
        program,
        shaderLocations,
        {{"vPosition", "position"}},
        newCurveMesh
    );
}

bool View::shouldWindowClose() {
    return glfwWindowShouldClose(window);
}

void View::closeWindow() {
    for (int i=0;i<objects.size();i++) {
        objects[i]->cleanup();
        delete objects[i];
    }
    objects.clear();
    glfwDestroyWindow(window);

    glfwTerminate();
}