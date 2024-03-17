using System;
using System.Runtime.CompilerServices;

public class AudioManager
{
    public enum EventIDs : uint
    {
        GUNSHOT = 1287408361U,  
        MUSIC1 = 1730564819U,
        STEP = 621108255U,
    }

    public static void PlaySource(IntPtr GOptr, EventIDs audio)
    {
        InternalCalls.PlaySource(GOptr, audio);
    }

    public static void StopSource(IntPtr GOptr, EventIDs audio)
    {
        InternalCalls.StopSource(GOptr, audio);
    }
}
