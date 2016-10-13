
import win32service
import win32serviceutil
import servicemanager
import win32event
import time
import sys
import os
from optparse import OptionParser
import socket
import stormtest.ClientAPI as st
import _winreg

# Port to listen on for STB Discovery messages
DEFAULT_LISTEN_PORT=50000

class StbDiscoveryException(Exception):
    pass

class ConfigServerException(Exception):
    """ Error communicating with the config server """
    pass

class Log(object):

    def __init__(self, log_level='error'):
        if log_level not in ['error', 'warn', 'info', 'debug']:
            raise StbDiscoveryException("Invalid log level: %s" % log_level)
        if log_level == 'error':
            self._log_levels = ['error']
        if log_level == 'warn':
            self._log_levels = ['error', 'warn']
        if log_level == 'info':
            self._log_levels = ['error', 'warn', 'info']
        if log_level == 'debug':
            self._log_levels = ['error', 'warn', 'info', 'debug']

class LogConsole(Log):
    """ Log messages to the console """

    def __init__(self, log_level):
        Log.__init__(self, log_level)

    def error(self, msg):
        if 'error' in self._log_levels:
            print "ERROR: %s" % msg

    def warn(self, msg):
        if 'warn' in self._log_levels:
            print "WARN: %s" % msg

    def info(self, msg):
        if 'info' in self._log_levels:
            print "INFO: %s" % msg

    def debug(self, msg):
        if 'debug' in self._log_levels:
            print "DEBUG: %s" % msg

class LogWinEvent(Log):
    """ Log messages to the windows event log """

    def __init__(self, log_level):
        Log.__init__(self, log_level)

    def error(self, msg):
        if 'error' in self._log_levels:
            servicemanager.LogErrorMsg(msg)

    def warn(self, msg):
        if 'warn' in self._log_levels:
            servicemanager.LogWarningMsg(msg)

    def info(self, msg):
        if 'info' in self._log_levels:
            servicemanager.LogInfoMsg(msg)

    def debug(self, msg):
        if 'debug' in self._log_levels:
            servicemanager.LogInfoMsg(msg)

