#ifndef MPJVP_BUFFER
#define MPJVP_BUFFER

#include <vulkan/vulkan_raii.hpp>

#include "render/LogicalDevice.hpp"

class Buffer
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;

    VkBuffer m_buffer;
    VkDeviceMemory m_bufferMemory;
    
public:
    Buffer(const std::shared_ptr<LogicalDevice> & logicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    virtual ~Buffer();

    void loadData(const void* data, VkDeviceSize size);

    inline VkBuffer & raw() { return m_buffer; }
};


#endif // MPJVP_BUFFER
