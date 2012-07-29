package com.zairon.GameControllerEngine;

/**
 * The game controller interface.
 * @author Michael Watkins
 *
 */
public class XBoxGameController extends GameController
{
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
            caps=GameControllerNatives.getXInputControllerCaps(getId());
            
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
        if(!GameControllerNatives.getXInputControllerState(getId(), getState()))
        {
            return false;
        }
        
        // state is updated, set the axis values for dead zone
        xyPoint.setPos(state.getX(), state.getY());
        zAxis.setP(state.getZ());
        leftTriggerAxis.setP(state.getLeftTrigger());
        rightTriggerAxis.setP(state.getRightTrigger());
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
     * Determine if the left trigger is pressed. 
     * XBox controllers can use XInput and have separate axes for left and right trigger,
     * so override this to be able to separate triggers.
     * @return true if left trigger is pressed, false otherwise
     */
    public boolean isLeftTriggerPressed()
    {
        return (leftTriggerAxis.getP() > GameControllerAxis.NEUTRAL);
    }
    
    /**
     * Determine if the right trigger is pressed.
     * XBox controllers can use XInput and have separate axes for left and right trigger,
     * so override this to be able to separate triggers.
     * @return true if right trigger is pressed, false otherwise
     */
    public boolean isRightTriggerPressed()
    {
        return (rightTriggerAxis.getP() > GameControllerAxis.NEUTRAL);
    }

}
