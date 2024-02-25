#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <optional>
#include <fstream>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_wayland.h>


/* Wayland code */
struct client_state {
    /* Globals */
    struct wl_display *wl_display = nullptr;
    struct wl_registry *wl_registry = nullptr;
    struct wl_shm *wl_shm = nullptr;
    struct wl_compositor *wl_compositor = nullptr;
    struct xdg_wm_base *xdg_wm_base = nullptr;
    /* Objects */
    struct wl_surface *wl_surface = nullptr;
    struct xdg_surface *xdg_surface = nullptr;
    struct xdg_toplevel *xdg_toplevel = nullptr;

    struct wp_single_pixel_buffer_manager_v1 *single_pixel_buffer_manager = nullptr;
	struct wp_viewporter *viewporter = nullptr;

	int width = 1280;
	int height = 720;
};

extern client_state state;

/*
* Including the prebuilt header from the lunarg sdk will load
* most functions, but not all.
* 
* Functions can also be dynamically loaded, using the call
* 
* PFN_vkVoidFunction vkGetInstanceProcAddr(
    VkInstance                                  instance,
    const char*                                 pName);

 or

 PFN_vkVoidFunction vkGetDeviceProcAddr(
	VkDevice                                    device,
	const char*                                 pName);

	We will look at this later, once we've created an instance and device.
*/