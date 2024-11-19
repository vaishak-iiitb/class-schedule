#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Course {
    string batch;
    string name;
    string prof;
    int credits;
};

struct ExamDetails {
    string batch;
    string name;
    int slot;
    string date;
    string room;
};

struct RoomMatrixCell {
    int slot;
    string batch;
    string subject;
};

vector<Course> coursesList;
vector<string> roomsList;
vector<string> dates;

vector<ExamDetails> batch1Schedule;
vector<ExamDetails> batch2Schedule;
vector<ExamDetails> batch3Schedule;
vector<ExamDetails> batch4Schedule;
vector<ExamDetails> batch5Schedule;

string slotTime(int slot) {
    return (slot == 1) ? "9:30 am - 12:30 pm" : "2:00 pm - 5:00 pm";
}

bool compareByDate(ExamDetails &a, ExamDetails &b) {
    return a.date < b.date;
}

string getQuotedString(ifstream &inFile) {
    string quoted;
    inFile >> ws; // Consume any leading whitespace
    if (inFile.peek() == '\"') {
        getline(inFile, quoted, '\"'); // Skip the initial quote
        getline(inFile, quoted, '\"'); // Read the actual content
    } else {
        inFile >> quoted; // Fallback for unquoted strings
    }
    return quoted;
}

vector<Course> loadCourseData() {
    ifstream inFile("courses.txt");

    if (!inFile.is_open()) {
        cout << "Error: courses.txt does not exist or cannot be opened." << endl;
        return {};
    }

    cout << "Data successfully loaded from file." << endl;
    vector<Course> courses;
    string batch;
    int batchCount;

    while (inFile >> batch >> batchCount) {
        for (int i = 0; i < batchCount; i++) {
            string name, prof;
            int credits;

            name = getQuotedString(inFile); // Read quoted name
            prof = getQuotedString(inFile); // Read quoted prof
            inFile >> credits;

            if (inFile.fail()) {
                cout << "Error reading course details for batch: " << batch << endl;
                break;
            }

            courses.push_back({batch, name, prof, credits});
        }
    }

    inFile.close();
    return courses;
}

vector<string> loadRoomData() {
    ifstream inFile("rooms.txt");

    if (!inFile.is_open()) {
        cout << "Error: rooms.txt does not exist or cannot be opened." << endl;
        return {};
    }

    cout << "Data successfully loaded from file." << endl;
    vector<string> rooms;
    string room;
    while (inFile >> room) {
        rooms.push_back(room);
    }
    inFile.close();
    return rooms;
}

vector<string> loadDateData() {
    ifstream inFile("dates.txt");

    if (!inFile.is_open()) {
        cout << "Error: dates.txt does not exist or cannot be opened." << endl;
        return {};
    }

    cout << "Data successfully loaded from file." << endl;
    vector<string> dates;
    string date;
    while (inFile >> date) {
        dates.push_back(date);
    }
    inFile.close();
    return dates;
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

        roomMatrix[currentDate][currentRoom] = {currentSlot, course.batch, course.name};

        ExamDetails exam = {course.batch, course.name, currentSlot, dates[currentDate], rooms[currentRoom]};

        if (course.batch == "IMT2023") {
            batch1Schedule.push_back(exam);
        } else if (course.batch == "IMT2024") {
            batch2Schedule.push_back(exam);
        } else if (course.batch == "IMT2025") {
            batch3Schedule.push_back(exam);
        } else if (course.batch == "IMT2026") {
            batch4Schedule.push_back(exam);
        } else if (course.batch == "IMT2027") {
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
            outFile << exam.batch << "," << exam.name << "," << slotTime(exam.slot) << "," << exam.date << "," << exam.room << "\n";
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

void printSchedule(string batch, vector<ExamDetails> &schedule) {
    cout << batch << ":\n";
    sort(schedule.begin(), schedule.end(), compareByDate);
    for (auto &exam : schedule) {
        cout << "Name: " << exam.name << ", Slot: " << slotTime(exam.slot) << ", Date: " << exam.date
             << ", Room: " << exam.room << endl;
    }
    cout << "\n";
}

int main() {
    coursesList = loadCourseData();
    roomsList = loadRoomData();
    dates = loadDateData();

    // Check if the total number of courses exceeds the available slots
    int totalSlots = roomsList.size() * dates.size() * 2; // 2 slots per day per room
    if (coursesList.size() > totalSlots) {
        cout << "Error: Timetable is not possible as the total number of courses exceeds the available slots." << endl;
        return 1;
    }

    // Check if the number of courses for any batch exceeds the available slots for that batch
    int maxSlotsPerBatch = dates.size() * 2; // 2 slots per day for each batch
    unordered_map<string, int> batchCourseCount;
    for (const auto &course : coursesList) {
        batchCourseCount[course.batch]++;
    }
    for (const auto &entry : batchCourseCount) {
        if (entry.second > maxSlotsPerBatch) {
            cout << "Error: Timetable is not possible as the number of courses for batch "  << entry.first << " exceeds the available slots for that batch." << endl;
            return 1;
        }
    }

    vector<vector<RoomMatrixCell>> roomMatrix(dates.size(), vector<RoomMatrixCell>(roomsList.size()));
    makeSchedule(coursesList, roomsList, dates, roomMatrix);

    cout << "\nExam Schedules:\n" << endl;

    printSchedule("IMT2023", batch1Schedule);
    printSchedule("IMT2024", batch2Schedule);
    printSchedule("IMT2025", batch3Schedule);
    printSchedule("IMT2026", batch4Schedule);
    printSchedule("IMT2027", batch5Schedule);

    exportToCSV();

    return 0;
}
