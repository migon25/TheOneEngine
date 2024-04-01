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
}