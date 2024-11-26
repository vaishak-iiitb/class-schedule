#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

class Professor {
private:
    string name;
    unordered_set<string> availability;
    vector<string> subjects;

public:
    Professor(const string& name, const vector<string>& subjects, const unordered_set<string>& availability)
        : name(name), subjects(subjects), availability(availability) {}

    string getName() const { return name; }
    const unordered_set<string>& getAvailability() const { return availability; }
    const vector<string>& getSubjects() const { return subjects; }

    void setName(const string& newName) { name = newName; }
    void setAvailability(const unordered_set<string>& newAvailability) { availability = newAvailability; }
    void setSubjects(const vector<string>& newSubjects) { subjects = newSubjects; }
};

class Classroom {
private:
    string id;
    unordered_set<string> availability;

public:
    Classroom(const string& id, const unordered_set<string>& availability)
        : id(id), availability(availability) {}

    string getId() const { return id; }
    const unordered_set<string>& getAvailability() const { return availability; }

    void setId(const string& newId) { id = newId; }
    void setAvailability(const unordered_set<string>& newAvailability) { availability = newAvailability; }
};

class Batch {
private:
    string name;
    unordered_map<string, int> required_subjects;

public:
    Batch(const string& name, const unordered_map<string, int>& required_subjects)
        : name(name), required_subjects(required_subjects) {}

    string getName() const { return name; }
    const unordered_map<string, int>& getRequiredSubjects() const { return required_subjects; }

    void setName(const string& newName) { name = newName; }
    void setRequiredSubjects(const unordered_map<string, int>& newSubjects) { required_subjects = newSubjects; }
};

class TimetableScheduler {
private:
    vector<Professor> professors;
    vector<Classroom> classrooms;
    vector<Batch> batches;
    unordered_map<string, vector<vector<tuple<string, string, string>>>> timetable;

    bool isSlotAvailable(int day, int period, const string& professor, const string& subject, 
                        const string& classroom, const string& batch) {
        string slotKey = to_string(day) + "," + to_string(period);
        
        auto prof = find_if(professors.begin(), professors.end(), 
            [&professor](const Professor& p) { return p.getName() == professor; });
        if (prof == professors.end() || prof->getAvailability().find(slotKey) == prof->getAvailability().end())
            return false;

        auto room = find_if(classrooms.begin(), classrooms.end(), 
            [&classroom](const Classroom& c) { return c.getId() == classroom; });
        if (room == classrooms.end() || room->getAvailability().find(slotKey) == room->getAvailability().end())
            return false;

        for (const auto& [otherBatchName, otherSchedule] : timetable) {
            if (otherBatchName == batch) continue;
            const auto& otherSlot = otherSchedule[day][period];
            if (!get<0>(otherSlot).empty() && (get<0>(otherSlot) == professor || get<2>(otherSlot) == classroom))
                return false;
        }
        return true;
    }

    bool scheduleBatch(int batchIndex) {
        if (batchIndex >= batches.size())
            return true;

        Batch& currentBatch = batches[batchIndex];
        auto& currentSchedule = timetable[currentBatch.getName()];

        for (const auto& [subject, periodsNeeded] : currentBatch.getRequiredSubjects()) {
            auto prof = find_if(professors.begin(), professors.end(), [&subject](const Professor& p) {
                const auto& subjects = p.getSubjects();
                return find(subjects.begin(), subjects.end(), subject) != subjects.end();
            });

            if (prof == professors.end())
                return false;

            int hoursScheduled = 0;
            for (int day = 0; day < 5 && hoursScheduled < periodsNeeded; ++day) {
                for (int period = 0; period < 4 && hoursScheduled < periodsNeeded; ++period) {
                    for (const auto& classroom : classrooms) {
                        if (get<0>(currentSchedule[day][period]).empty() &&
                            isSlotAvailable(day, period, prof->getName(), subject, 
                                         classroom.getId(), currentBatch.getName())) {
                            currentSchedule[day][period] = {prof->getName(), subject, classroom.getId()};
                            hoursScheduled++;
                            break;
                        }
                    }
                }
            }

            if (hoursScheduled < periodsNeeded)
                return false;
        }

        return scheduleBatch(batchIndex + 1);
    }

public:
    TimetableScheduler(const vector<Professor>& profs, const vector<Classroom>& rooms, 
                      const vector<Batch>& batchList)
        : professors(profs), classrooms(rooms), batches(batchList) {
        for (const auto& batch : batches) {
            timetable[batch.getName()] = vector<vector<tuple<string, string, string>>>(
                5, vector<tuple<string, string, string>>(4, {"", "", ""}));
        }
    }

    const vector<Professor>& getProfessors() const { return professors; }
    const vector<Classroom>& getClassrooms() const { return classrooms; }
    const vector<Batch>& getBatches() const { return batches; }
    const auto& getTimetable() const { return timetable; }

