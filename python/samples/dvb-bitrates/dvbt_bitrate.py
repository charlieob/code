# See http://www.computermodules.com/broadcast/bit-rate-calculation-for-COFDM-modulation.shtml
# ETS 300 744, March 1997
# "Digital Video Broadcasting (DVB); Framing structure, channel coding and modulation for digital terrestrial television"

# Here are the input parameters for the calculation.
# Could pass these in as arguments to the script, maybe...
bandwidth = 7
coderate = '1/2'
constellation = 'QPSK'
guardInterval = '1/4'

## Calculate the output bitrate of the DVB-T modulation
#
# @param    bandwidth   An int which indicates the bandwidth in MHz (6, 7 or 8)
# @param    fec         A string indicating the code rate (e.g. '7/8')
# @param    mod         A string describing the modulation type ('QPSK', '16-QAM' or '64-QAM')
# @frame    guard       A string indicating the guard interval (e.g. '1/32')
# @return   float       Bit rate in Mbps
def calc_bitrate(bandwidth, fec, mod, guard):

    # See ETS 300 744, p. 10, the note on channel spacing and system clock.
    # At 8 MHz the system clock is actually 64/7 MHz. Therefore for each of the 6, 7 or 8 MHz channel
    # spacing options, the bandwidth is scaled by 64/56.
    systemClock = float(64)/56

    # Inner interleaving results in 12 blocks of 126 bits = 1512 bits. 2k mode uses a FFT of size 2048,
    # which is then the total number of sub-carriers available for data in the OFDM frame, of which 1512
    # are used for data.
    # (see ETS 300 744, pp. 18-19).
    # In 8k mode, there are 48 blocks and an FFT with 8192 points, so everything is simply scaled up
    # by 4, and the bitrate is not affected.
    ofdmUsage = float(1512)/2048
    
    # Outer coding uses a Reed-Solomon code which adds 16 parity bytes to the 188-byte TS packet
    # (see ETS 300 744, pp. 12-13).
    outerCoding = float(188)/204
    
    innerCoding = {'1/2': 1.0/2,
                   '2/3': 2.0/3,
                   '3/4': 3.0/4,
                   '5/6': 5.0/6,
                   '7/8': 7.0/8}[fec]
    
    bitsPerSymbol = {  'QPSK': 2,
                     '16-QAM': 4,
                     '64-QAM': 6}[mod]
    
    guardInterval = { '1/4':  4.0/5,
                      '1/8':  8.0/9,
                     '1/16': 16.0/17,
                     '1/32': 32.0/33}[guard]
    
    return bandwidth * systemClock * ofdmUsage * outerCoding * innerCoding * bitsPerSymbol * guardInterval

print '     bandwidth =', bandwidth
print '     code rate =', coderate
print ' constellation =', constellation
print 'guard interval =', guardInterval
print '--------------------------------'
print 'output bit rate is %0.6f Mbps' % calc_bitrate(bandwidth, coderate, constellation, guardInterval)

#
# Let's try to reproduce the table on http://en.wikipedia.org/wiki/DVB-T
#
print
bandwidth = 8
print(" bandwidth = %d MHz" % bandwidth)
for constellation in ('QPSK', '16-QAM', '64-QAM'):
    print("%s" % ('-' * (10+8*4)))
    print("modulation = %s" % constellation)
    print("%10s%7s %7s %7s %7s " % ('code rate', '1/4', '1/8', '1/16', '1/32'))
    for fec in ('1/2', '2/3', '3/4', '5/6', '7/8'):
        str = "%7s   " % fec
        for guard in ('1/4', '1/8', '1/16', '1/32'):
            str += "%8.3f" % calc_bitrate(bandwidth, fec, constellation, guard)
        print str
