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

// Returns the transformation matrices for each rounded corner of a square
std::vector<glm::mat4> getTransforms() {
    return {
        glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 1.0f)),  // Top-left corner
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),   // Top-right corner
        glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 1.0f)), // Bottom-right corner
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f))   // Bottom-left  
    };
}

void View::display() {
    program.enable();
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    int numSquares = 11;            // Number of rounded squares
    float initScale = 1.0f;         // Initial scale of the square
    float scaleDecrement = 0.1f;    // Scale decrement per square

    std::vector<glm::mat4> transforms = getTransforms();

    // Draws each square, alternating between black and white and decreasing in size
    for (int i = 0; i < numSquares; i++) {

        // Assigns the color of the current square
        glm::vec4 color;
        if (i % 2 == 0) color = glm::vec4(0, 0, 0, 1);
        else color = glm::vec4(1, 1, 1, 1);

        // Scales for the current square
        float currentScale = initScale - (i * scaleDecrement);

        // Applies transforms for each rounded corner of the current square
        for (int j = 0; j < 4; j++) {
            // Applies rotation
            modelview = glm::rotate(glm::mat4(1.0), (float)glfwGetTime() * 2, glm::vec3(0,0,1));
            // Applies scaling
            modelview = glm::scale(modelview, glm::vec3(currentScale, currentScale, 1.0f));
            // Applies appropriate transform
            modelview *= transforms[j];                                                           

            // Sends modelview matrix to GPU
            glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));

            // Sends projection matrix to GPU
            glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // Sends the color to the GPU
            glUniform4fv(shaderLocations.getLocation("vColor"), 1, glm::value_ptr(color));

            // Draws the object
            objects[j]->draw();
        }
    }

    glFlush();
    program.disable();
    glfwSwapBuffers(window);
    glfwPollEvents();
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