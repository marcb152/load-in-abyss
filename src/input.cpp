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

#include <unordered_map>

namespace Abyss::Input
{
    // Map to store key-callback pairs
    static std::unordered_map<int, input_callback> keyBindings;
    static double prevMouseX = 0.0, prevMouseY = 0.0;
    
    void init(GLFWwindow* window)
    {
        m_window = window;

        // Setup input callbacks
        glfwSetKeyCallback(m_window, keyCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetScrollCallback(m_window, scrollCallback);
    }

    // Input callbacks
	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (key != GLFW_KEY_UNKNOWN)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
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

	void mouseButtonCallback([[maybe_unused]]GLFWwindow* window, int button, int action, int mods)
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

	void scrollCallback([[maybe_unused]]GLFWwindow* window, double xoffset, double yoffset)
	{
		scrollX += static_cast<float>(xoffset);
		scrollY += static_cast<float>(yoffset);
	}

    void bind(const int key, input_callback callback)
    {
        // Check if the key is valid
        if (key < 0 || key > GLFW_KEY_LAST)
            return;
        // TODO: That's bad code, refactor it with std::move (aka perfect forwarding)
        keyBindings[key] = std::move(callback);
    }

    void unbind(const int key)
    {
        // Check if the key is valid
        if (key < 0 || key > GLFW_KEY_LAST)
            return;
        if (const auto it = keyBindings.find(key); it != keyBindings.end())
        {
            keyBindings.erase(it);
        }
    }

    void updateCursor()
    {
        glfwGetCursorPos(m_window, &mouseX, &mouseY);
        mouseXDelta = mouseX - prevMouseX;
        mouseYDelta = mouseY - prevMouseY;
        prevMouseX = mouseX;
        prevMouseY = mouseY;
    }

	void setCursorVisible(bool visible)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        // Get Raw mouse motion when cursor is hidden
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, visible ? GLFW_FALSE : GLFW_TRUE);
    }

    bool getCursorVisible()
    {
        switch (glfwGetInputMode(m_window, GLFW_CURSOR))
        {
            case GLFW_CURSOR_NORMAL:
                return true;
            case GLFW_CURSOR_DISABLED:
            default:
                return false;
        }
    }

    GLFWwindow * m_window;
    bool keys[GLFW_KEY_LAST + 1] = {0};
    bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = {0};
    double scrollX = 0, scrollY = 0;
    double mouseX = 0, mouseY = 0;
    double mouseXDelta = 0, mouseYDelta = 0;
} // Abyss