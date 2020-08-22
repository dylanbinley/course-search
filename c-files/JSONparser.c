#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
bool compareStrings(char* s1, char* s2, int length);
void printCourse(char* string);
void printTeacher(char* string);
int printMember(char* input, char* name, int i);
int skipWhiteSpace(char* string, int i);
int skipWord(char* string, int i);
int printClassName(char* input, int i);
void printTimeAndDay(char* string);
int printDay(char * string, int i);
void printTime(char* string, int i);
void printEnrollmentInfo(char*string);
int skipToNumber(char * string, int i);
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
    bool isRecitation = true;
    //note: only printing first time slots for classes with multiple
    //may want to change later
    bool hasTime = false;
    bool hasTeacher = false;
    bool hasEnrollmentInfo = false;

    while(fgets(string, 1000, stdin) != NULL){
        if (compareStrings(string,subject, 4)){
            //firest check for lab or edgecase by going past course number
            //if it is a lab or edge case, do nothing 
            //int i = 0;
            //i = skipToNumber(string, i); 
            //int c;
            //while ((c =getchar()) >= '0' && c <= '9'){

            //}
            if (!isRecitation){
                if (!hasTime){
                    printf("\"days\":\"N/A\", \"time\":\"N/A\",");
                }
                if (!hasTeacher){
                    printf("\"tLname\":\"N/A\", \"tFname\":\"\",");
                }
                if (!hasEnrollmentInfo){
                    printf("\"capacity\":\"N/A\", \"enrollment\":\"N/A\",\"waitlist_capacity\":\"N/A\",\"waitlist_total\":\"N/A\",");
                }
                printf("\"extra\": \"\"},");
                putchar('\n');
            }
            if (getchar() == 'R'){
                isRecitation = true;
            }
            else{
                isRecitation = false;
                hasTime = false;
                hasTeacher = false;
                hasEnrollmentInfo = false;
                printCourse(string);
            }
        }
        if (!isRecitation && !hasTeacher && (compareStrings(string, "GA", 2) || compareStrings(string, "TA", 2) || 
                    compareStrings(string, "RA", 2) || compareStrings(string, "DEAN", 4) || compareStrings(string, "INST", 4))){ 
            printTeacher(string);
            hasTeacher = true;
        }
        else if (!isRecitation && !hasTime && compareStrings(string, "Bldg", 4)){
            printTimeAndDay(string);
            hasTime = true;
        }
        else if (!isRecitation && compareStrings(string, "Class Enrl", 10)){
            printEnrollmentInfo(string);
            hasEnrollmentInfo = true;
        }
        count++;
    }
    if (!isRecitation){
        if (!hasTime){
            printf("\"days\":\"N/A\", \"time\":\"N/A\",");
        }
        if (!hasTeacher){
            printf("\"tLname\":\"N/A\", \"tFname\":\"\",");
        }
        if (!hasEnrollmentInfo){
            printf("\"capacity\":\"N/A\", \"enrollment\":\"N/A\",\"waitlist_capacity\":\"N/A\",\"waitlist_total\":\"N/A\"");
        }
        printf("\"extra\": \"\"},");
        putchar('\n');
    }
}
bool compareStrings(char* s1, char* s2, int length){
    for (int i = 0; i < length; i++){
        if (s1[i] != s2[i]){ return false; } } return true; } void printCourse(char * string){ int i = 0; printf("{"); i = printMember(string, "subject", i);

            i = skipWhiteSpace(string, i);
            //i = printMember(string, "number", i);
            printf("\"%s\":\"", "number");
            int c;
            while ((c =string[i]) >= '0' && c <= '9'){
                putchar(c);
                i++;
            }
            printf("\",");
            i = skipWhiteSpace(string, i);
            if (string[i] == 'L' || string[i] == '-' || compareStrings(&string[i], "class", 5)){
                printf("=====DELETETHISLINE======");
                return;
            }
            i = printMember(string, "section", i);

            i = skipWhiteSpace(string, i);
            i = skipWord(string, i);
            i = skipWhiteSpace(string, i);
            i = printClassName(string, i);

        }
