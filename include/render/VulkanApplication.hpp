#ifndef FNES_VULKANAPPLICATION
#define FNES_VULKANAPPLICATION

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#include "render/Window.hpp"
#include "render/Instance.hpp"
#include "render/Surface.hpp"
#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"
#include "render/SwapChain.hpp"

#include "render/Vertex.hpp"

class VulkanApplication 
{
public:
    VulkanApplication();
    virtual ~VulkanApplication();

    void run();

private:
    const int MAX_FRAMES_IN_FLIGHT = 2;

    std::shared_ptr<Window>         window        ;
    std::shared_ptr<Instance>       instance      ;
    std::shared_ptr<Surface>        surface       ;
    std::shared_ptr<PhysicalDevice> physicalDevice;
    std::shared_ptr<LogicalDevice>  logicalDevice ;
    std::shared_ptr<SwapChain>      swapChain     ;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

private:
    void updateUniformBuffer(uint32_t currentImage);
    void drawFrame();
};

#endif // FNES_VULKANAPPLICATION
