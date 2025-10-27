#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "vulkanInstance.h"

namespace vulkanTutorial {
    class Texture {
        public:
            Texture();
            VkImage getImage();
            ~Texture();
        private:
		    VkImage mTextureImage;
		    VkDeviceMemory mTextureImageMemory;
		    VkImageView mTextureImageView;
		    VkSampler mTextureSampler;
    };
}

#endif