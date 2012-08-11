package com.zairon.GameControllerEngine;

/**
 * A windows game controller.
 * @author Michael Watkins
 *
 */
public class WindowsGameController extends XBoxGameController
{
    /**
     * Protect the controller from instantiation (make it come from the factory).
     */
    protected WindowsGameController()
    {
        super();
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
        leftTriggerAxis.setP(state.getLeftTrigger());
        rightTriggerAxis.setP(state.getRightTrigger());        
        // R axis is up/down axis, U is left/right axis
        // so flip them for the axis position
        ruPoint.setPos(state.getU(), state.getR());
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

}
