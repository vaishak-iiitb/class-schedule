#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <jni.h>
//#include "ExamScheduler.h" // This will be generated by javah
#include <vector>
#include <string>

using namespace std;

class Course {
private:
    string batch;
    string name;
    string prof;
    int credits;

public:
    Course(string batch, string name, string prof, int credits)
        : batch(batch), name(name), prof(prof), credits(credits) {}

    string getBatch() const { return batch; }
    string getName() const { return name; }
    string getProf() const { return prof; }
    int getCredits() const { return credits; }
};

class ExamDetails {
private:
    string batch;
    string name;
    int slot;
    string date;
    string room;

public:
    ExamDetails(string batch, string name, int slot, string date, string room)
        : batch(batch), name(name), slot(slot), date(date), room(room) {}

    string getBatch() const { return batch; }
    string getName() const { return name; }
    int getSlot() const { return slot; }
    string getDate() const { return date; }
    string getRoom() const { return room; }

    static bool compareByDate(const ExamDetails &a, const ExamDetails &b) {
        return a.getDate() < b.getDate();
    }
};

class RoomMatrixCell {
private:
    int slot;
    string batch;
    string subject;

public:
    RoomMatrixCell(int slot, string batch, string subject)
        : slot(slot), batch(batch), subject(subject) {}

    int getSlot() const { return slot; }
    string getBatch() const { return batch; }
    string getSubject() const { return subject; }
};

void makeSchedule(vector<Course> &courses, vector<string> &rooms, vector<string> &dates, vector<vector<RoomMatrixCell>> &roomMatrix);

vector<Course> coursesList;
vector<string> roomsList;
vector<string> dates;

vector<ExamDetails> batch1Schedule;
vector<ExamDetails> batch2Schedule;
vector<ExamDetails> batch3Schedule;
vector<ExamDetails> batch4Schedule;
vector<ExamDetails> batch5Schedule;

string jstring2string(JNIEnv* env, jstring jStr) {
    if (!jStr) return "";
    const char* cstr = env->GetStringUTFChars(jStr, nullptr);
    std::string str(cstr);
    env->ReleaseStringUTFChars(jStr, cstr);
    return str;
}

string slotTime(int slot) {
    return (slot == 1) ? "9:30 am - 12:30 pm" : "2:00 pm - 5:00 pm";
}

void printSchedule(string batch, vector<ExamDetails> &schedule) {
    cout << batch << ":\n";
    sort(schedule.begin(), schedule.end(), ExamDetails::compareByDate);
    for (auto &exam : schedule) {
        cout << "Name: " << exam.getName() << ", Slot: " << slotTime(exam.getSlot()) << ", Date: " << exam.getDate()
             << ", Room: " << exam.getRoom() << endl;
    }
    cout << "\n";
}

void makeSchedule(vector<Course> &courses, vector<string> &rooms, vector<string> &dates, vector<vector<RoomMatrixCell>> &roomMatrix) {

    int currentSlot = 1;
    int currentRoom = 0;
    int currentDate = 0;

    for (auto &course : courses) {
        if (currentSlot > 2 && currentRoom == 0 && currentDate == 0) {
            cout << "Error: Insufficient rooms or dates to schedule all exams." << endl;
            return;
        }

        roomMatrix[currentDate][currentRoom] = RoomMatrixCell(currentSlot, course.getBatch(), course.getName());

        ExamDetails exam(course.getBatch(), course.getName(), currentSlot, dates[currentDate], rooms[currentRoom]);

        if (course.getBatch() == "IMT2023") {
            batch1Schedule.push_back(exam);
        } else if (course.getBatch() == "IMT2024") {
            batch2Schedule.push_back(exam);
        } else if (course.getBatch() == "IMT2025") {
            batch3Schedule.push_back(exam);
        } else if (course.getBatch() == "IMT2026") {
            batch4Schedule.push_back(exam);
        } else if (course.getBatch() == "IMT2027") {
            batch5Schedule.push_back(exam);
        }

        currentDate++;
        if (currentDate == dates.size()) {
            currentDate = 0;
            currentSlot++;
            if (currentSlot > 2) {
                currentSlot = 1;
                currentRoom++;
                if (currentRoom == rooms.size()) {
                    currentRoom = 0;
                }
            }
        }
    }
}

