#include "Controller.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

Controller::Controller(Model& m,View& v) {
    model = m;
    view = v;
}

Controller::~Controller() {}

void Controller::run()
{
    view.init(this,model);
    while (!view.shouldWindowClose()) {
        view.display();
    }
    view.closeWindow();
    exit(EXIT_SUCCESS);
}

void Controller::onkey(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        // Increases inner circle radius on uppercase 'I' 
        if (key == GLFW_KEY_I && (mods & GLFW_MOD_SHIFT)) {
            model.updateInnerRadius(true);
            view.updateInnerCircle(model.getMeshes()[1]);
            view.updateCurve(model.getMeshes()[3]);
        }
        // Decreases inner circle radius on lowercase 'i'
        else if (key == GLFW_KEY_I && !(mods & GLFW_MOD_SHIFT)) {
            model.updateInnerRadius(false);
            view.updateInnerCircle(model.getMeshes()[1]);
            view.updateCurve(model.getMeshes()[3]);
        }
        // Toggles visibility of the seed curve on uppercase 'C' 
        else if (key == GLFW_KEY_C && (mods & GLFW_MOD_SHIFT)) {
            curveVisible = !curveVisible;
            view.setCurveVisibility(curveVisible);
        }
    }
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