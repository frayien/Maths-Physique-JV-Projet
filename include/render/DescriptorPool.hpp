#ifndef MPJVP_DESCRIPTORPOOL
#define MPJVP_DESCRIPTORPOOL

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>

#include "render/LogicalDevice.hpp"

class DescriptorPool
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;

    VkDescriptorPool m_descriptorPool;
    
public:
    DescriptorPool(const std::shared_ptr<LogicalDevice> & logicalDevice, size_t size);
    virtual ~DescriptorPool();

    void recreate(size_t size);

    inline VkDescriptorPool raw() const { return m_descriptorPool; }

private:
    void cleanup();
    void createPool(size_t size);
};

#endif // MPJVP_DESCRIPTORPOOL