void exportToCSV() {
    ofstream outFile("ExamSchedules.csv");

    if (!outFile.is_open()) {
        cout << "Error: Could not open file for writing!" << endl;
        return;
    }

    outFile << "Batch,Name,Time Slot,Date,Room\n";

    auto writeSchedule = [&outFile](vector<ExamDetails> &schedule) {
        for (auto &exam : schedule) {
            outFile << exam.getBatch() << "," << exam.getName() << "," << slotTime(exam.getSlot()) << "," << exam.getDate() << "," << exam.getRoom() << "\n";
        }
    };

    writeSchedule(batch1Schedule);
    writeSchedule(batch2Schedule);
    writeSchedule(batch3Schedule);
    writeSchedule(batch4Schedule);
    writeSchedule(batch5Schedule);

    outFile.close();
    cout << "Exam schedules exported to ExamSchedules.csv successfully!" << endl;
}

extern "C" {

JNIEXPORT void JNICALL Java_ExamScheduler_generateSchedule(JNIEnv* env, jobject thisObj, jobjectArray coursesArray, jobjectArray roomsArray, jobjectArray datesArray) {

    // Get array lengths
    jsize numCourses = env->GetArrayLength(coursesArray);
    jsize numRooms = env->GetArrayLength(roomsArray);
    jsize numDates = env->GetArrayLength(datesArray);

    // Convert courses array
    vector<Course> courses;
    jclass courseClass = env->FindClass("Courses");
    jmethodID getBatchMethod = env->GetMethodID(courseClass, "getBatch", "()Ljava/lang/String;");
    jmethodID getNameMethod = env->GetMethodID(courseClass, "getName", "()Ljava/lang/String;");
    jmethodID getProfMethod = env->GetMethodID(courseClass, "getProf", "()Ljava/lang/String;");
    jmethodID getCreditsMethod = env->GetMethodID(courseClass, "getCredits", "()I");

    for (int i = 0; i < numCourses; i++) {
        jobject courseObj = env->GetObjectArrayElement(coursesArray, i);

        jstring batchStr = (jstring)env->CallObjectMethod(courseObj, getBatchMethod);
        jstring nameStr = (jstring)env->CallObjectMethod(courseObj, getNameMethod);
        jstring profStr = (jstring)env->CallObjectMethod(courseObj, getProfMethod);
        jint credits = env->CallIntMethod(courseObj, getCreditsMethod);

        string batch = jstring2string(env, batchStr);
        string name = jstring2string(env, nameStr);
        string prof = jstring2string(env, profStr);

        courses.emplace_back(batch, name, prof, credits);

        env->DeleteLocalRef(courseObj);
    }

    // Convert rooms array
    vector<string> rooms;
    for (int i = 0; i < numRooms; i++) {
        jstring roomStr = (jstring)env->GetObjectArrayElement(roomsArray, i);
        rooms.push_back(jstring2string(env, roomStr));
        env->DeleteLocalRef(roomStr);
    }

    // Convert dates array
    vector<string> dates;
    for (int i = 0; i < numDates; i++) {
        jstring dateStr = (jstring)env->GetObjectArrayElement(datesArray, i);
        dates.push_back(jstring2string(env, dateStr));
        env->DeleteLocalRef(dateStr);
    }

    // Create room matrix and make schedule
    vector<vector<RoomMatrixCell>> roomMatrix(dates.size(),
        vector<RoomMatrixCell>(rooms.size(), RoomMatrixCell(0, "", "")));

    // Call the existing makeSchedule function
    makeSchedule(courses, rooms, dates, roomMatrix);

    printSchedule("IMT2023", batch1Schedule);
    printSchedule("IMT2024", batch2Schedule);
    printSchedule("IMT2025", batch3Schedule);
    printSchedule("IMT2026", batch4Schedule);
    printSchedule("IMT2027", batch5Schedule);
    exportToCSV();
}

}
