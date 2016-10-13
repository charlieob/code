import sys
from datetime import datetime

import stormtest.ClientAPI as st

cfgServer = 'daniels'
stServer = 'daniels'
searchField = 'SerialNumber'
searchStr = '0000000309'

print "ST Client Version: " + st.GetClientVersion()

st.SetMasterConfigurationServer(cfgServer)

ipAddr = 'ipaddr-'+str(datetime.today()).split(' ')[1]
#ipAddr = ''
print("fake ip address = %s" % ipAddr)

error = False

facilityData = st.GetFacilityData(cfgServer)
if not facilityData:
    error = True
    print("No config server %s found" % (cfgServer))

if not error:
    serverNames = st.GetServerNames(facilityData)
    if stServer not in serverNames:
        error = True
        print("No server %s found in facility %s" % (stServer, cfgServer))

if not error:
    serverData = st.GetServerField(facilityData, stServer, 'all')
    slotCount = serverData['SlotCount']
    #print("Server %s has %d slots" % (stServer, slotCount))

    slot = 0
    for s in range(1, slotCount+1):
        stbData = st.GetStbField(facilityData, stServer, s, 'all')
        try:
            if stbData[searchField] == searchStr:
                slot = s
                dutId = stbData['Id']
                break
        except:
            pass

    if slot > 0:
        print("STB found matching %s in slot %d of rack %s (%d)" % (searchStr, slot, stServer, dutId))
        
        if ipAddr != '':
            print("setting IP address to %s" % ipAddr)
            st.UpdateDutInstance(dutId, ipaddress=ipAddr)
            # or this alternative form...
            #st.UpdateDutInstanceInSlot(stServer, slot, ipaddress=ipAddr)
            
        print("getting IP address from database...")
        ipAddr = st.GetDutDetails(dutId)['ipaddress']
        print("IP address is %s" % (ipAddr))
    else:
        print("No STB found matching %s in server %s" % (searchStr, stServer))
