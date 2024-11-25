# Time Table Generator - Setup Instructions (Linux)

## File Structure
```
class-schedule/
    ├── TimeTableGenerator.java
    ├── jni_bridge.cpp
    ├── TimeTableScheduler(withRand).cpp
    ├── courses.txt
    ├── rooms.txt
    ├── Professors.txt
    └── dates.txt
```

## Steps to Run

1. **Generate Java Header File**
```bash
javac -h . TimeTableGenerator.java
```
This creates `TimeTableGenerator.h`

2. **Compile Native Library**
```bash
g++ -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/linux" -shared -fPIC jni_bridge.cpp -o libtimetable_native.so
```

3. **Set Library Path** (if needed)
```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
```

4. **Run Java Program**
```bash
java -Djava.library.path=. TimeTableGenerator
```

The program will generate a timetable and export it to CSV format.

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
