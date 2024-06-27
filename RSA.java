import java.math.BigInteger;

public class RSA {
	
	/*以下setter*/

    public BigInteger[] Setting(int bitLength, String YourMessage) {
        // 0. 素数 p と q を生成
    	BigInteger[] PandQ = generatePQ(bitLength);//通常は512
        BigInteger p = PandQ[0], q = PandQ[1];
        System.out.println("p: " + p + ", q: " + q);

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

        // 5. メッセージの設定
        String message = YourMessage;//"RSA Encryption Test"
        BigInteger messageBigInt = new BigInteger(message.getBytes());
        
        return new BigInteger[]{p, q, n, phi, e, d, messageBigInt};
    }
    
    /*以下getter*/
    
    // 暗号化メソッド
    public BigInteger encrypt(BigInteger message, BigInteger e, BigInteger n) {
        return message.modPow(e, n);
    }

    // 復号化メソッド
    public BigInteger decrypt(BigInteger encrypted, BigInteger d, BigInteger n) {
        return encrypted.modPow(d, n);
    }
    
    /*以下計算部*/
    
    // 素数 p と q を設定
    private static BigInteger[] generatePQ(int bitLength) {
    	BigInteger p = generatePrime(bitLength);
        BigInteger q = generatePrime(bitLength);
    	return new BigInteger[]{p, q};
    }

    // 指定されたビット数で素数を生成するメソッド
    private static BigInteger generatePrime(int bitLength) {
        BigInteger prime;

        // primeCertainty は素数判定の繰り返し回数で、値が大きいほど素数である確率が高くなる
        int primeCertainty = 100;

        while (true) {
            // 指定されたビット長に合わせてランダムなBigIntegerを生成
            prime = generateRandomBigInteger(bitLength);

            // 素数の候補が見つかったかチェック
            if (prime.isProbablePrime(primeCertainty)) {
                break;
            }
        }
        return prime;
    }

    // 指定されたビット長のランダムなBigIntegerを生成するメソッド
    private static BigInteger generateRandomBigInteger(int bitLength) {
        // 必要なビット数を32ビットのチャンクに分割して生成
        int numChunks = (bitLength + 31) / 32; // 32ビットのチャンクの数
        BigInteger result = BigInteger.ZERO;

        for (int i = 0; i < numChunks; i++) {
            int randomBits = new java.util.Random().nextInt();
            // チャンクを適切な位置にシフトして追加
            result = result.shiftLeft(32).or(BigInteger.valueOf(randomBits & 0xFFFFFFFFL));
        }

        // 指定されたビット長に切り詰める
        result = result.and(BigInteger.ONE.shiftLeft(bitLength).subtract(BigInteger.ONE));

        // 最上位ビットを1に設定して、ビット長を保証
        result = result.setBit(bitLength - 1);

        return result.abs(); // 正の数にする
    }
}
