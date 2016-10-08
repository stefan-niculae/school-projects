package chatroom;

import java.awt.*;


class Constants {
    // For the server-client communication
    static final int PORT = 1024;

    // Communication commands
    static final String
            LOGIN_COMMAND = "LOGIN",
            PUBLIC_COMMAND = "PUBLIC",
            PRIVATE_COMMAND = "PRIVATE",
            ANNOUNCEMENT_COMMAND = "ANNOUNCEMENT",
            PARTICIPANTS_COMMAND = "PARTICIPANTS",
            CHANGE_COMMAND = "CHANGE",
            DISCONNECT_COMMAND = "DISCONNECT";

    static final String DELIMITER = new String(new char[]{(int) 30});

    // Instruction text color
    static final Color GREY = new Color(150, 150, 150);

}
