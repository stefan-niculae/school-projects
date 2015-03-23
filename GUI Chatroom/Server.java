package chatroom;

import java.io.*;
import java.net.*;
import java.util.*;


public class Server
{
    static final int PORT = 1024;
    static ServerSocket serverSocket;

    static class ClientInfo
    {
        Socket socket;
        PrintWriter output;
        public String username;

        public ClientInfo (Socket socket)
        {
            this.socket = socket;
            try {
                output = new PrintWriter (socket.getOutputStream (), true);
            } catch (IOException e) {
                System.out.println ("Could not get client output stream");
                e.printStackTrace ();
            }

            username = "";
        }
    }
    static List<ClientInfo> clients;

    public static void main (String[] args)
    {
        clients = new ArrayList<> ();

        try {
            serverSocket = new ServerSocket (PORT);
        } catch (IOException e) {
            System.out.println ("Could not open a socket on port " + PORT);
            e.printStackTrace ();
        }

        System.out.println ("> Ready for connections!");
        while (true) {
            Socket newClientSocket;
            try {
                newClientSocket = serverSocket.accept ();
                addClient (newClientSocket);
            } catch (IOException e) {
                System.out.println ("Client couldn't connect");
                e.printStackTrace ();
            }
        }

    }

    public static void broadcast (String sender, String message)
    {
        String toSend = sender + ": " + message;
        System.out.println ("Broadcasting '" + toSend + "'");
        clients.forEach (client -> client.output.println (toSend));
    }

    public static void announce (String message)
    {
        clients.forEach (client -> client.output.println ("> " + message));
    }

    public static void addClient (Socket socket)
    {
        ClientInfo newClient = new ClientInfo (socket);

        System.out.println ("> New client accepted");
        clients.add (newClient);

        new ConnectionThread (newClient).start ();
    }

    public static void removeClient (ClientInfo client)
    {
        announce (client.username + " disconnected");
        clients.remove (client);
    }
}

class ConnectionThread extends Thread
{
    Server.ClientInfo clientInfo;
    Scanner networkInput;

    public ConnectionThread (Server.ClientInfo client)
    {
        this.clientInfo = client;
    }

    @Override
    public void run ()
    {
        try {
            networkInput = new Scanner (clientInfo.socket.getInputStream ());
        } catch (IOException e) {
            System.out.println ("Could not get input from client");
            e.printStackTrace ();
        }

        while (true) {

            // Wait for a message
            while (!networkInput.hasNextLine ())
                try {
                    sleep (5);
                } catch (InterruptedException e) {
                    e.printStackTrace ();
                }
            String message = networkInput.nextLine ();

            if (message.equals ("QUIT")) {
                try {
                    clientInfo.socket.close ();
                    Server.removeClient (clientInfo);
                    System.out.println ("> " + clientInfo.username + " disconnected");
                } catch (IOException e) {
                    System.out.println ("Could not disconnect client");
                    e.printStackTrace ();
                }
                break;
            }

            else {

                if (clientInfo.username.isEmpty ()) {
                    clientInfo.username = message;
                    System.out.println ("> " + message + " authenticated");
                    Server.announce (message + " connected");
                }

                else
                    Server.broadcast (clientInfo.username, message);
            }

        }
    }
}
