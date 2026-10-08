/*
 * Student Record Management System
 * --------------------------------
 * Features:
 *   - Add, display, search, update and delete student records
 *   - Each student has a roll number, name and marks in 3 subjects
 *   - Average and grade are calculated automatically
 *   - Records are saved to a binary file and loaded on startup
 *
 * Compile:  gcc -Wall -o student_records student_record_system.c
 * Run:      ./student_records
 */

#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN     50
#define SUBJECTS     3
#define DATA_FILE    "students.dat"

typedef struct {
    int   roll;
    char  name[NAME_LEN];
    int   marks[SUBJECTS];
    float average;
    char  grade;
} Student;

static Student students[MAX_STUDENTS];
static int student_count = 0;

/* ---------- Input helpers ---------- */

static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static int read_int(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            clear_input_buffer();
            return value;
        }
        printf("  Invalid number, please try again.\n");
        clear_input_buffer();
    }
}

static void read_line(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL)
        buffer[strcspn(buffer, "\n")] = '\0';
    else
        buffer[0] = '\0';
}

static int read_marks_value(int subject) {
    int mark;
    while (1) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "  Marks in subject %d (0-100): ", subject);
        mark = read_int(prompt);
        if (mark >= 0 && mark <= 100)
            return mark;
        printf("  Marks must be between 0 and 100.\n");
    }
}

/* ---------- Core logic ---------- */

static void calculate_result(Student *s) {
    int total = 0;
    for (int i = 0; i < SUBJECTS; i++)
        total += s->marks[i];

    s->average = (float)total / SUBJECTS;

    if (s->average >= 90)      s->grade = 'A';
    else if (s->average >= 75) s->grade = 'B';
    else if (s->average >= 60) s->grade = 'C';
    else if (s->average >= 40) s->grade = 'D';
    else                       s->grade = 'F';
}

static int find_student_index(int roll) {
    for (int i = 0; i < student_count; i++)
        if (students[i].roll == roll)
            return i;
    return -1;
}

static void read_marks(Student *s) {
    for (int i = 0; i < SUBJECTS; i++)
        s->marks[i] = read_marks_value(i + 1);
}

static void print_header(void) {
    printf("\n%-8s %-25s %-8s %-8s %-8s %-10s %-6s\n",
           "Roll", "Name", "Sub1", "Sub2", "Sub3", "Average", "Grade");
    printf("---------------------------------------------------------------------\n");
}

static void print_student(const Student *s) {
    printf("%-8d %-25s %-8d %-8d %-8d %-10.2f %-6c\n",
           s->roll, s->name, s->marks[0], s->marks[1], s->marks[2],
           s->average, s->grade);
}

/* ---------- Menu operations ---------- */

static void add_student(void) {
    if (student_count >= MAX_STUDENTS) {
        printf("Storage full. Cannot add more students.\n");
        return;
    }

    Student s;
    s.roll = read_int("Enter roll number: ");

    if (find_student_index(s.roll) != -1) {
        printf("A student with roll number %d already exists.\n", s.roll);
        return;
    }

    read_line("Enter name: ", s.name, NAME_LEN);
    if (s.name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }

    read_marks(&s);
    calculate_result(&s);

    students[student_count++] = s;
    printf("Student added successfully.\n");
}

static void display_all(void) {
    if (student_count == 0) {
        printf("No student records found.\n");
        return;
    }

    print_header();
    for (int i = 0; i < student_count; i++)
        print_student(&students[i]);
    printf("\nTotal students: %d\n", student_count);
}

static void search_student(void) {
    int roll = read_int("Enter roll number to search: ");
    int idx = find_student_index(roll);

    if (idx == -1) {
        printf("No student found with roll number %d.\n", roll);
        return;
    }

    print_header();
    print_student(&students[idx]);
}

static void update_student(void) {
    int roll = read_int("Enter roll number to update: ");
    int idx = find_student_index(roll);

    if (idx == -1) {
        printf("No student found with roll number %d.\n", roll);
        return;
    }

    Student *s = &students[idx];
    printf("Updating record for %s (press Enter to keep the current name).\n", s->name);

    char new_name[NAME_LEN];
    read_line("New name: ", new_name, NAME_LEN);
    if (new_name[0] != '\0')
        strcpy(s->name, new_name);

    printf("Re-enter marks:\n");
    read_marks(s);
    calculate_result(s);

    printf("Student record updated.\n");
}

static void delete_student(void) {
    int roll = read_int("Enter roll number to delete: ");
    int idx = find_student_index(roll);

    if (idx == -1) {
        printf("No student found with roll number %d.\n", roll);
        return;
    }

    for (int i = idx; i < student_count - 1; i++)
        students[i] = students[i + 1];
    student_count--;

    printf("Student record deleted.\n");
}

/* ---------- File handling ---------- */

static int save_to_file(void) {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("Error: could not open %s for writing.\n", DATA_FILE);
        return 0;
    }

    fwrite(&student_count, sizeof(int), 1, fp);
    fwrite(students, sizeof(Student), student_count, fp);
    fclose(fp);
    return 1;
}

static void load_from_file(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        return;  /* No file yet: start with an empty database. */
    }

    int count = 0;
    if (fread(&count, sizeof(int), 1, fp) == 1 &&
        count >= 0 && count <= MAX_STUDENTS) {
        student_count = (int)fread(students, sizeof(Student), count, fp);
    }
    fclose(fp);
}

/* ---------- Main menu ---------- */

static void print_menu(void) {
    printf("\n=============================\n");
    printf("  STUDENT RECORD MANAGEMENT\n");
    printf("=============================\n");
    printf("1. Add student\n");
    printf("2. Display all students\n");
    printf("3. Search student by roll number\n");
    printf("4. Update student\n");
    printf("5. Delete student\n");
    printf("6. Save and exit\n");
    printf("-----------------------------\n");
}

int main(void) {
    load_from_file();
    printf("Loaded %d student record(s).\n", student_count);

    int choice;
    do {
        print_menu();
        choice = read_int("Enter your choice: ");

        switch (choice) {
            case 1: add_student();       break;
            case 2: display_all();       break;
            case 3: search_student();    break;
            case 4: update_student();    break;
            case 5: delete_student();    break;
            case 6:
                if (save_to_file())
                    printf("Records saved. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please select 1-6.\n");
        }
    } while (choice != 6);

    return 0;
}
