package chatroom;

import java.awt.*;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;


public class Server {
    ServerSocket serverSocket;

    class ClientInfo {
        Socket socket;
        PrintWriter clientInput;
        String username;
        Color color;

        ClientInfo(Socket socket) {
            this.socket = socket;
            try {
                clientInput = new PrintWriter(socket.getOutputStream(), true);
            } catch (IOException e) {
                System.out.println("Could not get client clientInput stream");
                e.printStackTrace();
            }

            // Send the clients list before authenticating so the client can tell
            // the user whether the username is already taken
            sendParticipantsInfo(clientInput);
            username = "";
        }

        void authenticate(String username) {
            this.username = username;
            infoOfUser.put(username, this);
            this.color = nextColor();
        }
    }

    private ArrayList<ClientInfo> clients;
    private HashMap<String, ClientInfo> infoOfUser;

    private final Color[] COLORS = {
            Color.BLUE,
            Color.RED,
            Color.GREEN,
            Color.CYAN,
            Color.MAGENTA,
            Color.YELLOW,
    };
    private HashSet<Color> unusedColors;

    static Server instance;

    public static void main(String[] args) {
        instance = new Server();
    }

    //region Starting Process
    public Server() {
        initialization();
        acceptConnections();
    }

    void initialization() {
        clients = new ArrayList<>();
        infoOfUser = new HashMap<>();

        unusedColors = new HashSet<>();
        // Every color is unused at the beginning
        Collections.addAll(unusedColors, COLORS);
    }

    void acceptConnections() {
        try {
            serverSocket = new ServerSocket(Constants.PORT);
        } catch (IOException e) {
            System.out.println("Could not open a socket on port " + Constants.PORT);
            e.printStackTrace();
        }

        AcceptingClientsThread acceptingClientsThread = new AcceptingClientsThread();
        acceptingClientsThread.start();

        System.out.println("> Ready for connections!");
    }
    //endregion


    //region Content Sending
    void sendPublicMessage(String sender, String message) {
        String toSend = Constants.PUBLIC_COMMAND + Constants.DELIMITER + sender + Constants.DELIMITER + message;
        System.out.println("Broadcasting '" + toSend + "'");
        clients.forEach(client -> client.clientInput.println(toSend));
    }

    void sendPrivateMessage(String sender, String recipient, String message) {
        String toSend = Constants.PRIVATE_COMMAND + Constants.DELIMITER + sender + Constants.DELIMITER + message;
        System.out.println("Whispering to " + recipient + " " + toSend);
        infoOfUser.get(recipient).clientInput.println(toSend);
    }

    private void sendAnnouncement(String sender, String message) {
        String toSend = Constants.ANNOUNCEMENT_COMMAND + Constants.DELIMITER + sender + Constants.DELIMITER + message;
        System.out.println("Announcing '" + toSend + "'");
        clients.forEach(client -> client.clientInput.println(toSend));
    }

    private void sendParticipantsInfo() {
        System.out.println("Sending the participants info to everyone");
        clients.forEach(client -> sendParticipantsInfo(client.clientInput));
    }

    private void sendParticipantsInfo(PrintWriter singleClient) {
        String toSend = Constants.PARTICIPANTS_COMMAND + Constants.DELIMITER + participantsAsString();

//        System.out.println ("> Telling the newly connected: " + toSend);
        singleClient.println(toSend);
    }
    //endregion


    //region Participants Managing
    void addClient(Socket socket) {
        ClientInfo newClient = new ClientInfo(socket);

        System.out.println("> New client accepted");
        clients.add(newClient);

        ClientOutputProcessingThread clientOutputProcessingThread = new ClientOutputProcessingThread(newClient);
        clientOutputProcessingThread.start();
    }

    void authenticate(ClientInfo info, String username) {
        if (!info.username.isEmpty())
            System.out.println("Received login but it the client is already authenticated");

        else {
            info.authenticate(username);

            System.out.println("> " + username + " authenticated");
            sendParticipantsInfo();
            sendAnnouncement(username, "connected");
        }
    }

