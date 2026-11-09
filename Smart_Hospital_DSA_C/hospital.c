#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"

static Patient *patients = NULL;
static Patient *hashTable[HASH_SIZE];
static Queue normalQueue;
static PriorityQueue emergencyQueue;
static Stack treatedStack;
static int nextId = 1001;
static int totalRegistered = 0;

static int hashFunction(int id) { return id % HASH_SIZE; }

static Patient *findById(int id) {
    Patient *p = hashTable[hashFunction(id)];
    while (p != NULL) {
        if (p->id == id) return p;
        p = p->hashNext; /* traverse only this hash bucket's chain */
    }
    return NULL;
}

/* Insert into both the master linked list and the hash table's bucket chain. */
static void insertPatient(Patient *p) {
    int index = hashFunction(p->id);
    p->next = patients;
    patients = p;
    p->hashNext = hashTable[index];
    hashTable[index] = p;
}

/* Queue: FIFO for normal patients. */
static int enqueue(Queue *q, int id) {
    if (q->count == MAX_QUEUE) return 0;
    q->ids[q->rear] = id;
    q->rear = (q->rear + 1) % MAX_QUEUE;
    q->count++;
    return 1;
}

static int dequeue(Queue *q, int *id) {
    if (q->count == 0) return 0;
    *id = q->ids[q->front];
    q->front = (q->front + 1) % MAX_QUEUE;
    q->count--;
    return 1;
}

/* Priority queue implemented as a binary max-heap.
   Emergency patients are assigned priority 2; normal patients priority 1.
   This queue stores emergency patients only. */
static void heapPush(PriorityQueue *pq, int id) {
    int i = pq->count++;
    pq->ids[i] = id;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->ids[parent] >= pq->ids[i]) break;
        int temp = pq->ids[parent];
        pq->ids[parent] = pq->ids[i];
        pq->ids[i] = temp;
        i = parent;
    }
}

static int heapPop(PriorityQueue *pq, int *id) {
    if (pq->count == 0) return 0;
    *id = pq->ids[0];
    pq->count--;
    if (pq->count == 0) return 1;
    pq->ids[0] = pq->ids[pq->count];

    int i = 0;
    while (1) {
        int left = 2 * i + 1, right = 2 * i + 2, largest = i;
        if (left < pq->count && pq->ids[left] > pq->ids[largest]) largest = left;
        if (right < pq->count && pq->ids[right] > pq->ids[largest]) largest = right;
        if (largest == i) break;
        int temp = pq->ids[i];
        pq->ids[i] = pq->ids[largest];
        pq->ids[largest] = temp;
        i = largest;
    }
    return 1;
}

/* Stack: last treated patient is on top. */
static void pushHistory(int id) {
    if (treatedStack.top < MAX_HISTORY - 1)
        treatedStack.ids[++treatedStack.top] = id;
}

void initializeSystem(void) {
    memset(&normalQueue, 0, sizeof(normalQueue));
    memset(&emergencyQueue, 0, sizeof(emergencyQueue));
    treatedStack.top = -1;
    memset(hashTable, 0, sizeof(hashTable));
}

void registerPatient(void) {
    Patient *p = (Patient *)malloc(sizeof(Patient));
    if (!p) {
        printf("\nMemory allocation failed.\n");
        return;
    }

    p->id = nextId++;
    printf("\nEnter patient name (no spaces): ");
    scanf("%49s", p->name);
    printf("Enter age: ");
    if (scanf("%d", &p->age) != 1 || p->age < 0) {
        printf("Invalid age. Registration cancelled.\n");
        free(p);
        while (getchar() != '\n');
        return;
    }
    printf("Patient type (1 = Emergency, 0 = Normal): ");
    if (scanf("%d", &p->emergency) != 1 || (p->emergency != 0 && p->emergency != 1)) {
        printf("Invalid patient type. Registration cancelled.\n");
        free(p);
        while (getchar() != '\n');
        return;
    }
    p->active = 1;
    p->next = NULL;
    p->hashNext = NULL;

    /* Add to master linked list and hash index. */
    insertPatient(p);

    if (p->emergency) {
        if (emergencyQueue.count >= MAX_QUEUE) {
            printf("Emergency queue is full. Registration saved but not queued.\n");
            p->active = 0;
        } else {
            heapPush(&emergencyQueue, p->id);
        }
    } else if (!enqueue(&normalQueue, p->id)) {
        printf("Normal queue is full. Registration saved but not queued.\n");
        p->active = 0;
    }

    totalRegistered++;
    printf("\nPatient registered successfully!\nPatient ID: %d\n", p->id);
}

