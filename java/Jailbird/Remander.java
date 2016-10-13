/*
 * Remander.java
 *
 * Created on October 4, 2000, 2:37 PM
 */

package Jailbird;

/**
 *
 * @author  Administrator
 * @version 
 */
public class Remander extends Criminal {
    
    private int remandTime;

    /** Creates new Remander */
    public Remander() {
    }
    
    public void inputDetails() {
        
        System.out.print ("Remander name: ");
        name = ReadWrite.readString();
        // System.out.println (name);
        
        System.out.print ("Remand time: ");
        remandTime = ReadWrite.readInt();
    }
    
    public void printDetails() {
        System.out.println ("Remander " + name + ": " + remandTime + " days");
    }


}