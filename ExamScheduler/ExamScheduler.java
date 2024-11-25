import java.io.*;
import java.util.*;

public class ExamScheduler {

    private List<Courses> coursesList;
    private List<Room> rooms;
    private List<ExamDate> examDates;
    private List<ExamDetails> schedule;

    public ExamScheduler() {
        coursesList = new ArrayList<>();
        rooms = new ArrayList<>();
        examDates = new ArrayList<>();
        schedule = new ArrayList<>();
    }

    private void loadCourses() {
        try (
            BufferedReader br = new BufferedReader(
                new FileReader("courses.txt")
            )
        ) {
            String line;
            String currentBatch = "";

            while ((line = br.readLine()) != null) {
                line = line.trim();
                if (line.equals("x")) continue;

                if (line.matches("IMT\\d{4}\\s+\\d+")) {
                    String[] batchInfo = line.split("\\s+");
                    currentBatch = batchInfo[0];
                    continue;
                }

                if (!line.isEmpty() && line.startsWith("\"")) {
                    String[] parts = line.split("\"");
                    if (parts.length >= 5) {
                        String courseName = parts[1].trim();
                        String teacherName = parts[3].trim();
                        int credits = Integer.parseInt(parts[4].trim());

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
            System.err.println("Error reading courses file: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void loadRooms() {
        try (
            BufferedReader br = new BufferedReader(new FileReader("rooms.txt"))
        ) {
            String line;
            while ((line = br.readLine()) != null) {
                String roomName = line.trim();
                if (!roomName.isEmpty()) {
                    rooms.add(new Room(roomName));
                }
            }
        } catch (IOException e) {
            System.err.println("Error reading rooms file: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void loadDates() {
        try (
            BufferedReader br = new BufferedReader(new FileReader("dates.txt"))
        ) {
            String line;
            while ((line = br.readLine()) != null) {
                String date = line.trim();
                if (!date.isEmpty()) {
                    examDates.add(new ExamDate(date));
                }
            }
        } catch (IOException e) {
            System.err.println("Error reading dates file: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public void loadData() {
        loadCourses();
        loadRooms();
        loadDates();
    }

    public void createSchedule() {
        // Convert to arrays for JNI
        Courses[] coursesArray = coursesList.toArray(new Courses[0]);
        String[] roomsArray = rooms
            .stream()
            .map(Room::getRoomName)
            .toArray(String[]::new);
        String[] datesArray = examDates
            .stream()
            .map(ExamDate::getDate)
            .toArray(String[]::new);
        // Call native method
        generateSchedule(coursesArray, roomsArray, datesArray);
    }

    private native void generateSchedule(
        Courses[] courses,
        String[] rooms,
        String[] dates
    );

    public void printLoadedData() {
        System.out.println("\nLoaded Courses:");
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
        for (Room room : rooms) {
            System.out.println(room.getRoomName());
        }

        System.out.println("\nLoaded Dates:");
        for (ExamDate date : examDates) {
            System.out.println(date.getDate());
        }
    }

    static {
        System.loadLibrary("examscheduler");
    }

    public static void main(String[] args) {
        ExamScheduler scheduler = new ExamScheduler();
        scheduler.loadData();
        //scheduler.printLoadedData();
        scheduler.createSchedule();
    }
}
