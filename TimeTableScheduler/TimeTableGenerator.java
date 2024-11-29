public class TimeTableGenerator {
    static {

    /*Static block to load the native C++ library.
     The library file "timetable_native" contains the native implementation of the timetable generation logic.
     Ensure that the library is accessible and properly linked to avoid runtime errors.
    */
        
        System.loadLibrary("timetable_native");
    }

    /*Declaration of a native method `generateTimeTable`, which is implemented in the native library.
    This method bridges the Java environment with the C++ timetable generation functionality.
    */  
    private native void generateTimeTable();

    public static void main(String[] args) {
        try {
            // Create an instance of `TimeTableGenerator` and invoke the native method.
            new TimeTableGenerator().generateTimeTable();
            
        } catch (Exception e) {
            
            /*Catch and handle any exceptions thrown during the timetable generation process.
            Errors from the native code are propagated here and logged for debugging.
            */

            System.err.println("Error: " + e.getMessage());
        }
    }
}
