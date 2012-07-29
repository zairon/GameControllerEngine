#include <jni.h>
#include "com_zairon_GameControllerEngine_GameControllerNatives.h"
#include <stdio.h>
#include <windows.h>
#include "XInput.h"

/// The maximum number of buttons that can be tested
#define GCN_MAX_BUTTONS 32

/// The maximum value for a joystick axis
#define GCN_MAXIMUM_AXIS ((jfloat)65535.0)

/// Half of the maximum value for a joystick axis
#define GCN_HALF_AXIS ((jfloat)32767.5)

/// Maximum number of controllers for XInput
#define GCN_XINPUT_MAX_CONTROLLERS 4

/// Maximum number of buttons for XInput
#define GCN_XINPUT_MAX_BUTTONS 10

/// Number of buttons for XInput
#define GCN_XINPUT_NUM_BUTTONS GCN_XINPUT_MAX_BUTTONS

/// Number of axes for XInput
/// There are sort of 3 axes, but the left/right trigger are treated
/// separately for XInput like buttons, but having a range between 0 and 255
#define GCN_XINPUT_NUM_AXES 3

/// The minimum for a trigger axis in XInput
#define GCN_XINPUT_MIN_TRIGGER_AXIS ((jfloat)0.0)

/// The maximum for a trigger axis in XInput
#define GCN_XINPUT_MAX_TRIGGER_AXIS ((jfloat)255.0)

/// The minimum for a z-axis in XInput
#define GCN_XINPUT_MIN_Z_AXIS (-GCN_XINPUT_MAX_TRIGGER_AXIS)

/// The minimum for a z-axis in XInput
#define GCN_XINPUT_MAX_Z_AXIS (GCN_XINPUT_MAX_TRIGGER_AXIS)

/// The minimum axis value for XInput
#define GCN_XINPUT_MIN_AXIS -GCN_HALF_AXIS

/// The maximum axis value for XInput
#define GCN_XINPUT_MAX_AXIS GCN_HALF_AXIS

/// The name for a controller in XInput
/// This could use the subtype stuff, etc. but do later
#define GCN_XINPUT_CONTROLLER_NAME "XInputController"

/// Neutral for POV hat in XInput (same as result from winmm)
#define GCN_XINPUT_POV_NEUTRAL ((jfloat)655.35f)

/// Other POV direction constants for XInput
#define GCN_XINPUT_POV_UP ((jfloat)0.0f)
#define GCN_XINPUT_POV_UP_LEFT ((jfloat)315.0f)
#define GCN_XINPUT_POV_LEFT ((jfloat)270.0f)
#define GCN_XINPUT_POV_DOWN_LEFT ((jfloat)225.0f)
#define GCN_XINPUT_POV_DOWN ((jfloat)180.0f)
#define GCN_XINPUT_POV_DOWN_RIGHT ((jfloat)135.0f)
#define GCN_XINPUT_POV_RIGHT ((jfloat)90.0f)
#define GCN_XINPUT_POV_UP_RIGHT ((jfloat)45.0f)

/// Definitions for bitmasks corresponding to button offsets
#define GCN_XINPUT_A_BUTTON (0)
#define GCN_XINPUT_B_BUTTON (1)
#define GCN_XINPUT_X_BUTTON (2)
#define GCN_XINPUT_Y_BUTTON (3)
#define GCN_XINPUT_LEFT_SHOULDER_BUTTON (4)
#define GCN_XINPUT_RIGHT_SHOULDER_BUTTON (5)
#define GCN_XINPUT_BACK_BUTTON (6)
#define GCN_XINPUT_START_BUTTON (7)
#define GCN_XINPUT_LEFT_THUMB_BUTTON (8)
#define GCN_XINPUT_RIGHT_THUMB_BUTTON (9)
#define GCN_XINPUT_REST_OF_BUTTONS (GCN_XINPUT_RIGHT_THUMB_BUTTON+1)

/// Get the number of controllers
JNIEXPORT jint JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getNumControllers
    (
    JNIEnv *env,
    jobject obj
    )
{
    return (jint)joyGetNumDevs();
}

/// Throw a runtime exception. A maximum of 256 chars is allowed.
void throwRuntimeException
    (
    JNIEnv *env,
    char *format,
    ...
    )
{
    // make the message
    char message[256];
    va_list args;
    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    (*env)->ExceptionDescribe(env);
    (*env)->ExceptionClear(env);
    jclass newExcCls = (*env)->FindClass(env,"java/lang/RuntimeException");
    (*env)->ThrowNew(env, newExcCls, message);
}

