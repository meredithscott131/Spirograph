#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "../View/View.h"
#include "../Model/Model.h"
#include "Callbacks.h"

class Controller: public Callbacks
{
public:
    Controller(Model& m,View& v);
    ~Controller();
    void run();

    virtual void reshape(int width, int height);
    virtual void dispose();
    virtual void onkey(int key, int scancode, int action, int mods);
    virtual void error_callback(int error, const char* description);
private:
    View view;
    Model model;
    bool curveVisible = true;   // Visibility of the seed curve
};

#endif