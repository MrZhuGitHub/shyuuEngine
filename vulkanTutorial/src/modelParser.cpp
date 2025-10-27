#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#include "modelParser.h"

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

	}

	void ModelParser::AddInstance(const glm::mat4& modelMatrix) {

	}

	void ModelParser::ClearInstance() {

	}
}