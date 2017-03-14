package chatroom;


import java.awt.*;
import java.io.InputStream;
import java.util.Scanner;
import java.util.function.Consumer;


class Utils {
    static class ProcessWhenInput extends Thread {
        Scanner input;
        Consumer<String> handleResponse;
        boolean shouldRun = true;

        ProcessWhenInput() {
            // Fields will be set manually later
        }

        ProcessWhenInput(InputStream input, Consumer<String> handleResponse) {
            this.input = new Scanner(input);
            this.handleResponse = handleResponse;
        }

        @Override
        public void run() {
            do {
                waitForInput();
                handleResponse.accept(input.nextLine());
            }
            while (shouldRun);
        }

        void waitForInput() {
            while (!input.hasNextLine()) try {
                sleep(5);
            }
            // Required
            catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        void stopLoop() {
            shouldRun = false;
        }
    }

    static Color StringToColor(String string) {
        String[] split = string.split(" ");
        return new Color(Integer.parseInt(split[0]),
                Integer.parseInt(split[1]),
                Integer.parseInt(split[2]));
    }

    static String ColorToString(Color color) {
        return color.getRed() + " " + color.getGreen() + " " + color.getBlue();
    }

    static String nthSplitOnward(String string, String delimiter, int n) {
        String[] split = string.split(delimiter);
        int offset = 0;
        for (int i = 0; i < n; i++)
            offset += split[i].length() + delimiter.length();
        return string.substring(offset);
    }
}
