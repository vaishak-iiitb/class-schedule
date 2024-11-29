#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <random>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

class Professor {   // Class representing a professor   
private:
    string name;                           // Professor name
    unordered_set<string> availability;    // Set of available time slots
    vector<string> subjects;               // List of subjects the professor can teach

public:

    // Constructor to initialize professor's details
    Professor(const string& name, const vector<string>& subjects, const unordered_set<string>& availability)
        : name(name), subjects(subjects), availability(availability) {}


    // Getters
    string getName() const { return name; }
    const unordered_set<string>& getAvailability() const { return availability; }
    const vector<string>& getSubjects() const { return subjects; }
    // Setters
    void setName(const string& newName) { name = newName; }
    void setAvailability(const unordered_set<string>& newAvailability) { availability = newAvailability; }
    void setSubjects(const vector<string>& newSubjects) { subjects = newSubjects; }
};

class Classroom {   // Class representing a classroom
private:
    string id;      // Classroom name
    unordered_set<string> availability;  //Classroom availability

public:

    // Constructor to initialize classroom's details
    Classroom(const string& id, const unordered_set<string>& availability)
        : id(id), availability(availability) {}

    // Getters
    string getId() const { return id; }
    const unordered_set<string>& getAvailability() const { return availability; }

    // Setters
    void setId(const string& newId) { id = newId; }
    void setAvailability(const unordered_set<string>& newAvailability) { availability = newAvailability; }
};


class Batch {       // Class representing a batch of students
private:
    string name;    // Batch name
    unordered_map<string, int> required_subjects;  // Map of subject names to required hours

public:

    // Constructor to initialize batch details
    Batch(const string& name, const unordered_map<string, int>& required_subjects)
        : name(name), required_subjects(required_subjects) {}

    // Getters
    string getName() const { return name; }
    const unordered_map<string, int>& getRequiredSubjects() const { return required_subjects; }
    // Setters
    void setName(const string& newName) { name = newName; }
    void setRequiredSubjects(const unordered_map<string, int>& newSubjects) { required_subjects = newSubjects; }
};

class TimetableScheduler {  // Class to schedule the timetable
private:
    vector<Professor> professors;   // List of professors
    vector<Classroom> classrooms;   // List of classrooms
    vector<Batch> batches;          // List of batches
    unordered_map<string, vector<vector<tuple<string, string, string>>>> timetable;  // Timetable data structure
    random_device rd;               // Random device for scheduling
    mt19937 gen{rd()};              // Random number generator

    bool isSlotAvailable(int day, int period, const string& professor, const string& subject, 
                        const string& classroom, const string& batch) {
        // Helper function to check if a time slot is available for given parameters
        string slotKey = to_string(day) + "," + to_string(period);
        
        // Check professor availability
        auto prof = find_if(professors.begin(), professors.end(), 
            [&professor](const Professor& p) { return p.getName() == professor; });
        if (prof == professors.end() || prof->getAvailability().find(slotKey) == prof->getAvailability().end())
            return false;

        // Check classroom availability
        auto room = find_if(classrooms.begin(), classrooms.end(), 
            [&classroom](const Classroom& c) { return c.getId() == classroom; });
        if (room == classrooms.end() || room->getAvailability().find(slotKey) == room->getAvailability().end())
            return false;

        // Ensure no scheduling conflicts
        for (const auto& [otherBatchName, otherSchedule] : timetable) {
            if (otherBatchName == batch) continue;
            const auto& otherSlot = otherSchedule[day][period];
            if (!get<0>(otherSlot).empty() && (get<0>(otherSlot) == professor || get<2>(otherSlot) == classroom))
                return false;
        }
        return true;
    }

    bool scheduleBatch(int batchIndex) { // Recursive function to schedule subjects for a batch
        if (batchIndex >= batches.size())
            return true;

        Batch& currentBatch = batches[batchIndex];                      // Current batch to schedule
        auto& currentSchedule = timetable[currentBatch.getName()];      // Current batch's timetable

        // Iterate over each required subject for the batch
        for (const auto& [subject, periodsNeeded] : currentBatch.getRequiredSubjects()) {
            // Find a professor capable of teaching the subject
            auto prof = find_if(professors.begin(), professors.end(), [&subject](const Professor& p) {
                const auto& subjects = p.getSubjects();
                return find(subjects.begin(), subjects.end(), subject) != subjects.end();
            });

            if (prof == professors.end())       // If no professor is found, fail
                return false;

            int hoursScheduled = 0;
            while (hoursScheduled < periodsNeeded) {
                vector<tuple<int, int, string>> availableSlots;     // Store available slots
                

                // Check all possible time slots and classrooms
                for (int day = 0; day < 5; ++day) {
                    for (int period = 0; period < 4; ++period) {
                        for (const auto& classroom : classrooms) {
                            if (get<0>(currentSchedule[day][period]).empty() &&
                                isSlotAvailable(day, period, prof->getName(), subject, 
                                             classroom.getId(), currentBatch.getName())) {
                                availableSlots.emplace_back(day, period, classroom.getId());
                            }
                        }
                    }
                }

                if (availableSlots.empty())     // If no slots are available, fail
                    return false;

                // Randomly select a slot from available options
                uniform_int_distribution<> dist(0, availableSlots.size() - 1);
                auto [day, period, classroom] = availableSlots[dist(gen)];
                currentSchedule[day][period] = {prof->getName(), subject, classroom};       // Assign slot
                hoursScheduled++;
            }
        }

        return scheduleBatch(batchIndex + 1);       // Schedule the next batch
    }

public:
    // Constructor to initialize the scheduler
    TimetableScheduler(const vector<Professor>& profs, const vector<Classroom>& rooms, const vector<Batch>& batchList): professors(profs), classrooms(rooms), batches(batchList) {
        for (const auto& batch : batches) {
            timetable[batch.getName()] = vector<vector<tuple<string, string, string>>>(
                5, vector<tuple<string, string, string>>(4, {"", "", ""}));     // Initialize empty timetable
        }
    }
    // Getters
    const vector<Professor>& getProfessors() const { return professors; }
    const vector<Classroom>& getClassrooms() const { return classrooms; }
    const vector<Batch>& getBatches() const { return batches; }
    const auto& getTimetable() const { return timetable; }

    bool createTimetable() {  // Function to create the timetable
        return scheduleBatch(0);
    }

    void printTimetable() {   // Function to print the timetable to the console
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

    void exportToCSV() {     // Function to export the timetable to a CSV file
        ofstream outFile("timetable.csv");
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

        cout << "Timetable exported to timetable.csv" << endl;
        outFile.close();
    }
};



vector<Professor> readProfessorsFromFile(const string& filename) {  // Function to read professor data from a file
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
vector<Batch> readBatchesFromFile(const string& filename) {    // Function to read batch data from a file
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
            required_subjects[courseName] = hours/2;
        }

        batches.push_back(Batch(batchName, required_subjects));
    }

    return batches;
}
vector<Classroom> readClassroomsFromFile(const string& filename) {   // Function to read classroom data from a file
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

void makeTimetable(){   // Main function to initialize and generate the timetable
    vector<Batch> batches = readBatchesFromFile("courses_1.txt");
    vector<Classroom> classrooms = readClassroomsFromFile("rooms_1.txt");
    vector<Professor> professors = readProfessorsFromFile("professors_1.txt");

    

    TimetableScheduler scheduler(professors, classrooms, batches);
    if (scheduler.createTimetable()) {
        cout << "Timetable created successfully!\n";
        scheduler.printTimetable();
        scheduler.exportToCSV();
    } else {
        cout << "Failed to create timetable.\n";
    }
}
