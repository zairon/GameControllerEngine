package com.zairon.GameControllerEngine;

/**
 * A game controller with no actions.
 * @author Michael Watkins
 *
 */
public class NullGameController extends GameController
{
    /**
     * Protect the controller from instantiation (make it come from the factory).
     */
    public NullGameController()
    {
    }
    
    @Override
    public boolean poll()
    {
        // polling is no-action for the null controller
        return state.isConnected();
    }

    @Override
    public boolean reconnect()
    {
        // reconnect is a no-action for the null controller
        return state.isConnected();
    }

    @Override
    public boolean isConnected()
    {
        return state.isConnected();
    }

}
