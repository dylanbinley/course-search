#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(){
    char string[1000];

    while(fgets(string, 1000, stdin) != NULL){
        int i = 0;
        bool isValid = false;
        while (string[i++] != '\n' && string[i] != '\0'){
            if (string[i] == '}'){
               isValid = true; 
            }
            if (i < 100 && strncmp(&string[i], "DELETE", 5) == 0){
                isValid = false;
                break;
            }
        }
        if (isValid){
            printf("%s", string);
        }

    }
}
