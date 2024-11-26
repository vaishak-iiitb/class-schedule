public class TimeTableGenerator {
    static {
        System.loadLibrary("timetable_native");
    }

    private native void generateTimeTable();

    public static void main(String[] args) {
        try {
            new TimeTableGenerator().generateTimeTable();
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
        }
    }
}
