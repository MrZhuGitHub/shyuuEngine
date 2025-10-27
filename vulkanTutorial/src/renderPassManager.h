#ifndef _RENDER_PASS_MANAGER_H_
#define _RENDER_PASS_MANAGER_H_

#include "vulkanInstance.h"
#include "texture.h"

#include <vector>
#include <memory>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <future>

class RenderPass;

namespace vulkanTutorial {

    class DependencyNode {
        public:
            DependencyNode(uint32_t id);
            ~DependencyNode();

            void pushWriteNode(uint32_t id);
            void pushReadNode(uint32_t id);

            uint32_t getId() const;
            std::vector<uint32_t> getWriteNode() const;
            std::vector<uint32_t> getReadNode() const;

            static uint32_t GeneratedDependencyNodeId();

        private:
            uint32_t mId;
            std::vector<uint32_t> mWrite;
            std::vector<uint32_t> mRead;

            static uint32_t kDependencyNodeId;
    };

    class RenderPassNode : public DependencyNode {
        public:
            RenderPassNode(uint32_t id, std::shared_ptr<RenderPass> renderPass);
            ~RenderPassNode();

            std::shared_ptr<RenderPass> getRenderPass();

            std::vector<std::future<VkCommandBuffer>> RecordCommand();
            void ExecuteCommand(VkCommandBuffer PrimaryCommandBuffer);

        private:
            std::shared_ptr<RenderPass> mRenderPass;
            std::vector<std::future<VkCommandBuffer>> mSecondaryCommandBuffer;
    };

    class ResourceNode : public DependencyNode {
        public:
            ResourceNode(uint32_t id, std::shared_ptr<Texture> texture);
            ~ResourceNode();

            std::shared_ptr<Texture> getTexture();

            void ConfigureImageBarrier();
            void InstallImageBarrier(VkCommandBuffer PrimaryCommandBuffer);
        private:
            std::shared_ptr<Texture> mTexture;
            VkImageMemoryBarrier mImageBarrier;
    };

    class RenderPassManager {
        public:
            RenderPassManager();
            ~RenderPassManager();
            void pushDependencyNode(std::shared_ptr<DependencyNode> dependencyNode);
            bool sortDAG();
            void prepare();
            void execute();

        private:
            std::vector<std::shared_ptr<DependencyNode>> mDependencyNodes;
            std::vector<std::shared_ptr<DependencyNode>> mTopologicalSortKahn;
            VkCommandBuffer mPrimaryCommandBuffer;
    };
}

#endif