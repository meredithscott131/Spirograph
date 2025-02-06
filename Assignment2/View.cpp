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

void View::init(Callbacks *callbacks,vector<util::PolygonMesh<VertexAttrib> >& meshes) 
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(800, 800, "The Rotating Square Illusion", NULL, NULL);
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
    // assuming it got created, get all the shader variables that it uses
    // so we can initialize them at some point
    // enable the shader program
    program.enable();
    shaderLocations = program.getAllShaderVariables();

    
    //now we create an object that will be used to render this mesh in opengl
    /*
     * now we create an ObjectInstance for it.
     * The ObjectInstance encapsulates a lot of the OpenGL-specific code
     * to draw this object
     */

    /* so in the mesh, we have some attributes for each vertex. In the shader
     * we have variables for each vertex attribute. We have to provide a mapping
     * between attribute name in the mesh and corresponding shader variable
     name.
     *
     * This will allow us to use PolygonMesh with any shader program, without
     * assuming that the attribute names in the mesh and the names of
     * shader variables will be the same.

       We create such a shader variable -> vertex attribute mapping now
     */
    map<string, string> shaderVarsToVertexAttribs;

    // currently there are only two per-vertex attribute: position and color
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
}


void View::display() {
    program.enable();
    glClearColor(0, 0, 0, 1); // Set the background color to black
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

    drawOuterCircle();
    drawInnerCircle();
    drawSeed();

    glFlush(); // Finish rendering
    program.disable();
    glfwSwapBuffers(window); // Swap the buffer to display the result
    glfwPollEvents(); // Handle user input
}

void View::drawOuterCircle() {
    modelview = glm::mat4(1.0f);

    // Send the modelview matrix to GPU
    glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    // Send the projection matrix to GPU
    glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Send the color to the GPU
    // Set color to red
    glUniform4fv(shaderLocations.getLocation("vColor"), 1, glm::value_ptr(glm::vec4(1, 0, 0, 1)));

    objects[0]->draw();
}

void View::drawInnerCircle() {
    static float theta = 0.0f; // Angle of movement around the outer circle
    float R = 300.0f; // Outer circle radius
    float r = 200.0f; // Inner circle radius
    float speed = 0.05f; // Adjust for animation speed

    // Update the angle
    theta += speed;

    // Calculate the center of the rolling inner circle
    float C_x = (R - r) * cos(theta);
    float C_y = (R - r) * sin(theta);

    // Calculate the rotation angle of the inner circle to maintain rolling
    float phi = -((R - r) / r) * theta;

    // Apply transformations
    modelview = glm::mat4(1.0f);
    modelview = glm::translate(modelview, glm::vec3(C_x, C_y, 0.0f)); // Move to rolling position
    modelview = glm::rotate(modelview, phi, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate for rolling

    // Send matrices to GPU
    glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Set color
    glUniform4fv(shaderLocations.getLocation("vColor"), 1, glm::value_ptr(glm::vec4(1, 0.5, 0, 1)));

    objects[1]->draw();
}

void View::drawSeed() {
    static float theta = 0.0f; // Angle for the inner circle's center movement
    static float phi = 0.0f;   // Angle for the seed's rotation inside the inner circle

    float R = 400.0f; // Outer circle radius
    float r = 200.0f; // Inner circle radius
    float seedDistance = r / 2.0f; // Halfway between center and edge of inner circle
    float speed = 0.05f;

    // Update angles
    theta += speed;  // Inner circle moves around the outer circle
    phi += speed * (R / r); // Seed rotates inside the inner circle

    // Compute inner circle center
    float C_x = (R - r) * cos(theta);
    float C_y = (R - r) * sin(theta);

    // Compute seed point inside the inner circle
    float S_x = C_x + seedDistance * cos(phi); // Offset from inner circle center
    float S_y = C_y + seedDistance * sin(phi);

    modelview = glm::mat4(1.0f);
    modelview = glm::translate(modelview, glm::vec3(S_x, S_y, 0.0f)); // Move to seed point

    // Send transformations to GPU
    glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Set color (e.g., green)
    glUniform4fv(shaderLocations.getLocation("vColor"), 1, glm::value_ptr(glm::vec4(0, 1, 0, 1)));

    objects[2]->draw(); // Assuming the 3rd mesh is the seed point
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