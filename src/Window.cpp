#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
Skeleton* Window::skel;
Skin* Window::skin;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Window::initializeObjects(int filen, char*  files[]) {

    skin = NULL;
    skel = NULL;
    Tokenizer token;
    char temp[256];
    
    bool result = true;
    for (int i = 0; i < filen; ++i) {
        if (files[i]) {
            token.Open(files[i]);
            token.GetToken(temp);
            if ( strcmp(temp, "balljoint") == 0) {
                //std::cout << "Foudn Skeleon\n\n";
                //Create a Skeleton
                skel = new Skeleton();
                result &= skel->Load(files[i], token);
            }
            else if (strcmp(temp, "positions") == 0) {

                std::cout << "Found skin: " << files[i] << "\n\n";
                skin = new Skin();
                result &= skin->Load(files[i], token);
            }
            // Finish
            token.Close();
        }

    }
    if (skin != NULL) {
        skin->myskel = skel;
    }

    return result;

}

void Window::cleanUp() {
    // Deallcoate the objects.
    if (skel != NULL) {
        delete skel;
    }
    if (skin != NULL) {
        delete skin;
    }

    //DeleteImgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete the shader program.
    glDeleteProgram(shaderProgram);
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    Cam = new Camera();
    Cam->SetAspect(float(width) / float(height));

    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;


    



    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);



    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    Cam->SetAspect(float(width) / float(height));
}

void Window::setupGUI(GLFWwindow* window)
{

    //Start GUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

// update and draw functions
void Window::idleCallback() {
    // Perform any updates as necessary.
    Cam->Update();

    if (skel != NULL) {
        skel->Update();
    }
    if (skin != NULL) {
        skin->Update();
    }
}

void Window::displayCallback(GLFWwindow* window) {

    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    // Render the objects.
    if (skel != NULL) {
        skel->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    if (skin != NULL) {
        skin->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();


    //ImGui New Frame
    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    //ImGui Rendering
    ImGui::Begin("DOF Editor");

    //io.AddFocusEvent(true);

    char buf[50];
    float f;
    //ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    //ImGui::SetKeyboardFocusHere();
    ImGui::Text("PLACEHOLDER");

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    
    // Swap buffers.
    glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*
     * TODO: Modify below to add your key callbacks.
     */

    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;

            default:
                break;
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            LeftDown = (action == GLFW_PRESS);
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            RightDown = (action == GLFW_PRESS);
        }
    }

}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    int maxDelta = 100;
    int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
    int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

    MouseX = (int)currX;
    MouseY = (int)currY;

    // Move camera
    // NOTE: this should really be part of Camera::Update()
    if (LeftDown) {
        const float rate = 1.0f;
        Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
        Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
    }
    if (RightDown) {
        const float rate = 0.005f;
        float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
        Cam->SetDistance(dist);
    }
}