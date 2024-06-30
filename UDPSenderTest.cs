using System;
using UnityEngine;
using System.Collections;

public class UdpSenderTest : MonoBehaviour
{
    void Start(){
      Main();
    }

    static void Main()
    {
        // データを送信するリモートホストとポート番号
        string remoteHost = "ip";
        int remotePort = 1024;
        
        System.Net.Sockets.UdpClient udp = new System.Net.Sockets.UdpClient();

        // 送信するデータ（x, y, zの値）
        int x = 100; // x座標
        int y = -200; // y座標
        int z = 0; // ボタンの状態

        // データを "x, y, z" の形式にフォーマットする
        string sendMsg = $"{x}, {y}, {z}";
        byte[] sendBytes = System.Text.Encoding.UTF8.GetBytes(sendMsg);

        udp.Send(sendBytes, sendBytes.Length, remoteHost, remotePort);
        udp.Close();

        Console.WriteLine($"UDPデータを {remoteHost}:{remotePort} に送信 -> {sendMsg}");
        Console.WriteLine("終了");
        Console.ReadLine();
    }
}
