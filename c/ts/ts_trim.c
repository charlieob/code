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

static char outputFileName[MAX_FILE_NAME_LENGTH];


/************************************************************************
 * PROTOTYPES
 ************************************************************************/

unsigned long findPcr(unsigned char *pBuf);
unsigned long long findPcr64(unsigned char *pBuf);


/************************************************************************
 * FUNCTIONS
 ************************************************************************/

int main (int argc, char *argv[])
{
    int status = NO_ERROR;
    FILE *pInFile = NULL;
    FILE *pOutFile = NULL;
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
    unsigned int targetTime = 0;
    unsigned long long pcr = 0;
    unsigned long long startPcr = 0;
    unsigned long long targetPcr = 0;
    unsigned long long lastPcr = 0;
    off64_t fileStartPos = 0;
    off64_t fileStopPos = 0;
    off64_t lastPcrPos = 0;
    bool targetPcrSet = FALSE;
    bool pcrWrap = FALSE;
    

    memset(readPacket, 0, sizeof(readPacket));
    
    if (status == NO_ERROR)
    {
        if (argc < 4)
        {
            printf("\nError: Too few arguments %u\n\n", argc);
            printf("Syntax: %s INPUT_FILE PCR_PID TIME(milliseconds)\n\n", argv[0]);
            printf("Warning: max time is about 13 hours - 2^32/90khz\n\n", argc);
            status = ERROR;
        }
    }

    if (status == NO_ERROR)
    {
        pcrPid = atoi(argv[2]);
        targetTime = atoi(argv[3]);
        printf("%u %u %u\n", pcrPid, targetTime, sizeof(off64_t));

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

        sprintf(outputFileName, "%s", argv[1]);
        outputFileName[inputFileNameLength - 1] = 0;
        sprintf(outputFileName, "%s_%us.ts", outputFileName, targetTime / 1000);
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

    /* Open output file */
    if (status == NO_ERROR)
    {
        pOutFile = fopen(outputFileName, "wb");
        if (pOutFile == NULL)
        {
            printf("Error opening output file: %s\n", outputFileName);
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
                pid = readPacket[2];
                pid |= ((((int)readPacket[1]) & 0x1F ) << 8 );

                if (pid == pcrPid)
                {
                    pcr = findPcr64(readPacket);
                    if (pcr != 0)
                    {
                        lastPcr = pcr;
                        lastPcrPos = filePos;
                        if (targetPcrSet == FALSE)
                        {
                            targetPcrSet = TRUE;
                            startPcr = pcr;
                            targetPcr = startPcr + 90 * targetTime;
                            if (targetPcr < pcr)
                            {
                                pcrWrap = TRUE;
                            }
                            else
                            {
                                pcrWrap = FALSE;
                            }
                            fileStartPos = filePos;
                        }

                        /* finish writing the file if we have reached the target time */
                        if (pcrWrap == FALSE)
                        {
                            if (pcr >= targetPcr)
                            {
                                fileStopPos = filePos;
                                break;
                            }
                        }
                        else
                        {
                            if ((pcr >= targetPcr) && (pcr <= startPcr))
                            {
                                fileStopPos = filePos;
                                break;
                            }
                        }
                    }
                }
                if (targetPcrSet == TRUE)
                {
                    fwrite(readPacket, packetLength, 1, pOutFile);
                }
            }
            else
            {
                /* reached end of file */
                fileStopPos = filePos;
            }
        }
        while (bytesRead == 1);
    }    

    /* print statistics */
    printf("\n");
    printf("Start PCR=0x%I64x\n", startPcr);
    printf("End PCR=0x%I64x\n", lastPcr);
    if (pcrWrap == TRUE)
        printf("PCR wrapped, length and bitrate calculations need fixing\n");
    printf("Actual length=%0.3f seconds\n", (float)(lastPcr - startPcr)/90000);
    printf("Start file position=%I64u\n", fileStartPos);
    printf("Stop file position=%I64u\n", fileStopPos);
    printf("Bytes written=%I64u\n", fileStopPos - fileStartPos);
    printf("Estimated bitrate=%d\n", 8*(lastPcrPos-fileStartPos)*90000/(lastPcr-startPcr));

    /* clean up */
    if (pInFile != NULL)
    {
        fclose(pInFile);
    }
    if (pOutFile != NULL)
    {
        fclose(pOutFile);
    }
    
    return 0;
}


unsigned long findPcr(unsigned char *pBuf)
{
    int status = NO_ERROR;
    unsigned long retVal = 0;
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
        /* to avoid using 64-bit numbers take only the 32 LSB of the PCR */
        retVal = ((unsigned long)(pBuf[6]) & 0xEF) << 25;
        retVal |= ((unsigned long)pBuf[7]) << 17;
        retVal |= ((unsigned long)pBuf[8]) << 9;
        retVal |= ((unsigned long)pBuf[9]) << 1;
        retVal |= ((unsigned long)pBuf[10]) >> 7;
    }

    return retVal;
}

unsigned long long findPcr64(unsigned char *pBuf)
{
    int status = NO_ERROR;
    unsigned long long retVal = 0;
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
        retVal = (unsigned long long)(pBuf[6]) << 32;
        retVal |= ((unsigned long long)pBuf[7]) << 24;
        retVal |= ((unsigned long long)pBuf[8]) << 16;
        retVal |= ((unsigned long long)pBuf[9]) << 8;
        retVal |= ((unsigned long long)pBuf[10]) << 0;

        retVal >>= 7;
    }

    return retVal;
}


