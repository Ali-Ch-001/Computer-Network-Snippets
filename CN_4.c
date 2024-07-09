#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    FILE *inputFile, *outputFile;
    
   
    char token[50]; 
    
    int num;
    inputFile = fopen("input.txt", "r");
    
    outputFile = fopen("output.txt", "w");
    
    while (fscanf(inputFile, "%s", token) != EOF) {
        if (isdigit(token[0])) {
            num = atoi(token);
           
            fprintf(outputFile, "%d\n", num);
        }
    }
    fclose(inputFile);
    fclose(outputFile);
     printf("numbers have been copied to output.txt\n");

    return 0;
}
