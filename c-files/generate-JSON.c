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
static void printTimeAndDay(char* line);
static void printTeacher(char* line);
static void printEnrollmentInfo(char*line);
static void printMissingCourseInfo(Course* course);
static int  printMember(char* input, char* name, int i);
static bool newCourseFound(char * line, char * subject);
static bool isTeacherLine(char * line);
static bool isScheduleLine(char * line);
static bool isEnrollmentLine(char * line);
static int  skipWhiteSpace(char* line, int i);
static int  skipWord(char* line, int i);
static int  skipToNumber(char * line, int i);

int main(int argc, char *argv[0]){
    char line[1000];
    char *subject;

    if (argc >1){
        subject = argv[1];
    }
    else {
        fprintf(stderr, "%s", "A subject needs to be included\n");
        return EXIT_FAILURE;
    }
    Course course = {0};

    while(fgets(line, 1000, stdin) != NULL){
        if (newCourseFound(line,subject)){
            //finish up previous course
            if (course.isValid){
                printMissingCourseInfo(&course); 
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
                printTeacher(line);
                course.hasTeacher = true;
            }
            else if (!course.hasTime && isScheduleLine(line)){
                printTimeAndDay(line);
                course.hasTime = true;
            }
            else if (!course.hasEnrollmentInfo && isEnrollmentLine(line)){
                printEnrollmentInfo(line);
                course.hasEnrollmentInfo = true;
            }
        }    
    }
    //finish printing final course
    if (course.isValid){
        printMissingCourseInfo(&course);
    }
}

static void printCourseNameAndNumber(char * line){ 
    int i = 0; 
    int c;
    //print subject
    printf("{"); 
    i = printMember(line, "subject", i);
    //print course number
    i = skipWhiteSpace(line, i);
    printf("\"%s\":\"", "number");
    while ((c =line[i]) >= '0' && c <= '9'){
        putchar(c);
        i++;
    }
    printf("\",");
    //print section number
    i = skipWhiteSpace(line, i);
    if (line[i] == 'L' || line[i] == '-' || strncmp(&line[i], "class", 5) == 0){
        printf("=====DELETETHISLINE======");
        return;
    }
    i = printMember(line, "section", i);
    //print course name
    i = skipWhiteSpace(line, i);
    i = skipWord(line, i);
    i = skipWhiteSpace(line, i);
    printf("\"%s\":\"", "name");
    if (line[i] >= 'a' && line[i] <= 'z'){
        printf("=====DELETETHISLINE======");
    }
    while ((c = line[i]) != EOF && c != '\r' && c != '\n'){
        if (c == ' '){
            if ((c = line[++i]) == ' ' || c == '\r' || c == '\n'){
                break;
            }
            putchar(' ');
        }
        putchar(c);
        i++;
    }
    printf("\",");
}

static void printTimeAndDay(char*line){
    int i = 0;
    int c; 
    //print days
    while (strncmp(&line[i], "Days:", 5) != 0){
        c = line[i++];
    }
    i += 5;
    i = skipWhiteSpace(line, i);
    i = printMember(line, "days", i);
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

static void printTeacher(char* line){
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

static void printEnrollmentInfo(char* line){
    int i = 0;
    char c;
    //print capacity
    i = skipToNumber(line, i);
    i = printMember(line, "capacity", i);
    //print enrollment
    i = skipToNumber(line, i);
    i = printMember(line, "enrollment", i);
    //print waitlist info
    i = skipWhiteSpace(line, i);
    if (strncmp(&line[i], "Class Wait", 7) != 0){
        printf("\"waitlist_capacity\": \"N/A\",");
        printf("\"waitlist_total\": \"N/A\","); 
        return;
    }
    i = skipToNumber(line, i);
    printf("\"%s\":\"", "waitlist_capacity");
    while ((c =line[i]) != ' ' && c != '\0' && c != '\r' && c != '\n'){
        putchar(c);
        i++;
    }
    printf("\",");
    if (c == '\0' || c == '\r' || c == '\n'){
        printf("\"waitlist_total\": \"N/A\","); 
        return;
    }
    while ((c = getchar()) < '0' || c > '9'){}
    printf("\"%s\":\"", "waitlist_total");
    putchar(c);
    while (!((c = getchar()) < '0' || c > '9')){
        putchar(c);
    }
    printf("\",");
}

static void printMissingCourseInfo(Course* course){
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

//prints attribute, returns the int to move cursor forwards
static int printMember(char* input, char* name, int i) {
    printf("\"%s\":\"", name);
    int c;
    while ((c =input[i]) != ' '){
        putchar(c);
        i++;
    }
    printf("\",");
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

static int skipWhiteSpace(char * line, int i){
    while (line[i] == ' '){
        i++;
    }
    return i;
}

static int skipWord(char* line, int i){
    while (line[i] != ' '){
        i++;
    }
    return i;
}

static int skipToNumber(char* line, int i){
    while (!(line[i] >='0' && line[i] <='9')){
        i++;
    }
    return i;
}
