#ifndef _LOAD_MODEL_H_
#define _LOAD_MODEL_H_

#include <string>
#include <vector>
#include <optional>

#include "tiny_gltf.h"

#include "glm/glm.hpp"

namespace vulkanTutorial
{
    enum AttributeType {
        POSITION = 0,
        NORMAL,
        TEXCOORD_0,
        TEXCOORD_1,
        TEXCOORD_2,
        TEXCOORD_3,
        COLOR_0,
        TANGENT,
        JOINTS_0,
        WEIGHTS_0,
        MAX,
    };

    enum SamplerOption {
        CLAMP_TO_EDGE,
        MIRRORED_REPEAT,
        REPEAT,
        NEAREST,
        LINEAR,
        NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR,
    };

    struct Attribute {
        AttributeType type;
        void* rawData;
        unsigned int stride;
        unsigned int size;
        unsigned int numComponents;
    };

    struct Texture {
        unsigned int width;
        unsigned int height;
        unsigned int component;
        std::vector<unsigned char> rawData;
        
        std::optional<SamplerOption> wrapS;
        std::optional<SamplerOption> wrapT;
        std::optional<SamplerOption> minFilter;
        std::optional<SamplerOption> magFilter;

        unsigned int texCoord;
    };

    struct Index {
        void* rawData;
        unsigned int stride;
        unsigned int size;
    };

    struct Material {
        std::optional<Texture> baseColorTexture;
        std::optional<Texture> roughnessTexture;
        std::optional<Texture> normalTexture;
        std::optional<double> metallicFactor;
        std::optional<double> roughnessFactor;
        std::optional<glm::vec4> baseColorFactor;
        std::optional<glm::vec3> emissiveFactor;
    };

    struct Primitive
    {
        std::vector<Attribute> attributes;
        Material material;
        Index indices;
    };

    class ModelLoader {
        public:
            ModelLoader(std::string path);
            bool Load();
            std::vector<Primitive>& get();
            ~ModelLoader();

        private:
            void Parse();
            Index ParseIndex(const tinygltf::Accessor& accessor);
            vulkanTutorial::Material ParseMaterial(const tinygltf::Material& material);
            Attribute ParseAttribute(const tinygltf::Accessor& accessor);

        private:
            std::vector<Primitive> mPrimitives;
            std::string mPath;
            tinygltf::Model mModel;

            static tinygltf::TinyGLTF kTinyGltfLoader;
    };

}

#endif