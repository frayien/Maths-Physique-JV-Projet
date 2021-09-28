#ifndef MPJVP_VULKANAPPLICATION
#define MPJVP_VULKANAPPLICATION

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <chrono>

#include "render/Window.hpp"

#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"
#include "render/CommandPool.hpp"
#include "render/SwapChain.hpp"

#include "render/Vertex.hpp"
#include "render/World.hpp"
#include "render/IApplication.hpp"

#include "render/ImGuiVulkan.hpp"

class VulkanApplication 
{
private:
    const int MAX_FRAMES_IN_FLIGHT = 2;

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
    std::shared_ptr<PhysicalDevice> m_physicalDevice;
    std::shared_ptr<LogicalDevice>  m_logicalDevice ;
    std::shared_ptr<CommandPool>    m_commandPool   ;
    std::shared_ptr<World>          m_world         ;
    std::shared_ptr<SwapChain>      m_swapChain     ;
    std::shared_ptr<ImGuiVulkan>    m_imGuiVulkan   ;

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    std::vector<VkFence> m_imagesInFlight;
    size_t m_currentFrame = 0;

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
};

#endif // MPJVP_VULKANAPPLICATION
