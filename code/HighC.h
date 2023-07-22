/***

THE FOLLOWING CODE IS IN THE PUBLIC DOMAIN:
YOU MAY USE IT FREELY, AS YOU WISH, WITH NO
RESTRICTIONS, AS LONG AS YOU RECOGNISE AND
ENFORCE THE FACT THAT CoffeeBrewer64 IS
THE LEGAL OWNER OF THE CODE

***/

/*

$$$$$$$\   $$$$$$\   $$$$$$\  $$\   $$\ $$\      $$\ $$$$$$$$\ $$\   $$\ $$$$$$$$\  $$$$$$\ $$$$$$$$\ $$$$$$\  $$$$$$\  $$\   $$\
$$  __$$\ $$  __$$\ $$  __$$\ $$ |  $$ |$$$\    $$$ |$$  _____|$$$\  $$ |\__$$  __|$$  __$$\\__$$  __|\_$$  _|$$  __$$\ $$$\  $$ |
$$ |  $$ |$$ /  $$ |$$ /  \__|$$ |  $$ |$$$$\  $$$$ |$$ |      $$$$\ $$ |   $$ |   $$ /  $$ |  $$ |     $$ |  $$ /  $$ |$$$$\ $$ |
$$ |  $$ |$$ |  $$ |$$ |      $$ |  $$ |$$\$$\$$ $$ |$$$$$\    $$ $$\$$ |   $$ |   $$$$$$$$ |  $$ |     $$ |  $$ |  $$ |$$ $$\$$ |
$$ |  $$ |$$ |  $$ |$$ |      $$ |  $$ |$$ \$$$  $$ |$$  __|   $$ \$$$$ |   $$ |   $$  __$$ |  $$ |     $$ |  $$ |  $$ |$$ \$$$$ |
$$ |  $$ |$$ |  $$ |$$ |  $$\ $$ |  $$ |$$ |\$  /$$ |$$ |      $$ |\$$$ |   $$ |   $$ |  $$ |  $$ |     $$ |  $$ |  $$ |$$ |\$$$ |
$$$$$$$  | $$$$$$  |\$$$$$$  |\$$$$$$  |$$ | \_/ $$ |$$$$$$$$\ $$ | \$$ |   $$ |   $$ |  $$ |  $$ |   $$$$$$\  $$$$$$  |$$ | \$$ |
\_______/  \______/  \______/  \______/ \__|     \__|\________|\__|  \__|   \__|   \__|  \__|  \__|   \______| \______/ \__|  \__|


DOCUMENTATION

This is HighC, a single C header file with the goal of making C a high level programming language through abstraction.

=== INCLUDED LIBRARIES ===
The only included libraries are stdio.h and stdlib.h. direct.h will be used on Windows and sys/stat.h on Linux and MacOS.

=== FLAGS ===

You can define HC_FLAGS_GL_INCLUDES to include the GL/ folder of headers.
You can define HC_FLAGS_INCLUDE_STDBOOL to include the stdbool.h header, allowing you to use boolean values with bool and true and false.

=== FEATURES ===

- Open websites
- Create files
- Create folders
- Check if a file exists
- Move a file
- Merge strings
- Convert int to char
- Square root
- Fast inverse square root
- Hypotenuse of a triangle

=== PATCHES ===

Some things in HighC still do not work. You can apply certain patches to either access these or make act in ways they are not meant to act.

Broken patch: HC_PATCH_BROKEN : Unlocks HC_ReadFileContents_RB

*/

/// HC_BROKEN patch appplied

#ifndef HIGHC_H_INCLUDED
#define HIGHC_H_INCLUDED


/*** malloc() and free() should NOT be used unless there is no alternative ***/

// INCLUDES

#include <stdio.h>
#include <stdlib.h>

#ifdef HC_FLAGS_GL_INCLUDES

#include <GL/gl.h>
#include <GL/glaux.h>
#include <GL/glcorearb.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glxext.h>
#include <GL/wglext.h>

#endif // HC_FLAGS_GL_INCLUDES

#ifdef HC_FLAGS_INCLUDE_STDBOOL
#include <stdbool.h>
#endif // HC_FLAGS_INCLUDE_STDBOOL

#ifdef _WIN32
    // Windows
    const char* HC_OperatingSystem = "Windows";
#elif __APPLE__
    // macOS
    const char* HC_OperatingSystem = "MacOS";
#else
    // Linux
    const char* HC_OperatingSystem = "Linux";
#endif

#ifdef _WIN32
#include <direct.h>
#define CREATE_DIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define CREATE_DIR(path) mkdir(path, 0777)
#endif

// TYPES

// HL_DateTime

typedef struct HL_DateTime
{
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

int HC_Check_FileExists(const char* path)
{
    FILE* file = fopen(path, "r");
    if (file != NULL)
    {
        // File exists
        fclose(file);
        return 1;
    }
    if (file == NULL)
    {
        // File does not exist
        fclose(file);
        return 0;
    }
    else
    {
        // Something else happened
        fclose(file);
        return -1;
    }
}

char* HC_MergeStrings(char* str1, char* str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    char* result = (char*)malloc(len1 + len2 + 1);
    if (result == NULL)
    {
        printf("Error: memory allocation failed for merge_strings()\n");
        return NULL;
    }

    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2 + 1); // Add the null terminator

    return result;
}

