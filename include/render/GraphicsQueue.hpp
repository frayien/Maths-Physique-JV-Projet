#ifndef MPJVP_GRAPHICSQUEUE
#define MPJVP_GRAPHICSQUEUE

#include <vulkan/vulkan_raii.hpp>

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
