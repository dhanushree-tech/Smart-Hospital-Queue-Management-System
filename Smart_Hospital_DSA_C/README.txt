SMART HOSPITAL QUEUE MANAGEMENT SYSTEM USING DSA IN C
=====================================================

PROJECT OBJECTIVE
To manage hospital patient registration, waiting queues, emergency priority,
treatment history and patient searching using data structures in C.

FILES
-----
main.c       - Menu-driven application and user interaction
hospital.c   - Data structure operations and project logic
hospital.h   - Patient structure, constants and function declarations

DSA CONCEPTS
------------
1. Queue:
   Normal patients are served in First In First Out (FIFO) order.

2. Priority Queue:
   Emergency patients are stored in a binary max-heap and are selected
   before normal patients.

3. Stack:
   Treated patient IDs are pushed onto a stack. The latest treated patient
   is displayed first (Last In First Out).

4. Linked List:
   Patient records are dynamically allocated and connected using pointers.

5. Hashing:
   Patient ID is converted to a hash-table index. Colliding IDs are stored
   in a separate linked chain in that bucket (separate chaining).

6. Searching:
   Patient details can be searched using the unique Patient ID.

HOW TO RUN IN VS CODE
---------------------
1. Install a C compiler (GCC / MinGW-w64) and the Microsoft C/C++ extension.
2. Extract this ZIP file.
3. Open the extracted folder in VS Code.
4. Open Terminal > New Terminal.
5. Compile:

   gcc main.c hospital.c -o hospital

6. Run on Windows:

   .\hospital.exe

   On Linux/macOS:

   ./hospital

SAMPLE TEST
-----------
Choose 1:
Name: Ravi
Age: 45
Patient type: 0
Patient ID will be 1001.

Choose 1 again:
Name: Kumar
Age: 30
Patient type: 1
Patient ID will be 1002.

Choose 2 to display the waiting list.
Choose 3 to treat next patient. Kumar (emergency) is selected first.
Choose 5 to display treatment history.

IMPORTANT NOTES
---------------
- Names are entered as one word in this beginner version.
- Patient data is stored in memory and is cleared when the program exits.
- Cancellation marks a patient inactive. The queue skips that patient later.
- This is an educational DSA prototype, not a real hospital medical system.
