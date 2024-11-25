#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// Structures for Professor, Classroom, and Batch
class Professor {
public:
    string name;
    unordered_set<string> availability; // Format: "day,period"
    vector<string> subjects;

    Professor(string name, vector<string> subjects, unordered_set<string> availability)
        : name(name), subjects(subjects), availability(availability) {}
};

class Classroom {
public:
    string id;
    unordered_set<string> availability; // Format: "day,period"

    Classroom(string id, unordered_set<string> availability)
        : id(id), availability(availability) {}
};

class Batch {
public:
    string name;
    unordered_map<string, int> required_subjects; // Subject -> Number of periods

    Batch(string name, unordered_map<string, int> required_subjects)
        : name(name), required_subjects(required_subjects) {}
};

// Helper function to check constraints
bool isSlotAvailable(
    int day, int period, const string& professor, const string& subject,
    const string& classroom, const string& batch,
    const vector<Professor>& professors, const vector<Classroom>& classrooms,
    const unordered_map<string, vector<vector<tuple<string, string, string>>>>& timetable) {
    
    string slotKey = to_string(day) + "," + to_string(period);

    auto prof = find_if(professors.begin(), professors.end(),
        [&professor](const Professor& p) { return p.name == professor; });
    if (prof == professors.end() || prof->availability.find(slotKey) == prof->availability.end())
        return false;

    auto room = find_if(classrooms.begin(), classrooms.end(),
        [&classroom](const Classroom& c) { return c.id == classroom; });
    if (room == classrooms.end() || room->availability.find(slotKey) == room->availability.end())
        return false;

    for (const auto& [otherBatchName, otherSchedule] : timetable) {
        if (otherBatchName == batch) continue;
        const auto& otherSlot = otherSchedule[day][period];
        if (!get<0>(otherSlot).empty() && (get<0>(otherSlot) == professor || get<2>(otherSlot) == classroom))
            return false;
    }

    return true;
}

// Backtracking function
bool scheduleBatch(
    int batchIndex, vector<Batch>& batches, const vector<Professor>& professors,
    const vector<Classroom>& classrooms,
    unordered_map<string, vector<vector<tuple<string, string, string>>>>& timetable) {

    if (batchIndex >= batches.size())
        return true; // All batches scheduled successfully

    Batch& currentBatch = batches[batchIndex];
    auto& currentSchedule = timetable[currentBatch.name];

    for (auto& [subject, periodsNeeded] : currentBatch.required_subjects) {
        auto prof = find_if(professors.begin(), professors.end(),
            [&subject](const Professor& p) { return find(p.subjects.begin(), p.subjects.end(), subject) != p.subjects.end(); });
        if (prof == professors.end())
            return false;

        int hoursScheduled = 0;
        while (hoursScheduled < periodsNeeded) {
            vector<tuple<int, int, string>> availableSlots;
            for (int day = 0; day < 5; ++day) {
                for (int period = 0; period < 4; ++period) {
                    for (const auto& classroom : classrooms) {
                        if (get<0>(currentSchedule[day][period]).empty() &&
                            isSlotAvailable(day, period, prof->name, subject, classroom.id, currentBatch.name, professors, classrooms, timetable)) {
                            availableSlots.emplace_back(day, period, classroom.id);
                        }
                    }
                }
            }

            if (availableSlots.empty())
                return false; // No available slots; backtrack

            auto [day, period, classroom] = availableSlots.front();
            currentSchedule[day][period] = {prof->name, subject, classroom};
            hoursScheduled++;
        }
    }

    if (scheduleBatch(batchIndex + 1, batches, professors, classrooms, timetable))
        return true;

    for (auto& day : currentSchedule)
        for (auto& period : day)
            period = {"", "", ""};
    return false;
}

// Wrapper function
bool createTimetable(
    vector<Professor>& professors, vector<Classroom>& classrooms,
    vector<Batch>& batches,
    unordered_map<string, vector<vector<tuple<string, string, string>>>>& timetable) {

    for (auto& batch : batches)
        timetable[batch.name] = vector<vector<tuple<string, string, string>>>(5, vector<tuple<string, string, string>>(4, {"", "", ""}));

    return scheduleBatch(0, batches, professors, classrooms, timetable);
}

