#ifndef FNES_VULKANAPPLICATION
#define FNES_VULKANAPPLICATION

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <set>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <cstring>
#include <optional>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <array>
#include <chrono>
#include <memory>

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
    void run();

private:
    const int MAX_FRAMES_IN_FLIGHT = 2;


    std::shared_ptr<Window>         window         ;
    std::shared_ptr<Instance>       instance       ;
    std::shared_ptr<Surface>        surface        ;
    std::shared_ptr<PhysicalDevice> physicalDevice ;
    std::shared_ptr<LogicalDevice>  logicalDevice  ;
    std::shared_ptr<SwapChain>      swapChain      ;



    
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;


private:

    bool hasStencilComponent(VkFormat format);

    void createCommandBuffers();
    void createSyncObjects();
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void initVulkan();
    void cleanupSwapChain();
    void recreateSwapChain();
    void updateUniformBuffer(uint32_t currentImage);
    void drawFrame();
    void mainLoop();
    void cleanup();
};

#endif // FNES_VULKANAPPLICATION
