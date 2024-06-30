import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;

public class UDP_SendTest {
    public static void main(String[] args) throws IOException {
        // 送信先のホストとポート
        String host = "0.0.0.0"; // ここに送信先のホストを指定
        int port = 1024; // ここに送信先のポートを指定

        // 送信回数
        int count = 1; // 送信する回数を指定

        // 送信するデータ（x, y, zの値）
        int x = 10; // x座標
        int y = 20; // y座標
        int z = 30; // 追加のデータ（例: ボタンの状態など）

        // データを "x, y, z" の形式にフォーマットする
        String sendData = x + "," + y + "," + z;
        byte[] data = sendData.getBytes("UTF-8");

        DatagramSocket sock = new DatagramSocket();

        for (int i = 0; i < count; i++) {
            DatagramPacket packet = new DatagramPacket(data, data.length, new InetSocketAddress(host, port));
            sock.send(packet);
            System.out.println("Sent UDP data to " + host + ":" + port + " -> " + sendData);

            try {
                Thread.sleep(1000); // .1秒待機
            } catch (InterruptedException e) {
                System.out.println("Thread interrupted: " + e.getMessage());
            }
        }
        sock.close();
    }
}
