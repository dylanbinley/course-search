#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
typedef struct Course {
    bool isValid;
    bool hasTime;
    bool hasTeacher;
    bool hasEnrollmentInfo;
} Course;

static void printCourseNameAndNumber(char* line);
static void printCourseTimeAndDay(char* line);
static void printCourseTeacher(char* line);
static void printCourseEnrollmentInfo(char*line);
static void printCourseMissingInfo(Course* course);
static int printAttribute(char* input, char* attribute, int i, bool isString);
static bool newCourseFound(char * line, char * subject);
static bool isTeacherLine(char * line);
static bool isScheduleLine(char * line);
static bool isEnrollmentLine(char * line);
static bool isEndOfInput(int c);
static int  skipWhiteSpace(char* line, int i);
static int  skipWord(char* line, int i);
static int  skipToNumber(char * line, int i);

int main(int argc, char **argv){
    char initial_line[1000];
    char *subject;
    char * line;
    if (argc >1){
        subject = argv[1];
    }
    else {
        fprintf(stderr, "%s", "A subject needs to be included\n");
        return EXIT_FAILURE;
    }
    Course course = {0};
    while(fgets(initial_line, 1000, stdin) != NULL){
        line = &initial_line[skipWhiteSpace(initial_line, 0)];
        if (newCourseFound(line,subject)){
            //finish up previous course
            if (course.isValid){
                printCourseMissingInfo(&course); 
            }
            //exclude course if it's a recitation
            if (getchar() == 'R'){
                course.isValid = false; 
                continue;
            }
            //set up new course
            Course newCourse = {true,false,false,false}; 
            course = newCourse;
            printCourseNameAndNumber(line);
        }
        if (course.isValid){
            if (!course.hasTeacher && isTeacherLine(line)){ 
                printCourseTeacher(line);
                course.hasTeacher = true;
            }
            else if (!course.hasTime && isScheduleLine(line)){
                printCourseTimeAndDay(line);
                course.hasTime = true;
            }
            else if (!course.hasEnrollmentInfo && isEnrollmentLine(line)){
                printCourseEnrollmentInfo(line);
                course.hasEnrollmentInfo = true;
            }
        }    
    }
    //finish printing final course
    if (course.isValid){
        printCourseMissingInfo(&course);
    }
}

static void printCourseNameAndNumber(char * line){ 
    int i = 0; 
    int c;
    //print subject
    printf("{"); 
    i = printAttribute(line, "subject", i, true);
    //print course number
    i = skipWhiteSpace(line, i);
    printf("\"%s\":", "number");
    while ((c =line[i]) >= '0' && c <= '9'){
        putchar(c);
        i++;
    }
    printf(",");
    //print section number
    i = skipWhiteSpace(line, i);
    if (line[i] == 'L' || line[i] == '-' || strncmp(&line[i], "class", 5) == 0){
        printf("=====DELETETHISLINE======");
        return;
    }
    i = printAttribute(line, "section", i, true);
    //print course name
    i = skipWhiteSpace(line, i);
    i = skipWord(line, i);
    i = skipWhiteSpace(line, i);
    printf("\"%s\":\"", "name");
    if (line[i] >= 'a' && line[i] <= 'z'){
        printf("=====DELETETHISLINE======");
    }
    while (strncmp(&line[i], "Lecture", 5) != 0 && (c = line[i]) != EOF && c != '\r' && c != '\n'){
        if (c == ' '){
            if (strncmp(&line[i+1], "Lecture", 5) == 0 || (c = line[++i]) == ' ' || c == '\r' || c == '\n'){
                break;
            }
            putchar(' ');
        }
        putchar(c);
        i++;
    }
    printf("\",");
}

static void printCourseTimeAndDay(char*line){
    int i = 0;
    int c; 
    //print days
    while (strncmp(&line[i], "Days:", 5) != 0){
        c = line[i++];
    }
    i += 5;
    i = skipWhiteSpace(line, i);
    i = printAttribute(line, "days", i, true);
    //print time
    while (strncmp(&line[i], "Time:", 5) != 0){
        c = line[i++];
    }
    i += 5;
    i = skipWhiteSpace(line, i);
    printf("\"%s\":\"", "time");
    while ((c = line[i]) != '\r' && c != '\n'){
        c = line[i++];
        putchar(c);
    }
    printf("\",");
}