/// Throw an illegal argument exception. A maximum of 256 chars is allowed.
void throwIllegalArgumentException
    (
    JNIEnv *env,
    char *format,
    ...
    )
{
    // make the message
    char message[256];
    va_list args;
    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    (*env)->ExceptionDescribe(env);
    (*env)->ExceptionClear(env);
    jclass newExcCls = (*env)->FindClass(env,"java/lang/IllegalArgumentException");
    (*env)->ThrowNew(env, newExcCls, message);
}

/// Helper function to get joystick capabilities
BOOL getJoyCaps
    (
    JNIEnv *env,
    int id,
    LPJOYCAPS jc
    )
{
    MMRESULT res=joyGetDevCaps(id + JOYSTICKID1, jc, sizeof(JOYCAPS));

    if(res!=JOYERR_NOERROR)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

/// Helper function to set floating point field
void setFloatField
    (
    JNIEnv *env,
    jclass objectCls,
    jobject object,
    char *fieldName,
    jfloat val
    )
{
    // get the field id using the floating point type signature  
    jfieldID fid = (*env)->GetFieldID(env, objectCls, fieldName, "F");
    
    if (fid == NULL) 
    {
        // have to have this field
        throwRuntimeException(env, "Cannot find Float field %s to set\n", fieldName);    
    }
 
    (*env)->SetFloatField(env, object, fid, val);

    return; 
}
    
/// Helper function to set integer field
void setIntField
    (
    JNIEnv *env,
    jclass objectCls,
    jobject object,
    char *fieldName,
    jint val
    )
{
    // get the field id using the integer type signature  
    jfieldID fid = (*env)->GetFieldID(env, objectCls, fieldName, "I");
    
    if (fid == NULL) 
    {
        // have to have this field
        throwRuntimeException(env, "Cannot find Int field %s to set\n", fieldName);    
    }
 
    (*env)->SetIntField(env, object, fid, val);

    return; 
}

/// Helper function to set boolean field
void setBooleanField
    (
    JNIEnv *env,
    jclass objectCls,
    jobject object,
    char *fieldName,
    jboolean val
    )
{
    // get the field id using the boolean type signature  
    jfieldID fid = (*env)->GetFieldID(env, objectCls, fieldName, "Z");
    
    if (fid == NULL) 
    {
        // have to have this field
        throwRuntimeException(env, "Cannot find Boolean field %s to set\n", fieldName);    
    }
 
    (*env)->SetBooleanField(env, object, fid, val);

    return; 
}

/// Helper function to set string field directly without allocating the jstring to use.
void setStringFieldDirect
    (
    JNIEnv *env,
    jclass objectCls,
    jobject object,
    char *fieldName,
    jstring val
    )
{
    // get the field id using the string type signature  
    jfieldID fid = (*env)->GetFieldID(env, objectCls, fieldName, "Ljava/lang/String;");
    
    if (fid == NULL) 
    {
        // have to have this field
        throwRuntimeException(env, "Cannot find String field %s to set\n", fieldName);    
    }
 
    // set the field directly, since don't need to allocate it
    (*env)->SetObjectField(env, object, fid, val);

    return; 
}
    
/// Helper function to set string field
void setStringField
    (
    JNIEnv *env,
    jclass objectCls,
    jobject object,
    char *fieldName,
    char *val
    )
{
    jstring jstr=(*env)->NewStringUTF(env, val);
    
    if(jstr==NULL)
    {
        // have to have this field
        throwRuntimeException(env, "Out of Memory setting String field %s\n", fieldName);    
    }
    
    setStringFieldDirect(env, objectCls, object, fieldName, jstr); 
    
    return; 
}
    
/// Helper function to set boolean array field
void setBooleanArrayFieldElement
    (
    JNIEnv *env,
    jclass objectCls,
    jobject object,
    char *fieldName,
    int index,
    jboolean val
    )
{
    // get the field id using the boolean array type signature  
    jfieldID fid = (*env)->GetFieldID(env, objectCls, fieldName, "[Z");
    
    if (fid == NULL) 
    {
        // have to have this field
        throwRuntimeException(env, "Cannot find Float field %s to set\n", fieldName);    
    }
 
    // get the array
    jbooleanArray array=(jbooleanArray)(*env)->GetObjectField(env, object, fid);
    
    // get the pointer to the array elements
    jboolean *elements=(*env)->GetBooleanArrayElements(env, array, NULL);
    
    // set the element in the array
    elements[index]=val;
    
    // release the elements, copying back the data to the buffer and release the elements buffer
    (*env)->ReleaseBooleanArrayElements(env, array, elements, 0);
    
    return; 
}

/// Helper function to set all of the elements boolean array field
void setBooleanArrayField
    (
    JNIEnv *env,
    jclass objectCls,
    jobject object,
    char *fieldName,
    int numValues,
    jboolean *val
    )
{
    // get the field id using the boolean array type signature  
    jfieldID fid = (*env)->GetFieldID(env, objectCls, fieldName, "[Z");
    
    if (fid == NULL) 
    {
        // have to have this field
        throwRuntimeException(env, "Cannot find Float field %s to set\n", fieldName);    
    }
 
    // get the array
    jbooleanArray array=(jbooleanArray)(*env)->GetObjectField(env, object, fid);
    
    /* Brute force method, use the region way
    // get the pointer to the array elements
    jboolean *elements=(*env)->GetBooleanArrayElements(env, array, NULL);
    
    int i;
    
    // set the elements in the array
    for(i=0 ; i < numValues ; ++i)
    {
        elements[i]=val[i];
    }
    
    // release the elements, copying back the data to the buffer and release the elements buffer
    (*env)->ReleaseBooleanArrayElements(env, array, elements, 0);
    */
    
    (*env)->SetBooleanArrayRegion(env, array, 0, numValues, val);
    
    return; 
}

/// Helper function to get joystick position
BOOL getJoyPos
    (
    JNIEnv *env,
    int id,
    LPJOYINFO ji
    )
{
    MMRESULT res=joyGetPos(id + JOYSTICKID1, ji);

    if(res!=JOYERR_NOERROR)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

/// Helper function to get extended joystick position
BOOL getJoyPosEx
    (
    JNIEnv *env,
    int id,
    LPJOYINFOEX ji
    )
{
    MMRESULT res=joyGetPosEx(id + JOYSTICKID1, ji);

    if(res!=JOYERR_NOERROR)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

/// Get whether a controller is plugged in
JNIEXPORT jboolean JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_isConnected
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    JOYINFO ji;

    // if we can get joystick position info, then the joystick is connected
    return (getJoyPos(env, id, &ji)==TRUE);
}

/// Helper function to format the controller name. The maximum is 256 characters.
jstring printControllerName
    ( 
    JNIEnv *env,
    char *format,
    ...
    )
{
    // make the message
    char output[256];
    va_list args;
    va_start(args, format);
    vsprintf(output, format, args);
    va_end(args);

    return (*env)->NewStringUTF(env, output);
}

/// Helper function to make the controller name. The maximum is 256 characters.
jstring makeControllerName
    ( 
    JNIEnv *env,
    int id,
    char *base
    )
{
    return printControllerName(env, "%s%d", base, id);
}

/// Get the name of a controller
JNIEXPORT jstring JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getControllerName
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    JOYCAPS jc;

    if(!getJoyCaps(env, id, &jc))
    {
        // throw an exception
        throwIllegalArgumentException(env, "Cannot get capabilities for id %d\n", id);
    }
    else
    {
        return makeControllerName(env, id, jc.szPname);
    }
    
}

/// Get number of buttons for the controller
JNIEXPORT jint JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getNumButtons
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    JOYCAPS jc;

    if(!getJoyCaps(env, id, &jc))
    {
        // throw an exception
        throwIllegalArgumentException(env, "Cannot get capabilities for id %d\n", id);
    }
    else
    {
        return ((jint)jc.wNumButtons);
    }
}

