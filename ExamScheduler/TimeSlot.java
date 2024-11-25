public class TimeSlot {

    private int slotNumber;
    private String timeRange;

    public TimeSlot(int slotNumber, String timeRange) {
        this.slotNumber = slotNumber;
        this.timeRange = timeRange;
    }

    public int getSlotNumber() {
        return slotNumber;
    }

    public String getTimeRange() {
        return timeRange;
    }
}
