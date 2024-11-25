import java.util.ArrayList;
import java.util.List;

public class ExamDate {

    private String date;
    private List<TimeSlot> availableSlots;

    public ExamDate(String date) {
        this.date = date;
        this.availableSlots = new ArrayList<>();
        // Add default time slots
        availableSlots.add(new TimeSlot(1, "9:30 am - 12:30 pm"));
        availableSlots.add(new TimeSlot(2, "2:00 pm - 5:00 pm"));
    }

    public String getDate() {
        return date;
    }

    public List<TimeSlot> getAvailableSlots() {
        return availableSlots;
    }
}
