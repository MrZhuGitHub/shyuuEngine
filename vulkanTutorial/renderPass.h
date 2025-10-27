#ifndef _RENDER_PASS_H_
#define _RENDER_PASS_H_

#include "vulkanInstance.h"
#include "graphicsPipeline.h"
#include "frameBuffer.h"
#include "renderPassManager.h"

#include <vector>
#include <thread>
#include <future>

namespace vulkanTutorial {
    class RenderPass {
        public:
            virtual std::shared_ptr<DependencyNode> setup(std::shared_ptr<VulkanInstance> vulkanInstance, std::shared_ptr<FrameBuffer> frameBuffer) = 0;
            std::future<VkCommandBuffer> recordCommand();
            void setGraphicsPipeLine(std::shared_ptr<GraphicsPipeLine> graphicsPipeLine) {
                mGraphicsPipeLine = graphicsPipeLine;
            }
        private:
            void loop();
        private:
            std::shared_ptr<GraphicsPipeLine> mGraphicsPipeLine;
            std::shared_ptr<std::thread> mRecordTask;
            VkCommandPool mCommandPool;
    };

    class ShadowPass : public RenderPass {

    };

    class SsaoPass : public RenderPass {

    };

    class SsrPass : public RenderPass {

    };

    class ToneMapPass : public RenderPass {

    };

    class TaaPass : public RenderPass {

    };

    class LightPass : public RenderPass {

    };
}
#endif