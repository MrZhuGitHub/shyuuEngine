#include "graphicsPipeline.h"
#include "modelParser.h"
#include "camera.h"

namespace vulkanTutorial
{

    void GraphicsPipeLine::draw(VkCommandBuffer commandBuffer, std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GeometryParser>> geometryParsers)
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChainExtent.width;
        viewport.height = (float)swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        for (auto& geometryParser : geometryParsers) {
            std::vector<GeometryBuffer> geometryBuffers = geometryParser->getGeometryBuffers();
            for (auto& instance : geometryBuffers) {
                vkCmdBindVertexBuffers(commandBuffer,
                    std::get<0>(instance.cmdBindVertexBuffers),
                    std::get<1>(instance.cmdBindVertexBuffers), 
                    std::get<2>(instance.cmdBindVertexBuffers), 
                    std::get<3>(instance.cmdBindVertexBuffers));

                vkCmdBindIndexBuffer(commandBuffer,
                    std::get<0>(instance.cmdBindIndexBuffer),
                    std::get<1>(instance.cmdBindIndexBuffer),
                    std::get<2>(instance.cmdBindIndexBuffer));

                vkCmdBindDescriptorSets(commandBuffer,
                    std::get<0>(instance.cmdBindDescriptorSets),
                    std::get<1>(instance.cmdBindDescriptorSets), 
                    std::get<2>(instance.cmdBindDescriptorSets), 
                    std::get<3>(instance.cmdBindDescriptorSets), 
                    std::get<4>(instance.cmdBindDescriptorSets), 
                    std::get<5>(instance.cmdBindDescriptorSets), 
                    std::get<6>(instance.cmdBindDescriptorSets));

                vkCmdDrawIndexed(commandBuffer,
                    std::get<0>(instance.cmdDrawIndexed),
                    std::get<1>(instance.cmdDrawIndexed), 
                    std::get<2>(instance.cmdDrawIndexed), 
                    std::get<3>(instance.cmdDrawIndexed), 
                    std::get<4>(instance.cmdDrawIndexed));
            }
        }
    }

}