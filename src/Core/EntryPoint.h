#pragma once
#include "Application.h"

// 
// Forward declare in GLOBAL namespace
// The game implements this to return their Application
// 
extern Frost::Application* CreateApplication();

int main()
{
    Frost::Application* app = CreateApplication();
    app->Run();
    delete app;
    return 0;
}