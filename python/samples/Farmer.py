#!/usr/bin/python
#############################################################################
#  Copyright (c) 2010 Silicon & Software Systems Limited
#  Whelan House, South County Business Park,
#  Leopardstown, Dublin 18, Ireland
#
#  All Rights Reserved
#
#  P R O P R I E T A R Y    &    C O N F I D E N T I A L
#
#  The copyright of this document is exclusively vested in Silicon & Software Systems Limited, without whose prior
#  written permission its contents must not be published,  adapted or reproduced in any form or by any means or
#  disclosed or issued to any third party.
#  -----------------------------------------------------
#  http://www.s3group.com
#  -----------------------------------------------------
#
#  This script checks stb's in stormtest racks.
#
###############################################################################
#windows service
import pythoncom
import win32serviceutil
import win32service
import win32event
import servicemanager

#external lock
import string,cgi,time
import threading
import time
from os import curdir, sep
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer

#farmer
import xmlrpclib
from stormtest.ClientAPI import *
import time
###########################################################
## Farmer
###########################################################

#Add Samsung to slot 2

MasterServer="http://stormtest12:8001/"
Current_server_name="None"
#This function parses serial log
STB_ARRAY=[["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3],["NA","NA",3]]
def SerialLogParser(slotNo,a_key,a_string):
    global STB_ARRAY
    global Current_server_name
    #get stb ip addresss
    if a_key=="inet addr:":
        RemoveObserve("inet addr:",slotNo)
        STB_ARRAY[slotNo-1][1]= a_string.split(':')[3].split(' ')[0]
        STB_ARRAY[slotNo-1][2]=STB_ARRAY[slotNo-1][2]-1
        print str(slotNo) +": IP detected"
    #look for stb type
    if a_key=="STB_TYPE:":
        RemoveObserve("STB_TYPE:",slotNo)
        STB_ARRAY[slotNo-1][0]=a_string.split('STB_TYPE:')[1].split('\r')[0]
        STB_ARRAY[slotNo-1][2]=STB_ARRAY[slotNo-1][2]-1
        print str(slotNo) +": Type detected"
    #check if boxes have 2 partitions
    if a_key==" sda: sda1 sda2":
        RemoveObserve(" sda: sda1 sda2",slotNo)
        STB_ARRAY[slotNo-1][2]=STB_ARRAY[slotNo-1][2]-1
        print str(slotNo) +": 2 Patritions detected"
    #boxes with have zero checks left will be added to slots. If you want to add stb's earlier modify below line
    if STB_ARRAY[slotNo-1][2]==0:
        AddStbToSlot(Current_server_name,slotNo,STB_ARRAY[slotNo-1][0],STB_ARRAY[slotNo-1][1])
        print "Stb " +str(slotNo)+" "+STB_ARRAY[slotNo-1][0]+" " +STB_ARRAY[slotNo-1][1]+" Ready"
  
def CheckSTB(rack,id,timeout=60):
    global STB_ARRAY
    #stb type
    
    STB_ARRAY[id-1][0]="NA"
    #stb ip address
    STB_ARRAY[id-1][1]="NA"
    STB_ARRAY[id-1][2]=3
    #this bit is part of retriving stb info
    status=ReserveSlot(id,"Sky+HD_remote.txt",[115200, 8, "none", 1, "none"],False)
    #if id <>6:
    if status == True:
        AddObserve ("STB_TYPE:",SerialLogParser,id,waitForEOL = True)
        AddObserve (" sda: sda1 sda2",SerialLogParser,id,waitForEOL = True)
        AddObserve ("inet addr:",SerialLogParser,id,waitForEOL = True)
    
#This function add stb to slot
def AddStbToSlot(server_name,slot_number,stb_type,description):
    global MasterServer
    
    proxy = xmlrpclib.ServerProxy(MasterServer)
    #server_name = proxy.GetServerList()[2][1][0]
    server_id = proxy.GetServerId(server_name)[2]
    #Check model and add if doesnt exist
    stb_models=proxy.GetStbModelInfo()[2]
    model_id=-1
    for model in range(0,len(stb_models)):
        if stb_models[model][0]==stb_type:
            model_id=stb_models[model][3]
    if model_id==-1:
        print "Model '"+stb_type+"' not found, Add model on stormtest admin web page first."
    print proxy.CreateDutInstanceByModel("stormtestuser",server_id,slot_number,model_id,stb_type+" ready",description,"NA");

#This function remove stb from slot    
def RemoveStbFromSlot(server_name,slot_number):
    global MasterServer
    proxy = xmlrpclib.ServerProxy(MasterServer)
    DutInstance = proxy.GetSlotIdFromAddress(server_name,slot_number)[2]
    DutInstanceId =proxy.GetDutInstance(DutInstance)
    print proxy.DeleteDutInstance("stormtestuser",DutInstanceId[2][0])

#This function checks all slots with doesnt have stb's    
def CheckAllStbs(timeout=120):
    global MasterServer
    global Current_server_name
    
    proxy = xmlrpclib.ServerProxy(MasterServer)
    ServerList = proxy.GetServerList()[2]
    for server_number in range(0,len(ServerList)):
        server_list = proxy.GetServerList()[2][server_number]
        server_name = server_list[0]
        slot_list = server_list[18]
        Current_server_name=server_name
        try:
            ConnectToServer(server_name,"Marcinw")
        except:
            break
        Number_of_stbs_to_check=0
        for slot in slot_list:
            #print slot
            slot_number = slot[0]
            stb_name = slot[2]
            stb_ip_address = slot[3]
            stb_type = slot[5]
            print "Checking server: " + server_name +" slot_"+ str(slot_number)
            #DutInstance = proxy.GetSlotIdFromAddress(server_name,slot_number)[2]
            #print DutInstance
            #DutInstanceId =proxy.GetDutInstance(DutInstance)
            #print DutInstanceId
            if stb_name=="":
                print "Slot requeres check:" +server_name + " slot_" + str(slot_number)
                try:
                    CheckSTB(server_name,slot_number)
                    Number_of_stbs_to_check=Number_of_stbs_to_check+1
                except:
                    print "Unable to check stb "+server_name + " slot_" + str(slot_number)
            else:
                STB_ARRAY[slot_number-1][0]=stb_type
                STB_ARRAY[slot_number-1][1]=stb_ip_address
        
        if Number_of_stbs_to_check<>0:

            #Powercycle all stb's added for check
            PowerOffSTB(5)
            PowerOnSTB(5)
            print "Waiting for stb's to report"       
            time.sleep(timeout)
            print STB_ARRAY
            print "Ekofrendly mode. Turning stb's off after test"
            PowerOffSTB(5)
            
        else:
            print "No stb's to check on server: "+ server_name
            print STB_ARRAY
        ReleaseServerConnection()

def CheckAllStbsLoop():
    while True:
        CheckAllStbs()
        print "Sleep for 5s"
        time.sleep(5)

def start_farmer():        
    farmer_task = threading.Thread(target=CheckAllStbsLoop)
    farmer_task.start()

###########################################################
## External locker. This part will be removed after moving to stormtest sheduller
###########################################################
Lock=threading.Lock()
external_lock=False
class MyHandler(BaseHTTPRequestHandler):
    
    def do_GET(self):
        global external_lock
        try:
            if self.path.endswith("lock"):   #our dynamic content
                self.send_response(200)
                self.send_header('Content-type',	'text/html')
                self.end_headers()
                Lock.acquire()
                if external_lock==False:
                    external_lock=True
                    self.wfile.write("Locked. lock will be released automaticaly after 10 minutes")
                else:
                    self.wfile.write("Failed to lock try again")
                Lock.release()
                return
            if self.path.endswith("release"):   #our dynamic content
                self.send_response(200)
                self.send_header('Content-type',	'text/html')
                self.end_headers()
                Lock.acquire()
                if external_lock==True:
                    external_lock=False
                    self.wfile.write("Unlocked.")
                else:
                    self.wfile.write("Failed to unlock.")
                Lock.release()
                return                
            return
                
        except IOError:
            self.send_error(404,'File Not Found: %s' % self.path)
#this is to protect against one process blocking everything
def release_lock_timeout():
    global external_lock
    while True:
        Lock.acquire()
        if external_lock==True:
            external_lock=False
            print "Lock timeout reached. releasing lock"
        Lock.release()
        time.sleep(60*10)
def http_server():
    server = HTTPServer(('', 8123), MyHandler)
    server.serve_forever()
    
def start_external_lock():
    external_lock_task0 = threading.Thread(target=release_lock_timeout)
    external_lock_task0.start()
    external_lock_task1 = threading.Thread(target=http_server)
    external_lock_task1.start()
    


#def stop_external_lock():    
#    socket.close()
#    external_lock_task0.stop()

###########################################################
## Service
###########################################################
class AppServerSvc (win32serviceutil.ServiceFramework):
    _svc_name_ = "StormTestFarmer"
    _svc_display_name_ = "StormTest Farmer"
    _svc_description = "Checks all empty slots in rack."
    def __init__(self,args):
        win32serviceutil.ServiceFramework.__init__(self,args)
        self.hWaitStop = win32event.CreateEvent(None,0,0,None)
        socket.setdefaulttimeout(60)

    def SvcStop(self):
        self.ReportServiceStatus(win32service.SERVICE_STOP_PENDING)
        win32event.SetEvent(self.hWaitStop)

    def SvcDoRun(self):
        servicemanager.LogMsg(servicemanager.EVENTLOG_INFORMATION_TYPE,
                              servicemanager.PYS_SERVICE_STARTED,
                              (self._svc_name_,''))
        self.main()

    def main(self):
        start_external_lock()
        start_farmer()

if __name__ == '__main__':
    #win32serviceutil.HandleCommandLine(AppServerSvc)
    start_external_lock()
    start_farmer()
