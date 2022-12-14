#include "render.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include "glm/glm.hpp"
#include <filesystem>
#include <map>
#include <unordered_map>


SDL_Window* window;
SDL_GLContext glContext;
std::string driverStr;
int nextFontId = 0;

// TODO: Horrible hack
std::unordered_map<int, std::unordered_map<std::string, int>> generatedTextRegistry;
std::unordered_map<int, TTF_Font*> fontRegistry;

float pixelCordToUvY(float y, int height) {
    return y / height;
}

float pixelCordToUvX(float x, int width) {
    return x / width;
}

int GenerateTextureFromText(std::string text, int fontId, CommancheColorRGB textColor) {
    GLuint texture;

    if (fontRegistry.find(fontId) == fontRegistry.end()) {
        Log::Err("Requested font for id " + std::to_string(fontId) + " couldn't find ");
        return -1;
    }

    // Check if we were generated font before
    if (generatedTextRegistry.find(fontId) == generatedTextRegistry.end()) {
        generatedTextRegistry.insert(std::make_pair(fontId, std::unordered_map<std::string, int>()));
    } else {
        if (generatedTextRegistry[fontId].find(text) != generatedTextRegistry[fontId].end()) {
            return generatedTextRegistry[fontId][text];
        }
    }


    TTF_Font* font = fontRegistry.at(fontId);
    SDL_Color color = {
        .r = (uint8_t)textColor.r,
        .g = (uint8_t)textColor.g,
        .b = (uint8_t)textColor.b,
        .a = 255
    };

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);

    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_Surface* s = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_BlitSurface(surface, NULL, s, NULL);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);
    glDisable(GL_TEXTURE_2D);

    generatedTextRegistry[fontId].insert(std::make_pair(text, texture));
    return texture;
}

int CommancheRenderer::LoadFont(const std::string& path, int fontSize) {

    TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);

    if (font == nullptr) {
        std::string err(SDL_GetError());
        Log::Err("error on loading font to OpenGL message: " + err);
        return -1;
    }

    fontRegistry.insert(std::make_pair(nextFontId, font));
    return nextFontId++;
}

void CommancheRenderer::Initialize(const std::string& title, int windowWidth, int windowHeight) {

    int glMajor = 0;
    int glMinor = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &glMajor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &glMinor);

    driverStr = "OpenGL " + std::to_string(glMajor) + "." + std::to_string(glMinor);

    // GL 3.2 Core + GLSL 150
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    std::string windoiwTitle = title + " - " + driverStr;
    window = SDL_CreateWindow(windoiwTitle.c_str(), 0, 0, windowWidth, windowHeight, window_flags);

    if (TTF_Init() != 0) {
        printf("Error: %s\n", TTF_GetError());
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        Log::Err("Error while creating renderer");
    }

    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    wnd = (void*)window;
    gctx = (void*)glContext;
}

void CommancheRenderer::DrawRectangle(int x, int y, int w, int h, CommancheColorRGB color) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);
    {
        // top left
        glColor3f(color.r, color.g, color.b);
        glVertex3f(x, y, 0);
        // bot left
        glColor3f(color.r, color.g, color.b);
        glVertex3f(x, y + h, 0);
        // bot right
        glColor3f(color.r, color.g, color.b);
        glVertex3f(x + w, y + h, 0);
        // top right
        glColor3f(color.r, color.g, color.b);
        glVertex3f(x + w, y, 0);
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CommancheRenderer::DrawImage(int textureId, int x, int y, int width, int height, int size, int offsetX, int offsetY) {
    glBindTexture(GL_TEXTURE_2D, textureId);

    int picH = 0;
    int picW = 0;

    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &picW);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &picH);

    glm::vec2 topLeft = glm::vec2(pixelCordToUvX(offsetX, picW), pixelCordToUvY(offsetY, picH));
    glm::vec2 topRight = glm::vec2(pixelCordToUvX(offsetX + size, picW), pixelCordToUvY(offsetY, picH));

    glm::vec2 botLeft = glm::vec2(pixelCordToUvX(offsetX, picW), pixelCordToUvY(offsetY + size, picH));
    glm::vec2 botRight = glm::vec2(pixelCordToUvX(offsetX + size, picW), pixelCordToUvY(offsetY + size, picH));

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    {
        // Top-left vertex (corner)
        glTexCoord2f(topLeft.x, topLeft.y);
        glVertex3f(x, y, 0);

        // Bottom-left vertex (corner)
        glTexCoord2f(botLeft.x, botLeft.y);
        glVertex3f(x, y + height, 0);

        // Bottom-right vertex (corner)
        glTexCoord2f(botRight.x, botRight.y);
        glVertex3f(x + width, y + height, 0);

        // Top-right vertex (corner)
        glTexCoord2f(topRight.x, topRight.y);
        glVertex3f(x + width, y, 0);
    }

    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
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

int CommancheRenderer::LoadTexture(std::string path) {
    SDL_Surface* surface = IMG_Load(path.c_str());

    GLint nofcolors;
    GLuint texture = NULL;
    GLenum texture_format = NULL;

    if (surface == NULL) {
        Log::Err("An error occured while trying to load image");
        return -1;
    }

    nofcolors = surface->format->BytesPerPixel;

    // contains an alpha channel
    if (nofcolors == 4) {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    } else if (nofcolors == 3) // no alpha channel
    {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    } else {
        printf("warning: the image is not truecolor...this will break ");
    }

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, nofcolors, surface->w, surface->h, 0,
    texture_format, GL_UNSIGNED_BYTE, surface->pixels);

    if (surface) {
        SDL_FreeSurface(surface);
    }

    return texture;
}

CommancheTextureInfo CommancheRenderer::GetTextureInfo(int id) {
    int w, h;
    int miplevel = 0;
    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

    CommancheTextureInfo inf;
    inf.width = w;
    inf.height = h;

    return inf;
}

void CommancheRenderer::Render() {
    SDL_GL_SwapWindow(window);
}

void CommancheRenderer::Destroy() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