/// Get number of axes for the controller
JNIEXPORT jint JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getNumAxes
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    JOYCAPS jc;

    if(!getJoyCaps(env, id, &jc))
    {
        // throw an exception
        throwIllegalArgumentException(env, "Cannot get capabilities for id %d\n", id);
    }
    else
    {
        return ((jint)jc.wNumAxes);
    }
}

/// Get controller capabilities
JNIEXPORT jobject JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getControllerCaps
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    JOYCAPS jc;
    jboolean connected;

    // clear the capabilities in case not connected
    memset(&jc, 0, sizeof(JOYCAPS));
    
    if(!getJoyCaps(env, id, &jc))
    {
        // not connected, but this is not fatal
        connected=FALSE;
    }
    else
    {
        // this is connected
        connected=TRUE;
    }
    
    // make a new instance of the game controller capabilities class
    // use the signature for the class to find the class
    jclass capsCls=(*env)->FindClass(env, "Lcom/zairon/GameControllerEngine/GameControllerCaps;");
    
    // get the constructor method, there are no parameters so use the empty constructor
    jmethodID capsMidConstructor=(*env)->GetMethodID(env, capsCls, "<init>", "()V");
    
    // create the new instance from the class
    jobject capsObject=(*env)->NewObject(env, capsCls, capsMidConstructor);
    
    // start setting the fields
    setFloatField(env, capsCls, capsObject, "minX", ((jfloat)jc.wXmin) );
    setFloatField(env, capsCls, capsObject, "maxX", ((jfloat)jc.wXmax) );
    setFloatField(env, capsCls, capsObject, "minY", ((jfloat)jc.wYmin) );
    setFloatField(env, capsCls, capsObject, "maxY", ((jfloat)jc.wYmax) );
    setFloatField(env, capsCls, capsObject, "minZ", ((jfloat)jc.wZmin) );
    setFloatField(env, capsCls, capsObject, "maxZ", ((jfloat)jc.wZmax) );
    setFloatField(env, capsCls, capsObject, "minR", ((jfloat)jc.wRmin) );
    setFloatField(env, capsCls, capsObject, "maxR", ((jfloat)jc.wRmax) );
    setFloatField(env, capsCls, capsObject, "minU", ((jfloat)jc.wUmin) );
    setFloatField(env, capsCls, capsObject, "maxU", ((jfloat)jc.wUmax) );
    setFloatField(env, capsCls, capsObject, "minV", ((jfloat)jc.wVmin) );
    setFloatField(env, capsCls, capsObject, "maxV", ((jfloat)jc.wVmax) );
    
    setIntField(env, capsCls, capsObject, "numButtons", ((jint)jc.wNumButtons) );
    setIntField(env, capsCls, capsObject, "numAxes", ((jint)jc.wNumAxes) );
    
    setBooleanField(env, capsCls, capsObject, "hasZ", ((jboolean)((jc.wCaps & JOYCAPS_HASZ)==JOYCAPS_HASZ)) );
    setBooleanField(env, capsCls, capsObject, "hasR", ((jboolean)((jc.wCaps & JOYCAPS_HASR)==JOYCAPS_HASR)) );
    setBooleanField(env, capsCls, capsObject, "hasU", ((jboolean)((jc.wCaps & JOYCAPS_HASU)==JOYCAPS_HASU)) );
    setBooleanField(env, capsCls, capsObject, "hasV", ((jboolean)((jc.wCaps & JOYCAPS_HASV)==JOYCAPS_HASV)) );
    setBooleanField(env, capsCls, capsObject, "hasPOV", ((jboolean)((jc.wCaps & JOYCAPS_HASPOV)==JOYCAPS_HASPOV)) );
    setBooleanField(env, capsCls, capsObject, "hasPOV4Dir", ((jboolean)((jc.wCaps & JOYCAPS_POV4DIR)==JOYCAPS_POV4DIR)) );
    setBooleanField(env, capsCls, capsObject, "hasPOVCts", ((jboolean)((jc.wCaps & JOYCAPS_POVCTS)==JOYCAPS_POVCTS)) );
    
    jstring controllerName=makeControllerName(env, id, jc.szPname);
    
    setStringFieldDirect(env, capsCls, capsObject, "name",  controllerName);
    
    return capsObject;
}

