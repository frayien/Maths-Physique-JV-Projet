#ifndef FNES_DESCRIPTORSETLAYOUT
#define FNES_DESCRIPTORSETLAYOUT

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

#include "render/LogicalDevice.hpp"

class DescriptorSetLayout
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;

    VkDescriptorSetLayout m_descriptorSetLayout;
    
public:
    DescriptorSetLayout(const std::shared_ptr<LogicalDevice> & logicalDevice);
    virtual ~DescriptorSetLayout();

    inline VkDescriptorSetLayout & raw() { return m_descriptorSetLayout; }
};

#endif // FNES_DESCRIPTORSETLAYOUT
