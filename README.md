# The Challenge

Create a program that, when given a message from a file and an integer key, encrypts the message by adding the ASCII number for each character to the character's index and to the key and then converts that sum to binary. The program should also be able to decrypt any encrypted messages it creates. Make the program as fast as possible in the language of your choice.

Read a walkthrough of my solution [here](https://samueldavis.net/encryption-challenge/) 

# Usage

## Encryption

To encrypt, specify your input and output files. Then use the `-e` flag with your key:

```
./bincrypt -e <key> -i <input_file> -o <output_file>
```

For a verbose output during encryption, use the `-v` flag:

```
./bincrypt -e <key> -i <input_file> -o <output_file> -v
```

## Decryption

To decrypt, again, specify your input and output files. Then use the `-d` flag with your key:

```
./bincrypt -d <key> -i <input_file> -o <output_file>
```

For a verbose output during encryption, use the `-v` flag:

```
./bincrypt -d <key> -i <input_file> -o <output_file> -v
```
