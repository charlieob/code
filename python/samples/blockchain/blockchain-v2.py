from hashlib import sha256

DIFFICULTY = 8

# import pprint

class block:
    def __init__(self, index, timestamp, data, previousHash = ''):
        self.index = index
        self.timestamp = timestamp
        self.data = data
        self.nonce = int(0)
        self.previousHash = previousHash
        self.hash = self.calculateHash()
        
    def __repr__(self):
        # return pprint.pformat(self.__dict__, indent=2)
        return 'block(index=' + repr(self.index) + ', timestamp=' + repr(self.timestamp) + ', data=' + repr(self.data) \
                + ', nonce = ' + repr(self.nonce) + ', previousHash = ' + self.previousHash + ', hash = ' + self.hash + ')'
        
    def calculateHash(self):
        h = sha256()
        s = str(self.index) + self.previousHash + self.timestamp + str(self.data) + str(self.nonce)
        h.update(s.encode('utf-8'))
        return h.hexdigest()
        
    def mineBlock(self, difficulty):
        while self.hash[0:difficulty] != '0' * difficulty:
            self.nonce += 1
            self.hash = self.calculateHash()
        print("Block mined: %s" % self.hash)

class blockchain:
    def __init__(self):
        self.chain = [self.createGenesisBlock()]
        self.difficulty = DIFFICULTY
        
    def __repr__(self):
        # return pprint.pformat(self.__dict__, indent=2)
        # return "'chain' : " + pprint.pformat(self.chain, indent=2)
        return repr(self.chain)
        
    def createGenesisBlock(self):
        return block(0, "01/01/2018", "Genesis block", "0")
        
    def getLatestBlock(self):
        return self.chain[-1]
        
    def addBlock(self, newblock):
        newblock.previousHash = self.getLatestBlock().hash
        newblock.mineBlock(self.difficulty)
        # newblock.hash = newblock.calculateHash()
        self.chain.append(newblock)
        
    def isChainValid(self):
        for i in range(len(self.chain))[1:]:
            currentBlock = self.chain[i]
            previousBlock = self.chain[i-1]
            if currentBlock.hash != currentBlock.calculateHash():
                return False
            if currentBlock.previousHash != previousBlock.hash:
                return False
        return True
        
# myCoin = blockchain()
# myCoin.addBlock(block(1, "08/01/2018", {"amount": 4}))
# myCoin.addBlock(block(2, "08/01/2018", {"amount": 8}))
# print "blockchain valid? %s" % myCoin.isChainValid()

# print myCoin

# myCoin.chain[1].data = {"amount": 100}
# print "blockchain valid? %s" % myCoin.isChainValid()
# myCoin.chain[1].hash = myCoin.chain[1].calculateHash()
# print "blockchain valid? %s" % myCoin.isChainValid()

# print myCoin
# pprint.pprint(myCoin, indent=2, width= 20)

import time
import datetime

myCoin = blockchain()

print("difficulty = %d" % DIFFICULTY)

print("Mining block 1...")
start_time = time.process_time()
myCoin.addBlock(block(1, datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"), {"amount": 14}))
elapsed = int(round(time.process_time() - start_time))
print("time taken = %d:%02d" % (elapsed/60, elapsed%60))

print("Mining block 2...")
start_time = time.process_time()
myCoin.addBlock(block(2, datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"), {"amount": 28}))
elapsed = int(round(time.process_time() - start_time))
print("time taken = %d:%02d" % (elapsed/60, elapsed%60))

print(myCoin)
