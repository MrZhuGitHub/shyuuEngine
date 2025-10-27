
// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.

#include "modelLoader.h"

#include <iostream>
#include <map>

namespace vulkanTutorial
{

std::map<int, SamplerOption> kMapSamplerOption = {
    {TINYGLTF_TEXTURE_WRAP_REPEAT, REPEAT},
    {TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE, CLAMP_TO_EDGE},
    {TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT, MIRRORED_REPEAT},
    {TINYGLTF_TEXTURE_FILTER_NEAREST, NEAREST},
    {TINYGLTF_TEXTURE_FILTER_LINEAR, LINEAR},
    {TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST, NEAREST_MIPMAP_NEAREST},
    {TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST, LINEAR_MIPMAP_NEAREST},
    {TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR, NEAREST_MIPMAP_LINEAR},
    {TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR, LINEAR_MIPMAP_LINEAR},
};

std::map<std::string, AttributeType> kMapAttributeType = {
    {"POSITION", POSITION},
    {"NORMAL", NORMAL},
    {"TEXCOORD_0", TEXCOORD_0},
    {"TEXCOORD_1", TEXCOORD_1},
    {"TEXCOORD_2", TEXCOORD_2},
    {"TEXCOORD_3", TEXCOORD_3},
    {"COLOR_0", COLOR_0},
    {"TANGENT", TANGENT},
    {"JOINTS_0", JOINTS_0},
    {"WEIGHTS_0", WEIGHTS_0},
};

tinygltf::TinyGLTF ModelLoader::kTinyGltfLoader;

ModelLoader::ModelLoader(std::string path)
    : mPath(path) {

}

bool ModelLoader::Load() {
    std::string err;
    std::string warn;

    bool ret = false;
    if (mPath.find(".glb") != std::string::npos) {
        ret = ModelLoader::kTinyGltfLoader.LoadBinaryFromFile(&mModel, &err, &warn, mPath);
    } else {
        ret = ModelLoader::kTinyGltfLoader.LoadASCIIFromFile(&mModel, &err, &warn, mPath);
    }

    // 检查警告和错误
    if (!warn.empty()) {
        std::cout << "Warn for " << mPath << ": " << warn << std::endl;
        return false;
    }
    if (!err.empty()) {
        std::cerr << "Err for " << mPath << ": " << err << std::endl;
        return false;
    }
    if (!ret) {
        std::cerr << "Failed to load: " << mPath << std::endl;
        return false;
    }

    std::cout << mPath << " load meshes " << mModel.meshes.size() << std::endl;

    Parse();

    return true;
}

void ModelLoader::Parse() {
    for (size_t i = 0; i < mModel.meshes.size(); i++) {
        const tinygltf::Mesh &mesh = mModel.meshes[i];
        std::cout << "mesh.primitives.size() = " << mesh.primitives.size() << std::endl;
        for (size_t j = 0; j < mesh.primitives.size(); j++) {
            const tinygltf::Primitive &primitive = mesh.primitives[j];

            Primitive instance;

            if (primitive.indices >= 0) {
                const tinygltf::Accessor &indexAccessor = mModel.accessors[primitive.indices];
                instance.indices = ParseIndex(indexAccessor);
            }

            for (const auto &attr : primitive.attributes) {
                std::cout << "      Attribute: " << attr.first << " (accessor index: " << attr.second << ")" << std::endl;
                const tinygltf::Accessor &accessor = mModel.accessors[attr.second];
                auto result = ParseAttribute(accessor);
                result.type = kMapAttributeType.at(attr.first);
                instance.attributes.push_back(result);
            }

            if (primitive.material >= 0) {
                std::cout << "      Material index: " << primitive.material << std::endl;
                const tinygltf::Material &material = mModel.materials[primitive.material];
                instance.material = ParseMaterial(material);
            }

            mPrimitives.push_back(instance);
        }
    }
}

std::vector<Primitive>& ModelLoader::get() {
    return mPrimitives;
}

ModelLoader::~ModelLoader() {

}

Index ModelLoader::ParseIndex(const tinygltf::Accessor& accessor) {
    Index result;

    const tinygltf::BufferView& bufferView = mModel.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buffer = mModel.buffers[bufferView.buffer];
    const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

    result.rawData = (void*)dataPtr;
    result.size = accessor.count;
    result.stride = bufferView.byteStride;

    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) { 
        std::cout << "indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT, stride = " << result.stride << std::endl;
    }

    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
        std::cout << "indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT, stride = " << result.stride << std::endl;
    }

    return result;
}

