#ifndef MPJVP_DESCRIPTORSETS
#define MPJVP_DESCRIPTORSETS

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render/LogicalDevice.hpp"
#include "render/DescriptorSetLayout.hpp"
#include "render/DescriptorPool.hpp"
#include "render/Buffer.hpp"

class DescriptorSets
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;
    std::shared_ptr<DescriptorPool> m_descriptorPool;

    std::vector<std::unique_ptr<Buffer> > m_uniformBuffers;
    std::vector<VkDescriptorSet> m_descriptorSets;
    
public:
    DescriptorSets(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<DescriptorPool> & descriptorPool, const DescriptorSetLayout & descriptorSetLayout, VkDeviceSize bufferSize, size_t size);
    ~DescriptorSets();

    void recreate(const DescriptorSetLayout & descriptorSetLayout, VkDeviceSize bufferSize, size_t size);

    inline VkDescriptorSet getRaw(size_t i) const { return m_descriptorSets[i]; }
    inline const std::unique_ptr<Buffer> & getUniformBuffer(size_t i) const { return m_uniformBuffers[i]; }
    inline size_t size() const { return m_descriptorSets.size(); }

private:
    void create(const DescriptorSetLayout & descriptorSetLayout, VkDeviceSize bufferSize, size_t size);
};

#endif // MPJVP_DESCRIPTORSETS
