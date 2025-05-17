/// ==========================================================================
/// Contains code based on the Igneous Engine.
/// Copyright (c) MissingBitStudios
/// Original source available at https://github.com/MissingBitStudios/igneous,
/// licensed under CC-BY-4.0 (https://creativecommons.org/licenses/by/4.0/).
///
/// Modifications made by Marc for Load In Abyss.
/// This modified file, as part of Load In Abyss, is licensed under AGPLv3.
/// ==========================================================================

#ifndef INPUT_HPP
#define INPUT_HPP

#include <functional>

#include "GLFW/glfw3.h"

namespace Abyss::Input
{
    using input_callback = std::function<void(GLFWwindow*)>;

    /**
     * Initialize the input system with the given GLFW window
     * @param window The GLFW window to use for input handling
     */
    void init(GLFWwindow* window);

    /**
     * Callback function for mouse scroll events
     * @param window The GLFW window that received the event
     * @param xoffset The scroll offset in the x direction
     * @param yoffset The scroll offset in the y direction
     */
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    /**
     * Callback function for mouse button events
     * @param window The GLFW window that received the event
     * @param button The mouse button that was pressed or released
     * @param action The action (press or release)
     * @param mods Any modifier keys that were held down
     */
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    /**
     * Callback function for key events
     * @param window The GLFW window that received the event
     * @param key The key that was pressed or released
     * @param scancode The scancode of the key
     * @param action The action (press or release)
     * @param mods Any modifier keys that were held down
     */
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /**
     * Bind a callback function to be executed when a specific key is released
     * @param key The GLFW key code to bind
     * @param callback The function to call when the key is released
     */
    void bind(int key, input_callback callback);

    /**
     * Unbind a callback function from a specific key
     * @param key The GLFW key code to unbind
     */
    void unbind(int key);

    /**
     * Update the cursor position and mouse deltas
     */
    void updateCursor();

    /**
     * Set the visibility of the cursor
     * @param visible True to show cursor, false to hide it
     */
    void setCursorVisible(bool visible);

    /**
     * Get the visibility state of the cursor
     * @return True if the cursor is visible, false if it is hidden
     */
    bool getCursorVisible();

    extern GLFWwindow* m_window;
    extern bool keys[GLFW_KEY_LAST + 1];
    extern bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
    extern double scrollX, scrollY;
    extern double mouseX, mouseY;
    extern double mouseXDelta, mouseYDelta;
} // Abyss

#endif //INPUT_HPP
