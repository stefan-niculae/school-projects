package chatroom;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.HashMap;
import java.util.function.Consumer;
import java.util.function.Predicate;


public class Client {
    private static Socket socket;

    private static GUIHandler guiHandler;
    private static PrintWriter networkOutput;
    private static Utils.ProcessWhenInput receiverThread;

    private static boolean connected;
    static boolean loggedIn;

    public static void main(String[] args) {
        // Show the GUI the user will interact with
        guiHandler = new GUIHandler();

        // Connection is different from authentication
        connect();
    }

    //region Connection Handling
    static boolean connect() {
        InetAddress host;
        try {
            host = InetAddress.getLocalHost();
        } catch (UnknownHostException e) {
            System.out.println("Could not get local address");
            e.printStackTrace();
            return false;
        }

        try {
            socket = new Socket(host, Constants.PORT);
        } catch (IOException e) {
            System.out.println("Could not connect to the server");
            e.printStackTrace();
            return false;
        }
        try {
            receiverThread = new Utils.ProcessWhenInput(socket.getInputStream(),
                    Client::handleResponse);
        } catch (IOException e) {
            e.printStackTrace();
        }
        receiverThread.start();

        try {
            networkOutput = new PrintWriter(socket.getOutputStream(), true);

        } catch (IOException e) {
            System.out.println("Could not get network input/output stream!");
            e.printStackTrace();
        }

        System.out.println("Connected");
        return connected = true;
    }

    static boolean disconnect() {
        connected = false;
        receiverThread.stopLoop();
        networkOutput.println(Constants.DISCONNECT_COMMAND);

        try {
            socket.close();
        } catch (IOException e) {
            System.out.println("Could not disconnect!");
            e.printStackTrace();
            return false;
        }

        // An unauthenticated user can still disconnect
        if (loggedIn) {
            loggedIn = false;
            guiHandler.notLoggedView();
        }

        System.out.println("Disconnected");
        return true;
    }

    static boolean shutDown() {
        networkOutput.println(Constants.DISCONNECT_COMMAND);

        // We don't place this in the disconnect function because the boolean connected
        // is not well synchronized between the connect() and disconnect() functions
        if (!connected)
            return true;

        try {
            socket.close();
        } catch (IOException e) {
            System.out.println("Could not disconnect!");
            e.printStackTrace();
            return false;
        }

        System.out.println("Shut down");
        return true;
    }
    //endregion


    //region Username Handling
    static void login(String username) {
        loggedIn = true;
        sendUsername(username);
        guiHandler.connectedView();
    }

    static void changeUsername(String newUser) {
        System.out.println("Renaming to " + newUser);
        networkOutput.println(Constants.CHANGE_COMMAND + Constants.DELIMITER + newUser);
    }
    //endregion


    //region Send Handling
    private static void sendUsername(String username) {
        System.out.println("Authenticating as " + username);
        networkOutput.println(Constants.LOGIN_COMMAND + Constants.DELIMITER + username);
    }

    static void sendPublicMessage(String message) {
        System.out.println("Publicly sending " + message);
        networkOutput.println(Constants.PUBLIC_COMMAND + Constants.DELIMITER + message);
    }

    static void sendPrivateMessage(String recipient, String message) {
        System.out.println("Privately sending to " + recipient + " " + message);
        networkOutput.println(Constants.PRIVATE_COMMAND + Constants.DELIMITER + recipient + Constants.DELIMITER + message);
    }
    //endregion


    //region Receive Handling
    private static void handleResponse(String response) {
        System.out.println("Server output = " + response.replace(Constants.DELIMITER, "_"));

        String[] split = response.split(Constants.DELIMITER);
        String command = split[0];

        switch (command) {

            case Constants.PARTICIPANTS_COMMAND:
                if (split.length > 1)
                    Client.guiHandler.updateParticipants(Utils.nthSplitOnward(response, Constants.DELIMITER, 1));
                break;

            case Constants.ANNOUNCEMENT_COMMAND:
                guiHandler.addConversationResponse(guiHandler.roomConversation,
                        "",
                        split[1],
                        split[2],
                        false);
                break;

            case Constants.PUBLIC_COMMAND:
                guiHandler.addConversationResponse(guiHandler.roomConversation,
                        "",
                        split[1],
                        split[2],
                        true);
                break;

            case Constants.PRIVATE_COMMAND:
                guiHandler.addConversationResponse(guiHandler.privateConversation,
                        "[From]",
                        split[1],
                        split[2],
                        true);
                break;

            default:
                System.out.println("Unknown command received: " + command);
                break;
        }
    }
    //endregion
}

