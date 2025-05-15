/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 * https://github.com/jpcy/bgfx-minimal-example/blob/master/helloworld.cpp
 */
#include <iostream>
#include <cstdio>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#include "input.hpp"
#include "renderer.hpp"

using namespace Abyss;

static void glfw_errorCallback(const int error, const char *description)
{
	std::cerr << "GLFW error " << error << ": " << description << std::endl;
}

void windowSizeCallback([[maybe_unused]]GLFWwindow* window, const int width, const int height)
{
    renderer::resize(width, height);
}

static void quit_callback(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main()
{
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the
    // <b>lang</b> variable name to see how CLion can help you rename it.
    auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";
	// Create a GLFW window without an OpenGL context.
	glfwSetErrorCallback(glfw_errorCallback);
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow *window = glfwCreateWindow(1024, 768, "helloworld", nullptr, nullptr);
	if (!window)
		return 1;
    glfwSetWindowSizeCallback(window, windowSizeCallback);

	// Initialize bgfx using the native window handle and window resolution.
	bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND)
	{
		init.platformData.ndt = glfwGetWaylandDisplay();
		init.platformData.nwh = (void*)(uintptr_t)glfwGetWaylandWindow(window);
        init.platformData.type = bgfx::NativeWindowHandleType::Wayland;
	}
	else if (glfwGetPlatform() == GLFW_PLATFORM_X11)
	{
		init.platformData.ndt = glfwGetX11Display();
		init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
        init.platformData.type = bgfx::NativeWindowHandleType::Default;
	}
#elif BX_PLATFORM_OSX
	init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
	init.platformData.nwh = glfwGetWin32Window(window);
#endif
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	init.resolution.width = (uint32_t)width;
	init.resolution.height = (uint32_t)height;
	init.resolution.reset = BGFX_RESET_VSYNC;
    // BGFX Debug
    init.debug = true;

    renderer::init(init);
    Input::init(window);
    Input::bind(GLFW_KEY_ESCAPE, quit_callback);

    // Main loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		// Handle window resize.
		glfwGetWindowSize(window, &width, &height);

		renderer::render(width, height);
	}
    renderer::reset();
	glfwTerminate();
	return 0;
}