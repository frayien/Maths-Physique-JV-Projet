#ifndef MPJVP_GRAPHICSPIPELINE
#define MPJVP_GRAPHICSPIPELINE

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

    VkPipeline m_pipeline;
    VkPipelineLayout m_pipelineLayout;
    
public:
    GraphicsPipeline(const std::shared_ptr<LogicalDevice> & logicalDevice, VkFormat imageFormat, VkFormat depthFormat, VkExtent2D extent, VkSampleCountFlagBits msaaSampleCount);
    virtual ~GraphicsPipeline();

    void recreate(VkFormat imageFormat, VkFormat depthFormat, VkExtent2D extent, VkSampleCountFlagBits msaaSampleCount);

    inline std::shared_ptr<DescriptorSetLayout> & getDescriptorSetLayout() { return m_descriptorSetLayout; }
    inline std::shared_ptr<RenderPass> & getRenderPass() { return m_renderPass; }

    inline VkPipeline & raw() { return m_pipeline; }
    inline VkPipelineLayout & rawLayout() { return m_pipelineLayout; }

private:
    void create(VkExtent2D extent, VkSampleCountFlagBits msaaSampleCount);
    void cleanup();

    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);
};

#endif // MPJVP_GRAPHICSPIPELINE
