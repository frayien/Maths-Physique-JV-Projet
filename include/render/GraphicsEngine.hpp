#ifndef MPJVP_GRAPHICSENGINE
#define MPJVP_GRAPHICSENGINE

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <set>
#include <vector>
#include <list>
#include <string>
#include <chrono>
#include <optional>
#include <limits>
#include <fstream>

#include "render/Window.hpp"
#include "render/ImGuiVulkan.hpp"

#include "render/IImGuiFrameGenerator.hpp"

#include "render/UniformBufferObjectCamera.hpp"
#include "render/UniformBufferObjectTransform.hpp"

#include "render/Camera.hpp"
#include "render/LightSource.hpp"
#include "render/BufferedShapes.hpp"
#include "render/shape/IShapeGenerator.hpp"

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() 
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class GraphicsEngine 
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

    std::unique_ptr<Window>                                  m_window;
    std::unique_ptr<vk::raii::Context>                       m_context;
    std::unique_ptr<vk::raii::Instance>                      m_instance;
    std::unique_ptr<vk::raii::SurfaceKHR>                    m_surface;
    std::unique_ptr<vk::raii::PhysicalDevice>                m_physicalDevice;
    vk::SampleCountFlagBits                                  m_msaaSampleCount = vk::SampleCountFlagBits::e1;
    std::unique_ptr<vk::raii::Device>                        m_device;
    std::unique_ptr<vk::raii::Queue>                         m_graphicsQueue;
    std::unique_ptr<vk::raii::Queue>                         m_presentQueue;
    std::unique_ptr<vk::raii::CommandPool>                   m_commandPool;
    std::unique_ptr<vk::raii::SwapchainKHR>                  m_swapchain;
    vk::Format                                               m_swapchainImageFormat;
    vk::Extent2D                                             m_swapchainExtent;
    std::vector<std::unique_ptr<vk::raii::ImageView> >       m_swapchainImageViews;
    std::unique_ptr<vk::raii::DescriptorSetLayout>           m_descriptorSetLayout;
    std::unique_ptr<vk::raii::RenderPass>                    m_renderPass;
    std::unique_ptr<vk::raii::PipelineLayout>                m_graphicsPipelineLayout;
    std::unique_ptr<vk::raii::Pipeline>                      m_graphicsPipeline;
    std::unique_ptr<vk::raii::Image>                         m_colorImage;
    std::unique_ptr<vk::raii::DeviceMemory>                  m_colorImageMemory;
    std::unique_ptr<vk::raii::ImageView>                     m_colorImageView;
    std::unique_ptr<vk::raii::Image>                         m_depthImage;
    std::unique_ptr<vk::raii::DeviceMemory>                  m_depthImageMemory;
    std::unique_ptr<vk::raii::ImageView>                     m_depthImageView;
    std::vector<std::unique_ptr<vk::raii::Framebuffer> >     m_frameBuffers;
    std::unique_ptr<vk::raii::DescriptorPool>                m_descriptorPool;
    std::vector<std::unique_ptr<vk::raii::Buffer> >          m_uniformBuffers;
    std::vector<std::unique_ptr<vk::raii::DeviceMemory> >    m_uniformBufferMemories;
    std::vector<std::unique_ptr<vk::raii::Buffer> >          m_uniformBufferDynamics;
    std::vector<std::unique_ptr<vk::raii::DeviceMemory> >    m_uniformBufferDynamicMemories;
    std::unique_ptr<vk::raii::DescriptorSets>                m_descriptorSets;
    std::unique_ptr<vk::raii::CommandBuffers>                m_commandBuffers;
    
    std::unique_ptr<ImGuiVulkan>                             m_imGuiVulkan;

    std::vector<std::unique_ptr<vk::raii::Semaphore> >       m_imageAvailableSemaphores;
    std::vector<std::unique_ptr<vk::raii::Semaphore> >       m_renderFinishedSemaphores;
    std::vector<std::unique_ptr<vk::raii::Fence> >           m_inFlightFences;
    std::vector<vk::raii::Fence*>           m_imagesInFlight;
    size_t                                                   m_currentFrame = 0;


    IImGuiFrameGenerator* m_imguiFrameGenerator;
    std::vector<std::unique_ptr<BufferedShapes>> m_bufferedShapes;

    Camera m_camera;
    LightSource m_lightSource;
    std::vector<Shape> m_shapes;

public:
    GraphicsEngine(IImGuiFrameGenerator* imguiFrameGenerator);
    virtual ~GraphicsEngine();

    bool windowShouldClose() const;
    void windowPollEvents();

    void clear();
    void draw(IShapeGenerator & shape);
    void display();

    inline Camera & getCamera() { return m_camera; }
    inline LightSource & getLightSource() { return m_lightSource; }
    inline Window & getWindow() { return *m_window; }
    
private:
    uint32_t acquireImage();
    void executeCommandBuffer(uint32_t imageIndex);

private:
    void loadBuffers(uint32_t currentImage);

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
    vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const;
    vk::Format findDepthFormat() const;
public:
    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;
private:
    // logical device initialization
    void initDevice();
    // command pool initialization
    void initCommandPool();
public:
    void copyBuffer(vk::raii::Buffer & src, vk::raii::Buffer & dest, vk::DeviceSize size) const;
    std::unique_ptr<vk::raii::Buffer> makeBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage) const;
private:
    // swapchain initialization
    vk::SurfaceFormatKHR chooseSwapSurfaceFormat();
    vk::PresentModeKHR chooseSwapPresentMode();
    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR & capabilities);
    uint32_t chooseSwapImageCount(const vk::SurfaceCapabilitiesKHR & capabilities);
    void initSwapchain();
    void recreateSwapchain();
    // descriptor set layout initialization
    void initDescriptorSetLayout();
    // render pass initialization
    void initRenderPass();
    // graphics pipeline initialization
    static std::vector<char> readFile(const std::string& filename);
    vk::raii::ShaderModule createShaderModule(const std::vector<char>& code);
    void initGraphicsPipeline();
    // framebuffers initialization
public:
    std::unique_ptr<vk::raii::Image> makeImage(vk::Extent2D extent, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage) const;
    std::unique_ptr<vk::raii::DeviceMemory> makeDeviceMemory(vk::MemoryRequirements memRequirements, vk::MemoryPropertyFlags memProperties) const;
    std::unique_ptr<vk::raii::ImageView> makeImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) const;
private:
    void initFramebuffers();
    // descriptor pool initialization
    void initDescriptorPool();
    // descriptor sets initialization
    void initDescriptorSets();
    void initDynamicBuffer(size_t i, size_t dynamicBufferSize);
    void updateDescriptorSet(size_t i);
    // command buffers initialization
    void initCommandBuffers();
    void recordCommandBuffer(size_t i);
private:
    // imgui initialization
    void initImGui();
    // sync objects initialization
    void initSyncObjects();
};

#endif // MPJVP_GRAPHICSENGINE
