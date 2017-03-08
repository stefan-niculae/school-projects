/*
 * Created by JFormDesigner on Wed Apr 22 20:25:06 EEST 2015
 */

package chatroom;

import javax.swing.*;
import java.awt.*;

public class ClientForm {
    public ClientForm() {
        initComponents();
        clientLayout.pack();
    }

    private void initComponents() {
        // JFormDesigner - Component initialization - DO NOT MODIFY  //GEN-BEGIN:initComponents
        // Generated using JFormDesigner Evaluation license - stefan niculae
        clientLayout = new JFrame();
        disconnectedLayout = new JPanel();
        connectButton = new JButton();
        usernameTextField = new JTextField();
        connectedLayout = new JPanel();
        loggedInPanel = new JPanel();
        changeUsernameButton = new JButton();
        loggedInText = new JTextField();
        disconnectButton = new JButton();
        tabbedPane = new JTabbedPane();
        roomTab = new JPanel();
        roomSplitPane = new JSplitPane();
        participantsScroll = new JScrollPane();
        participantsText = new JTextPane();
        roomPanel = new JPanel();
        roomConversationScroll = new JScrollPane();
        roomConversationText = new JTextPane();
        roomInput = new JPanel();
        roomTextField = new JTextField();
        roomSendButton = new JButton();
        privateTab = new JPanel();
        privateConversationScroll = new JScrollPane();
        privateConversationText = new JTextPane();
        privateInput = new JPanel();
        privateRecipientBox = new JComboBox();
        privateTextField = new JTextField();
        privateSendButton = new JButton();

        //======== clientLayout ========
        {
            clientLayout.setResizable(false);
            clientLayout.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
            clientLayout.setName("Chatroom");
            clientLayout.setVisible(true);
            clientLayout.setMinimumSize(new Dimension(500, 500));
            clientLayout.setTitle("Chatroom Client");
            Container clientLayoutContentPane = clientLayout.getContentPane();
            clientLayoutContentPane.setLayout(new CardLayout());

            //======== disconnectedLayout ========
            {
                disconnectedLayout.setPreferredSize(new Dimension(500, 500));

                // JFormDesigner evaluation mark
//                disconnectedLayout.setBorder(new javax.swing.border.CompoundBorder(
//                    new javax.swing.border.TitledBorder(new javax.swing.border.EmptyBorder(0, 0, 0, 0),
//                        "JFormDesigner Evaluation", javax.swing.border.TitledBorder.CENTER,
//                        javax.swing.border.TitledBorder.BOTTOM, new java.awt.Font("Dialog", java.awt.Font.BOLD, 12),
//                        java.awt.Color.red), disconnectedLayout.getBorder())); disconnectedLayout.addPropertyChangeListener(new java.beans.PropertyChangeListener(){public void propertyChange(java.beans.PropertyChangeEvent e){if("border".equals(e.getPropertyName()))throw new RuntimeException();}});

                disconnectedLayout.setLayout(null);

                //---- connectButton ----
                connectButton.setText("Log In");
                disconnectedLayout.add(connectButton);
                connectButton.setBounds(195, 290, 80, connectButton.getPreferredSize().height);
                disconnectedLayout.add(usernameTextField);
                usernameTextField.setBounds(130, 200, 200, 35);

                { // compute preferred size
                    Dimension preferredSize = new Dimension();
                    for (int i = 0; i < disconnectedLayout.getComponentCount(); i++) {
                        Rectangle bounds = disconnectedLayout.getComponent(i).getBounds();
                        preferredSize.width = Math.max(bounds.x + bounds.width, preferredSize.width);
                        preferredSize.height = Math.max(bounds.y + bounds.height, preferredSize.height);
                    }
                    Insets insets = disconnectedLayout.getInsets();
                    preferredSize.width += insets.right;
                    preferredSize.height += insets.bottom;
                    disconnectedLayout.setMinimumSize(preferredSize);
                    disconnectedLayout.setPreferredSize(preferredSize);
                }
            }
            clientLayoutContentPane.add(disconnectedLayout, "disconnected");

            //======== connectedLayout ========
            {
                connectedLayout.setLayout(new BorderLayout());

                //======== loggedInPanel ========
                {
                    loggedInPanel.setRequestFocusEnabled(false);
                    loggedInPanel.setLayout(new BorderLayout());

                    //---- changeUsernameButton ----
                    changeUsernameButton.setText("Change User");
                    changeUsernameButton.setMinimumSize(new Dimension(115, 27));
                    changeUsernameButton.setPreferredSize(new Dimension(115, 27));
                    changeUsernameButton.setRequestFocusEnabled(false);
                    changeUsernameButton.setNextFocusableComponent(roomTextField);
                    loggedInPanel.add(changeUsernameButton, BorderLayout.WEST);

                    //---- loggedInText ----
                    loggedInText.setColumns(20);
                    loggedInText.setEditable(false);
                    loggedInText.setComponentOrientation(ComponentOrientation.LEFT_TO_RIGHT);
                    loggedInText.setComponentPopupMenu(null);
                    loggedInText.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
                    loggedInText.setMinimumSize(new Dimension(100, 10));
                    loggedInText.setNextFocusableComponent(disconnectButton);
                    loggedInText.setPreferredSize(new Dimension(383, 10));
                    loggedInText.setVerifyInputWhenFocusTarget(false);
                    loggedInText.setMargin(new Insets(4, 20, 0, 0));
                    loggedInText.setBackground(new Color(232, 232, 232));
                    loggedInText.setBorder(null);
                    loggedInPanel.add(loggedInText, BorderLayout.CENTER);

                    //---- disconnectButton ----
                    disconnectButton.setText("Disconnect");
                    disconnectButton.setMinimumSize(new Dimension(115, 27));
                    disconnectButton.setPreferredSize(new Dimension(115, 27));
                    disconnectButton.setRequestFocusEnabled(false);
                    disconnectButton.setNextFocusableComponent(roomTextField);
                    loggedInPanel.add(disconnectButton, BorderLayout.EAST);
                }
                connectedLayout.add(loggedInPanel, BorderLayout.NORTH);

                //======== tabbedPane ========
                {
                    tabbedPane.setFocusable(false);

                    //======== roomTab ========
                    {
                        roomTab.setNextFocusableComponent(roomTextField);
                        roomTab.setLayout(new BorderLayout());

                        //======== roomSplitPane ========
                        {
                            roomSplitPane.setFocusable(false);
                            roomSplitPane.setBorder(null);

                            //======== participantsScroll ========
                            {
                                participantsScroll.setPreferredSize(new Dimension(100, 20));
                                participantsScroll.setFocusable(false);
                                participantsScroll.setMinimumSize(new Dimension(100, 23));

                                //---- participantsText ----
                                participantsText.setEditable(false);
                                participantsScroll.setViewportView(participantsText);
                            }
                            roomSplitPane.setLeftComponent(participantsScroll);

                            //======== roomPanel ========
                            {
                                roomPanel.setFocusable(false);
                                roomPanel.setPreferredSize(new Dimension(89, 49));
                                roomPanel.setLayout(new BorderLayout());

                                //======== roomConversationScroll ========
                                {
                                    roomConversationScroll.setFocusable(false);
                                    roomConversationScroll.setAutoscrolls(true);

                                    //---- roomConversationText ----
                                    roomConversationText.setEditable(false);
                                    roomConversationScroll.setViewportView(roomConversationText);
                                }
                                roomPanel.add(roomConversationScroll, BorderLayout.CENTER);

                                //======== roomInput ========
                                {
                                    roomInput.setFocusable(false);
                                    roomInput.setLayout(new BorderLayout());

                                    //---- roomTextField ----
                                    roomTextField.setNextFocusableComponent(roomSendButton);
                                    roomInput.add(roomTextField, BorderLayout.CENTER);

                                    //---- roomSendButton ----
                                    roomSendButton.setText("Send");
                                    roomInput.add(roomSendButton, BorderLayout.EAST);
                                }
                                roomPanel.add(roomInput, BorderLayout.PAGE_END);
                            }
                            roomSplitPane.setRightComponent(roomPanel);
                        }
                        roomTab.add(roomSplitPane, BorderLayout.CENTER);
                    }
                    tabbedPane.addTab("Room", roomTab);

                    //======== privateTab ========
                    {
                        privateTab.setLayout(new BorderLayout());

                        //======== privateConversationScroll ========
                        {
                            privateConversationScroll.setFocusable(false);
                            privateConversationScroll.setAutoscrolls(true);

                            //---- privateConversationText ----
                            privateConversationText.setEditable(false);
                            privateConversationScroll.setViewportView(privateConversationText);
                        }
                        privateTab.add(privateConversationScroll, BorderLayout.CENTER);

                        //======== privateInput ========
                        {
                            privateInput.setLayout(new BorderLayout());

                            //---- privateRecipientBox ----
                            privateRecipientBox.setPreferredSize(new Dimension(100, 27));
                            privateRecipientBox.setMinimumSize(new Dimension(100, 27));
                            privateInput.add(privateRecipientBox, BorderLayout.WEST);
                            privateInput.add(privateTextField, BorderLayout.CENTER);

                            //---- privateSendButton ----
                            privateSendButton.setText("Send");
                            privateSendButton.setEnabled(false);
                            privateInput.add(privateSendButton, BorderLayout.EAST);
                        }
                        privateTab.add(privateInput, BorderLayout.SOUTH);
                    }
                    tabbedPane.addTab("Private", privateTab);
                }
                connectedLayout.add(tabbedPane, BorderLayout.CENTER);
            }
            clientLayoutContentPane.add(connectedLayout, "connected");
            clientLayout.setSize(500, 500);
            clientLayout.setLocationRelativeTo(null);
        }
        // JFormDesigner - End of component initialization  //GEN-END:initComponents
    }

    // JFormDesigner - Variables declaration - DO NOT MODIFY  //GEN-BEGIN:variables
    // Generated using JFormDesigner Evaluation license - stefan niculae
    protected JFrame clientLayout;
    private JPanel disconnectedLayout;
    protected JButton connectButton;
    protected JTextField usernameTextField;
    private JPanel connectedLayout;
    private JPanel loggedInPanel;
    protected JButton changeUsernameButton;
    protected JTextField loggedInText;
    protected JButton disconnectButton;
    private JTabbedPane tabbedPane;
    private JPanel roomTab;
    private JSplitPane roomSplitPane;
    private JScrollPane participantsScroll;
    protected JTextPane participantsText;
    private JPanel roomPanel;
    private JScrollPane roomConversationScroll;
    protected JTextPane roomConversationText;
    private JPanel roomInput;
    protected JTextField roomTextField;
    protected JButton roomSendButton;
    private JPanel privateTab;
    private JScrollPane privateConversationScroll;
    protected JTextPane privateConversationText;
    private JPanel privateInput;
    protected JComboBox privateRecipientBox;
    protected JTextField privateTextField;
    protected JButton privateSendButton;
    // JFormDesigner - End of variables declaration  //GEN-END:variables
}
