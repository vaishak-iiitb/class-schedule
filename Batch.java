import java.util.*;


public class Batch {
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
