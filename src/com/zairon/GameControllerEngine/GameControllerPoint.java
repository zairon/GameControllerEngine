package com.zairon.GameControllerEngine;

/**
 * A position for the game controller on a single axis. This implements a dead zone,
 * and when a position is assigned, if the value is in the dead zone it is set
 * to zero.
 * @author Michael Watkins
 *
 */
public class GameControllerPoint
{
    /**
     * The x-position on the axis
     */
    private float x;
    
    /**
     * The y-position on the axis
     */
    private float y;
    
    /**
     * Flag indicating the axis is neutral
     */
    private boolean neutral;
    
    /**
     * The dead zone.
     */
    private float deadZone;
    
    /**
     * Construct the controller point
     * @param x The x-axis position
     * @param y The y-axis position
     * @param deadZone The dead zone for the coordinates
     */
    public GameControllerPoint(float x, float y, float deadZone)
    {
        this.x=x;
        this.y=y;
        this.deadZone=deadZone;
    }
    
    /**
     * Construct the controller point at 0,0
     * @param deadZone The dead zone for the coordinates
     */
    public GameControllerPoint(float deadZone)
    {
        this(0.0f, 0.0f, deadZone);
    }
    
    /**
     * Get the dead zone radius
     * @return the dead zone radius
     */
    public float getDeadZone()
    {
        return deadZone;
    }

    /**
     * Set the dead zone radius
     * @param deadZone the new dead zone radius to use.
     */
    public void setDeadZone(float deadZone)
    {
        this.deadZone = deadZone;
        
        // run the current coordinates through the dead zone test
        testDeadZone(this.x, this.y);
    }
    
    /**
     * Get the x-axis position
     * @param x The new x-axis position
     */
    public float getX()
    {
        return x;
    }
    
    /**
     * Set the x-axis position
     * @param x The x-axis position
     */
    public void setX(float x)
    {
        // flow through the dead zone test
        testDeadZone(x, this.y);        
    }

    /**
     * Get the y-axis position
     * @param y The new y-axis position
     */
    public float getY()
    {
        return y;
    }
    
    /**
     * Set the y-axis position
     * @param y The y-axis position
     */
    public void setY(float y)
    {
        // flow through the dead zone test
        testDeadZone(this.x, y);        
    }

    /**
     * Set both positions
     * @param x The new x-axis position
     * @param y The new y-axis position
     */
    public void setPos(float x, float y)
    {
        // flow through the dead zone test
        testDeadZone(x, y);
    }
    
    /**
     * Set the controller position given an angle and deflection radius
     * @param angle The angle for the controller in degrees
     * @param radius The radius of deflection
     */
    public void setAngle(float angle, float radius)
    {
        double rads=Math.toRadians(angle);
        float x=radius * (float)Math.sin(rads);
        float y=radius * (float)Math.cos(rads);
        
        // run through the dead zone test
        testDeadZone(x, y);
    }
    
    /**
     * Given the current positions, return the angle of the controller
     * @return The angle of the controller in degrees
     */
    public float getAngle()
    {
        // get the angle for the coordinates and return in degrees
        return (float)Math.toDegrees(Math.atan2(this.y, this.x));
    }
    
    /**
     * If the axes are not deflected, then the controller point will indicate neutral.
     * @return The point is neutral.
     */
    public boolean isNeutral()
    {
        return neutral; 
    }
    
    /**
     * Test the coordinates for the dead zone. If the position is in
     * the radius of the dead zone, then zero both coordinates. If either
     * coordinate is within the dead zone, it will be zeroed separately.
     * @param x The x-axis position
     * @param y The y-axis position
     */
    private void testDeadZone(float x, float y)
    {
        float dist=(float)Math.sqrt((x * x) + (y * y));
        
        if(dist <= deadZone)
        {
            // both of the coordinates are within the dead zone, so make both zero
            this.x=0.0f;
            this.y=0.0f;
            
            // set the neutral flag
            neutral=true;
        }
        else
        {
            float xAbs=Math.abs(x);
            float yAbs=Math.abs(y);
            
            // if both are in the dead zone, make neutral
            if((xAbs <= deadZone) && (yAbs <= deadZone))
            {
                this.x=0.0f;
                this.y=0.0f;
                
                // set the neutral flag
                neutral=true;
            }
            else
            {
                // stick is not neutral
                neutral=false;
                
                // the stick position is outside the dead zone
                // now test individual coordinates for being in the dead zone
                if(xAbs <= deadZone)
                {
                    this.x=0.0f;
                }
                else
                {
                    this.x=x;
                }
                
                if(yAbs <= deadZone)
                {
                    this.y=0.0f;
                }
                else
                {
                    this.y=y;
                }
                
            }
            
        }
        
    }
    
    @Override
    public String toString()
    {
        return String.format("%f, %f", x, y);
    }
    
}
