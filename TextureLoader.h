#pragma once
/*
 * TextureLoader.h - Texture Loading with stb_image (merged header + implementation)
 *
 * stb_image.h must be in the project directory. Download from:
 * https://github.com/nothings/stb/blob/master/stb_image.h
 */

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <Windows.h>
#include <GL/gl.h>
#include <cstdio>

/*
 * Load an image file and create an OpenGL texture.
 * Returns GLuint texture ID on success, 0 on failure.
 */
GLuint loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* imageData = stbi_load(filename, &width, &height, &channels, 0);

    if (imageData == NULL) {
        fprintf(stderr, "[TextureLoader] WARNING: Failed to load '%s': %s\n", filename, stbi_failure_reason());
        return 0;
    }

    printf("[TextureLoader] Loaded '%s' (%dx%d, %d ch)\n", filename, width, height, channels);

    GLenum format = GL_RGB;
    if (channels == 4) format = GL_RGBA;
    else if (channels == 1) format = GL_LUMINANCE;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(imageData);
    return textureID;
}
