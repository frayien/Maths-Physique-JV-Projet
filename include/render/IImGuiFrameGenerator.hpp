#ifndef MPJVP_IIMGUIFRAMEGENERATOR
#define MPJVP_IIMGUIFRAMEGENERATOR

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

class IImGuiFrameGenerator
{
public:
    IImGuiFrameGenerator() = default;
    virtual ~IImGuiFrameGenerator() = default;

    virtual void createFrame() = 0;
};

#endif // MPJVP_IIMGUIFRAMEGENERATOR
