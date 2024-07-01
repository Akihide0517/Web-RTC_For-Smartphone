using System;
using UnityEngine;
using System.Collections;

public class SmartPhone_UDPSender_ForOculus : MonoBehaviour
{
    private System.Net.Sockets.UdpClient udp;
    private string remoteHost = "ipv4 here!";
    private int remotePort = 1024;

    void Start()
    {
        //udp = new System.Net.Sockets.UdpClient();
    }

    void Update()
    {
        udp = new System.Net.Sockets.UdpClient();
        SendJoystickData();
        udp.Close();
    }

    void OnDestroy()
    {
        udp.Close();
    }

    private void SendJoystickData()
    {
        // Get joystick values
        Vector2 vectorR = OVRInput.Get(OVRInput.RawAxis2D.RThumbstick);

        // Convert joystick values to int for transmission
        int x = (int)(vectorR.x * 100);
        int y = (int)(vectorR.y * 100);

        int z = UpdateButtonState(OVRInput.RawButton.RIndexTrigger, OVRInput.RawTouch.RIndexTrigger); // Placeholder for button state if needed
        if(z == 0){
          z = UpdateButtonState(OVRInput.RawButton.LIndexTrigger, OVRInput.RawTouch.LIndexTrigger);
        }

        // Format data as "x, y, z"
        string sendMsg = $"{x}, {y}, {z}";
        byte[] sendBytes = System.Text.Encoding.UTF8.GetBytes(sendMsg);

        udp.Send(sendBytes, sendBytes.Length, remoteHost, remotePort);

        Debug.Log($"UDPデータを {remoteHost}:{remotePort} に送信 -> {sendMsg}");
    }

    private int UpdateButtonState(OVRInput.RawButton button, OVRInput.RawTouch touch)
    {
        int args = 0;

        if (OVRInput.Get(button))
        {
            // Button is pressed
            args = 1;
        }
        else
        {
            // Button is neither pressed nor touched
            args = 0;
        }

        return args;
    }
}
