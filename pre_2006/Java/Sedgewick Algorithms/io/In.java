import java.io.*;

public class In
{
    private static int c;
    private static boolean blank()
    {
	return Character.isWhitespace((char) c);
    }
    private static void readC()
    {
	try
	    {
		c = System.in.read();
	    }
	catch (IOException e)
	    {
		c = -1;
	    }
    }
    public static void init()
    {
	readC();
    }
    public static boolean empty()
    {
	return c == -1;
    }
    public static String getString()
    {
	String s = "";
	while (!(empty() || blank()))
	    {
		s += (char) c; readC(); 
	    }
	while (!empty() && blank())
	    readC();

    return s;
    }
    public static int getInt()
    {
	return Integer.parseInt(getString());
    }
    public static double getDouble()
    {
	return Double.parseDouble(getString());
    }
}


