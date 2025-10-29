#ifndef _SWAPCHAIN_H_
#define _SWAPCHAIN_H_

#include "vulkanInstance.h"

namespace vulkanTutorial {
    class Texture;

    class SwapChain {
        public:
            SwapChain(unsigned int width, unsigned int height);
            ~SwapChain();

            VkSurfaceKHR create(std::shared_ptr<VulkanInstance> vulkanInstance);

            VkSwapchainKHR getSwapChain() {
                return swapChain;
            }

            std::shared_ptr<Texture> getTexture(int index) {
                return mTextures[index];
            }

        private:
            unsigned int mHeight;
            unsigned int mWidth;
            GLFWwindow* window;

            VkSwapchainKHR swapChain;

            std::vector<std::shared_ptr<Texture>> mTextures;
    };
}

#endif