package calculator.client;


import calculator.utils.CalculatorRemote;
import calculator.utils.Constants;

import javax.swing.*;
import javax.swing.text.AbstractDocument;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.DocumentFilter;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;


public class Client {
    private static final String HOST = "localhost";

    public static void main(String[] args) throws RemoteException, NotBoundException {
        Registry registry = LocateRegistry.getRegistry(HOST, Constants.PORT);
        CalculatorRemote calculator = (CalculatorRemote) registry.lookup(Constants.ID);

        new GUIHandler(calculator);
    }
}

class GUIHandler {
    private CalculatorRemote calculator;
    private CalculatorFrame frame;
    private static final int MAX_DIGITS = 14;

    private enum Operations {
        addition, subtraction, multiplication, division,
        rise, sqrt, inversing, factorial
    }

    private Operations enteredOperation;
    private double lastEntered;

    GUIHandler(CalculatorRemote calculator) {
        this.calculator = calculator;

        frame = new CalculatorFrame();
        WireButtons();
        ClearMemory();
    }

    //region Initialization
    private void WireButtons() {
        frame.Button0.addActionListener(e -> Append("0"));
        frame.Button1.addActionListener(e -> Append("1"));
        frame.Button2.addActionListener(e -> Append("2"));
        frame.Button3.addActionListener(e -> Append("3"));
        frame.Button4.addActionListener(e -> Append("4"));
        frame.Button5.addActionListener(e -> Append("5"));
        frame.Button6.addActionListener(e -> Append("6"));
        frame.Button7.addActionListener(e -> Append("7"));
        frame.Button8.addActionListener(e -> Append("8"));
        frame.Button9.addActionListener(e -> Append("9"));

        frame.PointButton.addActionListener(e -> Append("."));
        //frame.Backspace.addActionListener (e -> Backspace ());

        frame.PlusButton.addActionListener(e -> RegisterOperation(Operations.addition));
        frame.MinusButton.addActionListener(e -> RegisterOperation(Operations.subtraction));
        frame.StarButton.addActionListener(e -> RegisterOperation(Operations.multiplication));
        frame.SlashButton.addActionListener(e -> RegisterOperation(Operations.division));
        frame.PowButton.addActionListener(e -> RegisterOperation(Operations.rise));
        frame.SqrtButton.addActionListener(e -> RegisterOperation(Operations.sqrt));
        frame.InverseButton.addActionListener(e -> RegisterOperation(Operations.inversing));
        frame.FactorialButton.addActionListener(e -> RegisterOperation(Operations.factorial));

        frame.EqualsButton.addActionListener(e -> ComputeResult());

        frame.ClButton.addActionListener(e -> ClearMemory());
        frame.StButton.addActionListener(e -> Store());
        frame.LdButton.addActionListener(e -> Load());
        frame.MPlusButton.addActionListener(e -> MPlus());
        frame.MMinusButton.addActionListener(e -> MMinus());

        AbstractDocument document = (AbstractDocument) frame.Display.getDocument();
        document.setDocumentFilter(new MathOnlyFilter(frame.Display));
    }
    //endregion

    //region Display Conversion
    private void Show(double number) {
        String toDisplay = String.format("%.4f", number);

        // If it has no fractionary part, don't show the trailing zero
        if ((int) number == number)
            toDisplay = "" + (int) number;

        if (toDisplay.length() > MAX_DIGITS)
            toDisplay = String.format("%10.4E", number);

        toDisplay = toDisplay.replace(",", ".");
        frame.Display.setText(toDisplay);
    }

    private void ShowError() {
        frame.Display.setText("ERROR");
    }

    private double ParseInput() {
        String text = frame.Display.getText();

        if (text.isEmpty())
            return 0;
        if (text.equals("ERROR"))
            return 0;

        return Double.parseDouble(text);
    }
    //endregion

