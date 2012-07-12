#include <jni.h>
#include "com_zairon_GameControllerEngine_GameControllerNatives.h"
#include <stdio.h>
#include <windows.h>

/// The maximum number of buttons that can be tested
#define GCN_MAX_BUTTONS 32

/// The maximum value for a joystick axis
#define GCN_MAXIMUM_AXIS ((jfloat)65535.0)

/// Half of the maximum value for a joystick axis
#define GCN_HALF_AXIS ((jfloat)32767.5)

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
    setFloatField(env, stateCls, stateObject, "y", (jfloat)calcJoyPos(ji.dwYpos));
    setFloatField(env, stateCls, stateObject, "z", (jfloat)calcJoyPos(ji.dwZpos));
    setFloatField(env, stateCls, stateObject, "r", (jfloat)calcJoyPos(ji.dwRpos));
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
