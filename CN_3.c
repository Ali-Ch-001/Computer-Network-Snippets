#include <stdio.h>
#include <stdlib.h>

int main() {
    
    FILE *file = fopen("input.txt", "r");

    if (file == NULL) {
        printf("Unable to open the file.\n");
        return 1; 
    }

    
    char buffer[1024]; 
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    
    fclose(file);

    return 0; 
}