int skipWhiteSpace(char * string, int i){
    while (string[i] == ' '){
        i++;
    }
    return i;
}
int printMember(char* input, char* name, int i) {
    printf("\"%s\":\"", name);
    int c;
    while ((c =input[i]) != ' '){
        putchar(c);
        i++;
    }
    printf("\",");
    return i;
}
int skipWord(char* string, int i){
    while (string[i] != ' '){
        i++;
    }
    return i;
}
int printClassName(char * input, int i) {
    printf("\"%s\":\"", "name");
    int c;
    if (input[i] >= 'a' && input[i] <= 'z'){
        printf("=====DELETETHISLINE======");
    }
    while ((c = input[i]) != EOF && c != '\r' && c != '\n'){
        if (c == ' '){
            if ((c = input[++i]) == ' ' || c == '\r' || c == '\n'){
                break;
            }
            putchar(' ');
        }
        putchar(c);
        i++;
    }
    printf("\",");
    return i;
}
void printTimeAndDay(char*string){
    int i = 0;
    int c; 
    while (!compareStrings(&string[i], "Days:", 5)){
        c = string[i++];
    }
    i += 5;
    i = skipWhiteSpace(string, i);
    i = printMember(string, "days", i);
    while (!compareStrings(&string[i], "Time:", 5)){
        c = string[i++];
    }
    i +=5;
    i = skipWhiteSpace(string, i);
    printTime(string, i);
}
void printTime(char*string, int i){
    char c;
    printf("\"%s\":\"", "time");
    while ((c = string[i]) != '\r' && c != '\n'){
        c = string[i++];
        putchar(c);
    }
    printf("\",");
}
void printTeacher(char* string){
    int i = 0;
    int c; 
    while (!compareStrings(&string[i], "Instructor:", 11)){
        c = string[i++];
    }
    i +=11;
    i = skipWhiteSpace(string, i);
    if (string[i] == '\r'){
        printf("\"tLname\": \"N/A\",");
        printf("\"tFname\": \"\",");
        return;
    }
    if (compareStrings(&string[i], "Staff", 5)){
        printf("\"tLname\": \"Staff\",");
        printf("\"tFname\": \"\",");
        return;
    }
    printf("\"%s\":\"", "tLname");
    c = string[i++];
    putchar(c);
    while ((c = string[i]) != ',' && c != ' ' && c != '\r'){
        if (c >= 'A' && c <= 'Z'){
            c -= 'A'-'a';
        }
        putchar(c);
        i++;
    }
    printf("\",");
    //skip to comma
    while ((c = string[i]) != ','){
        i++;
    }
    i++;
    i = skipWhiteSpace(string, i);

    printf("\"%s\":\"", "tFname");
    c = string[i++];
    putchar(c);
    while ((c = string[i]) != ' ' && c != '\r'){
        if (c >= 'A' && c <= 'Z'){
            c -= 'A'-'a';
        }
        putchar(c);
        i++;
    }
    printf("\",");
}
void printEnrollmentInfo(char* string){
    int i = 0;
    i = skipToNumber(string, i);
    i = printMember(string, "capacity", i);

    i = skipToNumber(string, i);
    i = printMember(string, "enrollment", i);
    i = skipWhiteSpace(string, i);
    if (!compareStrings(&string[i], "Class Wait", 7)){
        printf("\"waitlist_capacity\": \"N/A\",");
        printf("\"waitlist_total\": \"N/A\","); 
        return;
    }
    i = skipToNumber(string, i);
    printf("\"%s\":\"", "waitlist_capacity");
    int c;
    while ((c =string[i]) != ' ' && c != '\0' && c != '\r' && c != '\n'){
        putchar(c);
        i++;
    }
    printf("\",");
    if (c == '\0' || c == '\r' || c == '\n'){
        printf("\"waitlist_total\": \"N/A\","); 
        return;
    }

    //get waitlist total on next line, if it is there
    while ((c = getchar()) < '0' || c > '9'){
        //skip to number
    }
    printf("\"%s\":\"", "waitlist_total");
    putchar(c);
    while (!((c = getchar()) < '0' || c > '9')){
        putchar(c);
    }
    printf("\",");


}
int skipToNumber(char* string, int i){
    while (!(string[i] >='0' && string[i] <='9')){
        i++;
    }
    return i;
}
