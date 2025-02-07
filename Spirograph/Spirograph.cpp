#include "Model/Model.h"
#include "View/View.h"
#include "Controller/Controller.h"

int main(int argc,char *argv[])
{
    Model model;
    View view;
    Controller controller(model,view);
    controller.run();
}