void displayWaitingPatients(void) {
    int i, index;
    printf("\n--- EMERGENCY WAITING LIST ---\n");
    if (emergencyQueue.count == 0) printf("No emergency patients waiting.\n");
    for (i = 0; i < emergencyQueue.count; i++) {
        int id = emergencyQueue.ids[i];
        Patient *p = findById(id);
        if (p && p->active)
            printf("ID: %d | Name: %s | Age: %d\n", p->id, p->name, p->age);
    }

    printf("\n--- NORMAL WAITING LIST (FIFO) ---\n");
    if (normalQueue.count == 0) printf("No normal patients waiting.\n");
    index = normalQueue.front;
    for (i = 0; i < normalQueue.count; i++) {
        int id = normalQueue.ids[index];
        Patient *p = findById(id);
        if (p && p->active)
            printf("ID: %d | Name: %s | Age: %d\n", p->id, p->name, p->age);
        index = (index + 1) % MAX_QUEUE;
    }
}

void treatNextPatient(void) {
    int id;
    Patient *p = NULL;

    /* Emergency queue is always checked before normal FIFO queue. */
    while (heapPop(&emergencyQueue, &id)) {
        p = findById(id);
        if (p && p->active) break;
        p = NULL;
    }
    if (p == NULL) {
        while (dequeue(&normalQueue, &id)) {
            p = findById(id);
            if (p && p->active) break;
            p = NULL;
        }
    }

    if (!p) {
        printf("\nNo patients are waiting.\n");
        return;
    }

    p->active = 0;
    pushHistory(p->id);
    printf("\nNow treating patient:\nID: %d | Name: %s | Age: %d | Type: %s\n",
           p->id, p->name, p->age, p->emergency ? "Emergency" : "Normal");
}

void searchPatient(void) {
    int id;
    printf("\nEnter Patient ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return;
    }
    Patient *p = findById(id);
    if (p)
        printf("\nPatient found:\nID: %d\nName: %s\nAge: %d\nType: %s\nStatus: %s\n",
               p->id, p->name, p->age, p->emergency ? "Emergency" : "Normal",
               p->active ? "Waiting" : "Treated/Cancelled");
    else
        printf("\nPatient ID not found.\n");
}

void displayTreatmentHistory(void) {
    int i;
    if (treatedStack.top == -1) {
        printf("\nNo treatment history available.\n");
        return;
    }
    printf("\n--- RECENTLY TREATED PATIENTS (STACK: LIFO) ---\n");
    for (i = treatedStack.top; i >= 0; i--) {
        Patient *p = patients;
        while (p && p->id != treatedStack.ids[i]) p = p->next;
        if (p) printf("ID: %d | Name: %s\n", p->id, p->name);
    }
}

void cancelPatient(void) {
    int id;
    printf("\nEnter Patient ID to cancel: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return;
    }
    Patient *p = patients;
    while (p && p->id != id) p = p->next;
    if (!p) {
        printf("Patient not found.\n");
    } else if (!p->active) {
        printf("Patient is not currently waiting.\n");
    } else {
        /* Mark inactive; queue removal is lazy when the entry reaches the front/top. */
        p->active = 0;
        printf("Waiting registration cancelled for Patient ID %d.\n", id);
    }
}

void displayPatientCount(void) {
    int waiting = 0, treated = 0;
    Patient *p = patients;
    while (p) {
        if (p->active) waiting++;
        else treated++;
        p = p->next;
    }
    printf("\nTotal registered: %d\nCurrently waiting: %d\nTreated/cancelled: %d\n",
           totalRegistered, waiting, treated);
}

void cleanupSystem(void) {
    Patient *p = patients;
    while (p) {
        Patient *next = p->next;
        free(p);
        p = next;
    }
    patients = NULL;
}
