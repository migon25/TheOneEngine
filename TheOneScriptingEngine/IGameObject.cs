using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;

public class IGameObject
{
    private IntPtr containerGOptr;

    public string name;
    public bool isActive;
    public ITransform transform;
    public ISource source;

    public IGameObject()
    {
        containerGOptr = InternalCalls.GetGameObjectPtr();

        transform = new ITransform(containerGOptr);

        source = new ISource(containerGOptr);
    }

    public void Destroy()
    {
        InternalCalls.DestroyGameObject(containerGOptr);
    }
}