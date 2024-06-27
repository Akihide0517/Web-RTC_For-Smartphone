import java.math.BigInteger;

public class Main {
    public static void main(String[] args) {
        // RSAインスタンスを作成
        RSA rsa = new RSA();

        // ビット長とメッセージを設定
        int bitLength = 512; // 例: 512ビット
        String message = "RSA Encryption Test";

        // RSAの設定を取得
        BigInteger[] rsaSettings = rsa.Setting(bitLength, message);
        BigInteger n = rsaSettings[2], e = rsaSettings[4], d = rsaSettings[5], messageBigInt = rsaSettings[6];

        System.out.println("Original Message: " + message);
        System.out.println("Original Message as BigInteger: " + messageBigInt);

        // メッセージを暗号化
        BigInteger encryptedMessage = rsa.encrypt(messageBigInt, e, n);
        System.out.println("Encrypted Message: " + encryptedMessage);

        // 暗号化されたメッセージを復号化
        BigInteger decryptedMessage = rsa.decrypt(encryptedMessage, d, n);
        System.out.println("Decrypted Message as BigInteger: " + decryptedMessage);

        // 復号化されたメッセージを文字列に変換
        String decryptedMessageString = new String(decryptedMessage.toByteArray());
        System.out.println("Decrypted Message: " + decryptedMessageString);
    }
}
