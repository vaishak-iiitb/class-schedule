public class ExamDetails {
    
    /*
    The ExamDetails class represents the detailed information of an exam for a specific course.
    It includes details such as batch, course name, exam slot, date, room, and utility methods.
    */

    private String batch;   // The batch associated with the exam
    private String name;    // The name of the course for which the exam is conducted
    private int slot;       // The exam slot number at which the exam is conducted
    private String date;    // The date of the exam
    private String room;    // The room where the exam is conducted

    // Default constructor
    public ExamDetails() {}

    // Parameterized constructor
    public ExamDetails(
        String batch,
        String name,
        int slot,
        String date,
        String room
    ) {
        this.batch = batch;
        this.name = name;
        this.slot = slot;
        this.date = date;
        this.room = room;
    }

    // Getters
    public String getBatch() {
        return batch;
    }

    public String getName() {
        return name;
    }

    public int getSlot() {
        return slot;
    }

    public String getDate() {
        return date;
    }

    public String getRoom() {
        return room;
    }

    // Setters
    public void setBatch(String batch) {
        this.batch = batch;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setSlot(int slot) {
        this.slot = slot;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public void setRoom(String room) {
        this.room = room;
    }

    // toString method for easy printing
    @Override
    public String toString() {
        return (
            "ExamDetails{" +
            "batch='" +
            batch +
            '\'' +
            ", name='" +
            name +
            '\'' +
            ", slot=" +
            slot +
            ", date='" +
            date +
            '\'' +
            ", room='" +
            room +
            '\'' +
            '}'
        );
    }

    // Helper method to get slot time (similar to C++ version)
    public String getSlotTime() {
        return (slot == 1) ? "9:30 am - 12:30 pm" : "2:00 pm - 5:00 pm";
    }
}