    //region Display Modification
    private void Append(String string) {
        if (string.equals(".") && !CanAddPoint())
            return;

        String text = frame.Display.getText();
        if (ParseInput() == 0)
            text = "";
        if (text.length() < MAX_DIGITS)
            frame.Display.setText(text + string);
    }

    private boolean CanAddPoint() {
        return !frame.Display.getText().contains(".");
    }

    private void ClearDisplay() {
        frame.Display.setText("");
    }

    void Backspace() {
        String text = frame.Display.getText();
        if (text.length() > 0) {
            text = text.substring(0, text.length() - 1);
            frame.Display.setText(text);
        }
    }
    //endregion

    //region Operation Handling
    private void RegisterOperation(Operations operation) {
        enteredOperation = operation;
        alreadyProcessedOperation = false;

        // Protection against accidental double press of a button
        if (!frame.Display.getText().isEmpty())
            lastEntered = ParseInput();

        // Unary operations are computed immediately
        if (IsUnary(operation))
            ComputeResult();

            // Binary operations await the second operand
        else
            ClearDisplay();
    }

    private boolean alreadyProcessedOperation;
    private double entered;

    private void ComputeResult() {
        if (!alreadyProcessedOperation)
            entered = ParseInput();
        else {
            lastEntered = ParseInput();
        }

        double result = 0;
        boolean errorOccurred = false;

        try {
            switch (enteredOperation) {

                case addition:
                    result = calculator.add(lastEntered, entered);
                    break;

                case subtraction:
                    result = calculator.subtract(lastEntered, entered);
                    break;

                case multiplication:
                    result = calculator.multiply(lastEntered, entered);
                    break;

                case division:
                    result = calculator.divide(lastEntered, entered);
                    break;

                case rise:
                    result = calculator.raise(lastEntered, entered);
                    break;


                case sqrt:
                    result = calculator.squareRoot(lastEntered);
                    break;

                case inversing:
                    result = calculator.inverse(lastEntered);
                    break;

                case factorial:
                    result = calculator.factorial(lastEntered);
                    break;

            }
        } catch (ArithmeticException e) {
            errorOccurred = true;
        } catch (RemoteException e) {
            e.printStackTrace();
        }

        alreadyProcessedOperation = true;

        if (errorOccurred || Double.isNaN(result))
            ShowError();
        else
            Show(result);
    }

    private boolean IsUnary(Operations operation) {
        return operation == Operations.sqrt ||
                operation == Operations.inversing ||
                operation == Operations.factorial;
    }
    //endregion

    //region Memory Handling
    private void Store() {
        try {
            calculator.setMem(ParseInput());
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    private void Load() {
        try {
            Show(calculator.getMem());
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    private void ClearMemory() {
        try {
            calculator.setMem(0);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    private void MPlus() {
        try {
            calculator.addToMem(ParseInput());
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    private void MMinus() {
        try {
            calculator.subFromMem(ParseInput());
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }
    //endregion

}

class MathOnlyFilter extends DocumentFilter {
    private JTextField watched;

    MathOnlyFilter(JTextField watched) {
        this.watched = watched;
    }

    @Override
    public void insertString(FilterBypass fb, int offset, String string, AttributeSet attr) throws BadLocationException {
        String original = watched.getText();
        super.insertString(fb, offset, string, attr);
        if (!IsValid(watched.getText()))
            watched.setText(original);
    }

    @Override
    public void replace(FilterBypass fb, int offset, int length, String text, AttributeSet attrs) throws BadLocationException {
        String original = watched.getText();
        super.replace(fb, offset, length, text, attrs);
        if (!IsValid(watched.getText()))
            watched.setText(original);
    }

    private boolean IsValid(String string) {
        if (string.isEmpty())
            return true;

        if (string.equals("-") || string.equals("+"))
            return true;

        if (string.equals("ERROR"))
            return true;

        try {
            //noinspection ResultOfMethodCallIgnored
            Double.parseDouble(string);
        } catch (NumberFormatException e) {
            System.out.println("\tcant parse it!");
            return false;
        }

        return true;
    }
}
