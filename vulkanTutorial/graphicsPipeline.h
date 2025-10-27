#ifndef _GRAPHICS_PIPELINE_H_
#define _GRAPHICS_PIPELINE_H_

#include "vulkanInstance.h"

#include <vector>
#include <memory>
#include <string>

class Camera;
class GeometryParser;


namespace vulkanTutorial
{
    class GraphicsPipeLine {
        public:
            GraphicsPipeLine(VkPipeline pipeline);

            class Builder {
            public:
                Builder() noexcept;
                Builder(Builder const& rhs) noexcept;
                Builder(Builder&& rhs) noexcept;
                ~Builder() noexcept;
                Builder& operator=(Builder const& rhs) noexcept;
                Builder& operator=(Builder&& rhs) noexcept;

                Builder& shader(std::string vertex, std::string fragment);
                Builder& vertexInputBindingDescription(uint32_t count, VkVertexInputBindingDescription* bindingDescriptions);
                Builder& vertexInputAttributeDescription(uint32_t count, VkVertexInputAttributeDescription* attributeDescriptions);
                Builder& descriptorSetLayout(uint32_t count, VkDescriptorSetLayoutBinding* descriptorSetLayoutBindings);
                Builder& dynamicState(std::vector<VkDynamicState> dynamicStates);
                Builder& renderPass(VkRenderPass renderPass, uint32_t subPassId);
                std::shared_ptr<GraphicsPipeLine> build(std::shared_ptr<VulkanInstance> vulkanInstance);
            };

            VkPipeline get() {
                return mGraphicsPipeline;
            }

            void setViewport(VkCommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
            void setScissor(VkCommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

            virtual void draw(VkCommandBuffer commandBuffer, std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GeometryParser>> geometryParsers);

        private:
            VkPipeline mGraphicsPipeline;
    };
}

#endif
