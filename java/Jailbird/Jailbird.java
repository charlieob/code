/*
 * Jailbird.java
 *
 * Created on October 3, 2000, 2:42 PM
 */

package Jailbird;

/**
 *
 * @author  charlie
 * @version 0.1
 */

public class Jailbird extends Object {

    /** Creates new Jailbird */
    public Jailbird() {
    }

    /**
    * @param args the command line arguments
    */
    public static void main (String args[]) {
        Jail myJail = new Jail();
        char option;
        String name, rank;
        int grade;
        
        do {
            System.out.print ("Choose option (i, s, r, w, c, p, n or q): ");
            option = Character.toLowerCase(ReadWrite.readChar());
            // System.out.println (option);
            
            switch (option) {
                case 'i':
                    myJail.addInmate();
                    break;
                case 's':
                    myJail.addSecureInmate();
                    break;
                case 'r':
                    myJail.addRemander();
                    break;
                case 'w':
                    System.out.print ("Warder name: ");
                    name = ReadWrite.readString();
                    System.out.print ("Warder grade: ");
                    grade = ReadWrite.readInt();
                    Warder warder = new Warder (name, grade);
                    myJail.addWarder(warder);
                    break;
                case 'c':
                    System.out.print ("Chief warder name: ");
                    name = ReadWrite.readString();
                    System.out.print ("Chief warder grade: ");
                    grade = ReadWrite.readInt();
                    System.out.print ("Chief warder rank: ");
                    rank = ReadWrite.readString();
                    ChiefWarder chief = new ChiefWarder (name, grade, rank);
                    myJail.addChiefWarder(chief);
                    break;
                case 'p':
                    myJail.printList();
                    break;
                case 'n':
                    myJail.showNames();
                    break;
                case 'q':
                    System.out.println ("Goodbye.");
                    break;
                default:
                    System.out.println ("Unknown option \'" + option + '\'');
                    break;
            }
            
        } while (option != 'q');
    }

}