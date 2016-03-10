#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "senbuild_funcs.h"

int main(int argc, char *argv[])
{
    char filename[40] = "";
    int debug = 0;

    parseOptions(argv, argc, &debug, filename);

    srand(time(NULL));
    bzero(bigramArray, sizeof(bigramArray));

    strcpy(wordVector[0], "<START>");
    strcpy(wordVector[1], "<END>");

    parseFile(filename);

    if (debug) {
        emitMatrix();
    }

    buildSentence(filename);

    return 0;
}
