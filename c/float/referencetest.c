/*****************************************************************************
 *
 * (c) 2003, Silicon & Software Systems, Dublin, Ireland.
 *
 * Project          :  Toshiba MeP FPU
 * Module           :  Code to generate floating-point test vectors
 * Author           :  Charlie O'Brien
 * Creation Date    :  July 2003
 * File Name        :  referencetest.c
 * Description      :  File contains code which generates test vectors in
 *                     the standard format, given as input an operation and
 *                     operands(s), by running the operation on a reference
 *                     platform (Unix or Windows).
 *
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Name$
 *
 ****************************************************************************/

/* System Includes */

#include <stdio.h>
#include <stdlib.h>

#ifdef __sparc
#include <ieeefp.h>
#endif

#ifdef WIN32
#include <fpieee.h>
#endif

#include <float.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>

#ifdef __sparc
#define     HOST_X_INVALID  FP_X_INV
#define     HOST_X_OFLOW    FP_X_OFL
#define     HOST_X_UFLOW    FP_X_UFL
#define     HOST_X_DIVZERO  FP_X_DZ
#define     HOST_X_INEXACT  FP_X_IMP

#define     HOST_R_ZERO     FP_RZ
#define     HOST_R_PLUS     FP_RP
#define     HOST_R_MINUS    FP_RM
#define     HOST_R_NEAR     FP_RN
#endif

#ifdef WIN32
#define     HOST_X_INVALID  _SW_INVALID     // 0x10
#define     HOST_X_OFLOW    _SW_OVERFLOW    // 0x04
#define     HOST_X_UFLOW    _SW_UNDERFLOW   // 0x02
#define     HOST_X_DIVZERO  _SW_ZERODIVIDE  // 0x08
#define     HOST_X_INEXACT  _SW_INEXACT     // 0x01

#define     HOST_R_ZERO     _RC_CHOP
#define     HOST_R_PLUS     _RC_UP
#define     HOST_R_MINUS    _RC_DOWN
#define     HOST_R_NEAR     _RC_NEAR
#endif

/* Module Includes */

/* Local Constant Declarations */

/* Local Type Declarations */

typedef enum
{
    FADD,
    FSUB,
    FMUL,
    FDIV,
    FSQRT,
    FABS,
    FNEG,
    FMOV,
    FROUNDW,
    FTRUNCW,
    FCEILW,
    FFLOORW,
    FCVTS,
    FCVTW,
    FCMPF,
    FCMPU,
    FCMPE,
    FCMPUE,
    FCMPL,
    FCMPUL,
    FCMPLE,
    FCMPULE,
    FCMPFI,
    FCMPUI,
    FCMPEI,
    FCMPUEI,
    FCMPLI,
    FCMPULI,
    FCMPLEI,
    FCMPULEI
} fp32bitOp;


typedef union itof
{
    unsigned long i;
    float         f;
} itof;


/* Local Function Declarations */

//void __cdecl fphandler( int sig, int subcode );

void test_referenceOp (char *filename);
int main (int argc, char *argv[]);

/* Exported Variable Definitions */

/* Local Variable Definitions */

jmp_buf mark;
int     fperr;

/* Local Function Definitions */


void fphandler (
                int sig
#ifdef WIN32
                , int subcode
#endif
               )
{
    //signal (SIGFPE, fpexception);

#ifdef WIN32
    fperr = subcode;
#else
    fperr = sig;
#endif

#ifdef WIN32
    _fpreset();
#endif
    longjmp (mark, -1);
}



/******************************************************************************
 *
 * Function      : test_referenceOp
 *
 * Inputs 
 *
 *   Identifier  : filename
 *   Type        : char *
 *   Description : The name of the file containing the test vectors.
 *                 If a null pointer, then vectors are read from stdin.
 *
 * Outputs 
 *
 * Returns 
 *
 *   Type        : void
 *   Description : 
 *
 * Purpose       : Processes a set of test vectors.
 *
 *****************************************************************************/

