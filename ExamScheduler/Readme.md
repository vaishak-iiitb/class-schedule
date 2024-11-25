# Exam Schedule Generator - Setup Instructions (Linux)

## File Structure
```
ExamScheduler/
    ├── ExamScheduler.java
    ├── ExamScheduleAlgo.cpp
    ├── courses.txt
    ├── rooms.txt
    └── dates.txt
```

## Steps to Run

1. **Generate Java Header File**
```bash
javac -h . ExamScheduler.java
```

2. **Compile Native Library**
```bash
g++ -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/linux" -shared -fPIC ExamScheduleAlgo.cpp -o libexamscheduler.so
```

3. **Set Library Path (if needed)**
```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
```

4. **Run Java Program**
```bash
java -Djava.library.path=. ExamScheduler
```

The program will generate an exam schedule and export it to CSV format.

## Requirements
- JDK (Java Development Kit)
- G++ compiler
- JAVA_HOME environment variable set correctly

## Checking JAVA_HOME
```bash
echo $JAVA_HOME
```

If not set, find Java installation:
```bash
which java
java -version
update-alternatives --config java
```
