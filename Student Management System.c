/*
 * Student Management System
 * Menu-driven program: Add, Delete, Update, Search, Display
 * Uses a struct for each record and a binary file for permanent storage
 */

#include <stdio.h>
#include <string.h>

#define FILENAME "students.dat"

typedef struct {
    int id;
    char name[50];
    int age;
    float marks;
} Student;

/* ---------- Function declarations ---------- */
void addStudent();
void displayAll();
void searchStudent();
void updateStudent();
void deleteStudent();
int idExists(int id);

/* ---------- Function definitions ---------- */

/* Checks whether a given ID is already in the file.
   Returns 1 if found, 0 if not. Used to prevent duplicate IDs. */
int idExists(int id) {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) return 0; /* file doesn't exist yet, so no duplicates possible */

    Student temp;
    while (fread(&temp, sizeof(Student), 1, fp) == 1) {
        if (temp.id == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addStudent() {
    Student s;

    printf("Enter ID: ");
    scanf("%d", &s.id);

    if (idExists(s.id)) {
        printf("Error: A student with ID %d already exists.\n", s.id);
        return;
    }

    printf("Enter Name: ");
    scanf(" %49[^\n]", s.name); /* reads up to 49 chars including spaces */

    printf("Enter Age: ");
    scanf("%d", &s.age);

    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    FILE *fp = fopen(FILENAME, "ab"); /* append binary: adds without erasing existing records */
    if (fp == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Student added successfully.\n");
}

void displayAll() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found. File does not exist yet.\n");
        return;
    }

    Student temp;
    int count = 0;

    printf("\n%-6s %-20s %-6s %-8s\n", "ID", "Name", "Age", "Marks");
    printf("------------------------------------------\n");

    while (fread(&temp, sizeof(Student), 1, fp) == 1) {
        printf("%-6d %-20s %-6d %-8.2f\n", temp.id, temp.name, temp.age, temp.marks);
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf("No records found.\n");
    }
}

void searchStudent() {
    int id;
    printf("Enter ID to search: ");
    scanf("%d", &id);

    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found. File does not exist yet.\n");
        return;
    }

    Student temp;
    int found = 0;

    while (fread(&temp, sizeof(Student), 1, fp) == 1) {
        if (temp.id == id) {
            printf("\nRecord found:\n");
            printf("ID: %d\nName: %s\nAge: %d\nMarks: %.2f\n",
                   temp.id, temp.name, temp.age, temp.marks);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("No student found with ID %d.\n", id);
    }
}

void updateStudent() {
    int id;
    printf("Enter ID to update: ");
    scanf("%d", &id);

    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found. File does not exist yet.\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    Student s;
    int found = 0;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            found = 1;
            printf("Enter new Name: ");
            scanf(" %49[^\n]", s.name);
            printf("Enter new Age: ");
            scanf("%d", &s.age);
            printf("Enter new Marks: ");
            scanf("%f", &s.marks);
        }
        fwrite(&s, sizeof(Student), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (found) {
        printf("Student record updated successfully.\n");
    } else {
        printf("No student found with ID %d.\n", id);
    }
}

void deleteStudent() {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found. File does not exist yet.\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    Student s;
    int found = 0;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            found = 1;
            continue; /* skip writing this record, effectively deleting it */
        }
        fwrite(&s, sizeof(Student), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (found) {
        printf("Student record deleted successfully.\n");
    } else {
        printf("No student found with ID %d.\n", id);
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n===== Student Management System =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            /* clear invalid non-numeric input so the loop doesn't spin forever */
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6:
                printf("Exiting program. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please select between 1 and 6.\n");
        }
    }

    return 0;
}
