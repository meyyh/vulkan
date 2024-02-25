#pragma once
#include "config.h"
#include "frame.h"

class Engine {

public:

	Engine();

	~Engine();

private:

	//whether to print debug messages in functions
	bool debugMode = true;

    //vulkan instance things
    vk::Instance instance{ nullptr };
    vk::DebugUtilsMessengerEXT debugMessenger { nullptr };
	vk::DispatchLoaderDynamic dldi;
    VkSurfaceKHR vulkanSurface = VK_NULL_HANDLE;//Vk > vk:: as CHECK_VK_RESULT yells type diff

    //device things
    vk::PhysicalDevice physicalDevice{ nullptr };
    vk::Device device{ nullptr };
    vk::Queue graphicsQueue{ nullptr };
    vk::Queue presentQueue{ nullptr };
    vk::SwapchainKHR swapchain{ nullptr };
	std::vector<vkUtil::SwapChainFrame> swapchainFrames;
	vk::Format swapchainFormat;
	vk::Extent2D swapchainExtent;

    //pipline things

    //instance setup
    void make_instance();

    //device setup
    void make_device();

    //pipeline setup
    void make_pipeline();
};