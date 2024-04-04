using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

public class IGameObject : IObject
{
    public ITransform transform;
    public ISource source;

    public IGameObject() : base()
    {
        transform = new ITransform();

        source = new ISource();
    }
    public IGameObject(IntPtr GOref) : base(GOref)
    {
        containerGOptr = GOref;
        transform = new ITransform(GOref);

        source = new ISource(GOref);
    }

    public void Destroy()
    {
        InternalCalls.DestroyGameObject(containerGOptr);
    }

    public static IGameObject Find(string name)
    {
        IntPtr foundGOptr = InternalCalls.FindGameObject(name);

        if(foundGOptr == IntPtr.Zero)
        {
            //Insert error or something

            return new IGameObject();
        }

        IGameObject goToReturn = new IGameObject(foundGOptr);
        
        return goToReturn;
    }
    
    //Used to get any component except scripts
    public TComponent GetComponent<TComponent>() where TComponent : IComponent
    {
        Debug.Log("GetComponent called is: " + typeof(TComponent).ToString());

        IntPtr component = IntPtr.Zero;

        if(Enum.TryParse(typeof(TComponent).ToString(), out IComponent.ComponentType type)) 
        {
            component = InternalCalls.ComponentCheck(containerGOptr, (int)type);
        }

        if(component == IntPtr.Zero) 
        {
            Debug.Log("GetComponent called is: " + typeof(TComponent).ToString());
            return null;
        }

        TComponent componentToReturn = new IComponent(containerGOptr) as TComponent;


        Debug.Log("Component class to return: " + componentToReturn.ToString());

        if (componentToReturn != null)
            return componentToReturn;

        return null;
    }
}