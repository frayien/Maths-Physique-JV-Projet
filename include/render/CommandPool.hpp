#ifndef FNES_COMMANDPOOL
#define FNES_COMMANDPOOL

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render/LogicalDevice.hpp"

class CommandPool
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;
    
    VkCommandPool m_commandPool;
    
public:
    CommandPool(const std::shared_ptr<LogicalDevice> & logicalDevice, QueueFamilyIndices queueFamilyIndices);
    virtual ~CommandPool();

    inline VkCommandPool & raw() { return m_commandPool; }
};

#endif // FNES_COMMANDPOOL
