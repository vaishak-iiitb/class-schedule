public class TimeSlot {

    private int slotNumber;     // Identifier for the time slot (e.g., 1 or 2)
    private String timeRange;   // Time range for the slot (e.g., "9:30 am - 12:30 pm")

    public TimeSlot(int slotNumber, String timeRange) {  //Parameterized constructor to initialize a TimeSlot object with specific details.
        this.slotNumber = slotNumber;
        this.timeRange = timeRange;
    }


    //getters
    public int getSlotNumber() {
        return slotNumber;
    }

    public String getTimeRange() {
        return timeRange;
    }
}
