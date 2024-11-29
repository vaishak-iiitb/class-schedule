import java.io.*;
import java.util.*;


// Main class to manage the scheduling of exams
public class ExamScheduler {


    // Lists to store courses, rooms, exam dates, and the final schedule
    private List<Courses> coursesList;
    private List<Room> rooms;
    private List<ExamDate> examDates;
    private List<ExamDetails> schedule;


    // Constructor to initialize the lists
    public ExamScheduler() {
        coursesList = new ArrayList<>();
        rooms = new ArrayList<>();
        examDates = new ArrayList<>();
        schedule = new ArrayList<>();
    }


    // Method to load course details from the 'courses.txt' file    
    private void loadCourses() {
        try (
            BufferedReader br = new BufferedReader(
                new FileReader("courses.txt")
            )
        ) {
            String line;
            String currentBatch = "";   // Tracks the current batch for courses

            // Read the file line by line
            while ((line = br.readLine()) != null) {
                line = line.trim();     // Remove leading/trailing spaces

                if (line.equals("x")) continue; // Skip lines with 'x'

                // If the line matches the batch pattern (e.g., IMT2023 20)
                if (line.matches("IMT\\d{4}\\s+\\d+")) {
                    String[] batchInfo = line.split("\\s+");
                    currentBatch = batchInfo[0];    // Extract the batch name
                    continue;
                }

                // If the line starts with a quote, it represents a course detail
                if (!line.isEmpty() && line.startsWith("\"")) {
                    String[] parts = line.split("\"");

                    if (parts.length >= 5) {    // Ensure there are sufficient parts

                        String courseName = parts[1].trim();
                        String teacherName = parts[3].trim();
                        int credits = Integer.parseInt(parts[4].trim());

                        // Create a new Course object and add it to the list
                        Courses course = new Courses(
                            currentBatch,
                            courseName,
                            teacherName,
                            credits
                        );
                        coursesList.add(course);
                    }
                }
            }
        } catch (IOException e) {
            // Handle any file I/O errors
            System.err.println("Error reading courses file: " + e.getMessage());
            e.printStackTrace();
        }
    }


    // Method to load room details from the 'rooms.txt' file
    private void loadRooms() {
        try (
            BufferedReader br = new BufferedReader(new FileReader("rooms.txt"))
        ) {
            String line;
            while ((line = br.readLine()) != null) {
                String roomName = line.trim();
                if (!roomName.isEmpty()) {
                    rooms.add(new Room(roomName));  // Add the room to the list
                }
            }
        } catch (IOException e) {
            // Handle any file I/O errors
            System.err.println("Error reading rooms file: " + e.getMessage());
            e.printStackTrace();
        }
    }

    // Method to load exam dates from the 'dates.txt' file
    private void loadDates() {
        try (
            BufferedReader br = new BufferedReader(new FileReader("dates.txt"))
        ) {
            String line;
            while ((line = br.readLine()) != null) {
                String date = line.trim();
                if (!date.isEmpty()) {
                    examDates.add(new ExamDate(date));  // Add the date to the list
                }
            }
        } catch (IOException e) {
            // Handle any file I/O errors
            System.err.println("Error reading dates file: " + e.getMessage());
            e.printStackTrace();
        }
    }

    // Method to load data from all input files
    public void loadData() {
        loadCourses();
        loadRooms();
        loadDates();
    }

    // Method to create the exam schedule
    public void createSchedule() {
        // Convert Lists to Arrays for JNI compatibility
        Courses[] coursesArray = coursesList.toArray(new Courses[0]);
        String[] roomsArray = rooms
            .stream()
            .map(Room::getRoomName)
            .toArray(String[]::new);
        String[] datesArray = examDates
            .stream()
            .map(ExamDate::getDate)
            .toArray(String[]::new);
        // Call the native method to generate the schedule
        generateSchedule(coursesArray, roomsArray, datesArray);
    }


    // Native method to call the C++ function for schedule generatio
    private native void generateSchedule(
        Courses[] courses,
        String[] rooms,
        String[] dates
    );


    // Method to print the loaded data for debugging purposes
    public void printLoadedData() {
        System.out.println("\nLoaded Courses:");
        // Iterate and print details of each course
        for (Courses course : coursesList) {
            System.out.println(
                String.format(
                    "Batch: %s, Name: %s, Prof: %s, Credits: %d",
                    course.getBatch(),
                    course.getName(),
                    course.getProf(),
                    course.getCredits()
                )
            );
        }

        System.out.println("\nLoaded Rooms:");
        // Print all room names
        for (Room room : rooms) {
            System.out.println(room.getRoomName());
        }

        System.out.println("\nLoaded Dates:");
        // Print all exam dates
        for (ExamDate date : examDates) {
            System.out.println(date.getDate());
        }
    }

    // Static block to load the native library
    static {
        System.loadLibrary("examscheduler");
    }

    // Main method to run the application
    public static void main(String[] args) {
        ExamScheduler scheduler = new ExamScheduler();  // Create an instance
        scheduler.loadData();                           // Load data from input files
        //scheduler.printLoadedData();                  // Uncomment to print loaded data for debugging
        scheduler.createSchedule();                     // Generate the exam schedule
    }
}
