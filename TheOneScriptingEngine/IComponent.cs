using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class IComponent : IObject
{
	public enum ComponentType
	{

	}

   public IComponent() : base() { }
   public IComponent(IntPtr gameObjectRef) : base(gameObjectRef) { }
}
