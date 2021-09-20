#include "render/VulkanApplication.hpp"

#include <iostream>

int main() 
{
    VulkanApplication app;

    try 
    {
        app.run();
    } 
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/*
#include "Vector3f.hpp"

int _main()
{
    // required to use ""s and ""ns suffixes
    using namespace std::chrono_literals;

    const std::chrono::nanoseconds TIMESTEP(1s);
    const float DELTA_TIME = TIMESTEP.count() / 1'000'000'000;
    
    using clock = std::chrono::high_resolution_clock;

    std::chrono::nanoseconds lag(0ns);
    auto base_time = clock::now();

    Vector3f acceleration{1, 2, 1};
    Vector3f speed{0, 0, 0};
    Vector3f position{0, 0, 0};

    while(true) {
        auto current_time = clock::now();
        auto frame_time = current_time - base_time;
        base_time = current_time;
        lag += std::chrono::duration_cast<std::chrono::nanoseconds>(frame_time);

        while(lag >= TIMESTEP) {
            lag -= TIMESTEP;

            speed += (acceleration * DELTA_TIME);
            position += speed * DELTA_TIME + (DELTA_TIME * DELTA_TIME * acceleration / 2);

            std::cout << "================" << std::endl;
            std::cout << "acceleration = " << acceleration << std::endl;
            std::cout << "speed = " << speed << std::endl;
            std::cout << "position = " << position << std::endl;
            std::cout << "================" << std::endl << std::endl;
        }
    }
}*/