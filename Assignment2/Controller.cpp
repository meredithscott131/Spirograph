#include "Controller.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

Controller::Controller(Model& m,View& v) {
    model = m;
    view = v;
}

Controller::~Controller()
{

}

void Controller::run()
{
    vector<util::PolygonMesh<VertexAttrib> > meshes = model.getMeshes();
    view.init(this, meshes, model);
    while (!view.shouldWindowClose()) {
        view.display();
    }
    view.closeWindow();
    exit(EXIT_SUCCESS);
}

void Controller::onkey(int key, int scancode, int action, int mods)
{
}

// Reshape window while maintaining the aspect ratio
void Controller::reshape(int width, int height) 
{
    glViewport(0, 0, width, height);

    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    float orthoHeight = 500.0f;

    if (aspectRatio >= 1.0f) {
        view.projection = glm::ortho(-orthoHeight * aspectRatio, orthoHeight * aspectRatio, -orthoHeight, orthoHeight);
    } else {
        view.projection = glm::ortho(-orthoHeight, orthoHeight, -orthoHeight / aspectRatio, orthoHeight / aspectRatio);
    }
}

void Controller::dispose()
{
    view.closeWindow();
}

void Controller::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}