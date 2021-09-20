#ifndef FNES_DESCRIPTORPOOL
#define FNES_DESCRIPTORPOOL

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

#include "render/Buffer.hpp"
#include "render/UniformBufferObject.hpp"

#include "render/LogicalDevice.hpp"

class DescriptorPool
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;

    VkDescriptorPool m_descriptorPool;
    size_t m_size;
    
public:
    DescriptorPool(const std::shared_ptr<LogicalDevice> & logicalDevice, size_t size);
    virtual ~DescriptorPool();

    std::vector<VkDescriptorSet> createDescriptorSets(std::vector<std::shared_ptr<Buffer> > & uniformBuffers, VkDescriptorSetLayout descriptorSetLayout);
};

#endif // FNES_DESCRIPTORPOOL
