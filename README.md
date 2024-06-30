### スマートフォンをVCやOculusで操作できるようにする計画
目的:オキュラスからスマホを遠隔操作したいぃぃぃ、、、

(本プログラムの.inoはESP32-BLE-Mouse-master.zipライブラリとESP32が必要です)

## 動作確認済み項目
・ESP32-BleController-UDPver-Grobal.ino ...ESP32をマウスとしてスマホに接続→コントロールする。座標送信手段はUDP。

・UDP-SendTest.java ...UDP通信で接続テストを行うためのコード。

・UDPSenderTest.cs ...UDP通信でOculusを使った接続テストを行うためのコード。

・RSA系 ...ワンタイムパスワードに関するコード。

・その他 ...大抵は上にあげたコードの個別機能の実装テストです。(あくまでアーカイブとして残してある)

## 参考サイト
・.NET Tips ...MITライセンス

・CRAFT GoGo.(https://craft-gogo.com/m5stack-udp/) ...このサイトを参考に作成したプログラムの内、ほぼそのままであるESP32-UDP-Receiver-Global.inoについては一切の著作権を放棄します。（ライセンスが不明なので）

・monoqy blog(http://monqy.blogspot.com) ...このサイトを参考に作成したプログラムの内、ほぼそのままであるUDP_SendTest.javaについては一切の著作権を放棄します。（ライセンスが不明なので）