void test_referenceOp (char *filename)
{
    FILE*         fp;
    int           i;
    char          line[200];

    char          opcode[10];
    char          opnd1s[9], opnd2s[9];
    char          r;

    fp32bitOp     op;
    int           rnd;
    itof          opnd1, opnd2, res;

    int           exc;
    char          excstr[6];

    char          finished=0;

    int           jmpret;


    if (signal (SIGFPE, fphandler) == SIG_ERR)
    {
        printf ("Couldn't set SIGFPE\n");
        abort();
    }

    /* open file containing test vectors */

    if (filename == NULL)
    {
        fp = stdin;
    }
    else if (!(fp = fopen (filename, "r")))
    {
        printf ("ATTENTION: input file %s not found\n\n", filename);
        finished = 1;
    }
    else
    {
        printf ("Processing file %s ...\n", filename);
    }

    while (!finished)
    {
        if (fgets (line, sizeof(line), fp) == NULL)
            finished = 1;
        else
        {
            i = 0;
            while (isspace(line[i])) i++;
        }

        if ((!finished) && (line[i] != '#') && (line[i] != '\0'))
        {
            sscanf (line+i, "%s %s %s",
                    opcode, opnd1s, opnd2s);

/*
--
-- It would be possible to write code here which reads in a full test
-- vector and simply runs it on the reference platform, checking that
-- each vector passes.
-- The program could have 2 modes: generate or verify.
-- Only the generate mode is implemented so far.
--
*/

            /* decode the operation */

            if (!strcmp(opcode, "adds"))
                op = FADD;
            else if (!strcmp(opcode, "fadd"))
                op = FADD;
            else if (!strcmp(opcode, "fsub"))
                op = FSUB;
            else if (!strcmp(opcode, "muls"))
                op = FMUL;
            else if (!strcmp(opcode, "fmul"))
                op = FMUL;
            else if (!strcmp(opcode, "divs"))
                op = FDIV;
            else if (!strcmp(opcode, "fdiv"))
                op = FDIV;
            else if (!strcmp(opcode, "sqrts"))
                op = FSQRT;
            else if (!strcmp(opcode, "fsqrt"))
                op = FSQRT;
            else if (!strcmp(opcode, "fabs"))
                op = FABS;
            else if (!strcmp(opcode, "fneg"))
                op = FNEG;
            else if (!strcmp(opcode, "fmov"))
                op = FMOV;
            else if (!strcmp(opcode, "froundw"))
                op = FROUNDW;
            else if (!strcmp(opcode, "ftruncw"))
                op = FTRUNCW;
            else if (!strcmp(opcode, "fceilw"))
                op = FCEILW;
            else if (!strcmp(opcode, "ffloorw"))
                op = FFLOORW;
            else if (!strcmp(opcode, "cis"))
                op = FCVTS;
            else if (!strcmp(opcode, "fcvts"))
                op = FCVTS;
            else if (!strcmp(opcode, "ris"))
                op = FCVTW;
            else if (!strcmp(opcode, "fcvtw"))
                op = FCVTW;
            else if (!strcmp(opcode, "fcmpf"))
                op = FCMPF;
            else if (!strcmp(opcode, "fcmpu"))
                op = FCMPU;
            else if (!strcmp(opcode, "fcmpe"))
                op = FCMPE;
            else if (!strcmp(opcode, "fcmpue"))
                op = FCMPUE;
            else if (!strcmp(opcode, "fcmpl"))
                op = FCMPL;
            else if (!strcmp(opcode, "fcmpul"))
                op = FCMPUL;
            else if (!strcmp(opcode, "fcmple"))
                op = FCMPLE;
            else if (!strcmp(opcode, "fcmpule"))
                op = FCMPULE;
            else if (!strcmp(opcode, "fcmpfi"))
                op = FCMPFI;
            else if (!strcmp(opcode, "fcmpui"))
                op = FCMPUI;
            else if (!strcmp(opcode, "fcmpei"))
                op = FCMPEI;
            else if (!strcmp(opcode, "fcmpuei"))
                op = FCMPUEI;
            else if (!strcmp(opcode, "fcmpli"))
                op = FCMPLI;
            else if (!strcmp(opcode, "fcmpuli"))
                op = FCMPULI;
            else if (!strcmp(opcode, "fcmplei"))
                op = FCMPLEI;
            else if (!strcmp(opcode, "fcmpulei"))
                op = FCMPULEI;
            else
                printf ("unrecognised operation %s\n", opcode);

            /* decode the operands */
            opnd1.i = strtoul (opnd1s, (char**)NULL, 16);
            opnd2.i = strtoul (opnd2s, (char**)NULL, 16);

            for (r = 0; r <= 3; r++)
            {
                fperr = 0;
                jmpret = setjmp (mark);
                if (jmpret != 0)
                {
                    printf ("FP exception handler invoked (0x%x)\n", fperr);
                    exit(0);
                }

                switch (r)
                {
                    case 0:     rnd = HOST_R_ZERO;
                                break;
                    case 1:     rnd = HOST_R_PLUS;
                                break;
                    case 2:     rnd = HOST_R_MINUS;
                                break;
                    case 3:     rnd = HOST_R_NEAR;
                                break;
                }

                /*
                --
                -- Run the test vector on the reference platform
                --
                */

#ifdef __sparc
                fpsetround (rnd);

                fpsetmask (FP_X_OFL);

                fpsetsticky (0);
#endif
#ifdef WIN32
                _controlfp (rnd, _MCW_RC);

                _controlfp (~_EM_OVERFLOW, _MCW_EM);

                _clearfp();
#endif

                switch (op)
                {
                    case FADD:
                        res.f = opnd1.f + opnd2.f;
                        break;

                    case FSUB:
                        res.f = opnd1.f - opnd2.f;
                        break;

                    case FMUL:
                        res.f = opnd1.f * opnd2.f;
                        break;

                    case FDIV:
                        res.f = opnd1.f / opnd2.f;
                        break;

                    case FSQRT:
                        if (opnd1.f < 0)
                            opnd1.f = -opnd1.f;
                        res.f = (float) sqrt(opnd1.f);
                        break;

                    case FABS:
                        res.f = (float) fabs(opnd1.f);
                        break;

                    case FNEG:
                        res.f = -opnd1.f;
                        break;

                    case FMOV:
                        res.f = opnd1.f;
                        break;

                    case FROUNDW:
                        if (floor(opnd1.f) != ceil(opnd1.f))
                        {
                            if (floor(2*opnd1.f) != ceil(2*opnd1.f))
                                res.i = (long) (opnd1.f < 0 ?
                                        opnd1.f - 0.5 :
                                        opnd1.f + 0.5);
                            else
                                res.i = (long) ((long)(floor(opnd1.f)) % 2 ?
                                        opnd1.f + 0.5 :
                                        opnd1.f - 0.5);
                        }
                        else
                        {
                            res.i = (long) opnd1.f;
                        }
                        break;

                    case FTRUNCW:
                        res.i = (long) opnd1.f;
                        break;

                    case FCEILW:
                        res.i = (long) ceil(opnd1.f);
                        break;

                    case FFLOORW:
                        res.i = (long) floor(opnd1.f);
                        break;

                    case FCVTS:
                        res.f = (float) opnd1.i;
                        break;

                    case FCVTW:
                        if (rnd == HOST_R_NEAR)
                        {
                            if (floor(opnd1.f) != ceil(opnd1.f))
                            {
                                if (floor(2*opnd1.f) != ceil(2*opnd1.f))
                                    res.i = (long) (opnd1.f < 0 ?
                                            opnd1.f - 0.5 :
                                            opnd1.f + 0.5);
                                else
                                    res.i = (long) ((long)(floor(opnd1.f)) % 2 ?
                                            opnd1.f + 0.5 :
                                            opnd1.f - 0.5);
                            }
                            else
                            {
                                res.i = (long) opnd1.f;
                            }
                        }
                        else if (rnd == HOST_R_PLUS)
                        {
                            res.i = (long) ceil(opnd1.f);
                        }
                        else if (rnd == HOST_R_MINUS)
                        {
                            res.i = (long) floor(opnd1.f);
                        }
                        else if (rnd == HOST_R_ZERO)
                        {
                            res.i = (long) opnd1.f;
                        }
                        break;
                }

#ifdef __sparc
                exc = fpgetsticky();
#endif
#ifdef WIN32
                exc = _statusfp() & 0x1f;
#endif

                strcpy (excstr, "");

                if (exc & HOST_X_INEXACT)
                    strcat (excstr, "x");

                if (exc & HOST_X_DIVZERO)
                    strcat (excstr, "z");

                if (exc & HOST_X_UFLOW)
                    strcat (excstr, "u");

                if (exc & HOST_X_OFLOW)
                    strcat (excstr, "o");

                if (exc & HOST_X_INVALID)
                    strcat (excstr, "v");

                if (strlen(excstr) == 0)
                    strcat (excstr, "-");

                printf ("%7s ", opcode);

                switch (rnd)
                {
                    case HOST_R_ZERO:
                        printf ("%c ", 'z');
                        break;

                    case HOST_R_PLUS:
                        printf ("%c ", 'p');
                        break;

                    case HOST_R_MINUS:
                        printf ("%c ", 'm');
                        break;

                    case HOST_R_NEAR:
                        printf ("%c ", 'n');
                        break;
                }

                printf ("eq %2s ", excstr);

                printf ("%08x %08x ", opnd1.i, opnd2.i);

                printf ("%08x\n", res.i);
            }
        }
    }

    /* close input file */
    if (fp)
        fclose (fp);
}


