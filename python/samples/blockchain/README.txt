https://github.com/SavjeeTutorials/SavjeeCoin

Rewrote his example code (javascript) in python.

$ python -u blockchain-v2.py
difficulty = 8
Mining block 1...
Block mined: 00000000bde34b08a6a99eab65e0a2511cd0f6bb5098f143d0b60ec5d7ddd39b
time taken = 453:38
Mining block 2...
Block mined: 00000000f2d87e5a100a985c09a8a64f24786ce2e99b0ca5c274dc774d049aa3
time taken = 114:38
[block(index=0, timestamp='01/01/2018', data='Genesis block', nonce = 0L, previousHash = 0, hash = 6a26925d3118f38f7bea9e9bbaf7edc7fc0ee67c64385325a16957809256e27f), block(index=1, timestamp='2018-01-09 18:27:17', data={'amount': 14}, nonce = 6854674659L, previousHash = 6a26925d3118f38f7bea9e9bbaf7edc7fc0ee67c64385325a16957809256e27f, hash = 00000000bde34b08a6a99eab65e0a2511cd0f6bb5098f143d0b60ec5d7ddd39b), block(index=2, timestamp='2018-01-10 02:00:55', data={'amount': 28}, nonce = 1732332129L, previousHash = 00000000bde34b08a6a99eab65e0a2511cd0f6bb5098f143d0b60ec5d7ddd39b, hash = 00000000f2d87e5a100a985c09a8a64f24786ce2e99b0ca5c274dc774d049aa3)]

With difficulty=8, mining a hash took over 7 hours in one case with this simple script.
The nonce incremented to over 6 billion which is greater than maxint at 32 bits (2147m)
Other typical mining times:
difficulty = 5   up to 10 secs
difficulty = 6   up to  2 mins
difficulty = 7   up to 60 mins
Note: variation in the mining times is very large