/// Helper function to calculate the scaled joystick position given the value
jfloat calcJoyPos
    (
    DWORD pos
    )
{
    return ( ( ((jfloat)pos) - GCN_HALF_AXIS ) / GCN_HALF_AXIS);
}

/// Helper function to calculate the scaled joystick point-of-view hat position given the value
jfloat calcJoyPOV
    (
    DWORD pos
    )
{
    // the point of view is degrees multiplied by 100
    return ( ((jfloat)pos) / 100.0f);
}

/// Helper function to infer a left trigger value from the z-axis.
jfloat calcJoyLeftTrigger
    (
    jfloat zAxis
    )
{
    // the left trigger is a positive value
    if(zAxis > 0.0f)
    {
        return zAxis;
    }
    else
    {
        // not set
        return 0.0f;
    }
}

/// Helper function to infer a right trigger value from the z-axis.
jfloat calcJoyRightTrigger
    (
    jfloat zAxis
    )
{
    // the right trigger is a negative value
    if(zAxis < 0.0f)
    {
        // make this value positive
        return -zAxis;
    }
    else
    {
        // not set
        return 0.0f;
    }
}

/// Poll the controller
JNIEXPORT jboolean JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getControllerState
    (
    JNIEnv *env,
    jclass class,
    jint id,
    jobject stateObject
    )
{
    JOYINFOEX ji;
    jboolean connected;
    
    memset(&ji, 0, sizeof(ji));
    
    // set up the info structure to get all data
    ji.dwSize=sizeof(ji);
    ji.dwFlags=JOY_RETURNALL;

    if(!getJoyPosEx(env, id, &ji))
    {
        // not connected
        connected=FALSE;
    }
    else
    {
        // controller is connected
        connected=TRUE;
    }

    // get the class for the object passed in
    jclass stateCls = (*env)->GetObjectClass(env, stateObject);
 
    // start setting the fields
    setFloatField(env, stateCls, stateObject, "x", (jfloat)calcJoyPos(ji.dwXpos));
    // regular joystick has up as negative and down as positive
    // flip the sense of that to match XInput
    setFloatField(env, stateCls, stateObject, "y", -(jfloat)calcJoyPos(ji.dwYpos));

    // calculate the z-axis
    jfloat zAxis=(jfloat)calcJoyPos(ji.dwZpos);
    setFloatField(env, stateCls, stateObject, "z", zAxis);

    // figure out the trigger positions from the z-axis
    setFloatField(env, stateCls, stateObject, "leftTrigger", (jfloat)calcJoyLeftTrigger(zAxis));
    setFloatField(env, stateCls, stateObject, "rightTrigger", (jfloat)calcJoyRightTrigger(zAxis));

    // regular joystick has up as negative and down as positive
    // flip the sense of that to match XInput
    setFloatField(env, stateCls, stateObject, "r", -(jfloat)calcJoyPos(ji.dwRpos));
    
    setFloatField(env, stateCls, stateObject, "u", (jfloat)calcJoyPos(ji.dwUpos));
    setFloatField(env, stateCls, stateObject, "v", (jfloat)calcJoyPos(ji.dwVpos));
    
    setFloatField(env, stateCls, stateObject, "pov", (jfloat)calcJoyPOV(ji.dwPOV));
    
    setIntField(env, stateCls, stateObject, "buttonMap", (jint)ji.dwButtons);
    
    jboolean buttonStates[GCN_MAX_BUTTONS];
    
    int i;
    
    // get the button states
    for(i=0; i < GCN_MAX_BUTTONS ; ++i)
    {
        buttonStates[i]=((ji.dwButtons & 1)==1);
        
        // shift off the bit in the button map
        ji.dwButtons >>= 1;
    }
    
    // set the field values
    setBooleanArrayField(env, stateCls, stateObject, "buttons", GCN_MAX_BUTTONS, buttonStates);
    
    // set whether connected
    setBooleanField(env, stateCls, stateObject, "connected", connected );
    
    // return whether controller is connected
    return connected;
           
}

