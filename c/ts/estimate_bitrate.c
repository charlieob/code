#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************************************************************
 * DEFINES
 ************************************************************************/

#define bool            int
#define TRUE            (1)
#define FALSE           (0)

#define NO_ERROR        (0)
#define ERROR           (1)

#define PACKET_188      (188)
#define PACKET_204      (204)

#define SOP_SYMBOL      (0x47)
#define NULL_PID        (0x1FFF)

#define OUTPUT_VIDEO_PID        (513)
#define OUTPUT_AUDIO_PID        (514)

#define MAX_FILE_NAME_LENGTH    (512)


/************************************************************************
 * TYPES
 ************************************************************************/

/************************************************************************
 * VARIABLES
 ************************************************************************/

static unsigned char readPacket[PACKET_204];


/************************************************************************
 * PROTOTYPES
 ************************************************************************/

unsigned long long findPcr64(unsigned char *pBuf);


/************************************************************************
 * FUNCTIONS
 ************************************************************************/

int main (int argc, char *argv[])
{
    int status = NO_ERROR;
    FILE *pInFile = NULL;
    off64_t syncPos = 0;
    off64_t filePos = 0;
    size_t bytesRead = 0;
    int packetLength = 0;
    unsigned char q = 0;
    bool foundSync = FALSE;
    bool abortFlag = FALSE;
    bool pcrValid = FALSE;
    int pid = 0;
    int pcrPid = 0xFFFF; /* invalidate pid selection */
    unsigned int inputFileNameLength = 0;
    unsigned long long pcr = 0;
    unsigned long long maxPcr = 0;
    unsigned long long firstPcr = 0;
    unsigned long long lastPcr = 0;
    unsigned long long packetCount = 0;
    unsigned long long nullPacketCount = 0;
    unsigned long long pcrPacketCount = 0;
    off64_t firstPcrPos = 0;
    off64_t lastPcrPos = 0;
    bool firstPcrFound = FALSE;
    bool pcrWrap = FALSE;
    

    memset(readPacket, 0, sizeof(readPacket));
    
    if (status == NO_ERROR)
    {
        if (argc < 3)
        {
            printf("\nError: Too few arguments %u\n\n", argc-1);
            printf("Usage: %s INPUT_FILE PCR_PID\n\n", argv[0]);
            status = ERROR;
        }
    }

    if (status == NO_ERROR)
    {
        pcrPid = atoi(argv[2]);
        printf("%u %u %u\n", pcrPid, sizeof(off64_t), sizeof(unsigned long long));

        inputFileNameLength = strlen(argv[1]);
        while(inputFileNameLength > 0)
        {
            if (argv[1][inputFileNameLength - 1] == '.')
            {
                break;
            }
            else
            {
                inputFileNameLength--;
            }
        }
    }
    
    /* Open input file */
    if (status == NO_ERROR)
    {
        pInFile = fopen(argv[1], "rb");
        if (pInFile == NULL)
        {
            printf("Error opening input file: %s\n", argv[1]);
            status = ERROR;
        }
    }

    /* Find sync */
    if (status == NO_ERROR)
    {
        while ( (foundSync == FALSE) && (abortFlag == FALSE) )
        {
            bytesRead = fread( &q, 1, 1, pInFile);
            if(bytesRead == 0)
            {
                printf("Reached end of file, while trying to sync - aborting\n");
                abortFlag = TRUE;
            }
            else
            {
                if (q == SOP_SYMBOL)
                {
                    if (abortFlag == FALSE)
                    {
                        /* move one byte back, as we already read that byte */
                        fseeko64(pInFile, -1, SEEK_CUR);
                        syncPos = ftell(pInFile);

                        fseek(pInFile, syncPos + PACKET_188, SEEK_SET);
                        bytesRead = fread( &q, 1, 1, pInFile);
                        if (bytesRead == 0)
                        {
                            printf("Error reading file\n");
                            abortFlag = TRUE;
                        }
                    }

                    if (abortFlag == FALSE)
                    {
                        if (q == SOP_SYMBOL)
                        {
                            packetLength = PACKET_188;
                            foundSync = TRUE;
                        }
                    }

                    if ( (abortFlag == FALSE) && (foundSync == FALSE) )
                    {
                        fseeko64(pInFile, syncPos + PACKET_204, SEEK_SET);
                        bytesRead = fread( &q, 1, 1, pInFile);
                        if (bytesRead == 0)
                        {
                            printf("Error reading file\n");
                            abortFlag = TRUE;
                        }
                    }

                    if ( (abortFlag == FALSE) && (foundSync == FALSE) )
                    {
                        if (q == SOP_SYMBOL)
                        {
                            packetLength = PACKET_204;
                            foundSync = TRUE;
                        }
                    }
                }
            }
        }
    }

    /* abort if found no sync */
    if (status == NO_ERROR)
    {
        if (foundSync == TRUE)
        {
            printf("Found sync at file position %I64u. Packet length: %d\n", syncPos, packetLength);
        }
        else
        {
            printf("Could not find sync - aborting\n");
            status = ERROR;
        }
    }

    /* generate output file */
    if (status == NO_ERROR)
    {
        fseeko64(pInFile, syncPos, SEEK_SET);

        do
        {
            filePos = ftello64(pInFile);
            /* fread returns number of elements read, not bytes */
            bytesRead = fread(&readPacket, packetLength, 1, pInFile);

            if (bytesRead == 1)
            {
                packetCount++;
                pid = readPacket[2];
                pid |= ((((int)readPacket[1]) & 0x1F ) << 8 );
                
                if (pid == NULL_PID)
                {
                    nullPacketCount++;
                }
                else if (pid == pcrPid)
                {
                    pcrPacketCount++;
                    pcr = findPcr64(readPacket);
                    if (pcr != 0)
                    {
                        if (pcr < firstPcr)
                        {
                            pcrWrap = TRUE;
                        }
                        lastPcr = pcr;
                        lastPcrPos = filePos;
                        if (firstPcrFound == FALSE)
                        {
                            firstPcrFound = TRUE;
                            firstPcr = pcr;
                            firstPcrPos = filePos;
                        }
                    }
                }
            }
        }
        while (bytesRead == 1);
    }    

    if (status == NO_ERROR)
    {
        /* print statistics */
        printf("\n");
        maxPcr = 0x1ffffffffLL;
        maxPcr = maxPcr*300 + 299;
        printf("   #  TS  Packets = %I64u\n", packetCount);
        printf("   # Null Packets = %I64u\n", nullPacketCount);
        printf("   # PCR  Packets = %I64u\n", pcrPacketCount);
        printf("    Max PCR (hex) = %11I64x\n", maxPcr);
        printf("  First PCR (hex) = %11I64x\n", firstPcr);
        printf("   Last PCR (hex) = %11I64x\n", lastPcr);
        if (pcrWrap == TRUE)
            printf("PCR wrapped, length and bitrate calculations need fixing\n");
        printf("  Actual duration = %0.3f seconds\n", (float)(lastPcr - firstPcr)/27000000);
        printf("File position of first PCR = %I64u\n", firstPcrPos);
        printf("File position of last PCR  = %I64u\n", lastPcrPos);
        printf("Estimated bitrate = %d bps\n", 8*(lastPcrPos-firstPcrPos)*27000000/(lastPcr-firstPcr));
    }

    /* clean up */
    if (pInFile != NULL)
    {
        fclose(pInFile);
    }
    
    return 0;
}

