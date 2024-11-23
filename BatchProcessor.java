import java.util.*;

public class BatchProcessor {
    // Native method declaration
    public native void passBatchDataToCpp(ArrayList<Map.Entry<String, Integer>> courseCredits);

    // Load the native library
    static {
        System.loadLibrary("BatchProcessorJNI");
    }

    public static void main(String[] args) {
        BatchProcessor processor = new BatchProcessor();
        
        // Sample data (course name and credits)
        ArrayList<Map.Entry<String, Integer>> courseCredits = new ArrayList<>();
        courseCredits.add(new AbstractMap.SimpleEntry<>("Calculus", 2));
        courseCredits.add(new AbstractMap.SimpleEntry<>("Linear Algebra", 1));
        courseCredits.add(new AbstractMap.SimpleEntry<>("Thermodynamics", 1));

        // Pass the data to C++
        processor.passBatchDataToCpp(courseCredits);
    }
}
