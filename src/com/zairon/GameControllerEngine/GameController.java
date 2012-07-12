package com.zairon.GameControllerEngine;

/**
 * The game controller interface.
 * @author Michael Watkins
 *
 */
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
     * The game controller r/u point
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
    public boolean reconnect()
    {
        // poll for reconnection
        if(!poll())
        {
            return false;
        }
        else
        {
            // refresh the capabilities
            caps=GameControllerNatives.getControllerCaps(getId());
            
            return true;
        }

    }
    
    /**
     * Poll the game controller, refreshing the state of the controller. This will
     * also update whether the controller is connected.  Note that polling can return connected
     * without refreshing capabilities, so care must be taken when determining if connected.
     * @return true if the controller is connected, false otherwise.
     */
    public boolean poll()
    {
        // update the state
        if(!GameControllerNatives.getControllerState(getId(), getState()))
        {
            return false;
        }
        
        // state is updated, set the axis values for dead zone
        xyPoint.setPos(state.getX(), state.getY());
        zAxis.setP(state.getZ());
        ruPoint.setPos(state.getR(), state.getU());
        vAxis.setP(state.getV());
        
        // if the POV is neutral, use 0,0, otherwise figure out
        if(isPOVPressed())
        {
            povPoint.setAngle(state.getPOV(), DEFAULT_POV_RADIUS);    
        }
        else
        {
            povPoint.setPos(0.0f, 0.0f);
        }
        
        // return that the controller is connected
        return true;
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
     * Get the r/u point, corrected for dead zone.
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
}
