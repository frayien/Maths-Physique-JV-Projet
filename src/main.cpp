#include <iostream>

#include "render/GraphicsEngine.hpp"
#include "TestApplication.hpp"
#include "Application.hpp"

int main()
{
    //GraphicsEngine app{std::make_shared<TestApplication>()};
    GraphicsEngine graphics{std::make_shared<Application>()};

    try 
    {
        graphics.run();
    } 
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
