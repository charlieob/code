/*
 * Warder.java
 *
 * Created on October 4, 2000, 2:53 PM
 */

package Jailbird;

/**
 *
 * @author  Administrator
 * @version 
 */
public class Warder extends Object implements Identifiable {
    
    protected String name;
    protected int grade;

    /** Creates new Warder */
    public Warder(String n, int g) {
        name = n;
        grade = g;
    }

    public void showName() {
        System.out.println(name);
    }
}