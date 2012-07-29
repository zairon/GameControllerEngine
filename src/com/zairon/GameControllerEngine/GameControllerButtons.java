package com.zairon.GameControllerEngine;

/**
 * This is an enumeration for all of the possible buttons supported by the engine.
 * Note that not all button values are supported, depending on the game controller.
 * @author Michael Watkins
 *
 */
public enum GameControllerButtons
{
    BUTTON1("BUTTON1"),
    BUTTON2("BUTTON2"),
    BUTTON3("BUTTON3"),
    BUTTON4("BUTTON4"),
    BUTTON5("BUTTON5"),
    BUTTON6("BUTTON6"),
    BUTTON7("BUTTON7"),
    BUTTON8("BUTTON8"),
    BUTTON9("BUTTON9"),
    BUTTON10("BUTTON10"),
    BUTTON11("BUTTON11"),
    BUTTON12("BUTTON12"),
    BUTTON13("BUTTON13"),
    BUTTON14("BUTTON14"),
    BUTTON15("BUTTON15"),
    BUTTON16("BUTTON16"),
    BUTTON17("BUTTON17"),
    BUTTON18("BUTTON18"),
    BUTTON19("BUTTON19"),
    BUTTON20("BUTTON20"),
    BUTTON21("BUTTON21"),
    BUTTON22("BUTTON22"),
    BUTTON23("BUTTON23"),
    BUTTON24("BUTTON24"),
    BUTTON25("BUTTON25"),
    BUTTON26("BUTTON26"),
    BUTTON27("BUTTON27"),
    BUTTON28("BUTTON28"),
    BUTTON29("BUTTON29"),
    BUTTON30("BUTTON30"),
    BUTTON31("BUTTON31"),
    BUTTON32("BUTTON32"),
    /**
     * The left trigger control
     */
    LEFT_TRIGGER("LEFT_TRIGGER"),
    /**
     * The right trigger control
     */
    RIGHT_TRIGGER("RIGHT_TRIGGER"),
    /**
     * The maximum buttons value
     */
    MAX_BUTTONS("MAX_BUTTONS"),
    /**
     * An undefined button code
     */
    UNDEFINED("UNDEFINED");
    
    /**
     * The label for the controller button
     */
    private String label;

    /**
     * Construct the enumeration value
     * @param label The label for the enumeration value
     */
    private GameControllerButtons(String label)
    {
        this.label=label;
    }
    
    /**
     * Determine if the code passed in is a valid button code
     * @param code The code to test
     * @return true if a valid button code, false otherwise
     */
    public static boolean isValidCode(int code)
    {
        return( (code >= BUTTON1.ordinal()) && (code < MAX_BUTTONS.ordinal()) );
    }
    
    /**
     * Convert the code given into an enumeration value
     * @param code The button code to convert
     * @return An enumeration value matching the code
     */
    public static GameControllerButtons get(int code)
    {
        // loop through the possible flags
        for(GameControllerButtons button : GameControllerButtons.values())
        {
            // anything greater than or equal to max buttons is undefined
            if(button.ordinal()>=MAX_BUTTONS.ordinal())
            {
                break;
            }
            else if(button.ordinal() == code)
            {
                return button;
            }
                
        }
        
        return UNDEFINED;

    }
    
    /**
     * Test whether the integer code matches this button
     * @param code The code to test
     * @return true if this is the button in question, false otherwise
     */
    public boolean is(int code)
    {
        return (ordinal() == code);
    }

    @Override
    public String toString()
    {
        return this.label;
    }
}
