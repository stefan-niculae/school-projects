/*
 * Created by JFormDesigner on Sun Apr 26 20:30:06 EEST 2015
 */

package calculator.client;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;


public class CalculatorFrame {

    public static void main(String[] args) {
        double result = Math.pow(10, 3);
        System.out.println(String.format("%10.4E", result));
    }

    public CalculatorFrame() {
        initComponents();
    }

    protected void FrameKeyPressed(KeyEvent e) {
        // Opted against this
    }

    private void initComponents() {
        // JFormDesigner - Component initialization - DO NOT MODIFY  //GEN-BEGIN:initComponents
        // Generated using JFormDesigner Evaluation license - stefan niculae
        Frame = new JFrame();
        Display = new JTextField();
        StButton = new JButton();
        LdButton = new JButton();
        ClButton = new JButton();
        MPlusButton = new JButton();
        MMinusButton = new JButton();
        Button0 = new JButton();
        Button1 = new JButton();
        Button2 = new JButton();
        Button3 = new JButton();
        Button4 = new JButton();
        Button5 = new JButton();
        Button6 = new JButton();
        Button7 = new JButton();
        Button8 = new JButton();
        Button9 = new JButton();
        PointButton = new JButton();
        EqualsButton = new JButton();
        PlusButton = new JButton();
        MinusButton = new JButton();
        StarButton = new JButton();
        SlashButton = new JButton();
        PowButton = new JButton();
        SqrtButton = new JButton();
        InverseButton = new JButton();
        FactorialButton = new JButton();

        //======== Frame ========
        {
            Frame.setMinimumSize(new Dimension(300, 360));
            Frame.setVisible(true);
            Frame.setTitle("Calculator");
            Frame.setResizable(false);
            Frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
            Frame.addKeyListener(new KeyAdapter() {
                @Override
                public void keyPressed(KeyEvent e) {
                    FrameKeyPressed(e);
                }
            });
            Container FrameContentPane = Frame.getContentPane();
            FrameContentPane.setLayout(new GridBagLayout());
            ((GridBagLayout) FrameContentPane.getLayout()).columnWidths = new int[]{0, 0, 0, 0, 0, 0, 0};
            ((GridBagLayout) FrameContentPane.getLayout()).rowHeights = new int[]{0, 0, 0, 0, 0, 0, 0, 0, 0};
            ((GridBagLayout) FrameContentPane.getLayout()).columnWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0E-4};
            ((GridBagLayout) FrameContentPane.getLayout()).rowWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0E-4};

