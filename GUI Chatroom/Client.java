package chatroom;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.*;


public class Client
{
    static final int PORT = 1024;
    static InetAddress host;
    static Socket socket = null;

    static ClientFrame frame;
    static ClientListener listener;
    static Scanner networkInput;
    static PrintWriter networkOutput;
    static NetworkReceiverThread receiverThread;

    public static boolean connected = false;

    public static void main (String[] args)
    {
        listener = new ClientListener ();
        frame = new ClientFrame (listener);
        frame.addWindowListener (
                new WindowAdapter ()  {
                @Override
                public void windowClosing (WindowEvent e) {
                    System.exit (disconnect () ? 0 : 1);
                }
        });
    }

    public static boolean connect (String username)
    {
        try {
            host = InetAddress.getLocalHost ();
        } catch (UnknownHostException e) {
            System.out.println ("Could not get local address");
            e.printStackTrace ();
            return false;
        }
        try {
            socket = new Socket (host, PORT);
        } catch (IOException e) {
            System.out.println ("Could not connect to the server");
            e.printStackTrace ();
            return false;
        }

        try {
            networkInput = new Scanner (socket.getInputStream ());
            networkOutput = new PrintWriter (socket.getOutputStream (), true);

        } catch (IOException e) {
            System.out.println ("Could not get network input/output stream!");
            e.printStackTrace ();
        }

        receiverThread = new NetworkReceiverThread ();
        receiverThread.start ();
        networkOutput.println (username);
        frame.connectedView ();

        frame.responses.append ("> Welcome " + username + "!\n");
        return connected = true;
    }

    public static void send (String message)
    {
        networkOutput.println (message);
    }

    public static boolean disconnect ()
    {
        networkOutput.println ("QUIT");

        try {
            socket.close ();
            System.out.println ("Closing connection");
        } catch (IOException e) {
            System.out.println ("Could not disconnect!");
            e.printStackTrace ();
            return false;
        }

        frame.disconnectedView ();
        receiverThread.shouldListen = false;

        connected = false;
        return true;
    }
}

class NetworkReceiverThread extends Thread
{
    public boolean shouldListen = true;

    @Override
    public void run ()
    {
        String response;

        // Why can't I use Client.networkInput instead of this local one?!!
        Scanner input = null;
        try {
            input = new Scanner (Client.socket.getInputStream ());
        } catch (IOException e) {
            e.printStackTrace ();
        }

        do {
            // Wait for some network input
            while (!(input != null && input.hasNext ()) && shouldListen)
                try {
                    sleep (5);
                } catch (InterruptedException e) {
                    e.printStackTrace ();
                }

            try {
                response = Client.networkInput.nextLine ();
                Client.frame.responses.append (response + "\n");
            } catch (NoSuchElementException e) {
                // It's ok...
            }
        } while (Client.connected);

    }
}

class ClientListener implements ActionListener
{
    @Override
    public void actionPerformed (ActionEvent event)
    {
        if (event.getSource () == Client.frame.connectionButton) {

            if (!Client.connected) {
                String username = Client.frame.userInput.getText ();
                Client.connect (username);
            }

            else
                Client.disconnect ();
        }

        else if (event.getSource () == Client.frame.sendButton) {
            String message = Client.frame.userInput.getText ();
            if (!message.isEmpty ()) {
                Client.send (message);
                Client.frame.userInput.setText ("");
            }
        }
    }
}

class ClientFrame extends JFrame
{
    JTextField userInput;

    JTextArea responses;

    JPanel buttonsPanel;
    JButton connectionButton;
    JButton sendButton;


    public ClientFrame (ActionListener listener)
    {
        // Window initialization
        setSize (500, 500);
        setVisible (true);
        setTitle ("GUI Echo Client");

        // Text field initialization
        userInput = new JTextField ("user input", 20);
        add (userInput, BorderLayout.CENTER);

        // Text area initialization
        responses = new JTextArea (20, 25);
        responses.setWrapStyleWord (true);
        responses.setLineWrap (true);

        add (responses, BorderLayout.NORTH);

        // Buttons initialization
        connectionButton = new JButton ("Connection");
        sendButton       = new JButton ("Send");
        connectionButton .addActionListener (listener);
        sendButton       .addActionListener (listener);

        buttonsPanel = new JPanel ();
        buttonsPanel.add (connectionButton);
        buttonsPanel.add (sendButton);
        add (buttonsPanel, BorderLayout.SOUTH);

        pack ();

        disconnectedView ();
    }

    public void connectedView ()
    {
        userInput.setText ("Enter message to send");

        responses.setText ("");
        add (responses, BorderLayout.NORTH);

        connectionButton.setText ("Disconnect");

        remove (buttonsPanel);
        buttonsPanel.add (sendButton);
        add (buttonsPanel, BorderLayout.SOUTH);

        pack ();
    }

    public void disconnectedView ()
    {
        userInput.setText ("Enter username");

        remove (responses);
        connectionButton.setText ("Connect");

        remove (buttonsPanel);
        buttonsPanel.remove (sendButton);
        add (buttonsPanel, BorderLayout.SOUTH);

        pack ();
    }
}