#ifndef MPJVP_FRAMEBUFFERS
#define MPJVP_FRAMEBUFFERS

#include "render/LogicalDevice.hpp"

#include "render/Image.hpp"
#include "render/ImageView.hpp"
#include "render/FrameBuffer.hpp"

class FrameBuffers
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;

    std::shared_ptr<Image> m_colorImage;
    std::shared_ptr<ImageView> m_colorImageView;
    std::shared_ptr<Image> m_depthImage;
    std::shared_ptr<ImageView> m_depthImageView;
    std::vector<std::unique_ptr<FrameBuffer> > m_frameBuffers;
    
public:
    FrameBuffers(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<GraphicsPipeline> & graphicsPipeline, const std::vector<std::shared_ptr<ImageView> > & imageViews, VkExtent2D extent, VkFormat colorFormat, VkFormat depthFormat);
    virtual ~FrameBuffers();

    void recreate(const std::shared_ptr<GraphicsPipeline> & graphicsPipeline, const std::vector<std::shared_ptr<ImageView> > & imageViews, VkExtent2D extent, VkFormat colorFormat, VkFormat depthFormat);

    inline FrameBuffer & operator[](size_t i) { return *(m_frameBuffers[i]); }
    inline size_t size() const { return m_frameBuffers.size(); }

private:
    void createFrameBuffers(const std::shared_ptr<GraphicsPipeline> & graphicsPipeline, const std::vector<std::shared_ptr<ImageView> > & imageViews, VkExtent2D extent);
    void createImages(VkExtent2D extent, VkFormat colorFormat, VkFormat depthFormat);
};

#endif // MPJVP_FRAMEBUFFERS
