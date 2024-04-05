using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class GetComponentTestScript : MonoBehaviour
{
    IGameObject getGoTest;

    ITransform testTransform;
    IParticleSystem ps;

    GetComponentTestScript2 testScript;

    public override void Start()
    {
        getGoTest = IGameObject.Find("SM_Box_LowPoly");

        testScript = getGoTest.GetComponent<GetComponentTestScript2>();
    }

    public override void Update()
    {
        if (Input.GetKeyboardButton(Input.KeyboardCode.SPACEBAR))
        {
            testScript.Testing();
        }
    }
}
