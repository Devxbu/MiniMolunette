#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <ctype.h>

// Project base path
#define PROJECT_PATH "/Users/bahri/Documents/www/Projects/miniMolunette"

// ANSI colors for CLI
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[0;36m"
#define RESET   "\033[0m"

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        fprintf(stderr, RED "Error: Project name was not given.\n" RESET);
        fprintf(stderr, "Usage: %s <project_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *projectName = argv[1];
    int nameLen = strlen(projectName);

    // Validate name (format: cXX)
    projectName[0] = tolower(projectName[0]);
    if (!(nameLen == 3 && projectName[0] == 'c' && isdigit(projectName[1]) && isdigit(projectName[2])))
    {
        fprintf(stderr, RED "Error: Invalid project name format. Expected 'cXX' (e.g., c01)\n" RESET);
        return EXIT_FAILURE;
    }

    // Prepare paths
    char infoPath[PATH_MAX];
    snprintf(infoPath, sizeof(infoPath), "%s/projects/%s/info.txt", PROJECT_PATH, projectName);

    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror(RED "Error: getcwd() failed" RESET);
        return EXIT_FAILURE;
    }

    // Open info file
    FILE *fp = fopen(infoPath, "r");
    if (!fp)
    {
        perror(RED "Error opening info file" RESET);
        return EXIT_FAILURE;
    }

    // Read project count
    int projectCount = 0;
    if (fscanf(fp, "%d\n", &projectCount) != 1)
    {
        fprintf(stderr, RED "Error: Could not read project count.\n" RESET);
        fclose(fp);
        return EXIT_FAILURE;
    }

    // Allocate and read project names
    char **projectFiles = malloc(projectCount * sizeof(char *));
    if (!projectFiles)
    {
        fprintf(stderr, RED "Error: Memory allocation failed.\n" RESET);
        fclose(fp);
        return EXIT_FAILURE;
    }

    char line[256];
    for (int i = 0; i < projectCount && fgets(line, sizeof(line), fp); i++)
    {
        line[strcspn(line, "\n")] = '\0';
        projectFiles[i] = strdup(line);
        if (!projectFiles[i])
        {
            fprintf(stderr, RED "Error: Memory allocation failed.\n" RESET);
            fclose(fp);
            return EXIT_FAILURE;
        }
    }
    fclose(fp);

    // --- CLI Header ---
    printf("\n" CYAN "╭──────────────────────────────────────────────╮\n");
    printf("│       MiniMolunette — Automated Tester       │\n");
    printf("╰──────────────────────────────────────────────╯" RESET "\n");
    printf(YELLOW "Project: %s  |  Total Exercises: %d\n" RESET, projectName, projectCount);
    printf("───────────────────────────────────────────────\n");

    // --- Run tests ---
    for (int i = 0; i < projectCount; i++)
    {
        char testFilePath[PATH_MAX];
        char functionFilePath[PATH_MAX];
        char outputFilePath[PATH_MAX];
        char givenOutputPath[PATH_MAX];
        char expectedOutputPath[PATH_MAX];
        char command[PATH_MAX * 4];

        snprintf(testFilePath, sizeof(testFilePath), "%s/projects/%s/ex%.2d/main.c", PROJECT_PATH, projectName, i);
        snprintf(functionFilePath, sizeof(functionFilePath), "%s/ex%.2d/%s", cwd, i, projectFiles[i]);
        snprintf(outputFilePath, sizeof(outputFilePath), "%s/projects/%s/ex%.2d/main.out", PROJECT_PATH, projectName, i);
        snprintf(givenOutputPath, sizeof(givenOutputPath), "%s/projects/%s/ex%.2d/given_output.txt", PROJECT_PATH, projectName, i);
        snprintf(expectedOutputPath, sizeof(expectedOutputPath), "%s/projects/%s/ex%.2d/expected_output.txt", PROJECT_PATH, projectName, i);

        printf("\n" CYAN "=== [Project %d] %s ===\n" RESET, i, projectFiles[i]);

        // Compile
        snprintf(command, sizeof(command),
                 "gcc -Wall -Wextra -Werror \"%s\" \"%s\" -o \"%s\"",
                 functionFilePath, testFilePath, outputFilePath);

        int compileStatus = system(command);
        if (compileStatus != 0)
        {
            printf(RED "Compilation failed.\n" RESET);
            continue;
        }

        // Run program
        snprintf(command, sizeof(command), "\"%s\" > \"%s\"", outputFilePath, givenOutputPath);
        system(command);

        // Compare outputs
        snprintf(command, sizeof(command), "diff -u \"%s\" \"%s\" > diff_result.txt", expectedOutputPath, givenOutputPath);
        int diffStatus = system(command);

        if (diffStatus == 0)
        {
            printf(GREEN "Output matches expected result!\n" RESET);
        }
        else
        {
            printf(RED "Output differs from expected result.\n" RESET);
            FILE *diff = fopen("diff_result.txt", "r");
            if (diff)
            {
                printf(YELLOW "──── Differences ────\n" RESET);
                char diffLine[512];
                while (fgets(diffLine, sizeof(diffLine), diff))
                    printf("%s", diffLine);
                fclose(diff);
                printf(YELLOW "─────────────────────\n" RESET);
            }
        }
    }

    // Cleanup
    for (int i = 0; i < projectCount; i++)
        free(projectFiles[i]);
    free(projectFiles);

    printf("\n" GREEN "All tests completed!\n" RESET);
    return EXIT_SUCCESS;
}
