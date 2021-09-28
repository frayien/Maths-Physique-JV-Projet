#ifndef MPJVP_GRAPHICSPIPELINE
#define MPJVP_GRAPHICSPIPELINE

#include <vulkan/vulkan_raii.hpp>

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

    VkPipeline m_pipeline;
    VkPipelineLayout m_pipelineLayout;
    
public:
    GraphicsPipeline(const std::shared_ptr<LogicalDevice> & logicalDevice, const DescriptorSetLayout & descriptorSetLayout, VkFormat imageFormat, VkFormat depthFormat, VkExtent2D extent, VkSampleCountFlagBits msaaSampleCount);
    virtual ~GraphicsPipeline();

    void recreate(const DescriptorSetLayout & descriptorSetLayout, VkFormat imageFormat, VkFormat depthFormat, VkExtent2D extent, VkSampleCountFlagBits msaaSampleCount);

    inline std::shared_ptr<RenderPass> & getRenderPass() { return m_renderPass; }

    inline VkPipeline & raw() { return m_pipeline; }
    inline VkPipelineLayout & rawLayout() { return m_pipelineLayout; }

private:
    void create(const DescriptorSetLayout & descriptorSetLayout, VkExtent2D extent, VkSampleCountFlagBits msaaSampleCount);
    void cleanup();

    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);
};

#endif // MPJVP_GRAPHICSPIPELINE
