#ifndef _VULKAN_INSTANCE_H_
#define _VULKAN_INSTANCE_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <set>

namespace vulkanTutorial {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	class VulkanInstance {
		public:
			VulkanInstance(bool enbaleValidationLayers, std::string appName);
			~VulkanInstance();
			bool createInstance();
			bool setupDebugMessenger();
			bool pickPhysicalDevice();
			bool createLogicalDevice();

		private:
			bool checkValidationLayerSupport();
			std::vector<const char*> getRequiredExtensions();
			void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
			VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
			void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

			bool isDeviceSuitable(VkPhysicalDevice device);
			QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
			bool checkDeviceExtensionSupport(VkPhysicalDevice device);


		private:
			VkInstance mVulkanInstance;
			VkPhysicalDevice mPhysicalDevice;
			VkDevice mLogicalDevice;
			VkDebugUtilsMessengerEXT mDebugMessenger;
			VkQueue mGraphicsQueue;
			VkQueue mPresentQueue;

			bool mEnbaleValidationLayers;
			std::string mApplicationName;
	};
}

#endif