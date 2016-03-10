#ifndef SENBUILD_FUNCS_H
#define SENBUILD_FUNCS_H

void parseOptions(char *argv[], int argc, int *debug, char *filename);
void parseFile(char *filename);
void loadWord(char *word, int order);
int buildSentence(char* filename);
int nextWord(int word);
void emitMatrix(void);

#endif
