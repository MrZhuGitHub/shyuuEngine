#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vulkanInstance.h"
#include "glm/glm.hpp"

namespace vulkanTutorial {
    class Camera{
        public:
            Camera();
            void updateViewMatrix();
            void updateProjectionMatrix();
            VkDescriptorSet getDescriptorSet();
            ~Camera();
        private:
            VkDescriptorSet mDescriptorSet;

            std::vector<VkBuffer> mUniformBuffers;
            std::vector<VkDeviceMemory> mUniformBuffersMemory;
            std::vector<void*> mUniformBuffersMapped;
    };
}

#endif