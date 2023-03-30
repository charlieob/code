# See http://www.oly-tech.com/support/bandwidthcalculator.xls
# ETS 300 429, April 1998
# "Digital Video Broadcasting (DVB); Framing structure, channel coding and modulation for cable systems"

# Here are the input parameters for the calculation.
# Could pass these in as arguments to the script, maybe...
bandwidth = 8
constellation = '64-QAM'

## Calculate the output bitrate of the DVB-C modulation
#
# @param    bandwidth   An int which indicates the bandwidth in MHz (6, 7 or 8)
# @param    mod         A string describing the modulation type ('16-QAM' to '128-QAM')
# @return   float       Bit rate in Mbps
def calc_bitrate(bandwidth, mod):

    # See ETS 300 429, p. 16, cosine filtering applied to the mapped baseband signal prior to modulation
    # with a roll-off factor 0.15
    rolloffFactor = 1/1.15
    
    # Coding uses a Reed-Solomon code which adds 16 parity bytes to the 188-byte TS packet
    # (see ETS 300 429, p. 11).
    coding = float(188)/204
    
    bitsPerSymbol = {'16-QAM': 4,
                     '32-QAM': 5,
                     '64-QAM': 6,
                     '128-QAM': 7,
                     '256-QAM': 8}[mod]
    
    return bandwidth * rolloffFactor * coding * bitsPerSymbol

print '     bandwidth =', bandwidth
print ' constellation =', constellation
print '--------------------------------'
print 'output bit rate is %0.6f Mbps' % calc_bitrate(bandwidth, constellation)

#
# Let's try to reproduce the table on http://en.wikipedia.org/wiki/DVB-C
# (which appears to contain a few errors - now fixed by me! - 12 Jun 2012)
#
print
print("%40s" % 'Bandwidth (MHz)')
print("%10s%7s%7s%7s%7s%7s" % ('Modulation', '2', '4', '6', '8', '10'))
for constellation in ('16-QAM', '32-QAM', '64-QAM', '128-QAM', '256-QAM'):
    str = "%9s   " % constellation
    for bandwidth in (2, 4, 6, 8, 10):
        str += "%7.2f" % calc_bitrate(bandwidth, constellation)
    print str
