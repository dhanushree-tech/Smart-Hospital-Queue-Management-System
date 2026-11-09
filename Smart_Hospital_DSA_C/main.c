#include <stdio.h>
#include "hospital.h"

int main(void) {
    int choice;
    initializeSystem();

    do {
        printf("\n========================================\n");
        printf("   SMART HOSPITAL QUEUE MANAGEMENT\n");
        printf("========================================\n");
        printf("1. Register Patient\n");
        printf("2. Display Waiting Patients\n");
        printf("3. Treat Next Patient\n");
        printf("4. Search Patient by ID\n");
        printf("5. Display Treatment History\n");
        printf("6. Cancel Waiting Registration\n");
        printf("7. Display Patient Count\n");
        printf("0. Exit\n");
        printf("----------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Please enter a valid number.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }

        switch (choice) {
            case 1: registerPatient(); break;
            case 2: displayWaitingPatients(); break;
            case 3: treatNextPatient(); break;
            case 4: searchPatient(); break;
            case 5: displayTreatmentHistory(); break;
            case 6: cancelPatient(); break;
            case 7: displayPatientCount(); break;
            case 0: printf("\nExiting Hospital Management System. Thank you!\n"); break;
            default: printf("\nInvalid choice. Try again.\n");
        }
    } while (choice != 0);

    cleanupSystem();
    return 0;
}
