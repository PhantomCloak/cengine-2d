#include "render.h"
#include "glm/glm.hpp"
#include "opengl/GLShape.h"
#include "opengl/primitives.h"
#include "opengl/texture.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <unordered_map>

GLFWwindow* k_window;
std::string driverStr;
int nextFontId = 0;
glm::mat4 proj;

std::unordered_map<int, GLShape*> glShapes;
std::unordered_map<int, Shader> glShaders;
std::unordered_map<int, Texture> glTextures;

Texture* dummyTexture;

float pixelCordToUvY(float y, int height) {
    return y / height;
}

float pixelCordToUvX(float x, int width) {
    return x / width;
}

int GenerateTextureFromText(std::string text, int fontId, CommancheColorRGB textColor) {
    return -1;
}

int CommancheRenderer::LoadFont(const std::string& path, int fontSize) {
    return -1;
}

void CommancheRenderer::Initialize(const std::string& title, int windowWidth, int windowHeight) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    k_window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);

    wnd = k_window;
    if (k_window == NULL) {
        Log::Err("Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(k_window);
    gladLoadGL();

    glfwSetFramebufferSizeCallback(
    k_window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    const float aspectRatio = (float)16 / (float)9;
    const float w = 100.0f;
    const float h = w / aspectRatio;

    proj = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
}

void CommancheRenderer::InitializeShaders(const std::string& defaultShaderPath) {

    std::string vert = defaultShaderPath + "/default.vert";
    std::string frag = defaultShaderPath + "/default.frag";
    
    Shader defaultShader = Shader(vert.c_str(), frag.c_str());

    assert(!defaultShader.compiledSuccessfully);

    glShaders[DEFAULT_SHADER_SLOT] = defaultShader;
    glShapes[RECT_PRIMITIVE] = new GLShape(glRectVertices, sizeof(glRectVertices), &glShaders[DEFAULT_SHADER_SLOT]);
}

// void CommancheRenderer::DrawRectangle(float x, float y, float w, float h, float rotation, CommancheColorRGB color) {
// }


void CommancheRenderer::DrawImage(int textureId, float x, float y, float width, float height, float rotation, int offsetX, int offsetY) {
    GLShape* shape = glShapes[RECT_PRIMITIVE];
    Texture texture = glTextures[textureId];

    shape->BindShape();

    shape->Scale(glm::vec2(width, height));
    shape->SetProjection(proj);
    shape->Translate(glm::vec2(x, y));
    shape->Rotate(rotation);

    shape->SetOffset(glm::vec2(offsetX, offsetY));

    texture.Bind();

    shape->DrawShape();

    texture.Unbind();
}

bool CommancheRenderer::IsShaderValid(int shaderId) {
    Shader shader = glShaders[shaderId];
    return shader.compiledSuccessfully;
}

bool CommancheRenderer::IsTextureValid(int textureId) {
    if (glTextures.find(textureId) == glTextures.end()) {
        return false;
    }
    return true;
}

void CommancheRenderer::DrawText(int fontId, std::string message, int x, int y, int size, CommancheColorRGB color) {

    int textureToDraw = GenerateTextureFromText(message, fontId, color);
    CommancheTextureInfo inf = GetTextureInfo(textureToDraw);

    if (!glIsTexture(textureToDraw)) {
        Log::Err("Requested font for id " + std::to_string(fontId) + " does not exist in OpenGL");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureToDraw);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);
        glVertex2f(x, y);
        glTexCoord2f(1, 0);
        glVertex2f(x + inf.width, y);
        glTexCoord2f(1, 1);
        glVertex2f(x + inf.width, y + inf.height);
        glTexCoord2f(0, 1);
        glVertex2f(x, y + inf.height);
    }
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

int CommancheRenderer::LoadShader(const std::string& path, const std::string shaderName) {

    std::string vertexShaderPath;
    std::string fragmentShaderPath;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::string fileName = std::filesystem::path(entry.path()).stem();
        std::string extension = std::filesystem::path(entry.path()).extension();

        if (fileName != shaderName) {
            continue;
        }

        if (extension == ".vert") {
            vertexShaderPath = entry.path();
        } else if (extension == ".frag") {
            fragmentShaderPath = entry.path();

        } else {
            std::cout << "err on shader load no extension" << std::endl;
        }
    }

    Shader shader = Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    glShaders.insert(std::make_pair(shader.ID, shader));
    return shader.ID;
}

int CommancheRenderer::GetFrame() {
    return textureColorbuffer;
}

void CommancheRenderer::SetFrameSize(int width, int height) {

    glDeleteTextures(1, &textureColorbuffer);
    glDeleteFramebuffers(1, &framebuffer);

    glViewport(0, 0, width, height);

    // glDeleteFramebuffers(1, &framebuffer);

    // framebuffer configuration
    // -------------------------
    framebuffer = 0;

    // Create FBO & Textrue
    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &textureColorbuffer);

    // Bind FBO & Texture
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

    // generate texture according to WxH
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // FBO to Texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                  << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int CommancheRenderer::LoadTexture(const std::string& path) {
    Texture texture = Texture(path.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    if (!texture.isSuccess)
        return -1;

    texture.texUnit(&glShaders[DEFAULT_SHADER_SLOT], "tex0", 0);

    glTextures[texture.ID] = texture;
    return texture.ID;
}

CommancheTextureInfo CommancheRenderer::GetTextureInfo(int id) {
    Texture texture = glTextures[id];

    CommancheTextureInfo inf;
    inf.width = texture.height;
    inf.height = texture.width;

    return inf;
}

void CommancheRenderer::RenderStart() {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}
void CommancheRenderer::RenderEnd() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void CommancheRenderer::RenderApply() {
    glfwSwapBuffers(k_window);
}

void CommancheRenderer::Destroy() {
    glfwDestroyWindow(k_window);
    glfwTerminate();
}
