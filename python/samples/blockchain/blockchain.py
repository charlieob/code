from hashlib import sha256

# import pprint

# h = sha256("a string")
# print "type of sha256 = %s" % type(h)
# print "dir(sha256) = %s" % dir(h)
# print "sha256.digestsize = %d bytes" % h.digestsize
# print "sha256.hexdigest() = %d hex digits" % len(h.hexdigest())
# print "type of sha256.hexdigest() = %s" % type(h.hexdigest())
# print h.hexdigest()
# h.update("a new string")
# print h.hexdigest()

class block:
    def __init__(self, index, timestamp, data, previousHash = ''):
        self.index = index
        self.timestamp = timestamp
        self.data = data
        self.previousHash = previousHash
        self.hash = self.calculateHash()
        
    def __repr__(self):
        # return pprint.pformat(self.__dict__, indent=2)
        return 'block(index=' + repr(self.index) + ', timestamp=' + repr(self.timestamp) + ', data=' + repr(self.data) \
                + ', previousHash = ' + self.previousHash + ', hash = ' + self.hash + ')'
        
    def calculateHash(self):
        h = sha256()
        h.update(str(self.index) + self.previousHash + self.timestamp + str(self.data))
        return h.hexdigest()
        
class blockchain:
    def __init__(self):
        self.chain = [self.createGenesisBlock()]
        
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
        newblock.hash = newblock.calculateHash()
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
        
myCoin = blockchain()
myCoin.addBlock(block(1, "08/01/2018", {"amount": 4}))
myCoin.addBlock(block(2, "08/01/2018", {"amount": 8}))
print "blockchain valid? %s" % myCoin.isChainValid()

print myCoin

myCoin.chain[1].data = {"amount": 100}
print "blockchain valid? %s" % myCoin.isChainValid()
myCoin.chain[1].hash = myCoin.chain[1].calculateHash()
print "blockchain valid? %s" % myCoin.isChainValid()

print myCoin
# pprint.pprint(myCoin, indent=2, width= 20)
