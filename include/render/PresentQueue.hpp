#ifndef MPJVP_PRESENTQUEUE
#define MPJVP_PRESENTQUEUE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class PresentQueue
{
private:
    VkQueue m_presentQueue;
    
public:
    PresentQueue();
    virtual ~PresentQueue();

    inline VkQueue & raw() { return m_presentQueue; }
};

#endif // MPJVP_PRESENTQUEUE
