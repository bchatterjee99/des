Compile and Run:

1. Encryption Executable
>> gcc encrypt.c ofb.c des.c -o enc

2. Put plaintext into "plaintex.txt", key into "key"

3. Encrypt (takes input from "plaintext.txt", "key", outputs ciphertext to "ciphertext")
>> ./enc


4. Decryption Executable
>> gcc decrypt.c ofb.c des.c -o dec

5. Decrypt "ciphertext" file (takes input from "ciphertext", "key", outputs decrypted text to stdout)
>> ./dec