static void printCourseTeacher(char* line){
    int i = 0;
    int c; 
    while (strncmp(&line[i], "Instructor:", 11) != 0){
        c = line[i++];
    }
    i +=11;
    i = skipWhiteSpace(line, i);
    if (line[i] == '\r'){
        printf("\"tLname\": \"N/A\",");
        printf("\"tFname\": \"\",");
        return;
    }
    if (strncmp(&line[i], "Staff", 5) == 0){
        printf("\"tLname\": \"Staff\",");
        printf("\"tFname\": \"\",");
        return;
    }
    printf("\"%s\":\"", "tLname");
    c = line[i++];
    putchar(c);
    while ((c = line[i]) != ',' && c != ' ' && c != '\r'){
        if (c >= 'A' && c <= 'Z'){
            c -= 'A'-'a';
        }
        putchar(c);
        i++;
    }
    printf("\",");
    while ((c = line[i]) != ','){ 
        i++; 
    }
    i++;
    i = skipWhiteSpace(line, i);
    printf("\"%s\":\"", "tFname");
    c = line[i++];
    putchar(c);
    while ((c = line[i]) != ' ' && c != '\r'){
        if (c >= 'A' && c <= 'Z'){
            c -= 'A'-'a';
        }
        putchar(c);
        i++;
    }
    printf("\",");
}

static void printCourseEnrollmentInfo(char* line){
    int i = 0;
    //print capacity
    i = skipToNumber(line, i);
    i = printAttribute(line, "capacity", i, false);
    //print enrollment
    i = skipToNumber(line, i);
    i = printAttribute(line, "enrollment", i, false);
    //print waitlist info
    i = skipWhiteSpace(line, i);
    if (strncmp(&line[i], "Class Wait", 7) != 0){
        printf("\"waitlist_capacity\": \"N/A\",");
        printf("\"waitlist_total\": \"N/A\","); 
        return;
    }
    i = skipToNumber(line, i);
    i = printAttribute(line, "waitlist_capacity", i, false);
    i = skipToNumber(line, i);
    printAttribute(line, "waitlist_total", i, false);
}

static void printCourseMissingInfo(Course* course){
    if (!course->hasTime){
        printf("\"days\":\"N/A\", \"time\":\"N/A\",");
    }
    if (!course->hasTeacher){
        printf("\"tLname\":\"N/A\", \"tFname\":\"\",");
    }
    if (!course->hasEnrollmentInfo){
        printf("\"capacity\":\"N/A\", \"enrollment\":\"N/A\",\"waitlist_capacity\":\"N/A\",\"waitlist_total\":\"N/A\",");
    }
    printf("\"extra\": \"\"},\n");
}

//print attribute, return int to move cursor forward
static int printAttribute(char* input, char* attribute, int i, bool isString) {
    printf("\"%s\":", attribute);
    if (isString){
        putchar('\"');
    }
    while (input[i] != ' ')
    {
        if (isEndOfInput(input[i])){
            return -1;
        }
        putchar(input[i]);
        i++;
    }
    if (isString){
        putchar('\"');
    }
    putchar(',');
    return i;
}

static bool newCourseFound(char * line, char * subject){
    return strncmp(line,subject, 4) == 0;
}

static bool isTeacherLine(char * line){
    return strncmp(line, "GA", 2) == 0    || 
        strncmp(line, "TA", 2) == 0    || 
        strncmp(line, "RA", 2) == 0    || 
        strncmp(line, "DEAN", 4) == 0  || 
        strncmp(line, "INST", 4) == 0;
}

static bool isScheduleLine(char * line){
    return strncmp(line, "Bldg", 4) == 0;
}

static bool isEnrollmentLine(char * line){
    return strncmp(line, "Class Enrl", 10) == 0;
}

static bool isEndOfInput(int c){
    return c == EOF  ||
        c == '\0' ||
        c == '\r' ||
        c == '\n';
}
static int skipWhiteSpace(char * line, int i){
    while (line[i] == ' ' ){
        if (isEndOfInput(line[i])){
            return -1;
        }
        i++;
    }
    return i;
}

static int skipWord(char* line, int i){
    while (line[i] != ' ' ){
        if (isEndOfInput(line[i])){
            return -1;
        }
        i++;
    }
    return i;
}

static int skipToNumber(char* line, int i){
    while (!(line[i] >='0' && line[i] <='9')){
        if (isEndOfInput(line[i])){
            return -1;
        }
        i++;
    }
    return i;
}
