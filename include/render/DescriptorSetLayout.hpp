#ifndef MPJVP_DESCRIPTORSETLAYOUT
#define MPJVP_DESCRIPTORSETLAYOUT

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <array>

#include "render/LogicalDevice.hpp"

class DescriptorSetLayout
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;

    VkDescriptorSetLayout m_descriptorSetLayout;

public:
    DescriptorSetLayout(const std::shared_ptr<LogicalDevice> & logicalDevice);
    virtual ~DescriptorSetLayout();

    inline const VkDescriptorSetLayout & raw() const { return m_descriptorSetLayout; }
    inline size_t getBindingCount() const { return 2; }
};

#endif // MPJVP_DESCRIPTORSETLAYOUT
