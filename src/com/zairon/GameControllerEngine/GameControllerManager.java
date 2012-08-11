package com.zairon.GameControllerEngine;

import java.util.ArrayList;

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
     * Create a generic windows game controller
     * @param id The identifier for the control to create
     * @return The game controller instance
     */
    public static GameController createWindowsControllerOnly(int id)
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
        
        GameController controller;
        
        // make a generic controller
        controller=new WindowsGameController();
        
        // set up the instance
        controller.id=id;
        controller.caps=GameControllerNatives.getControllerCaps(id);
        controller.state=new GameControllerState();
        
        // do an initial poll
        controller.poll();
        
        return controller;
    }
    
    /**
     * Create an XBox-compatible controller if possible, otherwise a generic windows game controller
     * @param id The identifier for the control to create
     * @return The game controller instance
     */
    public static GameController createWindowsController(int id)
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
        
        GameController controller;
        
        // see if the controller is an XBox controller
        if(GameControllerNatives.isXInputController(id))
        {
            // make an XBox controller
            controller=new XBoxGameController();

            // set up the instance
            controller.id=id;
            controller.caps=GameControllerNatives.getXInputControllerCaps(id);
            controller.state=new GameControllerState();
        }
        else
        {
            // make a generic controller
            controller=new WindowsGameController();
            
            // set up the instance
            controller.id=id;
            controller.caps=GameControllerNatives.getControllerCaps(id);
            controller.state=new GameControllerState();
        }
        
        // do an initial poll
        controller.poll();
        
        return controller;
    }
    
    /**
     * Create a list of all of the connected Windows controllers (some may be XBox, some generic).
     * @return The list of all Windows controllers
     */
    public static ArrayList<GameController> getAllWindowsControllers()
    {
        ArrayList<GameController> list=new ArrayList<GameController>();
        
        for(int id=0; id < GameControllerNatives.getNumControllers() ; ++id)
        {
            GameController controller=createWindowsController(id);
            
            if(controller!=null)
            {
                list.add(controller);
            }
        }
        
        return list;
    }
    
    /**
     * Create a list of the connected Windows controllers, but all will be generic.
     * @return The list of all Windows controllers
     */
    public static ArrayList<GameController> getWindowsControllersOnly()
    {
        ArrayList<GameController> list=new ArrayList<GameController>();
        
        for(int id=0; id < GameControllerNatives.getNumControllers() ; ++id)
        {
            GameController controller=createWindowsControllerOnly(id);
            
            if(controller!=null)
            {
                list.add(controller);
            }
        }
        
        return list;
    }
    
    
    /**
     * Create an XBox-compatible controller or fail.
     * @param id The identifier for the control to create
     * @return The game controller instance or null if it cannot be created.
     */
    public static GameController createXBoxController(int id)
    {
        final int maxId=GameControllerNatives.getXInputNumControllers();
        
        // check the id limit
        if(id < 0)
        {
            throw new IllegalArgumentException("Cannot create id " + id +", value is illegal\n");
        }
        else if(id > maxId)
        {
            throw new IllegalArgumentException("Cannot create id " + id +", only supporting up to " + maxId + " controller IDs\n");
        }
        
        GameController controller;
        
        // see if the controller is an XBox controller
        if(GameControllerNatives.isXInputController(id))
        {
            // make an XBox controller
            controller=new XBoxGameController();

            // set up the instance
            controller.id=id;
            controller.caps=GameControllerNatives.getXInputControllerCaps(id);
            controller.state=new GameControllerState();
        }
        else
        {
            return null;
        }
        
        // do an initial poll
        controller.poll();
        
        return controller;
    }
    
    /**
     * Create a list of all of the connected XBox-compatible controllers only.
     * @return The list of all XBox controllers
     */
    public static ArrayList<GameController> getAllXBoxControllers()
    {
        ArrayList<GameController> list=new ArrayList<GameController>();
        
        for(int id=0; id < GameControllerNatives.getXInputNumControllers() ; ++id)
        {
            GameController controller=createXBoxController(id);
            
            if(controller!=null)
            {
                list.add(controller);
            }
        }
        
        return list;
    }
    
    /**
     * Create a null game controller
     * @param id The identifier for the control to create
     * @return The game controller instance
     */
    public static GameController createNullController(int id)
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
