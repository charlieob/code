/*
 * ChiefWarder.java
 *
 * Created on October 4, 2000, 3:30 PM
 */

package Jailbird;

/**
 *
 * @author  Administrator
 * @version 
 */
public class ChiefWarder extends Warder {
    
    private String rank;

    /** Creates new ChiefWarder */
    public ChiefWarder(String n, int g, String r) {
        super(n, g);
        rank = r;
    }

}