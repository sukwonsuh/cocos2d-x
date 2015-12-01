#include "AppDelegate.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <app.h>

USING_NS_CC;

int main(int argc, char **argv)
{
    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run(argc, argv);
}
