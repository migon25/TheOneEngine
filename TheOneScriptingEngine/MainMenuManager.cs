﻿using System;

public class MainMenuManager : MonoBehaviour
{
    public ICanvas canvas;
    float cooldown = 0;
    bool onCooldown = false;

    public MainMenuManager()
    {
        canvas = new ICanvas(InternalCalls.GetGameObjectPtr());
    }
    public override void Update()
    {
        float dt = InternalCalls.GetAppDeltaTime();
        bool toMove = false;
        int direction = 0;

        //Keyboard
        if (Input.GetKeyboardButton(Input.KeyboardCode.UP))
        {
            toMove = true;
            direction = -1;
            attachedGameObject.source.PlayAudio(AudioManager.EventIDs.HOVER);
        }

        if (Input.GetKeyboardButton(Input.KeyboardCode.DOWN))
        {
            toMove = true;
            direction = 1;
            attachedGameObject.source.PlayAudio(AudioManager.EventIDs.HOVER);
        }

        //Controller
        Vector2 movementVector = Input.GetControllerJoystick(Input.ControllerJoystickCode.JOY_LEFT);

        if (movementVector.y != 0.0f)
        {
            if(movementVector.y > 0.0f)
            {
                toMove = true;
                direction = 1;
                attachedGameObject.source.PlayAudio(AudioManager.EventIDs.HOVER);
            }
            else if(movementVector.y < 0.0f)
            {
                toMove = true;
                direction = -1;
                attachedGameObject.source.PlayAudio(AudioManager.EventIDs.HOVER);
            }
        }

        if (onCooldown && cooldown < 0.2f)
        {
            cooldown += dt;
        }
        else
        {
            cooldown = 0.0f;
            onCooldown = false;
        }

        // Select Button
        if(toMove && !onCooldown)
        {
            onCooldown = true;
            canvas.MoveSelection(direction);
        }


        // Selection Executters
        if ((Input.GetControllerButton(Input.ControllerButtonCode.X) || Input.GetKeyboardButton(Input.KeyboardCode.RETURN)) && canvas.GetSelection() == 0)
        {
            SceneManager.LoadScene("Level1");
            attachedGameObject.source.PlayAudio(AudioManager.EventIDs.CLICK);
        }

        if ((Input.GetControllerButton(Input.ControllerButtonCode.X) || Input.GetKeyboardButton(Input.KeyboardCode.RETURN)) && canvas.GetSelection() == 3)
        {
            InternalCalls.ExitApplication();
            attachedGameObject.source.PlayAudio(AudioManager.EventIDs.CLICK);
        }
    }
}