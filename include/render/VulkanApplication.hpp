#ifndef MPJVP_VULKANAPPLICATION
#define MPJVP_VULKANAPPLICATION

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <set>
#include <string>
#include <chrono>
#include <optional>

#include "render/Window.hpp"
/*
#include "render/CommandPool.hpp"
#include "render/SwapChain.hpp"

#include "render/Vertex.hpp"
#include "render/World.hpp"
#include "render/IApplication.hpp"

#include "render/ImGuiVulkan.hpp"

*/

class IApplication{};

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() 
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class VulkanApplication 
{
private:
    const int MAX_FRAMES_IN_FLIGHT = 2;

    const std::vector<const char*> m_deviceExtensions = 
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    const std::vector<const char*> m_validationLayers = 
    {
        "VK_LAYER_KHRONOS_validation",
    };

    // Validation layers should only be enabled when in debug mode
    #ifdef MPVJP__DEBUG
        const bool m_enableValidationLayers = true;
    #else
        const bool m_enableValidationLayers = false;
    #endif

    std::shared_ptr<Window>                   m_window;
    std::shared_ptr<vk::raii::Context>        m_context;
    std::shared_ptr<vk::raii::Instance>       m_instance;
    std::shared_ptr<vk::raii::SurfaceKHR>     m_surface;
    std::shared_ptr<vk::raii::PhysicalDevice> m_physicalDevice;
    vk::SampleCountFlagBits                   m_msaaSampleCount = vk::SampleCountFlagBits::e1;
    std::shared_ptr<vk::raii::Device>         m_device;
    std::shared_ptr<vk::raii::Queue>          m_graphicsQueue;
    std::shared_ptr<vk::raii::Queue>          m_presentQueue;
    std::shared_ptr<vk::raii::CommandPool>    m_commandPool;

    //std::shared_ptr<World>          m_world         ;
    //std::shared_ptr<SwapChain>      m_swapChain     ;
    //std::shared_ptr<ImGuiVulkan>    m_imGuiVulkan   ;

    //std::vector<VkSemaphore> m_imageAvailableSemaphores;
    //std::vector<VkSemaphore> m_renderFinishedSemaphores;
    //std::vector<VkFence> m_inFlightFences;
    //std::vector<VkFence> m_imagesInFlight;
    //size_t m_currentFrame = 0;

    std::vector<bool> m_needRecord;

    std::shared_ptr<IApplication> m_application;

public:
    VulkanApplication(const std::shared_ptr<IApplication> & application);
    virtual ~VulkanApplication();

    void run();

private:
    void update(uint32_t currentImage);
    void drawFrame();

    // window initialization
    void initWindow();
    // context initialization
    void initContext();
    // instance initialization
    bool checkValidationLayerSupport();
    void initInstance();
    // surface initialization
    void initSurface();
    // physical device initialization
    QueueFamilyIndices findQueueFamilies(const vk::raii::PhysicalDevice & device);
    bool checkDeviceExtensionSupport(const vk::raii::PhysicalDevice & device);
    bool isDeviceSuitable(const vk::raii::PhysicalDevice & device);
    vk::SampleCountFlagBits getMaxUsableSampleCount(const vk::raii::PhysicalDevice & device);
    void initPhysicalDevice();
    // logical device initialization
    void initDevice();
    // command pool initialization
    void initCommandPool();
    void copyBuffer(vk::raii::Buffer & src, vk::raii::Buffer & dest, vk::DeviceSize size);
};

#endif // MPJVP_VULKANAPPLICATION
