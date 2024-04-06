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
        name = "";

        transform = new ITransform();

        source = new ISource();
    }
    public IGameObject(IntPtr GOref) : base(GOref)
    {
        containerGOptr = GOref;
        name = InternalCalls.GetGameObjectName(containerGOptr);
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
            Debug.LogError("GameObject with name '" + name + "' not found.");

            return null;
        }

        IGameObject goToReturn = new IGameObject(foundGOptr);
        
        return goToReturn;
    }
    
    //Used to get any component except scripts
    public TComponent GetComponent<TComponent>() where TComponent : IComponent
    {
        Debug.Log("GetComponent called is: " + typeof(TComponent).ToString());

        IntPtr component = IntPtr.Zero;

        TComponent componentToReturn = null;

        if (Enum.TryParse(typeof(TComponent).ToString(), out IComponent.ComponentType type))
        {
            Debug.LogCheck("The GetType of the class is an internal class");
            component = InternalCalls.ComponentCheck(containerGOptr, (int)type);

            if (component != IntPtr.Zero) 
            {
                componentToReturn = new IComponent(containerGOptr) as TComponent;
            }
        }
        else if (typeof(TComponent).IsSubclassOf(typeof(MonoBehaviour)))
        {
            Debug.LogCheck("The GetType of the class is a script");
            component = InternalCalls.ComponentCheck(containerGOptr, (int)IComponent.ComponentType.MonoBehaviour, typeof(TComponent).ToString());

            if (component != IntPtr.Zero)
            {
                Debug.LogCheck("Component was found in the C++ game object");
                componentToReturn = InternalCalls.GetScript<TComponent>(containerGOptr, typeof(TComponent).ToString());
            }
        }

        
        if (componentToReturn != null)
        {
            Debug.LogCheck("Component class to return: " + componentToReturn.ToString());
            return componentToReturn;
        }
        else
        {
            Debug.LogError("Error finding component '" + typeof(TComponent).ToString() + "' in GameObject '" + name + "'. Check if the component exists in the Game Object.");
            return new IComponent() as TComponent;
        }
    }
}