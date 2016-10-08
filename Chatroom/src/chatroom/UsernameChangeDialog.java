/*
 * Created by JFormDesigner on Thu Apr 23 21:36:30 EEST 2015
 */

package chatroom;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;

public class UsernameChangeDialog extends JDialog {
    public UsernameChangeDialog(Frame owner) {
        super(owner);
        initComponents();
    }

    public UsernameChangeDialog(Dialog owner) {
        super(owner);
        initComponents();
    }

    private void initComponents() {
        // JFormDesigner - Component initialization - DO NOT MODIFY  //GEN-BEGIN:initComponents
        // Generated using JFormDesigner Evaluation license - stefan niculae
        dialogPane = new JPanel();
        userDialogField = new JTextField();
        userDialogButton = new JButton();

        //======== this ========
        setTitle("Change Username");
        Container contentPane = getContentPane();
        contentPane.setLayout(new BorderLayout());

        //======== dialogPane ========
        {
            dialogPane.setBorder(new EmptyBorder(12, 12, 12, 12));
            dialogPane.setMinimumSize(new Dimension(300, 300));
            dialogPane.setPreferredSize(new Dimension(300, 300));

            // JFormDesigner evaluation mark
//            dialogPane.setBorder(new javax.swing.border.CompoundBorder(
//                new javax.swing.border.TitledBorder(new javax.swing.border.EmptyBorder(0, 0, 0, 0),
//                    "JFormDesigner Evaluation", javax.swing.border.TitledBorder.CENTER,
//                    javax.swing.border.TitledBorder.BOTTOM, new java.awt.Font("Dialog", java.awt.Font.BOLD, 12),
//                    java.awt.Color.red), dialogPane.getBorder())); dialogPane.addPropertyChangeListener(new java.beans.PropertyChangeListener(){public void propertyChange(java.beans.PropertyChangeEvent e){if("border".equals(e.getPropertyName()))throw new RuntimeException();}});

            dialogPane.setLayout(null);
            dialogPane.add(userDialogField);
            userDialogField.setBounds(35, 90, 225, 35);

            //---- userDialogButton ----
            userDialogButton.setText("Ok");
            dialogPane.add(userDialogButton);
            userDialogButton.setBounds(100, 170, 90, userDialogButton.getPreferredSize().height);

            { // compute preferred size
                Dimension preferredSize = new Dimension();
                for(int i = 0; i < dialogPane.getComponentCount(); i++) {
                    Rectangle bounds = dialogPane.getComponent(i).getBounds();
                    preferredSize.width = Math.max(bounds.x + bounds.width, preferredSize.width);
                    preferredSize.height = Math.max(bounds.y + bounds.height, preferredSize.height);
                }
                Insets insets = dialogPane.getInsets();
                preferredSize.width += insets.right;
                preferredSize.height += insets.bottom;
                dialogPane.setMinimumSize(preferredSize);
                dialogPane.setPreferredSize(preferredSize);
            }
        }
        contentPane.add(dialogPane, BorderLayout.CENTER);
        pack();
        setLocationRelativeTo(getOwner());
        // JFormDesigner - End of component initialization  //GEN-END:initComponents
    }

    // JFormDesigner - Variables declaration - DO NOT MODIFY  //GEN-BEGIN:variables
    // Generated using JFormDesigner Evaluation license - stefan niculae
    private JPanel dialogPane;
    protected JTextField userDialogField;
    protected JButton userDialogButton;
    // JFormDesigner - End of variables declaration  //GEN-END:variables
}