/// XINPUT VERSION

/// Get the number of controllers for XInput
JNIEXPORT jint JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getXInputNumControllers
    (
    JNIEnv *env,
    jobject obj
    )
{
    // per the XInput docs, only four controllers are supported.
    return (jint)GCN_XINPUT_MAX_CONTROLLERS;
}

/// Helper function to get XInput capabilities
BOOL getXInputCaps
    (
    JNIEnv *env,
    int id,
    XINPUT_CAPABILITIES *xc
    )
{
    // clear the capabilities
    memset(xc, 0, sizeof(XINPUT_CAPABILITIES));

    // get only gamepads
    DWORD res=XInputGetCapabilities(id, XINPUT_FLAG_GAMEPAD, xc);

    if(res!=ERROR_SUCCESS)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

/// Helper function to get XInput state
BOOL getXInputState
    (
    JNIEnv *env,
    int id,
    XINPUT_STATE *xs
    )
{
    // clear the state
    memset(xs, 0, sizeof(XINPUT_STATE));

    DWORD res=XInputGetState(id, xs);

    if(res!=ERROR_SUCCESS)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

/// Get whether a controller is a valid XInput controller
JNIEXPORT jboolean JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_isXInputController
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    // check the id
    if(id >= GCN_XINPUT_MAX_CONTROLLERS)
    {
        // not an XInput valid controller
        return ((jboolean)FALSE);
    }
    
    XINPUT_CAPABILITIES xc;

    // get the x-input capabilities
    if(!getXInputCaps(env, id, &xc))
    {
        // not an XInput controller
        return ((jboolean)FALSE);
    }

    // check the capabilities
    // it must be a gamepad
    //return (xc.Type==XINPUT_DEVTYPE_GAMEPAD);
    return ((jboolean)TRUE);
}

/// Get whether a controller is plugged in for XInput
JNIEXPORT jboolean JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_isXInputConnected
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    XINPUT_STATE xs;

    // if the state retrieval works, then the controller is connected
    return (getXInputState(env, id, &xs)==TRUE);    
}

