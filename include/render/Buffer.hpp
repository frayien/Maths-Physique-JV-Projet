#ifndef MPJVP_BUFFER
#define MPJVP_BUFFER

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"
#include "render/CommandPool.hpp"

class Buffer
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;
    std::shared_ptr<CommandPool> m_commandPool;

    VkBuffer m_buffer;
    VkDeviceMemory m_bufferMemory;
    
public:
    Buffer(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, PhysicalDevice & physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    virtual ~Buffer();

    void loadData(const void* data, VkDeviceSize size);
    void copyFrom(Buffer & src, VkDeviceSize size);

    inline VkBuffer & raw() { return m_buffer; }
};


#endif // MPJVP_BUFFER
