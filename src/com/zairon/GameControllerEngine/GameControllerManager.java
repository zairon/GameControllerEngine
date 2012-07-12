package com.zairon.GameControllerEngine;

/**
 * Factory that creates controller instances.
 * @author Michael Watkins
 *
 */
public abstract class GameControllerManager
{
    /**
     * Flag indicating if any controllers are supported by the platform.
     * @return true if game controllers are supported, false otherwise.
     */
    public static boolean isSupported()
    {
        return GameControllerNatives.isSupported();
    }
    
    /**
     * Create a controller instance.
     * @param id The identifier for the control to create.
     * @return The game controller instance or null if not connected
     */
    public static GameController createController(int id)
    {
        // use the operating system to figure out which controller to make
        if(GameControllerNatives.isWindows())
        {
            return createWindowsController(id);
        }
        else
        {
            // not supported, return the null connection instance
            return createNullController(id);
        }
    }
    
    /**
     * Create a windows game controller
     * @param id The identifier for the control to create
     * @return The game controller instance
     */
    protected static GameController createWindowsController(int id)
    {
        final int maxId=GameControllerNatives.getNumControllers();
        
        // check the id limit
        if(id < 0)
        {
            throw new IllegalArgumentException("Cannot create id " + id +", value is illegal\n");
        }
        else if(id > maxId)
        {
            throw new IllegalArgumentException("Cannot create id " + id +", only supporting up to " + maxId + " controller IDs\n");
        }
        
        WindowsGameController controller=new WindowsGameController();
        
        // set up the instance
        controller.id=id;
        controller.caps=GameControllerNatives.getControllerCaps(id);
        controller.state=new GameControllerState();
        
        // do an initial poll
        controller.poll();
        
        return controller;
    }
    
    /**
     * Create a null game controller
     * @param id The identifier for the control to create
     * @return The game controller instance
     */
    protected static GameController createNullController(int id)
    {
        NullGameController controller=new NullGameController();
        
        // set up the instance
        controller.id=id;
        controller.caps=new GameControllerCaps();
        
        // force the name
        controller.caps.name=String.format("NullController%d", id);
        
        controller.state=new GameControllerState();
        // the null controller is always connected
        controller.state.connected=true;
        
        return controller;
    }
    
    
}
