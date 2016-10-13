import xmlrpclib
from datetime import datetime, date, timedelta

cfgServer = 'http://daniels:8001'

#serialNum = '0000000309'
#serialNum = '97300100123456789'
searchStr = 'TP200686A-L-126'
#searchStr = 'STB-088'
# field [0] is Name
# field [2] is Serial Number
# field [16] is dut id
searchLoc = 0

ipAddr = 'ipaddr-'+str(datetime.today()).split(' ')[1]
#ipAddr = ''
print("fake ip address = %s" % ipAddr)

cfg = xmlrpclib.ServerProxy(cfgServer)
getVer = cfg.GetVersion()
print "GetVersion() = " + getVer
v1, v2, day, time = [int(n) for n in getVer.split('.')]
buildDate = date.fromordinal(day + date(2000,1,1).toordinal())
buildTime = str(timedelta(seconds=time*2))
print("Server version %d.%d, built %s at %s" % (v1, v2, buildDate, buildTime))

allStbs = cfg.SearchForStb("charlie",  ["Name==*"], False, True)[2]
print("%d STBs found by SearchForStb()" % len(allStbs))

stbs = [s for s in allStbs if s[searchLoc] == searchStr]
stbIds = [s[16] for s in stbs]
if stbs == []:
    print("ERROR: no STB found matching %s" % (searchStr))
else:
    for stbId in stbIds:
        print("STB found matching %s, stb id = %d" % (searchStr, stbId))
    if len(stbs) > 1:
        print("WARNING: %d STBs found matching %s" % (len(stbs), searchStr))

allStbs = cfg.GetStbDetails()[2]
#print allStbs

stbs = [s for s in allStbs if s[0] in stbIds]
#print stbs
for s in stbs:
    id, rack, slot = s[0], s[8], s[7]
    print("STB found matching %s in slot %d of rack %s (%d)" % (searchStr, slot, rack, id))
    cfg.UpdateDutInstance("charlie", id, {'ipAddress':ipAddr})
    print("IP address of STB %d is <%s>" % (id, cfg.GetDutDetails(id)[2]['ipaddress']))
