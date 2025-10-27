#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "vulkanInstance.h"

#include <vector>
#include <memory>
#include <tuple>

#include "modelLoader.h"
#include "texture.h"

#include "glm/glm.hpp"

namespace vulkanTutorial
{
	struct GeometryBuffer {

		std::tuple<uint32_t, uint32_t, const VkBuffer*, const VkDeviceSize*> cmdBindVertexBuffers;
		std::tuple<VkBuffer, VkDeviceSize, VkIndexType> cmdBindIndexBuffer;
		std::tuple<VkPipelineBindPoint, VkPipelineLayout, uint32_t, uint32_t, const VkDescriptorSet*, uint32_t, const uint32_t*> cmdBindDescriptorSets;
		std::tuple<uint32_t, uint32_t, uint32_t, int32_t, uint32_t> cmdDrawIndexed;

		/*
		//index
		VkBuffer mIndexBuffer;
		VkDeviceMemory mIndexBufferMemory;

		//vbo
		std::vector<VkBuffer> mVkBuffer;
		std::vector<VkDeviceMemory> mVertexBufferMemory;

		//ubo
		VkDescriptorSet mDescriptorSet;

		std::vector<VkBuffer> mUniformBuffers;
		std::vector<VkDeviceMemory> mUniformBuffersMemory;
		std::vector<void*> mUniformBuffersMapped;

		std::vector<Texture> mTextures;
		*/
	};

	class GeometryParser {
	public:
		GeometryParser();
		~GeometryParser();

		virtual bool Parse() = 0;
		virtual void AddInstance(const glm::mat4& modelMatrix) = 0;
		virtual void ClearInstance() = 0;

		std::vector<GeometryBuffer> getGeometryBuffers() {
			return mGeometryBuffers;
		}

	private:
		std::vector<GeometryBuffer> mGeometryBuffers;
	};

	class ModelParser : GeometryParser {
	public:
		ModelParser(std::shared_ptr<ModelLoader> modelLoader);
		~ModelParser();

		virtual bool Parse();
		virtual void AddInstance(const glm::mat4& modelMatrix);
		virtual void ClearInstance();

	private:
		std::shared_ptr<ModelLoader> mModelLoader;
	};
}


#endif