#
# All the necessary information here comes from the ETSI standard ETSI EN 302 307:
#   "Digital Video Broadcasting (DVB);Second generation framing structure, channel coding and
#    modulation systems for Broadcasting, Interactive Services, News Gathering and other
#    broadband satellite applications (DVB-S2)"
#
# See also http://voyager.ericsson.net/uploads/documents/originals/DVB-S2%20Bit%20Rate%20Calculator.xls for a
# working implementation of the bitrate calculation.
#

# Here are the input parameters for the calculation.
# Could pass these in as arguments to the script, maybe...
packetSize = 188
symbolRate = 27.5   # Mbaud
coderate = '2/3'
modulation = '8PSK'
pilots = 'ON'
fecFrame = 'normal'

## Calculate the output bitrate of the DVB-S2 modulation
#
# Returned value is zero in the case of an unsupported configuration.
#
# @param    symbolRate  float (symbol rate in MBaud)
# @param    mod         A string describing the modulation type ('QPSK' or '8PSK')
# @param    fec         A string indicating the code rate (e.g. '2/3')
# @param    pilots      A string indicating the pilots insertion option ('OFF' or 'ON')
# @frame    frame       A string indicating the FECFRAME length ('normal' or 'short')
# @return   float       Bit rate in Mbps
def calc_bitrate(symbolRate, mod, fec, pilots, frame):

    # The number of bits of information carried per symbol in the modulation scheme.
    bitsPerSymbol = {'QPSK': 2,
                     '8PSK': 3}[mod]

    # The number of bits in the FECFRAME, see ETSI EN 302 307 V1.2.1 p.14
    # The standard states that 'short' is not applicable for broadcast services so maybe we
    # never encounter anything other than 'normal' in our applications anyway.
    fecFrameLength = {'normal': 64800,
                       'short': 16200}[frame]

    # Variable with the value '1' if pilot insertion is chosen, otherwise '0'
    pilotsInserted = {'OFF': 0,
                       'ON': 1}[pilots]

    # The following table incorporates the details of
    #   - which code rates are supported for each modulation format (QPSK or 8PSK)
    #   - the frame length at each code rate for both the normal and 'short' case
    # See ETSI EN 302 307 V1.2.1 pp. 14 and 20-21.
    fecTable = (['1/4', '1/3', '2/5', '1/2', '3/5', '2/3', '3/4', '4/5', '5/6', '8/9', '9/10'],
                [ None,  None,  None,  None, '3/5', '2/3', '3/4',  None, '5/6', '8/9', '9/10'],
                (16008, 21408, 25728, 32208, 38688, 43040, 48408, 51648, 53840, 57472, 58192),
                ( 3072,  5232,  6312,  7032,  9552, 10632, 11712, 12432, 13152, 14232,  None))

    try:
        # 8PSK does not support all the available FEC rates
        # (see ETSI EN 302 307 V1.2.1 p.14).
        # The table lookup (index() call) will fail in the case of an unsupported rate.
        t = {'QPSK': 0,
             '8PSK': 1}[mod]
        i = fecTable[t].index(fec)

        # Now we use the index to look up the table to retrieve the frame length (described as
        # BBFRAME with length K_bch in ETSI EN 302 307 V1.2.1 pp. 20-21).
        t = {'normal': 2,
              'short': 3}[frame]
        frameLength = fecTable[t][i]
        
    except:
        # Must have been given an unsupported configuration
        return 0.0
    
    # frameLength could be 'None', i.e. in the case of FEC=9/10 with short FEC frame (see table)
    if frameLength == None:
        return 0.0
    
    # See ETSI EN 302 307 V1.2.1 pp. 25 and 28-29 for a description of the mapping between
    # FECFRAME and 'slots', each of length 90 symbols.
    xFecFrameLength = fecFrameLength/bitsPerSymbol
    slots = xFecFrameLength/90
    
    # See ETSI EN 302 307 V1.2.1 p.29 for description of pilot framing
    pilotFramingEfficiency = float(90)*slots / (90*(slots+1) + 36*pilotsInserted*int((slots-1)/16))
    
    # And now the calculation...
    return float(symbolRate) * bitsPerSymbol * packetSize/188 * (frameLength-80)/fecFrameLength * pilotFramingEfficiency

print 'symbol rate =', symbolRate
print '  code rate =', coderate
print ' modulation =', modulation
print '     pilots =', pilots
print '  FEC Frame =', fecFrame
print '------------------------'
print 'output bit rate is %0.6f Mbps' % calc_bitrate(symbolRate, modulation, coderate, pilots, fecFrame)

