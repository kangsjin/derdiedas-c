#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "random_util.h"
#include "string_trim.h"
#include "terminal_util.h"

#define len(arr) (sizeof(arr) / sizeof(arr[0]))

typedef struct
{
    char article[10];
    char noun[50];
    char english[50];
} NounData;

void printResult(int totalCount, int wrongCount, int *wrongQuestions, NounData quize_data[]);

void printHeader() {
    printf(" ______  _______  ______      ______  _____ _______      ______  _______ _______\n");
    printf(" |     \\ |______ |_____/      |     \\   |   |______      |     \\ |_____| |______\n");
    printf(" |_____/ |______ |    \\_      |_____/ __|__ |______      |_____/ |     | ______|\n");
    printf("                                                                          created by SJ Kang\n");
}

int main() {
    char *articles[] = {"", "der", "die", "das"};
    // Set up the SIGINT signal handler
    signal(SIGINT, handle_sigint);

    // Save the current terminal output
    save_terminal_output(TEMP_FILE);

    // Clear the screen
    clear_screen();

    // Print the title
    printHeader();
    printf("Please enter how many questions you want to practice. (up to 100) > ");

    int num;
    char english_meaning_flag;
    int count = scanf("%i", &num);
    if (count == 0) {
        printf("Please enter a valid number.\n");
        return 1;
    }
    if (num > 100) num = 100;
    fflush(stdin);

    while (english_meaning_flag != 'y' && english_meaning_flag != 'n') {
        clear_screen();
        printHeader();
        printf("Would you like to see the English meaning as hint? y/n > ");
        scanf("%c", &english_meaning_flag);
        fflush(stdin);
    }

    clear_screen();
    printHeader();
    printf("Okay good. I created random %i questions for you. Let's get started!\n\n\n", num);

    NounData quiz_data[1024];
    const int max = 1024;
    char line[max];
    char *pLine;
    FILE *in = fopen("data/A1.db", "r");
    int quize_idx = 0;
    while (!feof(in)) {
        pLine = fgets(line, max, in);
        if (pLine == NULL) {
            break;
        }
        char *temp = strtok(pLine, ",");
        strcpy(quiz_data[quize_idx].article, temp);
        temp = strtok(NULL, ",");
        strcpy(quiz_data[quize_idx].noun, temp);
        temp = strtok(NULL, ",");
        strcpy(trim(quiz_data[quize_idx].english), trim(temp));

        quize_idx++;
    }
    fclose(in);

    srand(time(0));
    int *randomArray = malloc(num * sizeof(int));
    if (randomArray == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    generateUniqueRandomNumbers(randomArray, num, 0, quize_idx);

    int correct_count = 0;
    int wrong_questions[num];
    int wrong_pos = 0;
    for (int i = 0; i < num; i++) {
        printf(" ________________________\n");
        printf("/\\                       \\\n");
        printf("\\_|   Q.%-3d of %-11d|\n", i + 1, num);
        printf("  |                       |\n");
        printf("  |   %-20s|\n", quiz_data[randomArray[i]].noun);

        if (english_meaning_flag == 'y') {
            printf("  |   %-20s|\n", quiz_data[randomArray[i]].english);
            printf("  |                       |\n");
        } else {
            printf("  |                       |\n");
        }

        printf("  | (1)der (2)die (3)das  |\n");
        printf("  |   ____________________|_\n");
        printf("   \\_/_____________________/ your answer > ");

        // printf("Q.%i %s\n", i + 1, quiz_data[randomArray[i]].noun);
        // printf("(1)der (2)die (3)das\n");

        int user_answer;
        scanf("%i", &user_answer);

        if (user_answer < 1 || user_answer > 3) {
            // +-------------+
            //|    stone    |
            //+-------------+
            printf("+------------------------------------------------+\n");
            printf("|    Please enter valid number between 1 to 3.    |\n");
            printf("+------------------------------------------------+\n");
            i--;
            continue;
        }

        clear_screen();
        printHeader();
        if (strcmp(articles[user_answer], quiz_data[randomArray[i]].article) != 0) {
            printf("X------------------------------------------------X\n");
            printf("|    Wrong! The answer is %s %-19s|\n", quiz_data[randomArray[i]].article, quiz_data[randomArray[i]].noun);
            printf("X------------------------------------------------X\n");
            wrong_questions[wrong_pos++] = randomArray[i];
        } else {
            printf("O------------------------------------------------O\n");
            printf("|    Correct! The answer is %s %-17s|\n", quiz_data[randomArray[i]].article, quiz_data[randomArray[i]].noun);
            printf("O------------------------------------------------O\n");
        }
    }

    printf("Press any key to see the result. > ");
    fflush(stdin);
    scanf("%*c");

    clear_screen();
    fflush(stdin);
    printResult(num, wrong_pos, wrong_questions, quiz_data);
    fflush(stdin);
    printf("Press any key to end. > ");

    int n;
    scanf("%i", &n);
    // Restore the original terminal output
    restore_terminal_output(TEMP_FILE);
    // Clean up
    remove(TEMP_FILE);
}

void printResult(int totalCount, int wrongCount, int *wrongQuestions, NounData quize_data[]) {
    printf("═══════════════════════════════════════\n");
    printf("                 RESULT                \n");
    printf("═══════════════════════════════════════\n");

    if (wrongCount == 0) {
        printf(" Congratulations! You got all the questions right!\n");
    } else {
        printf("        Score:  %-3d/ %-13d\n", totalCount - wrongCount, totalCount);
        printf("═══════════════════════════════════════\n");
        printf("        The words you got wrong    \n");
        printf("═══════════════════════════════════════\n");

        int line = wrongCount % 3 == 0 ? wrongCount / 3 : (wrongCount / 3) + 1;
        for (int i = 0; i < wrongCount; i++) {
            char *word;
            if (i % 3 == 2 || i == wrongCount - 1) {
                word = "   %s %-20s\n";
                // printf("   %s %-20s\n", quize_data[wrongQuestions[i]].article, quize_data[wrongQuestions[i]].noun);
            } else {
                word = "   %s %-20s";
            }
            printf(word, quize_data[wrongQuestions[i]].article, quize_data[wrongQuestions[i]].noun);
        }
        printf("═══════════════════════════════════════\n");
    }
}