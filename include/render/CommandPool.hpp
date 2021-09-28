#ifndef MPJVP_COMMANDPOOL
#define MPJVP_COMMANDPOOL

#include <vulkan/vulkan_raii.hpp>

#include "render/LogicalDevice.hpp"

#include "render/Buffer.hpp"

class CommandPool
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;
    
    VkCommandPool m_commandPool;
    
public:
    CommandPool(const std::shared_ptr<LogicalDevice> & logicalDevice);
    virtual ~CommandPool();

    void copyBuffer(Buffer & src, Buffer & dest, VkDeviceSize size);

    inline VkCommandPool & raw() { return m_commandPool; }
};

#endif // MPJVP_COMMANDPOOL
