import java.io.*;
import java.util.*;
import java.util.regex.*;
/*
public class Course {
    private String courseName;
    private String professorName;
    private int credits;
    private int lectures;

    public Course(String courseName, String professorName, int credits) {
        this.courseName = courseName;
        this.professorName = professorName;
        this.credits = credits;
    }

    // Getters and setters
    public String getCourseName() {
        return courseName;
    }

    public String getProfessorName() {
        return professorName;
    }

    public int getCredits() {
        return credits;
    }

    public int getLectures() {
        return getCredits() / 2;
    }

    @Override
    public String toString() {
        return "Course{" +
               "courseName='" + courseName + '\'' +
               ", professorName='" + professorName + '\'' +
               ", credits=" + credits +
               '}';
    }
}
*/
class Batch {
    private String batchId;
    private int numCourses;
    private List<Course> courses;

    public Batch(String batchId, int numCourses, List<Course> courses) {
        this.batchId = batchId;
        this.numCourses = numCourses;
        this.courses = courses;
    }

    public String getBatchId() {
        return batchId;
    }

    public int getNumCourses() {
        return numCourses;
    }

    public List<Course> getCourses() {
        return courses;
    }
}

public class BatchFileParser {

    // Method to parse the input file and return an ArrayList of Batch objects
    public static ArrayList<Batch> parseBatchFile(String filename) {
        ArrayList<Batch> batches = new ArrayList<>();

        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = br.readLine()) != null) {
                // Read batch ID and number of courses
                String[] batchInfo = line.split(" ");
                String batchId = batchInfo[0];
                int numCourses = Integer.parseInt(batchInfo[1]);

                List<Course> courses = new ArrayList<>();

                // Read courses for the current batch
                for (int i = 0; i < numCourses; i++) {
                    line = br.readLine();
                    // Use regex to correctly extract the course name, professor name, and number of students
                    Pattern pattern = Pattern.compile("\"([^\"]+)\"\\s+\"([^\"]+)\"\\s+(\\d+)");
                    Matcher matcher = pattern.matcher(line);

                    if (matcher.find()) {
                        String courseName = matcher.group(1);  // Course name
                        String professorName = matcher.group(2);  // Professor name
                        int numStudents = Integer.parseInt(matcher.group(3));  // Number of students

                        // Create Course object and add to the courses list
                        courses.add(new Course(courseName, professorName, numStudents));
                    }
                }

                // Create Batch object and add to the batches list
                batches.add(new Batch(batchId, numCourses, courses));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return batches;
    }

    // Method to create a Vector of (Course, Credits) for each Batch
    public static Vector<Map.Entry<String, Integer>> createBatchCourseCreditsVector(Batch batch) {
        Vector<Map.Entry<String, Integer>> courseCreditsVector = new Vector<>();

        // Iterate over the courses in the batch
        for (Course course : batch.getCourses()) {
            // Create a Map.Entry of course name and (credits/2)
            courseCreditsVector.add(new AbstractMap.SimpleEntry<>(course.getCourseName(), course.getLectures()));
        }

        return courseCreditsVector;
    }

    public static void main(String[] args) {
        // Replace "batches.txt" with the path to your file
        ArrayList<Batch> batchList = parseBatchFile("courses.txt");

        // Iterate over batches and create a Vector of (Course, Credits) for each Batch
        for (Batch batch : batchList) {
            System.out.println("Batch ID: " + batch.getBatchId());
            Vector<Map.Entry<String, Integer>> courseCreditsVector = createBatchCourseCreditsVector(batch);

            // Print the Vector of courses and their credits for this batch
            for (Map.Entry<String, Integer> entry : courseCreditsVector) {
                System.out.println("Course: " + entry.getKey() + ", Lectures: " + entry.getValue());
            }

            System.out.println();
        }
    }
}
