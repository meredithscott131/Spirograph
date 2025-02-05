#include "View.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexAttribWithColor.h"


View::View() {

}

View::~View(){

}

void View::init(Callbacks *callbacks, vector<util::PolygonMesh<VertexAttrib>>& meshes, Model& model) 
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(800, 800, "Spirograph", NULL, NULL);
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
    program.enable();
    shaderLocations = program.getAllShaderVariables();

    map<string, string> shaderVarsToVertexAttribs;

    shaderVarsToVertexAttribs["vPosition"] = "position";

    for (int i=0;i<meshes.size();i++) {
        util::ObjectInstance *obj = new util::ObjectInstance("triangles");
        obj->initPolygonMesh<VertexAttrib>(
            program,                    // the shader program
            shaderLocations,            // the shader locations
            shaderVarsToVertexAttribs,  // the shader variable -> attrib map
            meshes[i]);                      // the actual mesh object

        objects.push_back(obj);

    }

    
	int width = 800, height = 800; // Fixed initial dimensions
    glfwGetFramebufferSize(window,&width,&height);

    // Fixed orthographic projection based on the initial window size
    projection = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f);
    glViewport(0, 0, width, height);

    frames = 0;
    time = glfwGetTime();

    this->model = &model; // Assign the model pointer

    // Generate and bind the Vertex Array Object (VAO)
    glGenVertexArrays(1, &circleVAO);
    glBindVertexArray(circleVAO);

    // Generate and bind the Vertex Buffer Object (VBO)
    glGenBuffers(1, &circleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);

    // Generate the circle outline vertices
    model.generateCircleOutline(10.0f, 100); // Adjust radius and segments as needed
    vector<VertexAttrib> circleVertices = model.getCircleVertices();

    // Upload vertex data to the VBO
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(VertexAttrib), &circleVertices[0], GL_STATIC_DRAW);

    // Define the layout of the vertex data
    GLint posAttrib = shaderLocations.getLocation("vPosition");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttrib), (void*)0);

    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void View::display() {
    program.enable();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind the VAO
    glBindVertexArray(circleVAO);

    // Set the modelview and projection matrices
    glm::mat4 modelview = glm::mat4(1.0f);
    glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Set the color uniform
    glm::vec4 color(1, 1, 1, 1); // White color
    glUniform4fv(shaderLocations.getLocation("vColor"), 1, glm::value_ptr(color));

    // Draw the circle outline using GL_LINE_STRIP
    glDrawArrays(GL_LINE_STRIP, 0, model->getCircleVertices().size());

    // Unbind the VAO
    glBindVertexArray(0);

    glFlush();
    program.disable();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool View::shouldWindowClose() {
    return glfwWindowShouldClose(window);
}

void View::closeWindow() {
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->cleanup();
        delete objects[i];
    }
    objects.clear();

    // Delete the VBO and VAO
    glDeleteBuffers(1, &circleVBO);
    glDeleteVertexArrays(1, &circleVAO);

    glfwDestroyWindow(window);
    glfwTerminate();
}