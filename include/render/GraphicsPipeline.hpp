#ifndef FNES_GRAPHICSPIPELINE
#define FNES_GRAPHICSPIPELINE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <fstream>

#include "render/LogicalDevice.hpp"
#include "render/RenderPass.hpp"
#include "render/DescriptorSetLayout.hpp"

#include "render/Vertex.hpp"

class GraphicsPipeline
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;
    std::shared_ptr<RenderPass> m_renderPass;
    std::shared_ptr<DescriptorSetLayout> m_descriptorSetLayout;

    VkPipeline m_graphicsPipeline;
    VkPipelineLayout m_pipelineLayout;
    
public:
    GraphicsPipeline(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<RenderPass> & renderPass, const std::shared_ptr<DescriptorSetLayout> & descriptorSetLayout, VkExtent2D extent, VkSampleCountFlagBits msaaSampleCount);
    virtual ~GraphicsPipeline();

private:
    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);
};

#endif // FNES_GRAPHICSPIPELINE