class StormFacilityData(object):

    _MAX_CACHE_AGE = 7.0 # seconds

    def _print_cache(self):
        for key,val in self._config_cache.items():
            print("dut_id: %s : %s" % (key, val))

    def _get_server_nslots(self, facility, server_name):
        """ Get number of slots in a storm server """

        server = st.GetServerField(facility, server_name, 'all')
        if not server:
            raise ConfigServerException("Unable to retrieve server fields")

        return server['SlotCount']

    def _update_config_cache(self):

        """ Maintain the config cache """
        
        # The config cache is a time aged cache - if it's more than
        # _MAX_CACHE_AGE seconds old, we throw it away, and get a new one.
        # This serves a couple of purposes:
        #
        #  A STB will send a bunch of advertisment messages, in a short space
        #  of time, when it boots. We want to ignore all but the first one we
        #  receive. Assuming the first message doesn't agree with the cache, we
        #  will update the config server, and then subsequent messages will
        #  agree with the cache, and be ignored.
        #
        #  If we receive many messages from different STB's in a short space of
        #  time, we don't want to contact the config server when each message
        #  arrives (it takes a few seconds to query the config server), as
        #  doing so would effectively mean we aren't able to handle anything
        #  but a trivial load.
        #
        # Obviously, having a cache is racy, but there's no way to avoid this;
        # even without an explicit cache, we still have the same race, only
        # it's smaller. The main thing we're racing against is someone adding
        # or removing boxes from a rack. If a box we're interesting in is
        # removed, that's no problem - we'll be updating a dut_id which is
        # still valid, and if not, we ignore the failure. If a box is added,
        # then we'll miss updating the ip address for that box - this is a
        # problem. A human is highly unlikely to be able to add a box to the
        # rack on the config server, and then power cycle it in a very short
        # space of time, so the age can be fairly big. 5 or 10 seconds is
        # probably about right.

        now = time.time()
        if now - self._cache_update_time > self._MAX_CACHE_AGE:
            self._config_cache = {}
        else:
            return

        # For each server in the facility, retrieve dut_id and ipaddr of each 
        # populated slot in the server. Put this stuff into the cache.

        facility = st.GetFacilityData(self._config_server_addr)
        if not facility:
            raise ConfigServerException("Unable to retrieve facility data")

        server_names = st.GetServerNames(facility)
        if not server_names:
            raise ConfigServerException("Unable to retrieve storm server names")

        for server_name in server_names:
            nslots = self._get_server_nslots(facility, server_name)
            if not nslots:
                raise ConfigServerException("Unable to determine nslots in storm server %s" % server_name)

            for slot in range(1, nslots+1):
                stb = st.GetStbField(facility, server_name, slot, 'all')
                if not stb:
                    raise ConfigServerException("Unable to retrieve stb fields")

                dut_id = stb['Id']
                # An unpopulated or misconfigured slot will have no dut_id
                if dut_id:
                    details = st.GetDutDetails(dut_id)
                    if not details:
                        raise ConfigServerException("Unable to retrieve STB IP address")

                    serial = details['serialnumber']
                    if serial:
                        self._config_cache[serial] = {
                                'dut_id' : dut_id,
                                'ipaddress' : details['ipaddress']}

        print "config cache: ", self._config_cache
        self._cache_update_time = time.time()
                
    def __init__(self, log, config_server_addr):
        self._log = log
        self._config_server_addr = config_server_addr
        self._config_cache = None
        self._cache_update_time = 0.0


        st.SetMasterConfigurationServer(config_server_addr)

    def get_stb_config(self, serial):
        """ Retrieve config dictionary for STB identified by serial. This may
        be None, and is retrieved from a cache after initial retrieval from the 
        config server.

        throws ConfigServerException
        """
        
        self._update_config_cache()
        #self._print_cache()

        if serial not in self._config_cache:
            return None

        return self._config_cache[serial]

    def set_stb_ipaddr(self, serial, ipaddr):
        """ Update the IP Address for the stb corresponding to serial. This 
        will push the changes back to the config server, and update the cache. 

        throws ConfigServerException
        """

        dut_id = self._config_cache[serial]['dut_id']

        # Update the config server first, then the cache
        err = st.UpdateDutInstance(dut_id, ipaddress=ipaddr)
        if not err:
            # Failure may be caused because of cache race, and in general
            # we need to be a bit resilient to bad stuff on the config 
            # server...
            self._log.warn("Failed to update STB with serial %s" % serial)
        self._config_cache[serial]['ipaddress'] = ipaddr


class StbDiscovery(object):

    def _update_storm_facility(self, new_cfg):

        try:
            exist_cfg = self._storm_facility.get_stb_config(new_cfg['serial'])

            # According to the config server, this box is not in a rack; 
            # ignore it...
            if not exist_cfg:
                self._log.warn("STB serial %s not in rack. Ignoring" %
                        new_cfg['serial'])
                return

            # Change the IP address of the STB if it is different to that in
            # the config server database
            if exist_cfg['ipaddress'] != new_cfg['ipaddress']:
                self._log.info("Updating IP address of STB %s to %s" %
                        (new_cfg['serial'], new_cfg['ipaddress']))
                self._storm_facility.set_stb_ipaddr(new_cfg['serial'], 
                        new_cfg['ipaddress'])

        except ConfigServerException, e:
            self._log.error("Failed to update config server database: %s" % e)

    def __init__(self, log, listen_port, config_server_addr, listen_only=False):
        if not config_server_addr and not listen_only:
            raise StbDiscoveryException("Either config_server_addr or listen_only must be specified")

        self._log = log
        self._listen_port = listen_port
        self._listen_only = listen_only
        self._continue_running = True

        if not listen_only:
            self._storm_facility = StormFacilityData(log, config_server_addr)
        else:
            self._storm_facility = None

    def run(self):
        """ Listen for stb discovery messages, and update the config server DB
        required """

        self._log.info("StormTest Client Version: %s" % st.GetClientVersion())

        self._continue_running = True

        addr = ('', self._listen_port)
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.settimeout(0.5)
        sock.bind(addr)

        while self._continue_running:

            # Listen for STE discovery message. This is a polling loop.
            try:
                data, addr = sock.recvfrom(1024)
                print "sock addr:" + addr[0]
                print "raw sock data:" + data.replace(chr(0), '_')
                ste_instance = {
                        'version'   : data.split(',')[0].strip(chr(0)),
                        'serial'    : data.split(',')[1].strip(chr(0)),
                        'ipaddress' : data.split(',')[2].strip(chr(0)),
                        'mac'       : data.split(',')[3].strip(chr(0))}
            except socket.timeout:
                continue

            self._log.debug("STE instance discovered: serial=%s version=%s ip=%s mac=%s" 
                    % (ste_instance['serial'], ste_instance['version'], 
                        ste_instance['ipaddress'], ste_instance['mac']))

            if self._storm_facility:
                self._update_storm_facility(ste_instance)

        # exit
        sock.close()

    def stop(self):
        self._continue_running = False

