/*
 * VisualJail.java
 *
 * Created on October 5, 2000, 3:31 PM
 */

package VisualJail;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;

/**
 *
 * @author  Administrator
 * @version 
 */
public class VisualJail extends Object {

    /** Creates new VisualJail */
    public VisualJail() {
    }
        
    public static void main (String[] args) {
        new JailFrame();
    }
}

class JailFrame extends JFrame {
    private JTextArea ta;
    private JTextField tf;
    private JButton button;
    
    public JailFrame() {
        setTitle("Visual Jail");
        
        ta = new JTextArea(10,30);
        getContentPane().add(ta, BorderLayout.CENTER);
        
        tf = new JTextField(20);
        
        button = new JButton("Enter");
        button.addActionListener(new EnterListener());
        
        JPanel bottomPanel = new JPanel();
        bottomPanel.add(tf);
        bottomPanel.add(button);
        
        getContentPane().add(bottomPanel, BorderLayout.SOUTH);
        
        pack();
        setVisible(true);
    }
    
    class EnterListener implements ActionListener {
        public void actionPerformed(ActionEvent ae) {
            ta.append(tf.getText() + '\n');
            tf.setText("");
        }
    }
}