#ifndef MPJVP_GRAPHICSQUEUE
#define MPJVP_GRAPHICSQUEUE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

class GraphicsQueue
{
private:
    VkQueue m_graphicsQueue;

public:
    GraphicsQueue();
    virtual ~GraphicsQueue();

    inline VkQueue & raw() { return m_graphicsQueue; }
};



#endif // MPJVP_GRAPHICSQUEUE
