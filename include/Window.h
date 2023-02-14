#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "core.h"
#include "Skeleton.h"
#include "Skin.h"
#include "Animation.h"
class Window {
public:
    // Window Properties
    static int width;
    static int height;
    static const char* windowTitle;
    static bool animating;

    // Objects to render
    static Skeleton* skel;
    static Skin* skin;
    static Animation* anim;

    // Shader Program
    static GLuint shaderProgram;

    // Act as Constructors and desctructors
    static bool initializeProgram();
    static bool initializeObjects(int filen, char* files[]);
    static void cleanUp();

    // for the Window
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);
    static void setupGUI(GLFWwindow* window);
    static void doGUI(Skeleton * skel);

    // update and draw functions
    static void idleCallback();
    static void displayCallback(GLFWwindow*);

    // helper to reset the camera
    static void resetCamera();

    // callbacks - for interaction
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double currX, double currY);
};