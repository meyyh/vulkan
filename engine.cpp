#include "engine.h"
#include "instance.h"
#include "logging.h"
#include "device.h"
#include "config.h"
#include "swapchain.h"
#include "pipeline.h"


//struct client_state state;

Engine::Engine() {

	if (debugMode) {
		std::cout << "Making a graphics engine\n";
	}

    make_instance();

	make_device();
}

void Engine::make_instance(){
	std::cout << "srfc-engine " << state.wl_surface << "\n";

	
	
	VkResult result;

    instance = vkInit::make_instance(debugMode, "wayland kill");
	dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);

	if (debugMode){
		debugMessenger = vkInit::make_debug_messenger(instance, dldi);
	}

	{
        VkWaylandSurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;


        createInfo.display = state.wl_display;
        createInfo.surface = state.wl_surface;
		std::cout << createInfo.surface << "!!!!!!!!!!!!!\n";
		std::cout << &state.wl_surface << "!!!!!!!!!!!!!engine.cpp\n";

        CHECK_VK_RESULT(vkCreateWaylandSurfaceKHR(instance, &createInfo, NULL, &vulkanSurface));
	}
}


void Engine::make_device() {

	physicalDevice = vkInit::choose_physical_device(instance, debugMode);
	device = vkInit::create_logical_device(physicalDevice, vulkanSurface ,debugMode);
	std::array<vk::Queue,2> queues = vkInit::get_queues(physicalDevice, device, vulkanSurface ,debugMode);
	graphicsQueue = queues[0];
	presentQueue = queues[1];
	vkInit::SwapChainBundle bundle = vkInit::create_swapchain(device, physicalDevice, vulkanSurface, state.width, state.height, debugMode);
	swapchain = bundle.swapchain;
	swapchainFrames = bundle.frames;
	swapchainFormat = bundle.format;
	swapchainExtent = bundle.extent;
	
}

void Engine::make_pipeline() {
	vkInit::GraphicsPipelineInBundle specification = {};

	vkInit::GraphicsPipelineOutBundle output = vkInit::make_graphics_pipeline(specification, debugMode);
}

Engine::~Engine() {

	if (debugMode) {
		std::cout << "Goodbye see you!\n";
	}

	for (vkUtil::SwapChainFrame frame : swapchainFrames) {
		device.destroyImageView(frame.imageView);
	}

	device.destroySwapchainKHR(swapchain);
	device.destroy();

	instance.destroySurfaceKHR(vulkanSurface);
	instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);

	/*
	* from vulkan_funcs.hpp:
	* 
	* void Instance::destroy( Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr,
                                            Dispatch const & d = ::vk::getDispatchLoaderStatic())
	*/
	instance.destroy();

	

}