package com.zairon.GameControllerEngine;

public abstract class GameController
{
    /**
     * The default dead zone to use for controller points.
     */
    public static final float DEFAULT_DEAD_ZONE=0.25f;

    /**
     * The maximum POV hat value
     */
    public static final float MAX_POV_VALUE=360.0f;
    
    /**
     * The default radius to determine the POV radius
     */
    public static final float DEFAULT_POV_RADIUS=1.0f;
    
    /**
     * The id for the controller
     */
    protected int id;

    /**
     * The capabilities for the controller
     */
    protected GameControllerCaps caps;
    
    /**
     * The state for the controller
     */
    protected GameControllerState state;
    
    /**
     * The game controller x/y point
     */
    protected GameControllerPoint xyPoint=new GameControllerPoint(DEFAULT_DEAD_ZONE);

    /**
     * The game controller z axis
     */
    protected GameControllerAxis zAxis=new GameControllerAxis(DEFAULT_DEAD_ZONE);

    /**
     * The game controller left trigger axis, only ranges between 0.0 and 1.0.
     */
    protected GameControllerAxis leftTriggerAxis=new GameControllerAxis(DEFAULT_DEAD_ZONE);

    /**
     * The game controller right trigger axis, only ranges between 0.0 and 1.0.
     */
    protected GameControllerAxis rightTriggerAxis=new GameControllerAxis(DEFAULT_DEAD_ZONE);

    /**
     * The game controller r/u point. The r axis is up/down and u is left/right.
     */
    protected GameControllerPoint ruPoint=new GameControllerPoint(DEFAULT_DEAD_ZONE);

    /**
     * The game controller v axis
     */
    protected GameControllerAxis vAxis=new GameControllerAxis(DEFAULT_DEAD_ZONE);

    /**
     * The game controller POV hat point
     */
    protected GameControllerPoint povPoint=new GameControllerPoint(0.0f);

    /**
     * Return whether the controller is connected
     * @return true if connected, false otherwise
     */
    public boolean isConnected()
    {
        return state.isConnected();
    }
    
    /**
     * Try to reconnect the controller if disconnected. This will refresh the capabilities
     * after doing poll to check for connection.
     * @param true if the reconnect was successful, false if the controller is still disconnected.
     */
    public abstract boolean reconnect();    
    /**
     * Poll the game controller, refreshing the state of the controller. This will
     * also update whether the controller is connected.  Note that polling can return connected
     * without refreshing capabilities, so care must be taken when determining if connected.
     * @return true if the controller is connected, false otherwise.
     */
    public abstract boolean poll();

    /**
     * Get the id for the controller
     * @return The ID for the controller
     */
    public int getId()
    {
        return id;
    }

    /**
     * Get the capabilities for the game controller
     * @return The capabilities for the game controller
     */
    public GameControllerCaps getCaps()
    {
        return caps;
    }
    
    /**
     * Get the name for the controller
     * @return The name for the controller
     */
    public String getName()
    {
        return caps.getName();
    }
    
    /**
     * Get the number of buttons for the controller
     * @return The number of the buttons for the controller
     */
    public int getNumButtons()
    {
        return caps.getNumButtons();
    }
    
    /**
     * Get the state for the controller. This needs to be refreshed will a call to poll.
     * @return The state of the controller
     */
    public GameControllerState getState()
    {
        return state;
    }
    
    /**
     * Get the x/y point, corrected for dead zone.
     * @return The x/y point
     */
    public GameControllerPoint getXY()
    {
        return xyPoint;
    }

    /**
     * Get the z axis value, corrected for dead zone.
     * @return The z axis value
     */
    public GameControllerAxis getZ()
    {
        return zAxis;
    }

    /**
     * Get the left trigger axis value, corrected for dead zone. Only between 0.0f and 1.0f
     * @return The left trigger axis value
     */
    public GameControllerAxis getLeftTrigger()
    {
        return leftTriggerAxis;
    }

    /**
     * Get the right trigger axis value, corrected for dead zone. Only between 0.0f and 1.0f
     * @return The right trigger axis value
     */
    public GameControllerAxis getRightTrigger()
    {
        return rightTriggerAxis;
    }

    /**
     * Get the r/u point, corrected for dead zone. Note that R is up/down and U is left/right.
     * @return The r/u point
     */
    public GameControllerPoint getRU()
    {
        return ruPoint;
    }

    /**
     * Get the v axis value, corrected for dead zone.
     * @return The v axis value
     */
    public GameControllerAxis getV()
    {
        return vAxis;
    }

    /**
     * Get the value of the POV hat
     * @return The POV hat point
     */
    public GameControllerPoint getPOV()
    {
        return povPoint;
    }
    
    /**
     * Return whether the POV hat is pressed or neutral.
     * @return true if the POV is being pressed, false if the POV is neutral
     */
    public boolean isPOVPressed()
    {
        // the POV is in neutral if the position reported is greater than or equal to 360.0f
        return (state.getPOV() < 360.0f);
    }

    /**
     * Determine if the left trigger is pressed. Override this to use other trigger criteria.
     * @return true if left trigger is pressed, false otherwise
     */
    public boolean isLeftTriggerPressed()
    {
        // if the z-axis is greater than the neutral axis position, then the left trigger is pressed
        // since the axis considers the dead zone, it will only trigger when the axis is past the dead zone.
        return (zAxis.getP() > GameControllerAxis.NEUTRAL);
    }
    
    /**
     * Determine if the right trigger is pressed. Override this to use other trigger criteria.
     * @return true if right trigger is pressed, false otherwise
     */
    public boolean isRightTriggerPressed()
    {
        // if the z-axis is less than the neutral axis position, then the right trigger is pressed
        // since the axis considers the dead zone, it will only trigger when the axis is past the dead zone.
        return (zAxis.getP() < GameControllerAxis.NEUTRAL);
    }
    
    /**
     * For controllers with triggers, this compares the z axis against a dead zone
     * @return
     */
    public boolean isTriggerPressed(GameControllerButtons button)
    {
        if(button==GameControllerButtons.LEFT_TRIGGER)
        {
            return isLeftTriggerPressed();
        }
        else if(button==GameControllerButtons.RIGHT_TRIGGER)
        {
            return isRightTriggerPressed();
        }
        else
        {
            // regular button, not a trigger
            return false;
        }
    }
    
    /**
     * Return true if a particular button is pressed
     * @param button The number for the button from 0 to MAX_BUTTONS
     * @return true if the button is pressed, false otherwise.
     */
    public boolean isPressed(int button)
    {
        // handle the special cases of triggers
        if(GameControllerButtons.LEFT_TRIGGER.is(button))
        {
            // if the z-axis is greater than maximum the z-deflection, then the left trigger is pressed
            return isLeftTriggerPressed();
        }
        else if(GameControllerButtons.RIGHT_TRIGGER.is(button))
        {
            // if the z-axis is less than the minimum z-deflection, then the right trigger is pressed
            return isRightTriggerPressed();
        }
        else
        {
            // kind of bad practice, but access the array directly
            return state.buttons[button];
        }
        
    }
        
    /**
     * Return true if a particular button is pressed
     * @param button The GameControllerButtons enumeration value
     * @return true if the button is pressed, false otherwise.
     */
    public boolean isPressed(GameControllerButtons button)
    {        
        return isPressed(button.ordinal());
    }

}
