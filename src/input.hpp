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
#include <string>

#include "GLFW/glfw3.h"

namespace Abyss::Input
{
    using input_callback = std::function<void(GLFWwindow*)>;

    void init(GLFWwindow* window);
    void setCursorVisible(bool visible);
    void setCursorPos(double xpos, double ypos);
    void windowSizeCallback(GLFWwindow* window, int width, int height);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void cursorEnterCallback(GLFWwindow* window, int entered);
    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void charModsCallback(GLFWwindow* window, unsigned int codepoint, int mods);
    void charCallback(GLFWwindow* window, unsigned int codepoint);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void bind(int key, input_callback callback);

    extern GLFWwindow* m_window;
    extern bool keys[GLFW_KEY_LAST + 1];
    extern bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
    extern double scrollX, scrollY;
    extern double mouseX, mouseY;
} // Abyss

#endif //INPUT_HPP