    void renameUser(String oldUser, String newUser, ClientInfo info) {
        System.out.println("Renaming " + oldUser + " to " + newUser);

        infoOfUser.remove(oldUser);
        infoOfUser.put(newUser, info);
        info.username = newUser;

        sendAnnouncement(oldUser, "renamed to " + newUser);
        sendParticipantsInfo();
    }

    void removeClient(ClientInfo client) {
        clients.remove(client);

        if (client.username.isEmpty())
            System.out.println("Unauthenticated client disconnected");

        else {
            sendAnnouncement(client.username, "disconnected");
            // The color is now available for use again
            unusedColors.add(client.color);
            // But the username is not
            infoOfUser.remove(client.username);

            System.out.println("> " + client.username + " disconnected");
            sendParticipantsInfo();
        }
    }

    private String participantsAsString() {
        String res = "";
        for (ClientInfo participant : clients)
            if (!participant.username.isEmpty())
                res += Utils.ColorToString(participant.color) + " " +
                        participant.username + Constants.DELIMITER;
        return res;
    }

    private Color nextColor() {
        // If there are still available colors
        if (!unusedColors.isEmpty()) {
            Color color = (Color) unusedColors.toArray()[0];
            // Now the color is used
            unusedColors.remove(color);
            return color;
        }

        // Generate a new random one if the predefined ones have ran out
        else {
            Random rand = new Random();
            return new Color(rand.nextInt(256),
                    rand.nextInt(256),
                    rand.nextInt(256));
        }
    }

    //endregion
}

class AcceptingClientsThread extends Thread {
    private ServerSocket serverSocket;

    @Override
    public void run() {
        waitWhileNull();
        serverSocket = Server.instance.serverSocket;

        // The running loop
        while (true)
            try {
                // Wait for a new connection
                Socket newClientSocket = serverSocket.accept();
                // Add it as soon as it connects
                Server.instance.addClient(newClientSocket);
            } catch (IOException e) {
                System.out.println("Client couldn't connect");
                e.printStackTrace();
            }
    }

    private void waitWhileNull() {
        try {
            //noinspection ResultOfMethodCallIgnored
            Server.instance.serverSocket.toString();
        } catch (NullPointerException e) {
            try {
                sleep(5);
            } catch (InterruptedException e1) {
                e1.printStackTrace();
            }
            waitWhileNull();
        }
    }
}

class ClientOutputProcessingThread extends Utils.ProcessWhenInput {
    private Server.ClientInfo clientInfo;

    ClientOutputProcessingThread(Server.ClientInfo client) {
        this.clientInfo = client;
        try {
            input = new Scanner(clientInfo.socket.getInputStream());
            handleResponse = this::handleResponse;
        } catch (IOException e) {
            System.out.println("Could not get input from client");
            e.printStackTrace();
        }
    }

    private void handleResponse(String response) {
        System.out.println("Received '" + response.replace(Constants.DELIMITER, "_") + "' from '" + clientInfo.username + "'");

        String[] split = response.split(Constants.DELIMITER);
        String command = split[0];

        switch (command) {
            case Constants.DISCONNECT_COMMAND:
                closeConnection();
                break;

            case Constants.LOGIN_COMMAND:
                Server.instance.authenticate(clientInfo, split[1]);
                break;

            case Constants.PUBLIC_COMMAND:
                Server.instance.sendPublicMessage(clientInfo.username, split[1]);
                break;

            case Constants.PRIVATE_COMMAND:
                Server.instance.sendPrivateMessage(clientInfo.username,
                        split[1],
                        Utils.nthSplitOnward(response, Constants.DELIMITER, 2));
                break;

            case Constants.CHANGE_COMMAND:
                Server.instance.renameUser(clientInfo.username, split[1], clientInfo);
                break;

            default:
                System.out.println("Unrecognized command: " + command);
                break;
        }
    }

    private void closeConnection() {
        shouldRun = true;
        try {
            clientInfo.socket.close();
            Server.instance.removeClient(clientInfo);
        } catch (IOException e) {
            System.out.println("Could not disconnect client");
            e.printStackTrace();
        }
    }
}
