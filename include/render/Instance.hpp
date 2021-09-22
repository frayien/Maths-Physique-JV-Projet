#ifndef MPJVP_INSTANCE
#define MPJVP_INSTANCE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <cstring>

class Instance
{
private:
    VkInstance m_instance;

    const std::vector<const char*> validationLayers = 
    {
        "VK_LAYER_KHRONOS_validation",
    };

    // Validation layers should only be enabled when in debug mode
    #ifdef NDEBUG
        const bool m_enableValidationLayers = false;
    #else
        const bool m_enableValidationLayers = true;
    #endif

public:
    Instance();
    virtual ~Instance();

    std::vector<VkPhysicalDevice> listPhysicalDevices();

    inline VkInstance & raw() { return m_instance; }

private:
    bool checkValidationLayerSupport();
};

#endif // MPJVP_INSTANCE
