import java.math.BigInteger;
import java.util.Random;

public class rsa_main {

    public static void main(String[] args) {
        // 素数 p と q を生成
        BigInteger p = generatePrime(512);
        BigInteger q = generatePrime(512);
        System.out.println("p: " + p);
        System.out.println("q: " + q);

        // 1. n = p * q を計算
        BigInteger n = p.multiply(q);
        System.out.println("n: " + n);

        // 2. φ(n) を計算
        BigInteger phi = (p.subtract(BigInteger.ONE)).multiply(q.subtract(BigInteger.ONE));
        System.out.println("φ(n): " + phi);

        // 3. 公開鍵 e を選択
        BigInteger e = BigInteger.valueOf(65537); // 通常は 65537 を使用
        System.out.println("e: " + e);

        // 4. 秘密鍵 d を計算 (d * e ≡ 1 (mod φ(n)) を満たす d を求める)
        BigInteger d = e.modInverse(phi);
        System.out.println("d: " + d);

        // メッセージを暗号化し、復号化する
        String message = "RSA Encryption Test";
        BigInteger messageBigInt = new BigInteger(message.getBytes());

        // 暗号化
        BigInteger encrypted = encrypt(messageBigInt, e, n);
        System.out.println("暗号化されたメッセージ: " + encrypted);

        // 復号化
        BigInteger decrypted = decrypt(encrypted, d, n);
        String decryptedMessage = new String(decrypted.toByteArray());
        System.out.println("復号化されたメッセージ: " + decryptedMessage);
    }

    // 指定されたビット数で素数を生成するメソッド
    private static BigInteger generatePrime(int bitLength) {
        Random random = new Random();
        BigInteger prime;

        // primeCertaintyは素数判定の繰り返し回数で、値が大きいほど素数である確率が高くなる
        int primeCertainty = 100;

        while (true) {
            prime = new BigInteger(bitLength, random);
            if (prime.isProbablePrime(primeCertainty)) {
                break;
            }
        }
        return prime;
    }

    // 暗号化メソッド
    private static BigInteger encrypt(BigInteger message, BigInteger e, BigInteger n) {
        return message.modPow(e, n);
    }

    // 復号化メソッド
    private static BigInteger decrypt(BigInteger encrypted, BigInteger d, BigInteger n) {
        return encrypted.modPow(d, n);
    }
}