/// Get the name of a controller for XInput
JNIEXPORT jstring JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getXInputControllerName
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    XINPUT_CAPABILITIES xc;

    if(!getXInputCaps(env, id, &xc))
    {
        // throw an exception
        throwIllegalArgumentException(env, "Cannot get XInput capabilities for id %d\n", id);
    }
    else
    {
        return makeControllerName(env, id, GCN_XINPUT_CONTROLLER_NAME);
    }
    
}

/// Get number of buttons for the controller for XInput
JNIEXPORT jint JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getXInputNumButtons
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    XINPUT_CAPABILITIES xc;

    if(!getXInputCaps(env, id, &xc))
    {
        // throw an exception
        throwIllegalArgumentException(env, "Cannot get XInput capabilities for id %d\n", id);
    }
    else
    {
        return GCN_XINPUT_NUM_BUTTONS;
    }
}

/// Get number of axes for the controller
JNIEXPORT jint JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getXInputNumAxes
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    XINPUT_CAPABILITIES xc;

    if(!getXInputCaps(env, id, &xc))
    {
        // throw an exception
        throwIllegalArgumentException(env, "Cannot get XInput capabilities for id %d\n", id);
    }
    else
    {
        return GCN_XINPUT_NUM_AXES;
    }
}

/// Get controller capabilities for XInput
JNIEXPORT jobject JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getXInputControllerCaps
    (
    JNIEnv *env,
    jobject obj,
    jint id
    )
{
    XINPUT_CAPABILITIES xc;
    jboolean connected;

    if(!getXInputCaps(env, id, &xc))
    {
        // not connected, but this is not fatal
        connected=FALSE;
    }
    else
    {
        // this is connected
        connected=TRUE;
    }
    
    // make a new instance of the game controller capabilities class
    // use the signature for the class to find the class
    jclass capsCls=(*env)->FindClass(env, "Lcom/zairon/GameControllerEngine/GameControllerCaps;");
    
    // get the constructor method, there are no parameters so use the empty constructor
    jmethodID capsMidConstructor=(*env)->GetMethodID(env, capsCls, "<init>", "()V");
    
    // create the new instance from the class
    jobject capsObject=(*env)->NewObject(env, capsCls, capsMidConstructor);
    
    // start setting the fields
    setFloatField(env, capsCls, capsObject, "minX", ((jfloat)GCN_XINPUT_MIN_AXIS) );
    setFloatField(env, capsCls, capsObject, "maxX", ((jfloat)GCN_XINPUT_MAX_AXIS) );
    setFloatField(env, capsCls, capsObject, "minY", ((jfloat)GCN_XINPUT_MIN_AXIS) );
    setFloatField(env, capsCls, capsObject, "maxY", ((jfloat)GCN_XINPUT_MAX_AXIS) );
    setFloatField(env, capsCls, capsObject, "minZ", ((jfloat)GCN_XINPUT_MIN_Z_AXIS) );
    setFloatField(env, capsCls, capsObject, "maxZ", ((jfloat)GCN_XINPUT_MAX_Z_AXIS) );
    setFloatField(env, capsCls, capsObject, "minR", ((jfloat)GCN_XINPUT_MIN_AXIS) );
    setFloatField(env, capsCls, capsObject, "maxR", ((jfloat)GCN_XINPUT_MAX_AXIS) );
    setFloatField(env, capsCls, capsObject, "minU", ((jfloat)GCN_XINPUT_MIN_AXIS) );
    setFloatField(env, capsCls, capsObject, "maxU", ((jfloat)GCN_XINPUT_MAX_AXIS) );
    // no V-axis for XInput
    setFloatField(env, capsCls, capsObject, "minV", ((jfloat)0.0f) );
    setFloatField(env, capsCls, capsObject, "maxV", ((jfloat)0.0f) );
    
    setIntField(env, capsCls, capsObject, "numButtons", ((jint)GCN_XINPUT_NUM_BUTTONS) );
    setIntField(env, capsCls, capsObject, "numAxes", ((jint)GCN_XINPUT_NUM_AXES) );
    
    setBooleanField(env, capsCls, capsObject, "hasZ", ((jboolean)TRUE) );
    setBooleanField(env, capsCls, capsObject, "hasR", ((jboolean)TRUE) );
    setBooleanField(env, capsCls, capsObject, "hasU", ((jboolean)TRUE) );
    setBooleanField(env, capsCls, capsObject, "hasV", ((jboolean)FALSE) );
    setBooleanField(env, capsCls, capsObject, "hasPOV", ((jboolean)TRUE) );
    setBooleanField(env, capsCls, capsObject, "hasPOV4Dir", ((jboolean)TRUE) );
    setBooleanField(env, capsCls, capsObject, "hasPOVCts", ((jboolean)FALSE) );
    
    jstring controllerName=makeControllerName(env, id, GCN_XINPUT_CONTROLLER_NAME);
    
    setStringFieldDirect(env, capsCls, capsObject, "name",  controllerName);
    
    return capsObject;
}

