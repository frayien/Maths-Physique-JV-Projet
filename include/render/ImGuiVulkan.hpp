#ifndef MPJVP_IMGUIVULKAN
#define MPJVP_IMGUIVULKAN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>
#include <array>

#include "render/Instance.hpp"
#include "render/LogicalDevice.hpp"
#include "render/PhysicalDevice.hpp"
#include "render/SwapChain.hpp"
#include "render/Window.hpp"
#include "Application.hpp"

class ImGuiVulkan
{
private:
    std::shared_ptr<Application>    m_application   ;
    std::shared_ptr<LogicalDevice>  m_logicalDevice ;
    std::shared_ptr<SwapChain>      m_swapChain     ;

    VkDescriptorPool m_imguiDescriptorPool;
    VkRenderPass m_imGuiRenderPass;
    VkCommandPool m_imGuiCommandPool;
    std::vector<VkCommandBuffer> m_imGuiCommandBuffers;
    std::vector<VkFramebuffer> m_imGuiFrameBuffers;

    char * projectiles[4] = { "Ball", "Heavy ball", "Laser", "Fireball" };
    std::vector<std::array<float, 3>> projectilesInitialVelocity =
    {
        {0.0f, 4.0f, 7.0f},
        {0.0f, 2.0f, 6.0f},
        {0.0f, 16.0f, 6.0f},
        {0.0f, 4.0f, 4.0f}
    };
    std::vector<float> projectilesMass = {4.0f, 12.0f, 0.2f, 2.0f};

    int currentIndex = 0;
    char* currentProjectile = projectiles[currentIndex];

    float currentMass = 1.0f;
    std::array<float, 3> currentInitialVelocity = { 0.0f,  4.0f,  7.0f};
    std::array<float, 3> currentInitialPosition = {-8.0f, -3.0f,  0.0f};
    float damping = 0.999f;

public:
    ImGuiVulkan(const std::shared_ptr<IApplication> & application, const std::shared_ptr<Window> & window, const std::shared_ptr<Instance> & instance, const std::shared_ptr<PhysicalDevice> & physicalDevice, const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<SwapChain> & swapChain);
    virtual ~ImGuiVulkan();

    void render(uint32_t m_imageIndex);
    void cleanup();
    void recreate();
    void createFrame();

    inline VkCommandBuffer getCommandBuffer(size_t i) { return m_imGuiCommandBuffers[i]; }

private:
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void createImGuiCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateFlags flags);
    void createImGuiCommandBuffers(VkCommandBuffer* commandBuffer, uint32_t commandBufferCount, VkCommandPool &commandPool);
};

#endif // MPJVP_IMGUIVULKAN