            //---- Display ----
            Display.setFont(new Font("Pocket Calculator", Font.PLAIN, 50));
            Display.setPreferredSize(new Dimension(14, 60));
            Display.setHorizontalAlignment(SwingConstants.RIGHT);
            Display.setMinimumSize(new Dimension(14, 60));
            FrameContentPane.add(Display, new GridBagConstraints(0, 1, 5, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- StButton ----
            StButton.setText("St");
            StButton.setMinimumSize(new Dimension(60, 60));
            StButton.setPreferredSize(new Dimension(60, 60));
            StButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            StButton.setBackground(Color.black);
            StButton.setForeground(new Color(128, 77, 0));
            FrameContentPane.add(StButton, new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- LdButton ----
            LdButton.setText("Ld");
            LdButton.setMinimumSize(new Dimension(60, 60));
            LdButton.setPreferredSize(new Dimension(60, 60));
            LdButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            LdButton.setBackground(Color.black);
            LdButton.setForeground(new Color(128, 77, 0));
            FrameContentPane.add(LdButton, new GridBagConstraints(1, 2, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- ClButton ----
            ClButton.setText("Cl");
            ClButton.setMinimumSize(new Dimension(60, 60));
            ClButton.setPreferredSize(new Dimension(60, 60));
            ClButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            ClButton.setBackground(Color.black);
            ClButton.setForeground(new Color(128, 77, 0));
            FrameContentPane.add(ClButton, new GridBagConstraints(2, 2, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- MPlusButton ----
            MPlusButton.setText("M+");
            MPlusButton.setMinimumSize(new Dimension(60, 60));
            MPlusButton.setPreferredSize(new Dimension(60, 60));
            MPlusButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            MPlusButton.setBackground(Color.black);
            MPlusButton.setForeground(new Color(128, 77, 0));
            FrameContentPane.add(MPlusButton, new GridBagConstraints(3, 2, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- MMinusButton ----
            MMinusButton.setText("M-");
            MMinusButton.setMinimumSize(new Dimension(60, 60));
            MMinusButton.setPreferredSize(new Dimension(60, 60));
            MMinusButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            MMinusButton.setBackground(Color.black);
            MMinusButton.setForeground(new Color(128, 77, 0));
            FrameContentPane.add(MMinusButton, new GridBagConstraints(4, 2, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- Button0 ----
            Button0.setText("0");
            Button0.setMinimumSize(new Dimension(60, 60));
            Button0.setPreferredSize(new Dimension(60, 60));
            Button0.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(Button0, new GridBagConstraints(1, 6, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- Button1 ----
            Button1.setText("1");
            Button1.setMinimumSize(new Dimension(60, 60));
            Button1.setPreferredSize(new Dimension(60, 60));
            Button1.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(Button1, new GridBagConstraints(0, 5, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- Button2 ----
            Button2.setText("2");
            Button2.setMinimumSize(new Dimension(60, 60));
            Button2.setPreferredSize(new Dimension(60, 60));
            Button2.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(Button2, new GridBagConstraints(1, 5, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- Button3 ----
            Button3.setText("3");
            Button3.setMinimumSize(new Dimension(60, 60));
            Button3.setPreferredSize(new Dimension(60, 60));
            Button3.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(Button3, new GridBagConstraints(2, 5, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- Button4 ----
            Button4.setText("4");
            Button4.setMinimumSize(new Dimension(60, 60));
            Button4.setPreferredSize(new Dimension(60, 60));
            Button4.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(Button4, new GridBagConstraints(0, 4, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- Button5 ----
            Button5.setText("5");
            Button5.setMinimumSize(new Dimension(60, 60));
            Button5.setPreferredSize(new Dimension(60, 60));
            Button5.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(Button5, new GridBagConstraints(1, 4, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- Button6 ----
            Button6.setText("6");
            Button6.setMinimumSize(new Dimension(60, 60));
            Button6.setPreferredSize(new Dimension(60, 60));
            Button6.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(Button6, new GridBagConstraints(2, 4, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- Button7 ----
            Button7.setText("7");
            Button7.setMinimumSize(new Dimension(60, 60));
            Button7.setPreferredSize(new Dimension(60, 60));
            Button7.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(Button7, new GridBagConstraints(0, 3, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- Button8 ----
            Button8.setText("8");
            Button8.setMinimumSize(new Dimension(60, 60));
            Button8.setPreferredSize(new Dimension(60, 60));
            Button8.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(Button8, new GridBagConstraints(1, 3, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- Button9 ----
            Button9.setText("9");
            Button9.setMinimumSize(new Dimension(60, 60));
            Button9.setPreferredSize(new Dimension(60, 60));
            Button9.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(Button9, new GridBagConstraints(2, 3, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- PointButton ----
            PointButton.setText(".");
            PointButton.setMinimumSize(new Dimension(60, 60));
            PointButton.setPreferredSize(new Dimension(60, 60));
            PointButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FrameContentPane.add(PointButton, new GridBagConstraints(0, 6, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- EqualsButton ----
            EqualsButton.setText("=");
            EqualsButton.setMinimumSize(new Dimension(60, 60));
            EqualsButton.setPreferredSize(new Dimension(60, 60));
            EqualsButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            EqualsButton.setBackground(new Color(245, 136, 11));
            FrameContentPane.add(EqualsButton, new GridBagConstraints(2, 6, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- PlusButton ----
            PlusButton.setText("+");
            PlusButton.setMinimumSize(new Dimension(60, 60));
            PlusButton.setPreferredSize(new Dimension(60, 60));
            PlusButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            PlusButton.setBackground(Color.black);
            PlusButton.setForeground(new Color(0, 156, 39));
            FrameContentPane.add(PlusButton, new GridBagConstraints(3, 3, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- MinusButton ----
            MinusButton.setText("-");
            MinusButton.setMinimumSize(new Dimension(60, 60));
            MinusButton.setPreferredSize(new Dimension(60, 60));
            MinusButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            MinusButton.setBackground(Color.black);
            MinusButton.setForeground(new Color(0, 156, 39));
            FrameContentPane.add(MinusButton, new GridBagConstraints(3, 4, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- StarButton ----
            StarButton.setText("×");
            StarButton.setMinimumSize(new Dimension(60, 60));
            StarButton.setPreferredSize(new Dimension(60, 60));
            StarButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            StarButton.setBackground(Color.black);
            StarButton.setForeground(new Color(0, 156, 39));
            FrameContentPane.add(StarButton, new GridBagConstraints(3, 5, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- SlashButton ----
            SlashButton.setText("/");
            SlashButton.setMinimumSize(new Dimension(60, 60));
            SlashButton.setPreferredSize(new Dimension(60, 60));
            SlashButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            SlashButton.setBackground(Color.black);
            SlashButton.setForeground(new Color(0, 156, 39));
            FrameContentPane.add(SlashButton, new GridBagConstraints(3, 6, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- PowButton ----
            PowButton.setText("⌃");
            PowButton.setPreferredSize(new Dimension(60, 60));
            PowButton.setMinimumSize(new Dimension(60, 60));
            PowButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            PowButton.setBackground(Color.black);
            PowButton.setForeground(new Color(0, 156, 39));
            FrameContentPane.add(PowButton, new GridBagConstraints(4, 3, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- SqrtButton ----
            SqrtButton.setText("\u221a");
            SqrtButton.setMinimumSize(new Dimension(60, 60));
            SqrtButton.setPreferredSize(new Dimension(60, 60));
            SqrtButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            SqrtButton.setBackground(Color.black);
            SqrtButton.setForeground(new Color(0, 156, 39));
            FrameContentPane.add(SqrtButton, new GridBagConstraints(4, 4, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- InverseButton ----
            InverseButton.setText("1/");
            InverseButton.setMinimumSize(new Dimension(60, 60));
            InverseButton.setPreferredSize(new Dimension(60, 60));
            InverseButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            InverseButton.setBackground(Color.black);
            InverseButton.setForeground(new Color(0, 156, 39));
            FrameContentPane.add(InverseButton, new GridBagConstraints(4, 5, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));

            //---- FactorialButton ----
            FactorialButton.setText("!");
            FactorialButton.setMinimumSize(new Dimension(60, 60));
            FactorialButton.setPreferredSize(new Dimension(60, 60));
            FactorialButton.setFont(new Font("Cambria Math", Font.BOLD, 20));
            FactorialButton.setBackground(Color.black);
            FactorialButton.setForeground(new Color(0, 156, 39));
            FrameContentPane.add(FactorialButton, new GridBagConstraints(4, 6, 1, 1, 0.0, 0.0,
                    GridBagConstraints.CENTER, GridBagConstraints.BOTH,
                    new Insets(0, 0, 0, 0), 0, 0));
            Frame.pack();
            Frame.setLocationRelativeTo(Frame.getOwner());
        }
        // JFormDesigner - End of component initialization  //GEN-END:initComponents

        Frame.pack();
    }

    // JFormDesigner - Variables declaration - DO NOT MODIFY  //GEN-BEGIN:variables
    // Generated using JFormDesigner Evaluation license - stefan niculae
    protected JFrame Frame;
    protected JTextField Display;
    protected JButton StButton;
    protected JButton LdButton;
    protected JButton ClButton;
    protected JButton MPlusButton;
    protected JButton MMinusButton;
    protected JButton Button0;
    protected JButton Button1;
    protected JButton Button2;
    protected JButton Button3;
    protected JButton Button4;
    protected JButton Button5;
    protected JButton Button6;
    protected JButton Button7;
    protected JButton Button8;
    protected JButton Button9;
    protected JButton PointButton;
    protected JButton EqualsButton;
    protected JButton PlusButton;
    protected JButton MinusButton;
    protected JButton StarButton;
    protected JButton SlashButton;
    protected JButton PowButton;
    protected JButton SqrtButton;
    protected JButton InverseButton;
    protected JButton FactorialButton;
    // JFormDesigner - End of variables declaration  //GEN-END:variables
}
