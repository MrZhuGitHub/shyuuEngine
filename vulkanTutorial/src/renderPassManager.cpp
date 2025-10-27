#include "renderPassManager.h"
#include "renderPass.h"

namespace vulkanTutorial {

uint32_t DependencyNode::kDependencyNodeId = 0;

DependencyNode::DependencyNode(uint32_t id)
    : mId(id) {

}

DependencyNode::~DependencyNode() {
    
}

uint32_t DependencyNode::getId() const {
    return mId;
}

std::vector<uint32_t> DependencyNode::getWriteNode() const {
    return mWrite;
}

std::vector<uint32_t> DependencyNode::getReadNode() const {
    return mRead;
}

void DependencyNode::pushWriteNode(uint32_t id) {
    mWrite.push_back(id);
}

void DependencyNode::pushReadNode(uint32_t id) {
    mRead.push_back(id);
}

uint32_t DependencyNode::GeneratedDependencyNodeId() {
    DependencyNode::kDependencyNodeId++;
    return DependencyNode::kDependencyNodeId;
}

RenderPassNode::RenderPassNode(uint32_t id, std::shared_ptr<RenderPass> renderPass)
    : DependencyNode(id)
    , mRenderPass(renderPass) {

    }

RenderPassNode::~RenderPassNode() {

}

std::vector<std::future<VkCommandBuffer>> RenderPassNode::RecordCommand() {
    mSecondaryCommandBuffer = mRenderPass->recordCommand();
    return mSecondaryCommandBuffer;
}

void RenderPassNode::ExecuteCommand(VkCommandBuffer PrimaryCommandBuffer) {
    VkRenderPassBeginInfo renderPassBeginInfo;
    VkCommandBuffer secondaryCommandBuffer = mSecondaryCommandBuffer.get();
    vkCmdBeginRenderPass(PrimaryCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
    vkCmdExecuteCommands(PrimaryCommandBuffer, 1, &secondaryCommandBuffer);
    vkCmdEndRenderPass(PrimaryCommandBuffer);
}

std::shared_ptr<RenderPass> RenderPassNode::getRenderPass() {
    return mRenderPass;
}

ResourceNode::ResourceNode(uint32_t id, std::shared_ptr<Texture> texture)
    : DependencyNode(id)
    , mTexture(texture) {

    }

ResourceNode::~ResourceNode() {

}

void ResourceNode::ConfigureImageBarrier() {
    mImageBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    mImageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    mImageBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    mImageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    mImageBarrier.image = mTexture->getImage();
}

void ResourceNode::InstallImageBarrier(VkCommandBuffer PrimaryCommandBuffer) {
    vkCmdPipelineBarrier(
        PrimaryCommandBuffer,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &mImageBarrier
    );
}

std::shared_ptr<Texture> ResourceNode::getTexture() {
    return mTexture;
}

RenderPassManager::RenderPassManager() {

}

RenderPassManager::~RenderPassManager() {

}

void RenderPassManager::pushDependencyNode(std::shared_ptr<DependencyNode> dependencyNode) {
    mDependencyNodes.push_back(dependencyNode);
}

bool RenderPassManager::sortDAG() {
    std::vector<std::shared_ptr<DependencyNode>> topologicalSortKahn;
    std::queue<std::shared_ptr<DependencyNode>> q;

    std::unordered_map<uint32_t, std::tuple<std::shared_ptr<DependencyNode>, int>> dependencyNodes;
    for (const auto& it : mDependencyNodes) {
        dependencyNodes.insert(std::make_pair(it->getId(), std::make_tuple(it, it->getReadNode().size())));
    }

    for (const auto& node : mDependencyNodes) {
        if (0 == node->getReadNode().size()) {
            q.push(node);
        }
    }

    while(!q.empty()) {
        std::shared_ptr<DependencyNode> node = q.front();
        q.pop();
        topologicalSortKahn.push_back(node);

        for (const auto& id : node->getReadNode()) {
            std::get<1>(dependencyNodes.at(id))--;
            if (0 == std::get<1>(dependencyNodes.at(id))) {
                q.push(std::get<0>(dependencyNodes.at(id)));
            }  
        }
    }

    if (topologicalSortKahn.size() == mDependencyNodes.size()) {
        mTopologicalSortKahn.clear();
        mTopologicalSortKahn = topologicalSortKahn;
        return true;
    } else {
        std::cout << "Graph has a cycle! Topological sort is impossible." << std::endl;
        return false;
    }
}
void RenderPassManager::prepare() {
    std::vector<std::future<VkCommandBuffer>> commandBuffers;
    for (const auto& node : mTopologicalSortKahn) {
        if (auto renderPassNode = std::dynamic_pointer_cast<RenderPassNode>(node)) {
            std::vector<std::future<VkCommandBuffer>> commandBuffer = renderPassNode->RecordCommand();
            commandBuffers.push_back(commandBuffer);
        }

        if (auto resourceNode = std::dynamic_pointer_cast<ResourceNode>(node)) {
            resourceNode->ConfigureImageBarrier();
        }
    }

    for (auto& commandBuffer : commandBuffers) {
        commandBuffer.get();
    }
}

void RenderPassManager::execute() {
    VkCommandBufferBeginInfo primaryBeginInfo;
    vkBeginCommandBuffer(mPrimaryCommandBuffer, &primaryBeginInfo);

    for (const auto& node : mTopologicalSortKahn) {
        if (auto renderPassNode = std::dynamic_pointer_cast<RenderPassNode>(node)) {
            renderPassNode->ExecuteCommand(mPrimaryCommandBuffer);
        }

        if (auto resourceNode = std::dynamic_pointer_cast<ResourceNode>(node)) {
            resourceNode->InstallImageBarrier(mPrimaryCommandBuffer);
        }
    }

    vkEndCommandBuffer(mPrimaryCommandBuffer);
    
}

}