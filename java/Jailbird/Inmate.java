/*
 * Inmate.java
 *
 * Created on October 3, 2000, 2:46 PM
 */

package Jailbird;

/**
 *
 * @author  charlie
 * @version 0.1
 */
public class Inmate extends Criminal {
    
    private int number;
    //private String name;

    /** Creates new Inmate */
    public Inmate() {
    }
    
    public void inputDetails() {
        System.out.print ("Inmate number: ");
        number = ReadWrite.readInt();
        // System.out.println (number);
        
        System.out.print ("Inmate name: ");
        name = ReadWrite.readString();
        // System.out.println (name);
    }
    
    public void printDetails() {
        System.out.println ("Inmate " + number + ": " + name);
    }

}