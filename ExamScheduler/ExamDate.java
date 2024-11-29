import java.util.ArrayList;
import java.util.List;

public class ExamDate {

    private String date;                        //String of date in DD/MM/YYYY format
    private List<TimeSlot> availableSlots;      //List of available slots on the given date

    public ExamDate(String date) {   
        /* Constructor which initializes the object with the given date, 
        and populates the list of available slots with default slots */
        this.date = date;
        this.availableSlots = new ArrayList<>();
        // Adds default time slots
        availableSlots.add(new TimeSlot(1, "9:30 am - 12:30 pm"));
        availableSlots.add(new TimeSlot(2, "2:00 pm - 5:00 pm"));
    }

    //getters
    public String getDate() {
        return date;
    }

    public List<TimeSlot> getAvailableSlots() {
        return availableSlots;
    }
}