// Function to print timetable
void printTimetable(const unordered_map<string, vector<vector<tuple<string, string, string>>>>& timetable) {
    const int width = 30;

    for (const auto& [batchName, schedule] : timetable) {
        cout << "\nTimetable for Batch: " << batchName << "\n";
        cout << left << setw(width) << "Period\\Day";
        for (int day = 0; day < 5; ++day)
            cout << left << setw(width) << "Day " + to_string(day + 1);
        cout << "\n";

        for (int period = 0; period < 4; ++period) {
            cout << left << setw(width) << "Period " + to_string(period + 1);
            for (int day = 0; day < 5; ++day) {
                const auto& [prof, subj, room] = schedule[day][period];
                string display = (subj.empty() ? "Free" : subj + " by " + prof + " in " + room);
                cout << left << setw(width) << display;
            }
            cout << "\n";
        }
    }
}

// Function to export timetable to CSV
void exportToCSV(const unordered_map<string, vector<vector<tuple<string, string, string>>>>& timetable) {
    ofstream outFile("Timetable.csv");
    if (!outFile) {
        cout << "Error opening file for writing.\n";
        return;
    }

    for (const auto& [batchName, schedule] : timetable) {
        outFile << "Timetable for Batch: " << batchName << "\n";
        outFile << "Period\\Day,";
        for (int day = 0; day < 5; ++day)
            outFile << "Day " << day + 1 << ",";
        outFile << "\n";

        for (int period = 0; period < 4; ++period) {
            outFile << "Period " << period + 1 << ",";
            for (int day = 0; day < 5; ++day) {
                const auto& [prof, subj, room] = schedule[day][period];
                string display = (subj.empty() ? "Free" : subj + " by " + prof + " in " + room);
                outFile << display << ",";
            }
            outFile << "\n";
        }
        outFile << "\n";
    }

    cout << "Timetable exported to Timetable.csv\n";
    outFile.close();
}

int main() {
    vector<Professor> professors = {
        {"Dr.A",{"Calculus", "Linear Algebra"}, {"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.B",{"Thermodynamics", "Optics"}, {"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.C", {"Inorganic Chemistry", "Biochemistry"},{"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.D", {"Molecular Biology", "Genetics"}, {"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.E", {"Ancient Civilizations", "Medieval History"},{"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.F", {"Differential Equations", "Statistics"},{"0,0","0,1""1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.G", {"Quantum Mechanics", "Nuclear Physics"},{"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.H", {"Modern Economics", "Renaissance History"},{"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.I", {"Astrophysics", "Organic Chemistry"},{"0,0","0,1","0,2","0,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.J", {"Philosophy", "Evolutionary Biology"},{"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.K", {"Political Science", "Cognitive Psychology"},{"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.L", {"Computer Science", "Artificial Intelligence"},{"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.M", {"Machine Learning", "Game Theory"},{"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Dr.N", {"Cryptography"}, {"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3" "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}}
    };
    vector<Batch> batches = {
        {"Batch1", {
            {"Calculus", 2},
            {"Linear Algebra", 2},
            {"Thermodynamics", 1},
            {"Molecular Biology", 2},
            {"Statistics", 1},
            {"Inorganic Chemistry", 1},
            {"Biochemistry", 1}
        }},
        {"Batch2", {
            {"Optics", 2},
            {"Quantum Mechanics", 1},
            {"Differential Equations", 1},
            {"Ancient Civilizations", 1},
            {"Genetics", 1}
        }},
        {"Batch3", {
            {"Nuclear Physics", 2},
            {"Medieval History", 1},
            {"Modern Economics", 1},
            {"Renaissance History", 1}
        }}, 
        {"Batch4", {
            {"Astrophysics", 2},
            {"Organic Chemistry", 1},
            {"Philosophy", 1},
            {"Evolutionary Biology", 1},
            {"Political Science", 1},
            {"Cognitive Psychology", 1}
        }}, 
        {"Batch5", {
            {"Computer Science", 2},
            {"Artificial Intelligence", 1},
            {"Machine Learning", 1},
            {"Game Theory", 1},
            {"Cryptography", 1}
        }} 
    };


    vector<Classroom> classrooms = {
        {"Room1", {"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Room2", {"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Room3", {"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Room4", {"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}},
        {"Room5", {"0,0","0,1","0,2","0,3", "1,0","1,1","1,2","1,3", "2,0","2,1","2,2","2,3","3,0","3,1","3,2","3,3","4,0","4,1","4,2","4,3"}}
    };

    unordered_map<string, vector<vector<tuple<string, string, string>>>> timetable;
    for (const auto& batch : batches) {
        timetable[batch.name] = vector<vector<tuple<string, string, string>>>(
            5, vector<tuple<string, string, string>>(4, {"", "", ""}));
    }

    if (createTimetable(professors, classrooms, batches, timetable)) {
        cout << "Timetable created successfully!\n";
        printTimetable(timetable);
        exportToCSV(timetable);
    } else {
        cout << "Failed to create timetable.\n";
    }

    return 0;
}
