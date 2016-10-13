/*
 * SecureInmate.java
 *
 * Created on October 4, 2000, 11:19 AM
 */

package Jailbird;

/**
 *
 * @author  Administrator
 * @version 
 */
public class SecureInmate extends Inmate {
    
    private int securityLevel;

    /** Creates new SecureInmate */
    public SecureInmate() {
    }
    
    public void inputDetails() {
        
        super.inputDetails();
        
        System.out.print ("Security Level: ");
        securityLevel = ReadWrite.readInt();
        // System.out.println (number);
    }
    
    public void printDetails() {
        
        super.printDetails();
        
        System.out.println("(security level " + securityLevel + ')');
    }
    

}