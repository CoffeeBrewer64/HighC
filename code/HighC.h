/***

THE FOLLOWING CODE IS IN THE PUBLIC DOMAIN:
YOU MAY USE IT FREELY, AS YOU WISH, WITH NO
RESTRICTIONS, AS LONG AS YOU RECOGNISE AND
ENFORCE THE FACT THAT CoffeeBrewer64 IS
THE LEGAL OWNER OF THE CODE

***/

#ifndef HIGHC_H_INCLUDED
#define HIGHC_H_INCLUDED

// HighC is a single header C library with the aim of simplifying many tasks, like creating files, creating folders, encrypting data etc.
// Functions have simple versions and complex versions.

/*** malloc() and free() should NOT be used unless there is no alternative ***/

// INCLUDES

#include <stdio.h>
#include <stdlib.h>

#ifdef GL_INCLUDES

#include <GL/gl.h>
#include <GL/glaux.h>
#include <GL/glcorearb.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glxext.h>
#include <GL/wglext.h>

#endif // GL_INCLUDES

#ifdef _WIN32
#include <direct.h>
#define CREATE_DIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define CREATE_DIR(path) mkdir(path, 0777)
#endif

// TYPES

// HL_DateTime

typedef struct HL_DateTime {
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned int microsecond;
} HL_DateTime;

// HC_OpenWebsite (and variations)

void HC_OpenWebsite_WIN(const char* url)
{
    const char* commandFormat = "start %s";
    char command[1024];
    sprintf(command, commandFormat, url);
    system(command);
}

void HC_OpenWebsite_MACOS(const char* url)
{
    const char* commandFormat = "open %s";
    char command[1024];
    sprintf(command, commandFormat, url);
    system(command);
}

void HC_OpenWebsite_LINUX(const char* url)
{
    const char* commandFormat = "xdg-open %s";
    char command[1024];
    sprintf(command, commandFormat, url);
    system(command);
}

void HC_OpenWebsite_AUTO(const char* url)
{
#ifdef _WIN32
    // Windows
    const char* commandFormat = "start %s";
#elif __APPLE__
    // macOS
    const char* commandFormat = "open %s";
#else
    // Linux
    const char* commandFormat = "xdg-open %s";
#endif

    char command[1024];
    sprintf(command, commandFormat, url);
    system(command);
}

// HC_CreateFile

void HC_CreateFile(const char* filename)
{
    FILE* file;
    file = fopen(filename, "w");

    if (file == NULL)
    {
        // Failed to create the file
        return;
    }

    fclose(file);
}

// HC_IntToChar

char* HC_IntToChar(int num)
{
    static char result[12]; // Assuming a maximum of 11 digits for the integer
    sprintf(result, "%d", num);
    return result;
}

// HC_CreateFolder

int HC_CreateFolder(const char* path)
{
    int result = CREATE_DIR(path);
    if (result == 0)
    {
        // Created folder successfully
        return 0;
    }
    else
    {
        // Failed to create folder
        return -1;
    }
}

// HC_FastInverseSquareRoot

float HC_FastInverseSquareRoot(float x)
{
    float xhalf = 0.5f * x;
    int i;
    memcpy(&i, &x, sizeof(int));   // copy float bits to an integer

    i = 0x5f3759df - (i >> 1);     // initial guess using magic number
    memcpy(&x, &i, sizeof(float)); // copy integer bits to a float
    x = x * (1.5f - xhalf * x * x); // Newton-Raphson iteration
    return x;
}

// HC_AccurateFastInverseSquareRoot

float HC_AccurateFastInverseSquareRoot(float x, int iterations)
{
    float xhalf = 0.5f * x;
    int i;
    memcpy(&i, &x, sizeof(int));   // copy float bits to an integer

    i = 0x5f3759df - (i >> 1);     // initial guess using magic number
    memcpy(&x, &i, sizeof(float)); // copy integer bits to a float

    // Perform additional Newton-Raphson iterations
    for (int j = 0; j < iterations; j++)
    {
        x = x * (1.5f - xhalf * x * x); // Newton-Raphson iteration
    }

    return x;
}

// HC_ReadFileContents_RB

// RB = Read binary

char* HC_ReadFileContents_RB(const char* filename)
{
    const int max = 1000000;
    static char buffer[max]; // Static buffer for file contents

    FILE* file = fopen(filename, "rb");
    if (file != NULL)
    {
        // Seek to the end of the file to determine its size
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (file_size < max)
        {
            // Read the file contents into the buffer
            fread(buffer, sizeof(char), file_size, file);
            buffer[file_size] = '\0'; // Null-terminate the string
        }
        else
        {
            fclose(file);
            return NULL; // File size exceeds buffer capacity
        }

        fclose(file);
        return buffer;
    }
    return NULL; // Failed to open the file
}

#endif // HIGHC_H_INCLUDED
