#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int isVowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}


void invertString(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

int main() {
    
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile == NULL) {
        printf("Unable to open the input file.\n");
        return 1; 
    }

 
    FILE *outputFile = fopen("output2.txt", "w");

    if (outputFile == NULL) {
        printf("Unable to create the output file.\n");
        fclose(inputFile); 
        return 1; 
    }

    char word[100];
   
    while (fscanf(inputFile, "%s", word) != EOF) {
        int hasVowel = 0;
        for (int i = 0; word[i] != '\0'; i++) {
            if (isVowel(word[i])) {
                hasVowel = 1;
                break;
            }
        }
        
        if (hasVowel) {
            invertString(word);
        }
        
        fprintf(outputFile, "%s ", word);
    }

    
    fclose(inputFile);
    fclose(outputFile);

    printf("Words have been inverted and copied to Output_File2.txt.\n");

    return 0; 
}

