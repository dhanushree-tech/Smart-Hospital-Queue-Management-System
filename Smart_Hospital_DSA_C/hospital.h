#ifndef HOSPITAL_H
#define HOSPITAL_H

#define NAME_LEN 50
#define HASH_SIZE 101
#define MAX_QUEUE 500
#define MAX_HISTORY 500

typedef struct Patient {
    int id;
    char name[NAME_LEN];
    int age;
    int emergency;       /* 1 = emergency, 0 = normal */
    int active;          /* 1 = waiting, 0 = treated/cancelled */
    struct Patient *next;
    struct Patient *hashNext;
} Patient;

typedef struct {
    int ids[MAX_QUEUE];
    int front, rear, count;
} Queue;

typedef struct {
    int ids[MAX_QUEUE];  /* max-heap: higher emergency priority first */
    int count;
} PriorityQueue;

typedef struct {
    int ids[MAX_HISTORY];
    int top;
} Stack;

void initializeSystem(void);
void registerPatient(void);
void displayWaitingPatients(void);
void treatNextPatient(void);
void searchPatient(void);
void displayTreatmentHistory(void);
void cancelPatient(void);
void displayPatientCount(void);
void cleanupSystem(void);

#endif
