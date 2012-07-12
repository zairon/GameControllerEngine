package com.zairon.GameControllerEngine;

/**
 * The capabilities of a game controller
 * @author Michael Watkins
 *
 */
public class GameControllerCaps
{
    /**
     * The name for the controller
     */
    protected String name;
    
    /**
     * The minimum value for the x-axis
     */
    private float minX;

    /**
     * The maximum value for the x-axis
     */
    private float maxX;

    /**
     * The minimum value for the y-axis
     */
    private float minY;

    /**
     * The maximum value for the y-axis
     */
    private float maxY;
    
    /**
     * The minimum value for the z-axis
     */
    private float minZ;

    /**
     * The maximum value for the z-axis
     */
    private float maxZ;

    /**
     * The minimum value for the r-axis
     */
    private float minR;

    /**
     * The maximum value for the r-axis
     */
    private float maxR;
    
    /**
     * The minimum value for the u-axis
     */
    private float minU;

    /**
     * The maximum value for the u-axis
     */
    private float maxU;

    /**
     * The minimum value for the v-axis
     */
    private float minV;

    /**
     * The maximum value for the v-axis
     */
    private float maxV;

    /**
     * The number of buttons for the game controller
     */
    private int numButtons;
    
    /**
     * The number of axes for the game controller
     */
    private int numAxes;
    
    /**
     * The controller has z-axis information
     */
    private boolean hasZ;
    
    /**
     * The controller has r-axis information
     */
    private boolean hasR;
    
    /**
     * The controller has u-axis information
     */
    private boolean hasU;
    
    /**
     * The controller has v-axis information
     */
    private boolean hasV;
    
    /**
     * The controller has point-of-view hat information
     */
    private boolean hasPOV;
    
    /**
     * The controller has discrete values for point-of-view hat information
     */
    private boolean hasPOV4Dir;
    
    /**
     * The controller has continuous values for point-of-view hat information
     */
    private boolean hasPOVCts;
    
    /**
     * Prevent instantiation; this only comes from the manager factory.
     */
    protected GameControllerCaps()
    {
    }
    
    /**
     * Get the name for the controller
     * @return The name for the controller
     */
    public String getName()
    {
        return name;
    }
    
    /**
     * Get the minimum value for the x-axis
     * @return the minimum value for the x-axis
     */
    public float getMinX()
    {
        return minX;
    }

    /**
     * Get the maximum value for the x-axis
     * @return the maximum value for the x-axis
     */
    public float getMaxX()
    {
        return maxX;
    }

    /**
     * Get the minimum value for the y-axis
     * @return the minimum value for the y-axis
     */
    public float getMinY()
    {
        return minY;
    }

    /**
     * Get the maximum value for the y-axis
     * @return the maximum value for the y-axis
     */
    public float getMaxY()
    {
        return maxY;
    }

    /**
     * Get the minimum value for the z-axis
     * @return the minimum value for the z-axis
     */
    public float getMinZ()
    {
        return minZ;
    }

    /**
     * Get the maximum value for the z-axis
     * @return the maximum value for the z-axis
     */
    public float getMaxZ()
    {
        return maxZ;
    }

    /**
     * Get the minimum value for the r-axis
     * @return the minimum value for the r-axis
     */
    public float getMinR()
    {
        return minR;
    }

    /**
     * Get the maximum value for the r-axis
     * @return the maximum value for the r-axis
     */
    public float getMaxR()
    {
        return maxR;
    }

    /**
     * Get the minimum value for the u-axis
     * @return the minimum value for the u-axis
     */
    public float getMinU()
    {
        return minU;
    }

    /**
     * Get the maximum value for the u-axis
     * @return the maximum value for the u-axis
     */
    public float getMaxU()
    {
        return maxU;
    }

    /**
     * Get the minimum value for the v-axis
     * @return the minimum value for the v-axis
     */
    public float getMinV()
    {
        return minV;
    }

    /**
     * Get the maximum value for the v-axis
     * @return the maximum value for the v-axis
     */
    public float getMaxV()
    {
        return maxV;
    }

    /**
     * Get the number of buttons for the controller
     * @return The number of buttons
     */
    public int getNumButtons()
    {
        return numButtons;
    }

    /**
     * Get the number of axes for the controller
     * @return The number of axes
     */
    public int getNumAxes()
    {
        return numAxes;
    }
    
    /**
     * The controller has z-axis information.
     * @return true if the controller has z-axis information
     */
    public boolean hasZ()
    {
        return this.hasZ;
    }

    /**
     * The controller has r-axis information.
     * @return true if the controller has r-axis information
     */
    public boolean hasR()
    {
        return this.hasR;
    }

    /**
     * The controller has u-axis information.
     * @return true if the controller has u-axis information
     */
    public boolean hasU()
    {
        return this.hasU;
    }

    /**
     * The controller has v-axis information.
     * @return true if the controller has v-axis information
     */
    public boolean hasV()
    {
        return this.hasV;
    }

    /**
     * The controller has point-of-view hat information.
     * @return true if the controller has point-of-view hat information
     */
    public boolean hasPOV()
    {
        return this.hasPOV;
    }

    /**
     * The controller has discrete values for point-of-view hat information.
     * @return true if the controller has discrete values for point-of-view hat information
     */
    public boolean hasPOV4Dir()
    {
        return this.hasPOV4Dir;
    }

    /**
     * The controller has discrete values for point-of-view hat information.
     * @return true if the controller has discrete values for point-of-view hat information
     */
    public boolean hasPOVCts()
    {
        return this.hasPOVCts;
    }
    
}
