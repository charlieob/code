package ThreadWriter;

import java.io.*;

// Totally trivial class to provide some basic console input functions.
// Written simply to allow numeric input without having to discuss
// exceptions first.
// Steve Jones (stevej@tgn.co.uk)
// 3rd Revision, August 2000

class ReadWrite
{
  private static BufferedReader m_rdr = new BufferedReader( new InputStreamReader( System.in ) ); 
  
  // readString - called directly when a String is required, and also used by the
  // other methods
  public static String readString()
  {
    try
    {
      return m_rdr.readLine();
    }
    catch( IOException e )
    {
      System.out.println( "Sorry, I didn't like that string" );
      return "";
    }
  }
  public static int readInt()
  {
    String strNum = readString();
    try
    {
      return Integer.parseInt( strNum );
    }
    catch( Exception e )
    {
      System.out.println( e.toString() );
      return 0;
    }
  }
  
  public static double readDouble()
  {
    String strNum = readString();
    try
    {
      return Double.valueOf( strNum ).doubleValue();
    }
    catch( Exception e )
    {
      System.out.println( e.toString() );
      return 0;
    }
  }
  
  // readChar actually requests a string and returns the first character of it
  
  public static char readChar()
  {
    String strChar = readString();
    try
    {
      if ( strChar.length() > 0 )
        return strChar.charAt( 0 );
      else
        return 0;
    }
    catch( Exception e )
    {
      System.out.println( e.toString() );
      return 0;
    }
  }
}