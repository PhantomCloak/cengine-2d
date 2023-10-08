#include "../../render.h"
#include "types/GLLine.h"
#include "types/GLShape.h"
#include "types/primitives.h"
#include "types/texture.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <ft2build.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>
#include <unordered_map>
#include FT_FREETYPE_H

GLFWwindow* k_window;
std::string driverStr;
glm::mat4 CommancheRenderer::ProjectionMat;
int CommancheRenderer::vo;
int CommancheRenderer::ho;
int CommancheRenderer::screenWidth;
int CommancheRenderer::screenHeight;

int nextFontId = 0;
const int scaleFactor = 1;

std::unordered_map<int, GLShape*> glShapes;
std::unordered_map<int, Shader> glShaders;
std::unordered_map<int, Texture> glTextures;

CommancheRenderer* CommancheRenderer::Instance;

int PPM = 0;

GLLine* test;

float pixelCordToUvY(float y, int height) {
    return y / height;
}
float pixelCordToUvX(float x, int width) {
    return x / width;
}

int CommancheRenderer::LoadFont(const std::string& path, int fontSize) {

    return -1;
}


struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    long Advance;           // Offset to advance to next glyph
};

std::map<char, Character> Characters;
void InitFreeType() {
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    Log::Inf("Window W: " + std::to_string(width) + " H: " + std::to_string(height));
    glViewport(0, 0, width, height);
}


void CommancheRenderer::Initialize(const std::string& title, int windowWidth, int windowHeight) {
    Instance = this;
    glfwInit();

    std::string titleStr = title + " - Backend [OpenGL]";

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

    const float w = 1920.0f / 2;
    const float h = 1080.0f / 2;

    screenWidth = w;
    screenHeight = h;
    ProjectionMat = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

    glfwSwapInterval(0);
    // OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        Log::Err("ERROR::FREETYPE: Could not init FreeType Library");
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) {
        Log::Err("ERROR::FREETYPE: Failed to load font");
        return;
    } else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++) {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void CommancheRenderer::InitializeShaders(const std::string& defaultShaderPath) {

    std::string defaultVert = defaultShaderPath + "/default.vert";
    std::string defaultFrag = defaultShaderPath + "/default.frag";

    std::string defaultFontVert = defaultShaderPath + "/default_font.vert";
    std::string defaultFontFrag = defaultShaderPath + "/default_font.frag";

    std::string solidRenderShaderVert = defaultShaderPath + "/solid_draw.vert";
    std::string solidRenderShaderFrag = defaultShaderPath + "/solid_draw.frag";

    Shader defaultShader = Shader(defaultVert.c_str(), defaultFrag.c_str());
    Shader defaultFontShader = Shader(defaultFontVert.c_str(), defaultFontFrag.c_str());
    Shader solidRenderShader = Shader(solidRenderShaderVert.c_str(), solidRenderShaderFrag.c_str());

    defaultFontShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(defaultFontShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(ProjectionMat));
    defaultFontShader.Deactivate();

    assert(defaultShader.compiledSuccessfully);

    glShaders[DEFAULT_SHADER_SLOT] = defaultShader;
    glShaders[DEFAULT_FONT_SHADER_SLOT] = defaultFontShader;
    glShaders[SOLID_RENDER_SHADER] = solidRenderShader;

    glShapes[RECT_PRIMITIVE] = new GLShape(glRectVertices, sizeof(glRectVertices), &glShaders[DEFAULT_SHADER_SLOT]);
    test = new GLLine(glm::vec2(0, 0), glm::vec2(0, 0), &defaultShader);
}


void CommancheRenderer::DrawRectRangle(float x, float y, float width, float height, float rotation) {
    Shader shader = glShaders[DEFAULT_SHADER_SLOT];
    shader.Activate();

    GLShape* shape = glShapes[RECT_PRIMITIVE];
    // Texture texture = glTextures[textureId];

    x *= scaleFactor;
    y *= scaleFactor;
    width *= scaleFactor;
    height *= scaleFactor;

    shape->BindShape();

    shape->Scale(glm::vec2(width, height));
    shape->SetProjection(ProjectionMat);
    shape->Translate(glm::vec2(x, y));
    shape->Rotate(rotation);

    // shape->SetOffset(glm::vec2(offsetX, offsetY));

    // texture.Bind();

    shape->DrawShape(true);

    // texture.Unbind();
    shader.Deactivate();
}

void CommancheRenderer::CDrawImage(int textureId, float x, float y, float width, float height, float rotation, float srcX, float srcY, float srcWidth, float srcHeight) {
    Shader shader = glShaders[DEFAULT_SHADER_SLOT];
    shader.Activate();

    GLShape* shape = glShapes[RECT_PRIMITIVE];
    Texture texture = glTextures[textureId];

    x *= scaleFactor;
    y *= scaleFactor;
    width *= scaleFactor;
    height *= scaleFactor;

    auto inf = GetTextureInfo(textureId);

    float uStart = srcX / inf.width;
    float vStart = srcY / inf.height;
    float uEnd = (srcX + srcWidth) / inf.width;
    float vEnd = (srcY + srcHeight) / inf.height;

    shape->SetUV(uStart, vStart, uEnd, vEnd);

    shape->BindShape();

    shape->Scale(glm::vec2(width, height));
    shape->SetProjection(ProjectionMat);
    shape->Translate(glm::vec2(x, y));
    shape->Rotate(rotation);

    texture.Bind();
    shape->DrawShape();
    texture.Unbind();

    shader.Deactivate();
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

int GenerateTextureFromText(std::string text, int fontId, CommancheColorRGB textColor) {
    return -1;
}

void CommancheRenderer::CDrawLine(float startx, float starty, float endx, float endy, CommancheColorRGB color) {
    startx = startx / screenWidth;
    endx = endx / screenWidth;
    starty = starty / screenHeight;
    endy = endy / screenHeight;

    Shader shader = glShaders[DEFAULT_SHADER_SLOT];
    shader.Activate();

    test->UpdateLine(glm::vec2(startx, starty), glm::vec2(endx, endy));
    test->SetProjection(ProjectionMat);

    test->DrawLine();
    shader.Deactivate();
};

void CommancheRenderer::CDrawText(int fontId, std::string message, int x, int y, int size, CommancheColorRGB color) {
    static bool isInit = false;
    static VAO* vao;
    static VBO* vbo;

    if (!isInit) {
        vao = new VAO();
        vbo = new VBO(NULL, sizeof(float) * 6 * 4, GL_DYNAMIC_DRAW);
        vao->LinkAttrib(vbo, 0, 4, GL_FLOAT, 4 * sizeof(float), 0);
        isInit = true;
    }

    // activate corresponding render state
    Shader shader = glShaders[DEFAULT_FONT_SHADER_SLOT];
    shader.Activate();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.r, color.g, color.b);
    glActiveTexture(GL_TEXTURE0);

    vao->Bind();
    // iterate through all characters
    std::string::const_iterator c;
    for (c = message.begin(); c != message.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * 1;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * 1;

        float w = ch.Size.x * 1;
        float h = ch.Size.y * 1;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos, ypos - h, 0.0f, 0.0f },
            { xpos, ypos, 0.0f, 1.0f },
            { xpos + w, ypos, 1.0f, 1.0f },

            { xpos, ypos - h, 0.0f, 0.0f },
            { xpos + w, ypos, 1.0f, 1.0f },
            { xpos + w, ypos - h, 1.0f, 0.0f }
        };


        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        vbo->Update(0, sizeof(vertices), vertices);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * 1; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    vao->Unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
    shader.Deactivate();
}


