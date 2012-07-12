package com.zairon.GameControllerEngine;

/**
 * This contains the state of the controller. Used with GameControllerNatives.getControllerState.
 * @author Michael Watkins
 *
 */
public class GameControllerState
{
    /**
     * The maximum number of buttons supported
     */
    public static final int MAX_BUTTONS=32;
    
    /**
     * Indicates whether the game controller is connected.
     */
    protected boolean connected;

    /**
     * The value for the x-axis of the controller
     */
    private float x;
    
    /**
     * The value for the y-axis of the controller
     */
    private float y;
    
    /**
     * The value for the z-axis of the controller
     */
    private float z;
    
    /**
     * The value for the r-axis of the controller
     */
    private float r;
    
    /**
     * The value for the u-axis of the controller
     */
    private float u;
    
    /**
     * The value for the v-axis of the controller
     */
    private float v;

    /**
     * The position for the point-of-view hat
     */
    private float pov;
    
    /**
     * The set flags for the buttons
     */
    private int buttonMap;
    
    /**
     * The array of buttons and whether they are pressed
     */
    private boolean buttons[]=new boolean[MAX_BUTTONS];
    
    /**
     * Return whether the controller is connected. If not connected, the capabilities
     * are questionable and the controller object needs to be reconnected.
     * @return true if connected, false otherwise
     */
    public boolean isConnected()
    {
        return connected;
    }
    
    /**
     * Get the value for the x-axis, ranging from -1.0 to 1.0
     * @return the x-axis value
     */
    public float getX()
    {
        return x;
    }

    /**
     * Get the value for the y-axis, ranging from -1.0 to 1.0
     * @return the y-axis value
     */
    public float getY()
    {
        return y;
    }

    /**
     * Get the value for the z-axis, ranging from -1.0 to 1.0
     * @return the z-axis value
     */
    public float getZ()
    {
        return z;
    }

    /**
     * Get the value for the r-axis, ranging from -1.0 to 1.0
     * @return the r-axis value
     */
    public float getR()
    {
        return r;
    }

    /**
     * Get the value for the u-axis, ranging from -1.0 to 1.0
     * @return the u-axis value
     */
    public float getU()
    {
        return u;
    }

    /**
     * Get the value for the v-axis, ranging from -1.0 to 1.0
     * @return the v-axis value
     */
    public float getV()
    {
        return v;
    }

    /**
     * Get the value for the point-of-view hat in degrees
     * @return the point-of-view hat position value
     */
    public float getPOV()
    {
        return pov;
    }

    /**
     * Get the button map
     * @return The button map
     */
    public int getButtonMap()
    {
        return buttonMap;
    }
    
    /**
     * Return true if a particular button is pressed
     * @param button The number for the button from 0 to MAX_BUTTONS
     * @return true if the button is pressed, false otherwise.
     */
    public boolean isPressed(int button)
    {
        return this.buttons[button];
    }
    
}
