package com.zairon.GameControllerEngine;

import java.util.ArrayList;

/**
 * This contains the natives for interacting with a game controller connected to the system.
 * @author Michael Watkins
 *
 */
public class GameControllerNatives
{    
    /**
     * The name to use for the native library
     */
    public static final String NATIVE_LIBRARY_NAME="GameControllerNatives";
    
    /**
     * The string for windows
     */
    public static final String WINDOWS_OS_STRING="Windows";
    
    /**
     * String that contains the operating system
     */
    public static final String OPERATING_SYSTEM=System.getProperty("os.name");
    
    /**
     * Determine if we are running under windows
     */
    protected static boolean windows = OPERATING_SYSTEM.contains(WINDOWS_OS_STRING);
    
    /**
     * Return whether the operating system is windows
     * @return true if Windows, false otherwise.
     */
    public static boolean isWindows()
    {
        return windows;
    }
    
    /**
     * Return whether game controllers are supported by the system.
     * @return true if game controllers are supported, false otherwise.
     */
    public static boolean isSupported()
    {
        // only support under Windows for now
        return isWindows();
    }
    
    /**
     * Get a string for the operating system
     * @return The operating system string
     */
    public static String getOsString()
    {
        if(isWindows())
        {
            return WINDOWS_OS_STRING;
        }
        else
        {
            // don't put anything, who knows
            return "";
        }
    }
    
    /**
     * Get the number of controllers supported by the system. This is not
     * the number of controllers connected.
     * @return The number of controllers supported by the system.
     */
    public static native int getNumControllers();
    
    /**
     * Get the name of the controller given
     * @param id The id of the controller to get the name for
     * @return The string with the name for the controller
     */
    public static native boolean isConnected(int id);
    
    /**
     * Get the name of the controller given
     * @param id The id of the controller to get the name for
     * @return The string with the name for the controller
     */
    public static native String getControllerName(int id);
    
    /**
     * Get a list with the id's of the connected controllers
     * @return An array list with the connected controllers.
     */
    public static ArrayList<Integer> getConnectedControllers()
    {
        ArrayList<Integer> list=new ArrayList<Integer>();
        
        int numControllers=getNumControllers();
        
        for(int i=0 ; i < numControllers ; ++i)
        {
            if(isConnected(i))
            {
                list.add(new Integer(i));
            }
        }
        
        return list;        
    }
    
    /**
     * Get the number of buttons.
     * @param id The id of the controller to get the number of buttons for
     * @return The number of buttons on the controller
     */
    public static native int getNumButtons(int id);
    
    /**
     * Get the capabilities for the controller
     * @param id The id of the controller to get the capabilities for
     * @return The capabilities for the controller.
     */
    public static native GameControllerCaps getControllerCaps(int id);
    
    /**
     * Get the controller state.
     * @param id The id of the controller to get the name for
     * @param state The object that will contain the controller state
     * @return true if the controller is connected, false if the controller is not connected.
     */
    public static native boolean getControllerState(int id, GameControllerState state);
    
    static
    {
        // load the library exactly once
        String arch=System.getProperty("sun.arch.data.model");
        String os=getOsString();
        String libName=os + NATIVE_LIBRARY_NAME + arch;
        System.out.println("Loading library " + libName +" for " + arch + " bit architecture");
        System.loadLibrary(libName);
        
    }

}
