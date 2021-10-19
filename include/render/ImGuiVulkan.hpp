#ifndef MPJVP_IMGUIVULKAN
#define MPJVP_IMGUIVULKAN

#include <vulkan/vulkan_raii.hpp>

#include <memory>
#include <vector>
#include <array>

#include "render/Window.hpp"
#include "Application.hpp"

class ImGuiVulkan
{
private:
    std::shared_ptr<Application>            m_application;
    std::shared_ptr<vk::raii::Device>       m_device;

    uint32_t m_graphicsFamily;
    vk::Extent2D m_swapchainExtent;
    size_t m_swapchainSize;

    std::unique_ptr<vk::raii::DescriptorPool>            m_imguiDescriptorPool;
    std::unique_ptr<vk::raii::RenderPass>                m_imGuiRenderPass;
    std::unique_ptr<vk::raii::CommandPool>               m_imGuiCommandPool;
    std::unique_ptr<vk::raii::CommandBuffers>            m_imGuiCommandBuffers;
    std::vector<std::unique_ptr<vk::raii::Framebuffer> > m_imGuiFrameBuffers;

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
    std::array<float, 3> currentAnchorPosition = { -8.0f,  0.0f,  0.0f};
    float currentSpringStiffness = 20.0f;
    float currentSpringRestLength = 2.0f;
    float currentK1DragCoef = 0.0f;
    float currentK2DragCoef = 0.1f;

public:
    ImGuiVulkan(
        const std::shared_ptr<IApplication> & application, 
        const std::shared_ptr<Window> & window, 
        const std::shared_ptr<vk::raii::Instance> & instance, 
        const std::shared_ptr<vk::raii::PhysicalDevice> & physicalDevice, 
        const std::shared_ptr<vk::raii::Device> & device,
        const std::shared_ptr<vk::raii::Queue> & graphicsQueue,
        const std::vector<std::shared_ptr<vk::raii::ImageView> > & swapchainImageViews,
        vk::Extent2D swapchainExtent,
        vk::Format imageFormat,
        uint32_t graphicsFamily);
    virtual ~ImGuiVulkan();

    void render(uint32_t m_imageIndex);
    void recreate(const std::vector<std::shared_ptr<vk::raii::ImageView> > & swapchainImageViews, vk::Extent2D swapchainExtent, vk::Format imageFormat);
    void createFrame();

    inline const vk::raii::CommandBuffer & getCommandBuffer(size_t i) { return (*m_imGuiCommandBuffers)[i]; }

private:
    std::shared_ptr<vk::raii::CommandBuffers> beginSingleTimeCommands();
    void endSingleTimeCommands(const std::shared_ptr<vk::raii::CommandBuffers> & commandBuffer, const std::shared_ptr<vk::raii::Queue> & graphicsQueue);
    void createImGuiCommandPool();
    void createImGuiCommandBuffers();
};

#endif // MPJVP_IMGUIVULKAN
