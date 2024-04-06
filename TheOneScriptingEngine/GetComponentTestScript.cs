using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class GetComponentTestScript : MonoBehaviour
{
    IGameObject getGoTest;
    IGameObject getGoTest2;

    //ITransform testTransform;
    //IParticleSystem ps;

    GetComponentTestScript2 testScript;
    GetComponentTestScript2 testScript2;

    public override void Start()
    {
        getGoTest = IGameObject.Find("SM_Box_LowPoly");
        getGoTest2 = IGameObject.Find("Empty GameObject");

        //testTransform = getGoTest.GetComponent<ITransform>();
        //ps = getGoTest.GetComponent<IParticleSystem>();
        testScript = getGoTest.GetComponent<GetComponentTestScript2>();
        testScript2 = getGoTest2.GetComponent<GetComponentTestScript2>();
    }

    public override void Update()
    {
        if (Input.GetKeyboardButton(Input.KeyboardCode.SPACEBAR))
        {
            testScript.ReduceLife1();
            testScript2.ReduceLife2();
        }
    }
}
