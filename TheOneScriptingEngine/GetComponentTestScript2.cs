using System;

public class GetComponentTestScript2 : MonoBehaviour
{
    public int life = 200;

    public override void Update()
    {
        if (Input.GetKeyboardButton(Input.KeyboardCode.K))
        {
            life -= 20;
        }
    }

    public void Testing()
    {
        Debug.Log("This is a test I am calling from TestScript2 and my current life is: " + life.ToString());
        life -= 50;
    }
}