class GUIHandler {
    private ClientForm window;
    private UsernameChangeDialog dialog;

    private CardLayout cardLayout;
    StyledDocument roomConversation;
    StyledDocument privateConversation;
    private StyledDocument participants;

    private String thisUser;
    private HashMap<String, Color> colorOfUser;

    GUIHandler() {
        // Components initialization
        initialize();
    }

    //region Initialization
    private void initialize() {
        window = new ClientForm();
        cardLayout = (CardLayout) window.clientLayout.getContentPane().getLayout();

        roomConversation = window.roomConversationText.getStyledDocument();
        privateConversation = window.privateConversationText.getStyledDocument();
        participants = window.participantsText.getStyledDocument();

        colorOfUser = new HashMap<>();

        // Make components do something
        programButtons();
    }

    private void programButtons() {
        //region [x] Button
        window.clientLayout.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                System.exit(Client.shutDown() ? 0 : 1);
            }
        });
        //endregion


        //region Login Input
        placeInstructions(window.usernameTextField, "Enter username");
        window.usernameTextField.addFocusListener(new EraseOnClick(window.usernameTextField));

        // Enable submission iff the username entered is valid
        window.usernameTextField.getDocument().addDocumentListener(new InputListener(window.connectButton, true, null));

        ActIfValid usernameEntry = new ActIfValid(window.usernameTextField,
                this::usernameAvailable,
                this::processUsernameSubmit,
                null);
        window.usernameTextField.addActionListener(usernameEntry);
        window.connectButton.addActionListener(usernameEntry);
        //endregion


        //region Change User Button
        window.changeUsernameButton.addActionListener(e -> {
            // User starts editing their username starting from the current one
            dialog.userDialogField.setText(thisUser);
            // Show the dialog
            dialog.setVisible(true);
        });
        //endregion


        //region Disconnect Button
        window.disconnectButton.addActionListener(e -> Client.disconnect());
        //endregion


        //region Room Conversation Input
        placeInstructions(window.roomTextField, "Enter public message");
        window.roomTextField.addFocusListener(new EraseOnClick(window.roomTextField));
        window.roomTextField.getDocument().addDocumentListener(new InputListener(window.roomSendButton, false, null));

        ActIfValid roomEntry = new ActIfValid(window.roomTextField,
                t -> !filterText(t).isEmpty(),
                this::processRoomSubmit,
                null);
        window.roomTextField.addActionListener(roomEntry);
        window.roomSendButton.addActionListener(roomEntry);
        //endregion


        //region Private Conversation Input
        placeInstructions(window.privateTextField, "Enter private message");
        window.privateTextField.addFocusListener(new EraseOnClick(window.privateTextField));
        window.privateTextField.getDocument().addDocumentListener(new InputListener(window.privateSendButton, false, window.privateRecipientBox));

        ActIfValid privateEntry = new ActIfValid(window.privateTextField,
                t -> !filterText(t).isEmpty(),
                this::processPrivateSubmit,
                window.privateRecipientBox);
        window.privateTextField.addActionListener(privateEntry);
        window.privateSendButton.addActionListener(privateEntry);
        //endregion


        //region Change User Dialog
        dialog = new UsernameChangeDialog(window.clientLayout);

        // Enable submission iff the username entered is valid
        dialog.userDialogField.getDocument().addDocumentListener(new InputListener(dialog.userDialogButton, true, null));

        ActionListener changeUserSubmission = e -> {

            String dialogInput, newUser = "";
            dialogInput = dialog.userDialogField.getText();
            newUser = filterText(dialogInput);
            if (usernameAvailable(newUser)) {
                dialog.setVisible(false);
                // Passed validation and is not the same one as before
                if (!newUser.isEmpty() && !newUser.equals(thisUser)) {
                    setNewUser(newUser);
                    Client.changeUsername(newUser);
                }
            }
        };
        dialog.userDialogField.addActionListener(changeUserSubmission);
        dialog.userDialogButton.addActionListener(changeUserSubmission);
        //endregion
    }


    //region Listeners
    private class ActIfValid implements ActionListener {
        JTextField watched;
        Predicate<String> validateText;
        Consumer<String> processText;
        JComboBox recipientBox;

        ActIfValid(JTextField watched, Predicate<String> validateText, Consumer<String> processText, JComboBox recipientBox) {
            this.watched = watched;
            this.validateText = validateText;
            this.processText = processText;
            this.recipientBox = recipientBox;
        }

        @Override
        public void actionPerformed(ActionEvent e) {
            String text = filterText(watched.getText());
            // The text gets processed iff it passes validation
            boolean hasPrivatePartner = true;
            if (recipientBox != null)
                try {
                    if (recipientBox.getSelectedItem().toString() != null)
                        hasPrivatePartner = true;
                } catch (NullPointerException exception) {
                    hasPrivatePartner = false;
                }

            if (validateText.test(text) && hasPrivatePartner)
                processText.accept(text);
        }
    }

    // Fields should erase the instructions when focused
    private class EraseOnClick implements FocusListener {
        final Color FOCUSED_COLOR = Color.BLACK;
        JTextField textField;

        EraseOnClick(JTextField textField) {
            this.textField = textField;
        }

        @Override
        public void focusGained(FocusEvent e) {
            // Erase the text and put the focused color in place
            textField.setText("");
            textField.setForeground(FOCUSED_COLOR);
        }

        @Override
        public void focusLost(FocusEvent e) {
        }
    }

    private class InputListener implements DocumentListener {
        JButton button;
        JComboBox privateSelection;
        boolean isUsername;

        InputListener(JButton button, boolean isUsername, JComboBox privateSelection) {
            this.button = button;
            this.isUsername = isUsername;
            this.privateSelection = privateSelection;
        }

        void enableIfValid(DocumentEvent e) {
            String text = "";
            try {
                text = e.getDocument().getText(0, e.getDocument().getLength());
            } catch (BadLocationException e1) {
                e1.printStackTrace();
            }

            text = filterText(text);
            boolean enabled = !text.isEmpty();
            if (isUsername)
                enabled &= usernameAvailable(text);

            if (privateSelection != null) {
                try {
                    //noinspection StatementWithEmptyBody
                    if (privateSelection.getSelectedItem().toString() != null)
                        ;
                } catch (NullPointerException exception) {
                    // Means there is no one selected
                    enabled = false;
                }
            }

            button.setEnabled(enabled);
        }

        @Override
        public void insertUpdate(DocumentEvent e) {
            enableIfValid(e);
        }

        @Override
        public void removeUpdate(DocumentEvent e) {
            enableIfValid(e);
        }

        @Override
        public void changedUpdate(DocumentEvent e) {
            enableIfValid(e);
        }
    }
    //endregion


    //region Processors
    private void processUsernameSubmit(String newUser) {
        setNewUser(newUser);
        Client.login(newUser);
    }

    private void processRoomSubmit(String message) {
        Client.sendPublicMessage(message);
        window.roomTextField.setText("");
    }

    private void processPrivateSubmit(String message) {
        String target = window.privateRecipientBox.getSelectedItem().toString();
        Client.sendPrivateMessage(target, message);
        addConversationResponse(privateConversation,
                "[To]",
                target,
                message,
                true);
        window.privateTextField.setText("");
    }
    //endregion Processors
    //endregion Initialization


    //region Username Handling
    private void setNewUser(String newUser) {
        thisUser = newUser;
        window.loggedInText.setText(newUser);
    }

    private boolean usernameAvailable(String username) {
        username = filterText(username);

        // This check happens when the user logged out and is trying to log back in using the same name
        if (username.equals(thisUser))
            return true;

        // An username can't be empty, but we have other parts checking for that

        // If it is in the map, that means there is another user that already has that name
        return !colorOfUser.keySet().contains(username);

    }

    private String filterText(String text) {
        // Replace the delimiter from the message
        text = text.replace(Constants.DELIMITER, "");
        // No trailing or padding whitespace
        text = text.trim();

        return text;
    }
    //endregion


    //region View Handling
    void notLoggedView() {
        // Automatically connect again
        Client.connect();
        // Show the disconnected layout
        cardLayout.show(window.clientLayout.getContentPane(), "disconnected");
    }

    void connectedView() {
        // Show the connected layout
        cardLayout.show(window.clientLayout.getContentPane(), "connected");

        try {
            // Clear the past conversations (even if it is the same user)
            roomConversation.remove(0, roomConversation.getLength());
            privateConversation.remove(0, privateConversation.getLength());
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
    }
    //endregion


    //region Content Handling
    private void placeInstructions(JTextField textField, String instructions) {
        // Let only the instruction be in the text and change their color to grey
        textField.setForeground(Constants.GREY);
        textField.setText(instructions);
    }

    void addConversationResponse(StyledDocument document, String prefix, String sender, String message, boolean userMessage) {
        String senderMessageSeparator = ": ";
        // No colon for server announcements
        if (!userMessage)
            senderMessageSeparator = " ";

        insertMessage(document, prefix, sender, senderMessageSeparator + message + "\n", true);
    }

    private void insertMessage(StyledDocument document, String prefix, String user, String content, boolean addTimestamp) {
        String sender = user;
        if (!prefix.isEmpty())
            sender = prefix + "\t" + sender;
        appendColored(document, sender, colorOfUser.get(user));

        if (addTimestamp)
            appendColored(document, "(" + currentTime() + ")", Constants.GREY);

        appendColored(document, content, Color.BLACK);
    }

    @SuppressWarnings("unchecked")
    void updateParticipants(String participantsList) {
        try {
            // Clear the old participants list
            participants.remove(0, participants.getLength());
        } catch (BadLocationException e) {
            // This won't be thrown unless there are sync issues
            e.printStackTrace();
        }

        // Split the participants
        String[] split = participantsList.split(Constants.DELIMITER);

        // Ignore if this is the first client connected (no other participants and this one hasn't authenticated yet)
        if (participantsList.length() != 0) {
            String[] users = extractUsers(split);

            if (Client.loggedIn) {
                // Remove the current user as an option for private talking
                String[] otherUsers = removeSelfUser(users);

                // Update the combo box details
                window.privateRecipientBox.setModel(new DefaultComboBoxModel<>(otherUsers));
                // The private send button is enabled iff there is someone else to talk to
                window.privateSendButton.setEnabled(otherUsers.length != 0);
            }
        }
    }

    private String[] extractUsers(String[] participants) {
        String[] users = new String[participants.length];

        colorOfUser.clear();
        for (int i = 0; i < users.length; i++) {
            // The order is: R G B username
            users[i] = Utils.nthSplitOnward(participants[i], " ", 3);

            // Add pair to the map
            colorOfUser.putIfAbsent(users[i],
                    Utils.StringToColor(participants[i]));

            // Add the user to the scrolling list
            insertMessage(this.participants,
                    "",
                    users[i], "\n",
                    false);
        }

        return users;
    }

    private String[] removeSelfUser(String[] users) {
        String[] result = new String[users.length - 1];

        int k = 0;
        for (String user : users)
            if (!user.equals(thisUser))
                result[k++] = user;

        return result;
    }

    private void appendColored(StyledDocument document, String string, Color color) {
        StyleContext style = StyleContext.getDefaultStyleContext();
        try {
            document.insertString(document.getLength(),
                    string,
                    style.addAttribute(SimpleAttributeSet.EMPTY,
                            StyleConstants.Foreground,
                            color));
        } catch (BadLocationException e) {
            // This occurs when the client disconnects but still some messages are received
        }
    }

    private String currentTime() {
        SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");
        return format.format(Calendar.getInstance().getTime());
    }
    //endregion
}
