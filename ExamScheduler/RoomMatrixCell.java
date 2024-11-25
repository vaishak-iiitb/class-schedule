public class RoomMatrixCell {

    private int slot;
    private String batch;
    private String subject;

    // Default constructor
    public RoomMatrixCell() {}

    // Parameterized constructor
    public RoomMatrixCell(int slot, String batch, String subject) {
        this.slot = slot;
        this.batch = batch;
        this.subject = subject;
    }

    // Getters
    public int getSlot() {
        return slot;
    }

    public String getBatch() {
        return batch;
    }

    public String getSubject() {
        return subject;
    }

    // Setters
    public void setSlot(int slot) {
        this.slot = slot;
    }

    public void setBatch(String batch) {
        this.batch = batch;
    }

    public void setSubject(String subject) {
        this.subject = subject;
    }

    // toString method for easy printing
    @Override
    public String toString() {
        return (
            "RoomMatrixCell{" +
            "slot=" +
            slot +
            ", batch='" +
            batch +
            '\'' +
            ", subject='" +
            subject +
            '\'' +
            '}'
        );
    }

    // Clone method for creating copies
    @Override
    public RoomMatrixCell clone() {
        return new RoomMatrixCell(this.slot, this.batch, this.subject);
    }

    // Equals method for comparison
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        RoomMatrixCell that = (RoomMatrixCell) o;
        return (
            slot == that.slot &&
            batch.equals(that.batch) &&
            subject.equals(that.subject)
        );
    }
}
