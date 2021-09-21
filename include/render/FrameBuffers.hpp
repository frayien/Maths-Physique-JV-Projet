#ifndef FNES_FRAMEBUFFERS
#define FNES_FRAMEBUFFERS

#include "render/Image.hpp"
#include "render/ImageView.hpp"
#include "render/FrameBuffer.hpp"

class FrameBuffers
{
private:
    std::shared_ptr<Image> m_colorImage;
    std::shared_ptr<ImageView> m_colorImageView;
    std::shared_ptr<Image> m_depthImage;
    std::shared_ptr<ImageView> m_depthImageView;
    std::vector<std::shared_ptr<FrameBuffer> > m_frameBuffers;
    
public:
    FrameBuffers();
    virtual ~FrameBuffers();
};

#endif // FNES_FRAMEBUFFERS