/// Helper function to calculate the scaled joystick position given the value
jfloat calcXInputPos
    (
    SHORT pos
    )
{
    return ( ( ((jfloat)pos) ) / GCN_HALF_AXIS);
}

/// Helper function to calculate the scaled joystick point-of-view hat position given the value
jfloat calcXInputPOV
    (
    WORD buttons
    )
{
    // see which way the POV hat is pointing
    BOOL up=(buttons & XINPUT_GAMEPAD_DPAD_UP)==XINPUT_GAMEPAD_DPAD_UP;
    BOOL down=(buttons & XINPUT_GAMEPAD_DPAD_DOWN)==XINPUT_GAMEPAD_DPAD_DOWN;
    BOOL left=(buttons & XINPUT_GAMEPAD_DPAD_LEFT)==XINPUT_GAMEPAD_DPAD_LEFT;
    BOOL right=(buttons & XINPUT_GAMEPAD_DPAD_RIGHT)==XINPUT_GAMEPAD_DPAD_RIGHT;

    if(up)
    {
        // see if left or right
        // this means they don't need to be checked when just left or right is set below
        // since we would be in this branch
        if(left)
        {
            return GCN_XINPUT_POV_UP_LEFT;
        }
        else if(right)
        {
            return GCN_XINPUT_POV_UP_RIGHT;
        }
        else
        {
            return GCN_XINPUT_POV_UP;
        }
    }
    else if(down)
    {
        // see if left or right
        // this means they don't need to be checked when just left or right is set below
        // since we would be in this branch
        if(left)
        {
            return GCN_XINPUT_POV_DOWN_LEFT;
        }
        else if(right)
        {
            return GCN_XINPUT_POV_DOWN_RIGHT;
        }
        else
        {
            return GCN_XINPUT_POV_DOWN;
        }

    }
    else if(left)
    {
        // up and down are both false, so this must be just left
        // since right and left are mutually exclusive
        return GCN_XINPUT_POV_LEFT;
    }
    else if(right)
    {
        // up and down are both false, so this must be just right
        // since right and left are mutually exclusive
        return GCN_XINPUT_POV_RIGHT;
    }
    else
    {
        // POV hat is neutral, return 655.35
        return GCN_XINPUT_POV_NEUTRAL;
    }

}

/// Helper function to calculate a single trigger value
jfloat calcXInputSingleTrigger
    (
    DWORD pos
    )
{
    return ( ( ((jfloat)pos)) / GCN_XINPUT_MAX_TRIGGER_AXIS);
}

/// Helper function to calculate the full z-axis value
jfloat calcXInputZAxis
    (
    DWORD left,
    DWORD right
    )
{
    // with winmm, the triggers for an xbox controller are the whole axis
    // the right trigger is negative z-axis values, and the left trigger is positive
    // so, subtract the right from the left trigger after scaling them to get the z-axis value
    return calcXInputSingleTrigger(left) - calcXInputSingleTrigger(right);
}

