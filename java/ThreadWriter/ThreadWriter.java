/*
 * ThreadWriter.java
 *
 * Created on October 5, 2000, 10:39 AM
 */

package ThreadWriter;

/**
 *
 * @author  Administrator
 * @version 
 */
public class ThreadWriter extends Object implements Runnable {
    
    boolean shouldStop;
    boolean shouldPause;

    /** Creates new ThreadWriter */
    public ThreadWriter() {
    }

    public void run() {
        shouldStop = false;
        shouldPause = false;
        
        char ch = (char) ('A' + (int)(Math.random()*26));
        
        System.out.println("thread(" + ch + ") starting");
        
        while (!shouldStop) {
            synchronized (this)
            {
            if (shouldPause)
            {
                try {
                    System.out.println("thread(" + ch + ") pausing");
                    this.wait();
                    System.out.println("thread(" + ch + ") resuming");
                }
                catch (InterruptedException ie)
                {
                }
            }
            }
            
            System.out.println(ch);
            
            try {
                Thread.sleep((int)(Math.random()*2000));
            }
            catch (InterruptedException e) {
            }
        }
        System.out.println("thread(" + ch + ") terminating");
    }
    
    public void go() {
        boolean finished = false;
        
        Thread threadA = new Thread(this);
        Thread threadB = new Thread(this);
        Thread threadC = new Thread(this);
        
        threadA.start();
        threadB.start();
        threadC.start();
        
        while (!finished)
        {
            char ch = ReadWrite.readChar();
        
            switch (ch) {
            case 'p':
                shouldPause = true;
                break;
            case 'r':
                shouldPause = false;
                synchronized(this)
                {
                    this.notify();
                }
                break;
            case 'a':
                shouldPause = false;
                synchronized(this)
                {
                    this.notifyAll();
                }
                break;
            case 'q':
                //System.out.println("**** quitting");
                shouldStop = true;
                finished = true;
                //System.exit(0);
                break;
            case 's':
                //System.out.println("**** stopping all threads");
                shouldStop = true;
                break;
            }
        }
        System.out.println("stopping application");
    }
    
    public static void main(String[] args) {
        //System.out.println("starting main");
        ThreadWriter tw = new ThreadWriter();
        //System.out.println("threadwriter created");
        tw.go();
        //System.out.println("gone");
    }
}