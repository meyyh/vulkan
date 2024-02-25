#pragma once
#include "config.h"

//this file exists to avoid circular imports with swapchain and engine

namespace vkUtil {
    struct SwapChainFrame {
		vk::Image image;
		vk::ImageView imageView;
	};
}