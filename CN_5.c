#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int isNonAlphabetWord(const char *str) {
    while (*str) {
        if (isalpha(*str)) {
            return 0; 
        }
        str++;
    }
    return 1; 
}

int main() {
    
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile == NULL) {
        printf("Unable to open the input file.\n");
        return 1; 
    }

   
    FILE *outputFile = fopen("output1.txt", "w");

    if (outputFile == NULL) {
        printf("Unable to create the output file.\n");
        fclose(inputFile); 
        return 1; 
    }

    char word[100];
   
    while (fscanf(inputFile, "%s", word) != EOF) {
        if (isNonAlphabetWord(word)) {
            fprintf(outputFile, "%s\n", word);
        }
    }

   
    fclose(inputFile);
    fclose(outputFile);

    printf("Non-alphabet words have been copied to output1.txt.\n");

    return 0; 
}

