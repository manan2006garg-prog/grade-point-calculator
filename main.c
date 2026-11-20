#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COURSES 20
#define NAME_SIZE 64
#define INPUT_SIZE 128

typedef struct {
    char name[NAME_SIZE];
    int credits;
    double grade_point;
} Course;

static int read_line(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) return 0;
    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) { }
    }
    return 1;
}

static int parse_double(const char *text, double *value) {
    char *end = NULL;
    errno = 0;
    *value = strtod(text, &end);
    if (text == end || errno == ERANGE) return 0;
    while (isspace((unsigned char)*end)) end++;
    return *end == '\0';
}

static int parse_int(const char *text, int *value) {
    char *end = NULL;
    errno = 0;
    long parsed = strtol(text, &end, 10);
    if (text == end || errno == ERANGE || parsed < 1 || parsed > 1000000) return 0;
    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;
    *value = (int)parsed;
    return 1;
}

static int read_int_range(const char *prompt, int min, int max) {
    char buffer[INPUT_SIZE];
    int value;
    while (1) {
        if (!read_line(prompt, buffer, sizeof(buffer))) {
            puts("Input ended.");
            exit(EXIT_FAILURE);
        }
        if (parse_int(buffer, &value) && value >= min && value <= max) return value;
        printf("Enter a whole number from %d to %d.\n", min, max);
    }
}

static double read_grade_point(void) {
    char buffer[INPUT_SIZE];
    double value;
    while (1) {
        if (!read_line("Grade point (0.0-10.0): ", buffer, sizeof(buffer))) {
            puts("Input ended.");
            exit(EXIT_FAILURE);
        }
        if (parse_double(buffer, &value) && value >= 0.0 && value <= 10.0) return value;
        puts("Enter a grade point from 0.0 to 10.0.");
    }
}

static int valid_name(const char *name) {
    if (name[0] == '\0') return 0;
    for (size_t i = 0; name[i] != '\0'; i++) {
        if (name[i] == ',' || name[i] == '\n' || name[i] == '\r') return 0;
    }
    return 1;
}

int main(void) {
    Course courses[MAX_COURSES];
    int course_count = read_int_range("Number of courses (1-20): ", 1, MAX_COURSES);
    int total_credits = 0;
    double weighted_points = 0.0;
    char line[INPUT_SIZE];

    for (int i = 0; i < course_count; i++) {
        printf("\nCourse %d\n", i + 1);
        while (1) {
            if (!read_line("Course name: ", line, sizeof(line))) {
                puts("Input ended.");
                return EXIT_FAILURE;
            }
            if (strlen(line) < NAME_SIZE && valid_name(line)) {
                strcpy(courses[i].name, line);
                break;
            }
            puts("Use a non-empty name under 64 characters without commas.");
        }
        courses[i].credits = read_int_range("Credits (1-6): ", 1, 6);
        courses[i].grade_point = read_grade_point();
        total_credits += courses[i].credits;
        weighted_points += courses[i].credits * courses[i].grade_point;
    }

    double sgpa = weighted_points / total_credits;
    printf("\nSemester summary\n");
    printf("%-24s %7s %12s\n", "Course", "Credits", "Grade point");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < course_count; i++) {
        printf("%-24s %7d %12.2f\n", courses[i].name,
               courses[i].credits, courses[i].grade_point);
    }
    printf("Total credits: %d\nWeighted SGPA: %.2f / 10.00\n", total_credits, sgpa);

    FILE *report = fopen("sgpa_report.txt", "w");
    if (report == NULL) {
        perror("Could not save sgpa_report.txt");
        return EXIT_FAILURE;
    }
    fprintf(report, "Semester GPA Report\n\nCourse,Credits,Grade Point\n");
    for (int i = 0; i < course_count; i++) {
        fprintf(report, "%s,%d,%.2f\n", courses[i].name,
                courses[i].credits, courses[i].grade_point);
    }
    fprintf(report, "\nTotal credits,%d\nWeighted SGPA,%.2f\n", total_credits, sgpa);
    if (fclose(report) != 0) {
        perror("Could not finish saving the report");
        return EXIT_FAILURE;
    }
    puts("Saved details to sgpa_report.txt.");
    return 0;
}