/******************************************************************************
 *
 * Function      : main
 *
 * Inputs 
 *
 *   Identifier  : argc
 *   Type        : int
 *   Description : 
 *
 *   Identifier  : argv[]
 *   Type        : char *
 *   Description : 
 *
 * Outputs 
 *
 * Returns 
 *
 *   Type        : int
 *   Description : 
 *
 * Purpose       : Processes command-line arguments and invokes the function
 *                 which runs the tests.
 *
 *****************************************************************************/

int main (int argc, char *argv[])
{
    char    c;
    char    *progname = *argv;

    if (sizeof(unsigned long) != sizeof(float))
    {
        printf ("This test code relies on the assumption that the sizes on the host machine\n");
        printf ("of unsigned long (%d bytes) and float (%d bytes) are the same.\n",
                sizeof(unsigned long), sizeof(float));
        printf ("This is not the case on this machine.\n");

        exit(0);
    }

    /* process command line arguments */

    while (--argc > 0 && (*++argv)[0] == '-')
    {
        while (c = *++argv[0])
        {
            switch (c)
            {
                case 'h':
                    argc = -1;
                    break;

                default:
                    printf ("%s: illegal option -%c\n", progname, c);
                    argc = -1;
                    break;
            }
        }
    }

    if (argc < 0)
    {
        printf ("Usage: %s [-h] [-w] [-x] [file(s)]\n", progname);
        printf ("    -h        print this help message\n");
        printf ("    -w        enable warnings\n");
        printf ("    -x        run in exception control mode\n");
        printf ("\n");
        printf ("The following are the expected formats for the test vectors.\n");
        printf ("\n");
        printf ("In normal mode (Cant IeeeCC754 format), 7 fields thus:\n");
        printf (" 1: operation (fadd | fsub | ...)\n");
        printf (" 2: rounding mode (z | n | p | m)\n");
        printf (" 3: condition for testing the result (eq | uo)\n");
        printf (" 4: exceptions (r | v | x | z | u | o)\n");
        printf (" 5: operand 1 in 32 bits (8 hex digits)\n");
        printf (" 6: operand 2 in 32 bits (8 hex digits)\n");
        printf (" 7:    result in 32 bits (8 hex digits)\n");
        printf ("\n");
        printf ("In exception control mode, 12 fields thus:\n");
        printf (" 1: operation (fadd | fsub | ...)\n");
        printf (" 2: rounding mode (z | n | p | m)\n");
        printf (" 3: exceptions (r | v | x | z | u | o)\n");
        printf (" 4: exception cause bits prior to operation (6 bits, 2 hex digits)\n");
        printf (" 5: exception enable bits (5 bits, 2 hex digits)\n");
        printf (" 6: exception flag bits prior to operation (5 bits, 2 hex digits)\n");
        printf (" 7: operand 1 in 32 bits (8 hex digits)\n");
        printf (" 8: operand 2 in 32 bits (8 hex digits)\n");
        printf (" 9:    result in 32 bits (8 hex digits)\n");
        printf ("10: flag (0 | 1) indicating whether result is written to destination\n");
        printf ("11: exception cause bits (5 bits, 2 hex digits)\n");
        printf ("12: exception flag bits following operation (5 bits, 2 hex digits)\n");
    }
    else
    {
        if (argc == 0)
        {
            /* perform test run with input from stdin */
            test_referenceOp (NULL);
        }
        else
        {
            /* perform test run from each specified input file */
            while (argc-- > 0)
                test_referenceOp (*argv++);
        }
    }

    return 0;
}