vulkanTutorial::Material ModelLoader::ParseMaterial(const tinygltf::Material& material) {
    vulkanTutorial::Material result;
    const auto& pbr = material.pbrMetallicRoughness;

    result.baseColorFactor = glm::vec4(pbr.baseColorFactor[0], pbr.baseColorFactor[1], pbr.baseColorFactor[2], pbr.baseColorFactor[3]);
    result.metallicFactor = pbr.metallicFactor;
    result.roughnessFactor = pbr.roughnessFactor;
    result.emissiveFactor = glm::vec3(material.emissiveFactor[0], material.emissiveFactor[1], material.emissiveFactor[2]);
    
    if (pbr.baseColorTexture.index > 0) {
        Texture texture;
        const tinygltf::Texture& gltfTexture = mModel.textures[pbr.baseColorTexture.index];
        const tinygltf::Image& image = mModel.images[gltfTexture.source];

        texture.width = image.width;
        texture.height = image.height;
        texture.rawData = image.image;
        texture.component = image.component;

        texture.texCoord = pbr.baseColorTexture.texCoord;

        if (gltfTexture.sampler >= 0 && gltfTexture.sampler < mModel.samplers.size()) {
            tinygltf::Sampler sampler = mModel.samplers[gltfTexture.sampler];
            
            texture.wrapS = kMapSamplerOption.at(sampler.wrapS);
            texture.wrapT = kMapSamplerOption.at(sampler.wrapT);
            texture.minFilter = kMapSamplerOption.at(sampler.minFilter);
            texture.magFilter = kMapSamplerOption.at(sampler.magFilter);
            
        }

        result.baseColorTexture = texture;
    }

    if (pbr.metallicRoughnessTexture.index > 0) {
        Texture texture;
        const tinygltf::Texture& gltfTexture = mModel.textures[pbr.metallicRoughnessTexture.index];
        const tinygltf::Image& image = mModel.images[gltfTexture.source];

        texture.width = image.width;
        texture.height = image.height;
        texture.rawData = image.image;
        texture.component = image.component;

        texture.texCoord = pbr.metallicRoughnessTexture.texCoord;

        if (gltfTexture.sampler >= 0 && gltfTexture.sampler < mModel.samplers.size()) {
            tinygltf::Sampler sampler = mModel.samplers[gltfTexture.sampler];

            texture.wrapS = kMapSamplerOption.at(sampler.wrapS);
            texture.wrapT = kMapSamplerOption.at(sampler.wrapT);
            texture.minFilter = kMapSamplerOption.at(sampler.minFilter);
            texture.magFilter = kMapSamplerOption.at(sampler.magFilter);

        }

        result.roughnessTexture = texture;
    }

    if (material.normalTexture.index > 0) {
        Texture texture;
        const tinygltf::Texture& gltfTexture = mModel.textures[material.normalTexture.index];
        const tinygltf::Image& image = mModel.images[gltfTexture.source];

        texture.width = image.width;
        texture.height = image.height;
        texture.rawData = image.image;
        texture.component = image.component;

        texture.texCoord = material.normalTexture.texCoord;

        if (gltfTexture.sampler >= 0 && gltfTexture.sampler < mModel.samplers.size()) {
            tinygltf::Sampler sampler = mModel.samplers[gltfTexture.sampler];

            texture.wrapS = kMapSamplerOption.at(sampler.wrapS);
            texture.wrapT = kMapSamplerOption.at(sampler.wrapT);
            texture.minFilter = kMapSamplerOption.at(sampler.minFilter);
            texture.magFilter = kMapSamplerOption.at(sampler.magFilter);

        }

        result.normalTexture = texture;
    }

    return result;
}

Attribute ModelLoader::ParseAttribute(const tinygltf::Accessor& accessor) {
    Attribute result;

    const tinygltf::BufferView& bufferView = mModel.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buffer = mModel.buffers[bufferView.buffer];
    const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

    result.rawData = (void*)dataPtr;
    result.stride = bufferView.byteStride;
    result.size = accessor.count;
    result.numComponents = tinygltf::GetNumComponentsInType(accessor.type);

    std::cout << "Address of char: " << (void*)buffer.data.data()
        << "\t" << bufferView.byteOffset
        << "\t" << accessor.byteOffset
        << "\t" << bufferView.byteStride
        << "\t" << accessor.count
        << "\t" << result.numComponents << std::endl;

    return result;
}

}