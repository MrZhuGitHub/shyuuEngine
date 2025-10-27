#ifndef _VULKAN_INSTANCE_H_
#define _VULKAN_INSTANCE_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vulkanTutorial {
	class VulkanInstance {
		public:


		private:
			VkInstance mVulkanInstance;
			VkPhysicalDevice mPhysicalDevice;
			VkDevice mLogicalDevice;
			VkDebugUtilsMessengerEXT mDebugMessenger;
			VkQueue mGraphicsQueue;
			VkQueue mPresentQueue;
	};
}

#endif