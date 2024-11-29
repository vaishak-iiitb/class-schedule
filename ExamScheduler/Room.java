public class Room {

    private String roomName;  // Name of the room
    private int capacity;     // Capacity of the room

    public Room(String roomName) {
        /*
        Constructor to initialize the Room object with a given name.
        Sets a default capacity of 30 for the room.
        */

        this.roomName = roomName;
        this.capacity = 30; // Default capacity, can be modified if needed
    }

    //getters
    public String getRoomName() {
        return roomName;
    }

    public int getCapacity() {
        return capacity;
    }
}
