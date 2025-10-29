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

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanInstance {
		public:
			VulkanInstance(bool enbaleValidationLayers, std::string appName);
			~VulkanInstance();

			bool init();

			VkDevice LogicalDevice() {
				return mLogicalDevice;
			}

			VkQueue GraphicsQueue() {
				return mGraphicsQueue;
			}

			VkQueue PresentQueue() {
				return mPresentQueue;
			}

			VkCommandPool createCommandPool();
			VkCommandBuffer beginSingleTimeCommands();
			void endSingleTimeCommands(VkCommandBuffer commandBuffer);

		private:
			bool createInstance();
			bool setupDebugMessenger();
			bool pickPhysicalDevice();
			bool createLogicalDevice();

			bool checkValidationLayerSupport();
			std::vector<const char*> getRequiredExtensions();
			void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
			VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
			void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

			bool isDeviceSuitable(VkPhysicalDevice device);
			QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
			bool checkDeviceExtensionSupport(VkPhysicalDevice device);
			SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		private:
			VkInstance mVulkanInstance;
			VkDebugUtilsMessengerEXT mDebugMessenger;

			VkPhysicalDevice mPhysicalDevice;
			QueueFamilyIndices mQueueFamilyIndices;

			VkDevice mLogicalDevice;
			VkQueue mGraphicsQueue;
			VkQueue mPresentQueue;

			bool mEnbaleValidationLayers;
			std::string mApplicationName;

			std::vector<VkCommandPool> mCommandPools;
			VkCommandPool mMainCommandPool;
	};
}

#endif