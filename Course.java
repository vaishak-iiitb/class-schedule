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
