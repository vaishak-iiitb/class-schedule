public class Courses {

    private String batch;  //The batch to which this course belongs to
    private String name;   //The name of the course
    private String prof;   //The professor teaching the course
    private int credits;   //The number of credits assigned to the course

    // Constructor
    public Courses(String batch, String name, String prof, int credits) {
        this.batch = batch;    
        this.name = name;
        this.prof = prof;
        this.credits = credits;
    }

    // Default constructor
    public Courses() {}

    // Getters
    public String getBatch() {
        return batch;
    }

    public String getName() {
        return name;
    }

    public String getProf() {
        return prof;
    }

    public int getCredits() {
        return credits;
    }

    // Setters
    public void setBatch(String batch) {
        this.batch = batch;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setProf(String prof) {
        this.prof = prof;
    }

    public void setCredits(int credits) {
        this.credits = credits;
    }
}
