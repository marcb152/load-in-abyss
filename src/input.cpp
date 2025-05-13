/// ==========================================================================
/// Contains code based on the Igneous Engine.
/// Copyright (c) MissingBitStudios
/// Original source available at https://github.com/MissingBitStudios/igneous,
/// licensed under CC-BY-4.0 (https://creativecommons.org/licenses/by/4.0/).
///
/// Modifications made by Marc for Load In Abyss.
/// This modified file, as part of Load In Abyss, is licensed under AGPLv3.
/// ==========================================================================

#include "input.hpp"

#include "renderer.hpp"
#include <unordered_map>

namespace Abyss::Input
{
    // Map to store key-callback pairs
    static std::unordered_map<int, input_callback> keyBindings;
    
    void init(GLFWwindow* window)
    {
        m_window = window;

        // Setup input callbacks
        glfwSetKeyCallback(m_window, keyCallback);
        glfwSetCharCallback(m_window, charCallback);
        glfwSetCharModsCallback(m_window, charModsCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetCursorPosCallback(m_window, cursorPosCallback);
        glfwSetCursorEnterCallback(m_window, cursorEnterCallback);
        glfwSetScrollCallback(m_window, scrollCallback);
        glfwSetWindowSizeCallback(m_window, windowSizeCallback);
    }

    // Input callbacks
	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (key != GLFW_KEY_UNKNOWN)
        {
            if (action == GLFW_PRESS)
            {
                keys[key] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                keys[key] = false;
                // Execute the bound callback if it exists
                auto it = keyBindings.find(key);
                if (it != keyBindings.end() && it->second != nullptr)
                {
                    it->second(window);
                }
            }
        }
    }
    
    void bind(int key, input_callback callback)
    {
        // TODO: That's bad code, refactor it with std::move (aka perfect forwarding)
        keyBindings[key] = callback;
    }

    void charCallback(GLFWwindow* window, unsigned int codepoint)
	{
	}

	void charModsCallback(GLFWwindow* window, unsigned int codepoint, int mods)
	{
	}

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST)
		{
			if (action == GLFW_PRESS)
			{
				mouseButtons[button] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				mouseButtons[button] = false;
			}
		}
	}

	void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		mouseX = xpos;
		mouseY = ypos;
	}

	void cursorEnterCallback(GLFWwindow* window, int entered)
	{
	}

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		scrollX += (float)xoffset;
		scrollY += (float)yoffset;
	}

	void windowSizeCallback(GLFWwindow* window, int width, int height)
	{
		// renderer::reset();
	}

	void setCursorPos(double xpos, double ypos)
	{
		glfwSetCursorPos(m_window, xpos, ypos);
	}

	void setCursorVisible(bool visible)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

    GLFWwindow* m_window;
    bool keys[GLFW_KEY_LAST + 1] = { 0 };
    bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
    double scrollX, scrollY = 0;
    double mouseX, mouseY = 0;
} // Abyss