    bool createTimetable() {
        return scheduleBatch(0);
    }
        void printTimetable() {
        int width = 25;

        vector<string> days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
        vector<string> slots = {"9:30 - 11:00", "11:15 - 12:45", "1:30 - 3:00", "3:15 - 4:45"};

        for (const auto& [batchName, schedule] : timetable) {
            cout << "\nTimetable for Batch: " << batchName << endl;
            cout << left << setw(width) << "Period\\Day";
            for (const auto& day : days)
                cout << left << setw(20) << day;
            cout << endl;

            for (int period = 0; period < slots.size(); ++period) {
                cout << left << setw(width) << slots[period];
                for (int day = 0; day < days.size(); ++day) {
                    const auto& [prof, subj, room] = schedule[day][period];
                    string display = (subj.empty() ? "Free" : subj + " by " + prof + " in " + room);
                    cout << left << setw(width) << display;
                }
                cout << endl;
            }
        }
    }



    void exportToCSV() {
        ofstream outFile("Timetable(withRandom).csv");
        if (!outFile) {
            cout << "Error opening file for writing." << endl;
            return;
        }

        vector<string> days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
        vector<string> slots = {"9:30 - 11:00", "11:15 - 12:45", "1:30 - 3:00", "3:15 - 4:45"};

        for (const auto& [batchName, schedule] : timetable) {
            outFile << "Timetable for Batch: " << batchName << endl;
            outFile << "Time\\Day,";
            for (const auto& day : days)
                outFile << day << ",";
            outFile << endl;

            for (int period = 0; period < slots.size(); ++period) {
                outFile << slots[period] << ",";
                for (int day = 0; day < days.size(); ++day) {
                    const auto& [prof, subj, room] = schedule[day][period];
                    string display = (subj.empty() ? "Free" : subj + " by " + prof + " in " + room);
                    outFile << display << ",";
                }
                outFile << endl;
            }
            outFile << endl;
        }

        cout << "Timetable exported to Timetable(withRandom).csv" << endl;
        outFile.close();
    }
};



vector<Professor> readProfessorsFromFile(const string& filename) {
    vector<Professor> professors;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string name, subjectsStr, availabilityStr;

        // Read name
        getline(ss, name, '\"');
        getline(ss, name, '\"');

        // Read subjects
        getline(ss, subjectsStr, '\"');
        getline(ss, subjectsStr, '\"');

        // Read availability
        getline(ss, availabilityStr, '\"');
        getline(ss, availabilityStr, '\"');

        vector<string> subjects;
        stringstream ssSubjects(subjectsStr);
        string subject;
        while (getline(ssSubjects, subject, ',')) {
            subjects.push_back(subject);
        }

        unordered_set<string> availability;
        stringstream ssAvailability(availabilityStr);
        string slot;
        while (getline(ssAvailability, slot, ')')) {
            if (!slot.empty() && slot[0] == '(') {
                slot = slot.substr(1); // Remove the leading '('
                availability.insert(slot);
            }
        }

        professors.emplace_back(name, subjects, availability);
    }

    return professors;
}
vector<Batch> readBatchesFromFile(const string& filename) {
    vector<Batch> batches;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string batchName;
        int numCourses;
        ss >> batchName >> numCourses;

        unordered_map<string, int> required_subjects;
        for (int i = 0; i < numCourses; ++i) {
            getline(file, line);
            stringstream courseStream(line);
            string courseName, professorName;
            int hours;
            courseStream >> quoted(courseName) >> quoted(professorName) >> hours;
            required_subjects[courseName] = hours;
        }

        batches.push_back(Batch(batchName, required_subjects));
    }

    return batches;
}
vector<Classroom> readClassroomsFromFile(const string& filename) {
    vector<Classroom> classrooms;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string name, availabilityStr;

        // Read name
        getline(ss, name, '\"');
        getline(ss, name, '\"');

        // Read availability
        getline(ss, availabilityStr, '\"');
        getline(ss, availabilityStr, '\"');

        unordered_set<string> availability;
        stringstream ssAvailability(availabilityStr);
        string slot;
        while (getline(ssAvailability, slot, ')')) {
            if (!slot.empty() && slot[0] == '(') {
                slot = slot.substr(1); // Remove the leading '('
                availability.insert(slot);
            }
        }

        classrooms.emplace_back(name, availability);
    }

    return classrooms;
}

void makeTimetable(){
    vector<Batch> batches = readBatchesFromFile("courses.txt");
    vector<Classroom> classrooms = readClassroomsFromFile("rooms.txt");
    vector<Professor> professors = readProfessorsFromFile("Professors.txt");



    TimetableScheduler scheduler(professors, classrooms, batches);
    if (scheduler.createTimetable()) {
        cout << "Timetable created successfully!\n";
        scheduler.printTimetable();
        scheduler.exportToCSV();
    } else {
        cout << "Failed to create timetable.\n";
    }
}
