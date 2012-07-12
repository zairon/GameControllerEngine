package com.zairon.GameControllerEngine;

/**
 * A position for the game controller on a single axis. This implements a dead zone,
 * and when a position is assigned, if the value is in the dead zone it is set
 * to zero.
 * @author Michael Watkins
 *
 */
public class GameControllerAxis
{
    /**
     * The position on the axis
     */
    private float p;
    
    /**
     * The dead zone.
     */
    private float deadZone;
    
    /**
     * Construct the controller axis
     * @param p The position on the axis
     * @param deadZone The dead zone for the axis
     */
    public GameControllerAxis(float p, float deadZone)
    {
        this.p=p;
        this.deadZone=deadZone;
    }
    
    /**
     * Construct the controller axis
     * @param deadZone The dead zone for the axis
     */
    public GameControllerAxis(float deadZone)
    {
        this(0.0f, deadZone);
    }

    
    /**
     * Get the dead zone for the axis
     * @return the dead zone for the axis
     */
    public float getDeadZone()
    {
        return deadZone;
    }

    /**
     * Set the dead zone for the axis
     * @param deadZone the new dead zone for the axis.
     */
    public void setDeadZone(float deadZone)
    {
        this.deadZone = deadZone;
        
        // run the current coordinate against the dead zone
        testDeadZone(this.p);
    }

    /**
     * Get the position on the axis
     * @param p The new position on axis
     */
    public float getP()
    {
        return p;
    }
    
    /**
     * Set the position on the axis
     * @param p The position on the axis
     */
    public void setP(float p)
    {
        // flow through the dead zone test
        testDeadZone(p);
    }
    
    /**
     * Test the coordinate for the dead zone. If the position is in
     * the radius of the dead zone, then zero the coordinate.
     * @param p The position on the axis
     */
    private void testDeadZone(float p)
    {
        if(Math.abs(p) <= deadZone)
        {
            this.p=0.0f;
        }
        else
        {
            this.p=p;
        }
        
    }

    @Override
    public String toString()
    {
        return Float.toString(p);
    }
    
}