/// Poll the controller
JNIEXPORT jboolean JNICALL Java_com_zairon_GameControllerEngine_GameControllerNatives_getXInputControllerState
    (
    JNIEnv *env,
    jclass class,
    jint id,
    jobject stateObject
    )
{
    XINPUT_STATE xs;
    jboolean connected;
    
    if(!getXInputState(env, id, &xs))
    {
        // not connected
        connected=FALSE;
    }
    else
    {
        // controller is connected
        connected=TRUE;
    }

    // get the class for the object passed in
    jclass stateCls = (*env)->GetObjectClass(env, stateObject);
 
    // start setting the fields
    // The left thumb stick is the x and y axis
    // x is the left/right axis and y is the up/down axis
    setFloatField(env, stateCls, stateObject, "x", (jfloat)calcXInputPos(xs.Gamepad.sThumbLX));
    setFloatField(env, stateCls, stateObject, "y", (jfloat)calcXInputPos(xs.Gamepad.sThumbLY));
    // calculate the z-axis from the trigger axes
    setFloatField(env, stateCls, stateObject, "z", (jfloat)calcXInputZAxis(xs.Gamepad.bLeftTrigger, xs.Gamepad.bRightTrigger));
    setFloatField(env, stateCls, stateObject, "leftTrigger", (jfloat)calcXInputSingleTrigger(xs.Gamepad.bLeftTrigger));
    setFloatField(env, stateCls, stateObject, "rightTrigger", (jfloat)calcXInputSingleTrigger(xs.Gamepad.bRightTrigger));
    // the right thumb stick is the r and u axis
    // r is the up/down axis and u is the left/right
    setFloatField(env, stateCls, stateObject, "r", (jfloat)calcXInputPos(xs.Gamepad.sThumbRY));
    setFloatField(env, stateCls, stateObject, "u", (jfloat)calcXInputPos(xs.Gamepad.sThumbRX));
    // there is no v axis on an XInput controller
    // always is -1.0f
    setFloatField(env, stateCls, stateObject, "v", (jfloat)-1.0f);
    
    setFloatField(env, stateCls, stateObject, "pov", (jfloat)calcXInputPOV(xs.Gamepad.wButtons));
    
    setIntField(env, stateCls, stateObject, "buttonMap", (jint)xs.Gamepad.wButtons);
    
    // keep the boolean field to the maximum number of buttons
    // rather than the GCN_XINPUT_MAX_BUTTONS, because that is what the state field uses
    jboolean buttonStates[GCN_MAX_BUTTONS];
    
    // have to manually set these
    // if the mask is set, set the state to true, otherwise false
    buttonStates[GCN_XINPUT_A_BUTTON]=((xs.Gamepad.wButtons & XINPUT_GAMEPAD_A)==XINPUT_GAMEPAD_A);
    buttonStates[GCN_XINPUT_B_BUTTON]=((xs.Gamepad.wButtons & XINPUT_GAMEPAD_B)==XINPUT_GAMEPAD_B);
    
    buttonStates[GCN_XINPUT_X_BUTTON]=((xs.Gamepad.wButtons & XINPUT_GAMEPAD_X)==XINPUT_GAMEPAD_X);
    buttonStates[GCN_XINPUT_Y_BUTTON]=((xs.Gamepad.wButtons & XINPUT_GAMEPAD_Y)==XINPUT_GAMEPAD_Y);
    
    buttonStates[GCN_XINPUT_LEFT_SHOULDER_BUTTON]=((xs.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)==XINPUT_GAMEPAD_LEFT_SHOULDER);
    buttonStates[GCN_XINPUT_RIGHT_SHOULDER_BUTTON]=((xs.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)==XINPUT_GAMEPAD_RIGHT_SHOULDER);
    
    buttonStates[GCN_XINPUT_BACK_BUTTON]=((xs.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)==XINPUT_GAMEPAD_BACK);
    
    buttonStates[GCN_XINPUT_START_BUTTON]=((xs.Gamepad.wButtons & XINPUT_GAMEPAD_START)==XINPUT_GAMEPAD_START);
    
    buttonStates[GCN_XINPUT_LEFT_THUMB_BUTTON]=((xs.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)==XINPUT_GAMEPAD_LEFT_THUMB);
    buttonStates[GCN_XINPUT_RIGHT_THUMB_BUTTON]=((xs.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)==XINPUT_GAMEPAD_RIGHT_THUMB);

    // clear the rest
    int i;

    for(i=GCN_XINPUT_REST_OF_BUTTONS; i < GCN_MAX_BUTTONS; ++i)
    {
        buttonStates[i]=((jboolean)FALSE);
    }
    
    // set the field values
    setBooleanArrayField(env, stateCls, stateObject, "buttons", GCN_MAX_BUTTONS, buttonStates);
    
    // set whether connected
    setBooleanField(env, stateCls, stateObject, "connected", connected );
    
    // return whether controller is connected
    return connected;
           
}