unsigned long long findPcr64(unsigned char *pBuf)
{
    int status = NO_ERROR;
    unsigned long long retVal = 0;
    unsigned long long pcrBase = 0;
    unsigned long long pcrExt = 0;
    unsigned char q = 0;

    if (status == NO_ERROR)
    {
        if (pBuf == NULL)
        {
            printf ("Bad parameters\n");
            status = ERROR;
        }
    }

    if (status == NO_ERROR)
    {
        /* check if we have adaptation field */
        q = pBuf[3] >> 4;
        q &= 0x03;
        if (q < 2)
        {
            status = ERROR;
        }
    }

    if (status == NO_ERROR)
    {
        /* check if we adaptation field contains any bytes */
        if (pBuf[4] == 0)
        {
            status = ERROR;
        }
    }

    if (status == NO_ERROR)
    {
        /* check if we have PCR */
        q = pBuf[5] >> 4;
        q &= 0x01;
        if (q == 0)
        {
            status = ERROR;
        }
    }

    if (status == NO_ERROR)
    {
        pcrBase = (unsigned long long)(pBuf[6]) << 32;
        pcrBase |= ((unsigned long long)pBuf[7]) << 24;
        pcrBase |= ((unsigned long long)pBuf[8]) << 16;
        pcrBase |= ((unsigned long long)pBuf[9]) << 8;
        pcrBase |= ((unsigned long long)pBuf[10]) << 0;
        pcrBase >>= 7;
        
        pcrExt = ((unsigned long long)pBuf[10]) & 0x1;
        pcrExt <<= 8;
        pcrExt |= ((unsigned long long)pBuf[11]);
        
        retVal = pcrBase*300 + pcrExt;
    }

    return retVal;
}


