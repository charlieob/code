/*
 * Jail.java
 *
 * Created on October 3, 2000, 4:17 PM
 */

package Jailbird;

/**
 *
 * @author  charlie
 * @version 0.1
 */

import java.util.*;

public class Jail extends Object {
    
    //public final static int SIZE = 10;

    //private Inmate[] inmateList;
    //private int count;
    
    Vector inmateList;
    Vector warderList;
 
    /** Creates new Jail */
    public Jail() {
        //inmateList = new Inmate[SIZE];
        //count = 0;
        
        inmateList = new Vector (3, 2);
        warderList = new Vector (3, 2);
    }
    
    public void addInmate() {
        Criminal inmate = new Inmate();
        inmate.inputDetails();
        //inmateList[count++] = inmate;
        //inmateList.add(inmate);
        inmateList.addElement(inmate);   // required for V1.1 ??
    }
    
    public void addSecureInmate() {
        SecureInmate inmate = new SecureInmate();
        inmate.inputDetails();
        //inmateList[count++] = inmate;
        inmateList.add(inmate);
    }
    
    public void addRemander() {
        Remander remander = new Remander();
        remander.inputDetails();
        inmateList.add(remander);
    }
    
    public void addWarder(Warder warder) {
        warderList.add(warder);
    }
    
    public void addChiefWarder(ChiefWarder cw) {
        warderList.add(cw);
    }
    
    public void printList() {
        //for (int i=0; i<count; i++)
        //     inmateList[i].printDetails();
        
        Iterator it = inmateList.iterator();
        while (it.hasNext())
            ((Criminal)it.next()).printDetails();
    }
    
    public void showNames() {
        Identifiable id;
        Iterator il = inmateList.iterator();
        while (il.hasNext()) {
            id = (Identifiable) il.next();
            id.showName();
        }
        
        Iterator wl = warderList.iterator();
        while (wl.hasNext()) {
            id = (Identifiable) wl.next();
            id.showName();
        }
    }

}
