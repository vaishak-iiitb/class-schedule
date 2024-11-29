# Timetable Scheduling System 

## About the Project

The **Timetable Scheduling System** is a command-line interface (CLI)-based tool designed to automate the generation of academic and exam timetables for educational institutions.
The system addresses challenges associated with manual scheduling, such as conflicting time slots, room allocation, and faculty availability.
By leveraging file input and output operations, it ensures efficient scheduling and exports the final timetables in CSV format for seamless integration.

### Key Features:
- **Automated Exam Scheduling:**  
  Generates batch-wise exam schedules, ensuring room availability.

- **Automated Academic Timetable Generation:**  
  Creates weekly academic timetables for all batches, optimizing room and professor availability.

- **Conflict Resolution:**  
  Detects and resolves scheduling conflicts, including faculty double-bookings and room allocation clashes.

- **Batch-wise Timetable Generation:**  
  Provides customized timetables for each batch in the college.

- **CSV Export:**  
  Outputs both exam schedules and academic timetables in a ready-to-deploy CSV format, similar to the official timetable provided by the institution.

### Scope:
- **Current Capabilities:**
  - **Class Timetable Generation:**  
    Automatically generate fully optimized timetables for all batches, accommodating all mandatory courses without faculty or room conflicts.
  
  - **Exam Timetable Generation:**  
    Create separate exam timetables that do not interfere with regular class schedules, ensuring smooth coordination during exam periods.

- **Future Capabilities:**
  - Generate personalized timetables for individual professors based on their availability.
  - Optimize the scheduling algorithm to improve efficiency and adaptability.
  - Expand the system to handle event scheduling, such as college sporting events.
  - Address scenarios where a single classroom cannot accommodate an entire batch.

### Built With  
- **Java** for input handling and exam scheduling.
- **C++** for backend scheduling algorithms and conflict resolution.
- **Java Native Interface (JNI)** to facilitate communication between the Java and C++ components.


---

## Getting Started

To set up and run the **Timetable Scheduling System**, follow the installation instructions for both the **Exam Schedule Generator** and **Time Table Generator** components as outlined below.

---

# Setup Instructions (Linux)

## File Structure
```
class-schedule/
├── ExamScheduler/
│   ├── Courses.java
│   ├── ExamDate.java
│   ├── ExamDetails.java
│   ├── ExamScheduleAlgo.cpp
│   ├── ExamScheduler.java
│   ├── Room.java
│   ├── RoomMatrixCell.java
│   ├── TimeSlot.java
│   ├── Readme.md
│   ├── courses.txt
│   ├── rooms.txt
│   └── dates.txt
└── TimeTableScheduler/
    ├── TimeTableGenerator.java
    ├── jni_bridge.cpp
    ├── TimeTableScheduler(withRand).cpp
    ├── Readme.md
    ├── courses_0.txt
    ├── courses_1.txt
    ├── rooms_0.txt
    ├── rooms_1.txt
    ├── professors_0.txt
    └── professors_1.txt
```
## Prerequisites
- **Operating System:** Linux
- **Java Development Kit (JDK)**
- **GNU C++ Compiler (g++)**
- **Environment Variables:**
  - `JAVA_HOME` must be set correctly to your JDK installation path within the terminal session.

## Setting Up `JAVA_HOME`

Before proceeding, ensure that the `JAVA_HOME` environment variable is set in your current terminal session. This is crucial for the JNI compilation step.

1. **Find Java Installation Path:**
   ```bash
   which java
   java -version
   ```
   Use the output to locate your JDK installation directory, for example, `/usr/lib/jvm/java-11-openjdk-amd64`.

2. **Set `JAVA_HOME`:**
   ```bash
   export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64
   ```
   Replace the path with your actual JDK installation path.

3. **Verify `JAVA_HOME`:**
   ```bash
   echo $JAVA_HOME
   ```
   This should output the path you set.

 **Note:** Ensure that you set `JAVA_HOME` in the same terminal session where you will perform the compilation and execution steps. Failure to do so may result in `g++` not finding JNI headers, leading to compilation errors.

## Steps to Run Exam Schedule Generator

 1. **Navigate to the Project Directory:**
   ```bash
   cd class-schedule/ExamScheduler
   ```

2. **Compile Java Classes and Generate Header File:**
   ```bash
   javac -h . *.java
   ```
   This command compiles all Java files and generates the necessary JNI header files (`.h` files) in the current directory.

