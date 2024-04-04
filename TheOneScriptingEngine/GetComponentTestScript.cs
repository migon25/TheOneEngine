using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class GetComponentTestScript : MonoBehaviour
{
    IGameObject getGoTest;
    GetComponentTestScript2 getComponentTestInterface;
    AdultXenomorphBehaviour getComponentTestCustomClass;
    ICanvas canvas;

    public override void Start()
    {
        getGoTest = IGameObject.Find("SM_Box_LowPoly");
    }

    public override void Update()
    {
        if (Input.GetKeyboardButton(Input.KeyboardCode.SPACEBAR))
        {
            Debug.Log("Testing Game Object name is: " + getGoTest.name);
            //Debug.Log("Testing GetComponent with Transform X is: " + getComponentTestInterface.position.x.ToString());
            //Debug.Log("Testing GetComponent with Xenomorph health is: " + getComponentTestCustomClass.life.ToString());
            ITransform testTransform = getGoTest.GetComponent<ITransform>();
            ICanvas testCanvas = getGoTest.GetComponent<ICanvas>();

            //Debug.Log("Result transform is: " + getComponentTestInterface.life);
        }
    }
}
