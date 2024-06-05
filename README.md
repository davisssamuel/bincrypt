# Encryption Challenge

Create a program that, when given a message from a file and an integer key, encrypts the message by adding the ASCII number for each character to the character's index and the key and then converts that sum to binary.

The program should also be able to decrypt any encrypted messages it creates.

Try to make the program as fast as possible in the language of your choice.

# Solution

I chose to write my solution in C so I could reasonably squeeze as much speed out of it as possible.

The first version of this program used text files containing ASCII 1s and 0s, but I moved away from that to use literal binary files which sped up the program a lot and made the algorithm much simpler.