3. **Compile Native Library:**
   Ensure that `JAVA_HOME` is exported in your terminal session.
   ```bash
   g++ -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/linux" -fPIC -shared ExamScheduleAlgo.cpp -o libexamscheduler.so
   ```
   - **Flags Explanation:**
     - `-I"$JAVA_HOME/include"` and `-I"$JAVA_HOME/include/linux"`: Include paths for JNI headers.
     - `-fPIC`: Generates position-independent code, which is required for shared libraries.
     - `-shared`: Creates a shared library.
     - `-o libexamscheduler.so`: Specifies the output file name.

4. **Set Library Path:**
   Add the current directory to `LD_LIBRARY_PATH` to ensure the JVM can locate the native library.
   ```bash
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)
   ```

5. **Run Java Program:**
   ```bash
   java -Djava.library.path=. ExamScheduler
   ```
   This command will generate an exam schedule and export it to CSV format (`ExamSchedules.csv`).

## Additional Notes

- **Data Files Location:** Ensure that the `courses.txt`, `rooms.txt`, and `dates.txt` files are located in the `ExamScheduler/` directory as specified in the file structure.

- **Error Handling:** If `g++` throws errors related to JNI headers or missing include paths, verify that `JAVA_HOME` is correctly set and exported in your current terminal session.

- **Clean Build (Optional):** To clean previous builds, you can remove the `libexamscheduler.so` file and regenerate it using the steps above.


## Steps to Run Time Table Generator

1. **Navigate to the Project Directory:**
   ```bash
   cd class-schedule/TimeTableScheduler
   ```

2. **Compile Java Classes and Generate Header File:**
   ```bash
   javac -h . TimeTableGenerator.java
   ```
   This command compiles the Java file and generates the necessary JNI header files (`TimeTableGenerator.h`) in the current directory.

3. **Compile Native Library:**
   Ensure that `JAVA_HOME` is exported in your terminal session.
   ```bash
   g++ -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/linux" -fPIC -shared jni_bridge.cpp TimeTableScheduler(withRand).cpp -o libtimetable_native.so
   ```
   - **Flags Explanation:**
     - `-I"$JAVA_HOME/include"` and `-I"$JAVA_HOME/include/linux"`: Include paths for JNI headers.
     - `-fPIC`: Generates position-independent code, which is required for shared libraries.
     - `-shared`: Creates a shared library.
     - `-o libtimetable_native.so`: Specifies the output file name.

4. **Set Library Path:**
   Add the current directory to `LD_LIBRARY_PATH` to ensure the JVM can locate the native library.
   ```bash
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)
   ```

5. **Run Java Program:**
   ```bash
   java -Djava.library.path=. TimeTableGenerator
   ```
   This command will generate a timetable and export it to CSV format (`timetable.csv`).

## Additional Notes

- **Data Files Location:** Ensure that the `courses_0.txt`, `courses_1.txt`, `rooms_0.txt`, `rooms_1.txt`, `professors_0.txt`, and `professors_1.txt` files are located in the `TimeTableScheduler/` directory as specified in the file structure.

- **Error Handling:** If `g++` throws errors related to JNI headers or missing include paths, verify that `JAVA_HOME` is correctly set and exported in your current terminal session.

- **Clean Build (Optional):** To clean previous builds, you can remove the `libtimetable_native.so` file and regenerate it using the steps above.


## Requirements

- **JDK (Java Development Kit):** Ensure that JDK is installed. You can verify by running:
  ```bash
  javac -version
  ```

- **GNU C++ Compiler:** Verify installation with:
  ```bash
  g++ --version
  ```

## Team members

 - **Team Members**  
      - A special thanks to each team member for their dedication and collaboration in bringing this project to life:  
         - [Akshat Batra](https://github.com/AkiBatra25) - IMT2023025
         - [Dhruv Ramesh Joshi](https://github.com/JDhruvR) - IMT2023032
         - [Ankith Kini](https://github.com/ANKITH33) - IMT2023075
         - [Arnav Oruganty](https://github.com/Arnav-Oruganty) - IMT2023078
         - [Vaishak Prasad Bhat](https://github.com/vaishak-iiitb) - IMT2023085 
         - [Harshit Krishna R](https://github.com/harshitKrishna-main) - IMT2023613