class StbDiscoveryService(win32serviceutil.ServiceFramework):

    _svc_name_ = "StbDiscoveryService"
    _svc_display_name_ = "STE STB Discovery Service"
    _svc_description_ = "Listen for notifications from STB's running STE, and update StormTest Config server IP address database appropriately"

    def __init__(self, args):
        win32serviceutil.ServiceFramework.__init__(self, args)

        # By default, the client libs will try and log stuff to console or
        # file; we're running as a service, and this behaviour will just cause
        # grief; turn it off.
        st.SetDebugLevel(0, 1)

        # Read config info from the registry
        self._reg = {}
        key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, 
                "Software\Silicon & Software Systems Ltd\StormTest Embedded Discovery Server")
        url = _winreg.QueryValueEx(key, "ConfigServerUrl")[0]
        self._reg['config_server_addr'] = url.split('//')[1].split(':')[0]
        self._reg['port'] = _winreg.QueryValueEx(key, "Port")[0]
        self._reg['log_level'] = _winreg.QueryValueEx(key, "LogLevel")[0]
        _winreg.CloseKey(key)

        self._log = LogWinEvent(self._reg['log_level'])

    def SvcStop(self):
        self._log.info("Stop pending")
        self._discovery.stop()
        self.ReportServiceStatus(win32service.SERVICE_STOP_PENDING)

    def SvcDoRun(self):
        self._log.info("Service starting...")

        self._discovery = StbDiscovery(self._log, self._reg['port'],
                self._reg['config_server_addr'])
        self._discovery.run()

        self._log.info("Service exiting...")

def get_opts():
    parser = OptionParser("""
%prog install | start | stop | remove | -f [--port PORT] --config CONFIG_SERVER_ADDR
""")

    parser.add_option("-f", "--foreground", 
            dest="foreground",
            action="store_true", 
            default=False,
            help="Run in the foreground instead of as a service")
    parser.add_option("-p", "--port", 
            dest="port", 
            default=DEFAULT_LISTEN_PORT,
            type="int", 
            help="Port to listen for STB Discovery messages")
    parser.add_option("-c", "--config", 
            dest="config_server_addr",
            type="string", 
            help="Address of the config server e.g. daniels")
    parser.add_option("-l", "--listen_only",
            dest="listen_only",
            action="store_true",
            default=False,
            help="Listen and print out details of discovery messages, but don't contact the config server")

    # options handled by win32serviceutil
    parser.add_option("", "--startup",
            dest="startup",
            help="")

    (options,args) = parser.parse_args()

    return options

if __name__=='__main__':

    opts = get_opts()
    if opts.foreground:
        log = LogConsole('debug')
        discovery = StbDiscovery(log, opts.port, opts.config_server_addr, 
                opts.listen_only)
        discovery.run()

    else: # run as a service
        err = win32serviceutil.HandleCommandLine(StbDiscoveryService)
        if err:
            # Installer relies on us exiting with non-zero code on error
            sys.exit(1) 


