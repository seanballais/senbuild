#define MAX_WORD_LEN   40
#define MAX_WORDS      1000

#define FIRST_WORD     0
#define MIDDLE_WORD    1
#define LAST_WORD      2

#define START_SYMBOL   0
#define END_SYMBOL     1

#include <stdlib.h>

#define getSRand() ((double)rand()/(double)RAND_MAX)
#define getRand(x) (int)((x) * getSRand())

static int currWord = 2;
char wordVector[MAX_WORDS][MAX_WORD_LEN];
int bigramArray[MAX_WORDS][MAX_WORDS];
int sumVector[MAX_WORDS];