int HC_MoveFile(const char* sourcePath, const char* destinationPath)
{
    FILE *sourceFile, *destinationFile;
    char buffer[4096]; // Buffer to hold data while copying

    // Open the source file for reading in binary mode
    sourceFile = fopen(sourcePath, "rb");
    if (sourceFile == NULL)
    {
        perror("Error opening source file");
        return 1;
    }

    // Open the destination file for writing in binary mode
    destinationFile = fopen(destinationPath, "wb");
    if (destinationFile == NULL)
    {
        perror("Error opening destination file");
        fclose(sourceFile);
        return 1;
    }

    // Copy data from source to destination
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0)
    {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    // Close the files
    fclose(sourceFile);
    fclose(destinationFile);

    // Remove the original file
    if (remove(sourcePath) != 0)
    {
        perror("Error deleting the original file");
        return 1;
    }

    return 0;
}

/*

$$\      $$\  $$$$$$\ $$$$$$$$\ $$\   $$\  $$$$$$\
$$$\    $$$ |$$  __$$\\__$$  __|$$ |  $$ |$$  __$$\
$$$$\  $$$$ |$$ /  $$ |  $$ |   $$ |  $$ |$$ /  \__|
$$\$$\$$ $$ |$$$$$$$$ |  $$ |   $$$$$$$$ |\$$$$$$\
$$ \$$$  $$ |$$  __$$ |  $$ |   $$  __$$ | \____$$\
$$ |\$  /$$ |$$ |  $$ |  $$ |   $$ |  $$ |$$\   $$ |
$$ | \_/ $$ |$$ |  $$ |  $$ |   $$ |  $$ |\$$$$$$  |
\__|     \__|\__|  \__|  \__|   \__|  \__| \______/


MATHS FUNCTIONS

*/

// HC_Maths_FastInverseSquareRoot

float HC_Maths_FastInverseSquareRoot(float x)
{
    float xhalf = 0.5f * x;
    int i;
    memcpy(&i, &x, sizeof(int));   // copy float bits to an integer

    i = 0x5f3759df - (i >> 1);     // initial guess using magic number
    memcpy(&x, &i, sizeof(float)); // copy integer bits to a float
    x = x * (1.5f - xhalf * x * x); // Newton-Raphson iteration
    return x;
}

// HC_Maths_AccurateFastInverseSquareRoot

float HC_Maths_AccurateFastInverseSquareRoot(float x, int iterations)
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

// HC_Maths_SquareRoot
double HC_Maths_SquareRoot(double num)
{
    double x = num;
    double y = 1.0;
    double epsilon = 0.000001; // Desired precision

    while (x - y > epsilon)
    {
        x = (x + y) / 2;
        y = num / x;
    }

    return x;
}

// HC_Maths_SquareRoot_Precise
double HC_Maths_SquareRoot_Precise(double num)
{
    double x = num;
    double y = 1.0;
    double epsilon = 0.0000000000000001; // Desired precision

    while (x - y > epsilon)
    {
        x = (x + y) / 2;
        y = num / x;
    }

    return x;
}

// HC_Maths_Triangle_Hypotenuse
double HC_Maths_Triangle_Hypotenuse(double a, double b)
{
    double c = HC_Maths_SquareRoot(a * a + b * b);
    return c;
}

// HC_Maths_Triangle_Hypotenuse_Precise
double HC_Maths_Triangle_Hypotenuse_Precise(double a, double b)
{
    double c = HC_Maths_SquareRoot_Precise(a * a + b * b);
    return c;
}

/*


$$$$$$$\   $$$$$$\ $$$$$$$$\  $$$$$$\  $$\   $$\ $$$$$$$$\  $$$$$$\
$$  __$$\ $$  __$$\\__$$  __|$$  __$$\ $$ |  $$ |$$  _____|$$  __$$\
$$ |  $$ |$$ /  $$ |  $$ |   $$ /  \__|$$ |  $$ |$$ |      $$ /  \__|
$$$$$$$  |$$$$$$$$ |  $$ |   $$ |      $$$$$$$$ |$$$$$\    \$$$$$$\
$$  ____/ $$  __$$ |  $$ |   $$ |      $$  __$$ |$$  __|    \____$$\
$$ |      $$ |  $$ |  $$ |   $$ |  $$\ $$ |  $$ |$$ |      $$\   $$ |
$$ |      $$ |  $$ |  $$ |   \$$$$$$  |$$ |  $$ |$$$$$$$$\ \$$$$$$  |
\__|      \__|  \__|  \__|    \______/ \__|  \__|\________| \______/

PATCHES

*/

#ifdef HC_PATCH_BROKEN
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
#endif // HC_PATCH_BROKEN

#endif // HIGHC_H_INCLUDED