int CommancheRenderer::CLoadShader(const std::string& path, const std::string shaderName) {

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

void CommancheRenderer::UpdateRenderTexture(glm::vec2 size) {
    float width = size.x;
    float height = size.y;

    glDeleteTextures(1, &textureColorbuffer);
    glDeleteFramebuffers(1, &framebuffer);

    glViewport(0, 0, width, height);

    glDeleteFramebuffers(1, &framebuffer);

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

int CommancheRenderer::CLoadTexture(const std::string& path) {
    Texture texture = Texture(path.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    if (!texture.isSuccess)
        return -1;

    glShaders[DEFAULT_SHADER_SLOT].Activate();
    texture.texUnit(&glShaders[DEFAULT_SHADER_SLOT], "tex0", 0);
    glShaders[DEFAULT_SHADER_SLOT].Deactivate();

    glTextures[texture.ID] = texture;
    return texture.ID;
}

void CommancheRenderer::DrawGrids() {
    static bool isInit = false;
    static VAO* vao;
    static VBO* vbo;

    Shader shader = glShaders[SOLID_RENDER_SHADER];
    shader.Activate();

    const int gridSize = 80;
    float gridWidth = 25;
    float gridHeight = 25;
    static float vertices[(gridSize + 1) * 4][2];

    if (!isInit) {
        vao = new VAO();
        vbo = new VBO(NULL, sizeof(float) * (gridSize * gridSize) * 2, GL_DYNAMIC_DRAW);
        vao->LinkAttrib(vbo, 0, 2, GL_FLOAT, 2 * sizeof(float), 0);

        glUniform4f(glGetUniformLocation(shader.ID, "color"), 0, 0, 0, 0.3f);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(ProjectionMat));

        int index = 0;

        for (int i = 0; i <= gridSize; i++) {
            // Horizontal lines
            vertices[index][0] = 0;
            vertices[index][1] = i * gridHeight;
            index++;
            vertices[index][0] = screenWidth;
            vertices[index][1] = i * gridHeight;
            index++;

            // Vertical lines
            vertices[index][0] = i * gridWidth;
            vertices[index][1] = 0;
            index++;
            vertices[index][0] = i * gridWidth;
            vertices[index][1] = screenHeight;
            index++;
        }

        vbo->Update(0, sizeof(vertices), vertices);
        isInit = true;
    }

    vao->Bind();

    glDrawArrays(GL_LINES, 0, (gridSize + 1) * 4);

    vao->Unbind();
}

CommancheTextureInfo CommancheRenderer::GetTextureInfo(int id) {
    Texture texture = glTextures[id];

    CommancheTextureInfo inf;
    inf.width = texture.width;
    inf.height = texture.height;

    return inf;
}

void CommancheRenderer::OffsetCamera(float vertical, float horizontal) {
    vo += vertical;
    ho += horizontal;

    ProjectionMat = glm::ortho(0.0f + vo, (float)screenWidth + vo, (float)screenHeight + ho, 0.0f + ho, -1.0f, 1.0f);
}

void CommancheRenderer::RenderStart() {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
}

void CommancheRenderer::BeginDraw() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void CommancheRenderer::RenderEnd() {
    glfwSwapBuffers(k_window);
}

void CommancheRenderer::EndDraw() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CommancheRenderer::Destroy() {
    glfwDestroyWindow(k_window);
    glfwTerminate();
}
