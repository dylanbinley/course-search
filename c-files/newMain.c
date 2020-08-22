#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
bool compareStrings(char* s1, char* s2, int length);
int main(int argc, char *argv[0]){
    char string[1000];
    char *subject = " ";
    if (argc >1){
        subject = argv[1];
    }
    else {
        fprintf(stderr, "%s", "A string argument is needed\n");
        return EXIT_FAILURE;
    }
    int count = 0;
    putchar('\n');
    printf("%s", "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    putchar('\n');
    putchar('\n');
    bool isRecitation = true;
    while(fgets(string, 1000, stdin) != NULL){
        if (compareStrings(string,subject, 4)/*string[0] == 'C'&& string[1] == 'O'&& string[2] == 'M'*/){
            if (!isRecitation){
                putchar('\n');
                printf("%s", "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
                putchar('\n');
                putchar('\n');
            }
            if (getchar() == 'R'){
                isRecitation = true;
            }
            else{
                isRecitation = false;
                printf("%s",string);
            }
        }
        if (!isRecitation && (string[0] == 'I' || string[0] == 'G' || compareStrings(string, "TA", 2) || 
                    compareStrings(string, "RA", 2) || compareStrings(string, "DEAN", 4) || compareStrings(string, "Bldg", 4))){ 
            printf("%s",string);
        }
        count++;
    }
    if (!isRecitation){
        putchar('\n');
        printf("%s", "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        putchar('\n');
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
