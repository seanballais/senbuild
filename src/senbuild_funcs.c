#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "constants.h"
#include "senbuild_funcs.h"

void
parseOptions(char *argv[], int argc, int *debug, char *filename)
{
    int opt;
    int error = 1;
    *debug = 0;

    if (argc > 1) {
        while ((opt = getopt(argc, argv, "vf:")) != -1) {
            switch (opt) {
                case 'v':
                    // Verbose mode
                    *debug = 1;
                    break;
                case 'f':
                    // File name option
                    strcpy(filename, optarg);
                    error = 0;
                    break;
                default:
                    // Any other option is an errorS
                    error = 1;
            }
        }
    }

    if (error) {
        printf("\nUsage:\n\n");
        printf("\t%s -f <filename> -v \n\n", argv[0]);
        printf("\t\t -f corpus filename\n\t\t -verbose mode\n\n");
        exit(0);
    }

    return;
}

void
parseFile(char *filename)
{
    FILE *fp;
    int input;
    int index = 0;
    char word[MAX_WORD_LEN + 1];
    int first = 1;

    fp = fopen(filename, "r");
    while (!feof(fp)) {
        input = fgetc(fp);
        if (input == EOF) {
            if (index > 0) {
                // For the last word, update the matrix accordingly.
                word[index++] = 0;
                loadWord(word, LAST_WORD);
                index = 0;
            }
        } else if (((char) input == 0x0d) || ((char) input == 0x0a) ||
                   ((char) input == ' ')) {
            if (index > 0) {
                word[index++] = 0;
                if (first) {
                    // First word in the sentence
                    loadWord(word, FIRST_WORD);
                    index = 0;
                    first = 0;
                } else {
                    // Middle word of the sentence
                    loadWord(word, MIDDLE_WORD);
                    index = 0;
                }
            }
        } else if (((char) input == '.') || ((char) input == '?')) {
            // Handle punctuation by ending the current sequence
            word[index++] = 0;
            loadWord(word, MIDDLE_WORD);
            loadWord(word, LAST_WORD);
            index = 0;
            first = 1;
        } else {
            // Skip white space
            if (((char) input != 0x0a) && ((char) input != ',')) {
                word[index++] = (char) input;
            }
        }
    }

    fclose(fp);
}

void
loadWord(char *word, int order)
{
    int wordIndex;
    static int lastIndex = START_SYMBOL;

    // See if the word has already been recorded
    for (wordIndex = 2; wordIndex < currWord; wordIndex++) {
        if (!strcmp(wordVector[wordIndex], word)) {
            break;
        }
    }

    if (wordIndex == currWord) {
        if (currWord == MAX_WORDS) {
            printf("\tToo many words, increase MAX_WORDS\n\n");
            exit(-1);
        }

        // Doesn't exist so add it in
        strcpy(wordVector[currWord++], word);
    }

    if (order == FIRST_WORD) {
        bigramArray[START_SYMBOL][wordIndex]++;
        sumVector[START_SYMBOL]++;
    } else if (order == LAST_WORD) {
        bigramArray[wordIndex][END_SYMBOL]++;
        bigramArray[END_SYMBOL][wordIndex]++;
        sumVector[END_SYMBOL]++;
    } else {
        // Load word as the middle of a sequence
        bigramArray[lastIndex][wordIndex]++;
        sumVector[lastIndex]++;
    }

    lastIndex = wordIndex;

    return;
}

int
buildSentence(char* filename)
{
    FILE *fp;
    if (strcmp(filename, "") != 0) {
        fp = fopen("data.txt", "a");
    }

    int word = START_SYMBOL;
    int max = 0;

    printf("\n");

    // Start with a random word
    word = nextWord(word);

    if (strcmp(filename, "") != 0) {
        fprintf(fp, "\n");
    }

    // Loop unti we have reached the end of the random sequence
    while (word != END_SYMBOL) {
        // Emit the current word
        printf("%s ", wordVector[word]);

        if (strcmp(filename, "") != 0) {
            fprintf(fp, "%s ", wordVector[word]);
        }

        // Next word please
        word = nextWord(word);

        // Only allow a maximum number of words
        max += getRand(12) + 1;

        // Break when we have reached the end
        if (max >= 100) {
            break;
        }
    }

    printf("%c.\n\n", 8);

    if (strcmp(filename, "") != 0) {
        fprintf(fp, "%c.", 8);
        fclose(fp);
    }

    return 0;
}

int
nextWord(int word)
{
    int nextWord = word + 1;
    int max = sumVector[word];
    int lim;
    int sum = 0;

    lim = getRand(max) + 1;

    while (nextWord != word) {
        // Bound the limit of the array using modulus
        nextWord = nextWord % currWord;

        // Keep a sum for the roulette wheel
        sum += bigramArray[word][nextWord];

        if (sum >= lim) {
            return nextWord;
        }

        nextWord++;
    }

    return nextWord;
}

void
emitMatrix(void)
{
    int x;
    int y;

    printf("\n");
    for (x = 0; x < currWord; x++) {
        printf("%20s : ", wordVector[x]);

        for (y = 0; y < currWord; y++) {
            printf("%d ", bigramArray[x][y]);
        }

        printf(" : %d\n", sumVector[x]);
    }

    return;
}
