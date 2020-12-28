#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
bool compareStrings(char* s1, char* s2, int length);
int main(){
    char string[1000];

    while(fgets(string, 1000, stdin) != NULL){
        int i = 0;
        bool isValid = false;
        while (string[i++] != '\n' && string[i] != '\0'){
            if (string[i] == '}'){
               isValid = true; 
            }
            if (i < 100 && compareStrings(&string[i], "DELETE", 6)){
                isValid = false;
                break;
            }
        }
        if (isValid){
            printf("%s", string);
        }

    }
}
bool compareStrings(char* s1, char* s2, int length){
    for (int i = 0; i < length; i++){
        if (s1[i] != s2[i]){
            return false; 
        } 
    } 
    return true; 
}
