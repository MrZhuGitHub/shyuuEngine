#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#include "modelParser.h"

#include "modelLoader.h"

namespace vulkanTutorial {

	GeometryParser::GeometryParser() {

	}

	GeometryParser::~GeometryParser() {

	}

	ModelParser::ModelParser(std::shared_ptr<ModelLoader> modelLoader)
		: mModelLoader(modelLoader)
		, GeometryParser() {

	}

	ModelParser::~ModelParser() {

	}

	bool ModelParser::Parse() {
		std::vector<Primitive>& primitives = mModelLoader->get();
		for (auto& primitive : primitives) {
			GeometryBuffer geometryBuffer;
			//parse index
			
			//parse attribute

			//parse material

			mGeometryBuffers.push_back(geometryBuffer);
		}
	}

	void ModelParser::AddInstance(const glm::mat4& modelMatrix) {

	}

	void ModelParser::ClearInstance() {

	}
}