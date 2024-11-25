public class Room {

    private String roomName;
    private int capacity;

    public Room(String roomName) {
        this.roomName = roomName;
        this.capacity = 30; // Default capacity, can be modified if needed
    }

    public String getRoomName() {
        return roomName;
    }

    public int getCapacity() {
        return capacity;
    }
}
