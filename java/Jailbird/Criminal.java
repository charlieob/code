/*
 * Criminal.java
 *
 * Created on October 4, 2000, 2:30 PM
 */

package Jailbird;

/**
 *
 * @author  Administrator
 * @version 
 */
public abstract class Criminal extends Object implements Identifiable {
    
    protected String name;

    /** Creates new Criminal */
    public Criminal() {
    }
    
    public abstract void inputDetails();
    public abstract void printDetails();

    public void showName() {
        System.out.println(name);
    }
}