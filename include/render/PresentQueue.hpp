#ifndef MPJVP_PRESENTQUEUE
#define MPJVP_PRESENTQUEUE

#include <vulkan/vulkan_raii.hpp>

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
