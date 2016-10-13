#include <stdio.h>
#include <float.h>
#include <math.h>

#ifdef __sparc
#include <ieeefp.h>
#endif

#ifdef WIN32
#include <fpieee.h>
#endif

#ifdef __sparc
#define CLR_EXC_STATUS      (fpsetsticky(0))
#define GET_EXC_STATUS      (fpgetsticky())
#define GET_EXC_MASK        (fpgetmask())
#define GET_ROUNDING_MODE   (fpgetround())
#define INVALID_FLAG        FP_X_INV
#endif
#ifdef WIN32
#define CLR_EXC_STATUS      (_clearfp())
#define GET_EXC_STATUS      (_clearfp())
#define GET_EXC_MASK        (_controlfp(0,0) & MCW_EM)
#define GET_ROUNDING_MODE   ((_controlfp(0,0) & MCW_RC) >> 8)
#define GET_PRECISION       ((_controlfp(0,0) & MCW_PC) >> 16)
#define INVALID_FLAG        SW_INVALID
#endif

typedef union itof
{
    unsigned long   l;
    float           f;
} itof;


int main ()
{
    itof n;
    long l;
    int exc;
    int err;

#if defined(_M_IX86)
    printf ("_M_IX86 defined\n");
#else
    printf ("_M_IX86 not defined\n");
#endif

#if defined(_M_M68K)
    printf ("_M_M68K defined\n");
#else
    printf ("_M_M68K not defined\n");
#endif

    printf ("\n");


    printf ("type            sizeof(type)\n");
    printf ("----------------------------\n");
    printf ("unsigned long     %2d\n", sizeof(unsigned long));
    printf ("float             %2d\n", sizeof(float));
    printf ("itof              %2d\n", sizeof(itof));
    printf ("int               %2d\n", sizeof(int));
    printf ("long              %2d\n", sizeof(long));
    printf ("short             %2d\n", sizeof(short));
    printf ("\n");

    printf ("Exception flags:\n");
#ifdef __sparc
    printf ("0x%02x   invalid\n", FP_X_INV);
    printf ("0x%02x   divide by zero\n", FP_X_DZ);
    printf ("0x%02x   overflow\n", FP_X_OFL);
    printf ("0x%02x   underflow\n", FP_X_UFL);
    printf ("0x%02x   inexact\n", FP_X_IMP);
#endif
#ifdef WIN32
    printf ("0x%02x   invalid\n", SW_INVALID);
    printf ("0x%02x   divide by zero\n", SW_ZERODIVIDE);
    printf ("0x%02x   overflow\n", SW_OVERFLOW);
    printf ("0x%02x   underflow\n", SW_UNDERFLOW);
    printf ("0x%02x   inexact\n", SW_INEXACT);
#endif
    printf ("\n");

    printf ("Rounding modes:\n");
#ifdef __sparc
    printf ("0x%02x   nearest\n", FP_RN);
    printf ("0x%02x   to zero\n", FP_RZ);
    printf ("0x%02x   to plus infinity\n", FP_RP);
    printf ("0x%02x   to minus infinity\n", FP_RM);
#endif
#ifdef WIN32
    printf ("0x%02x   nearest\n", (RC_NEAR & MCW_RC) >> 8);
    printf ("0x%02x   to zero\n", (RC_CHOP & MCW_RC) >> 8);
    printf ("0x%02x   to plus infinity\n", (RC_UP & MCW_RC) >> 8);
    printf ("0x%02x   to minus infinity\n", (RC_DOWN & MCW_RC) >> 8);
#endif
    printf ("\n");

#ifdef WIN32
    printf ("Precision:\n");
    printf ("0x%02x   64 bits\n", (PC_64 & MCW_PC) >> 16);
    printf ("0x%02x   53 bits\n", (PC_53 & MCW_PC) >> 16);
    printf ("0x%02x   24 bits\n", (PC_24 & MCW_PC) >> 16);

    printf ("\n");
#endif

/*
#ifdef WIN32
    printf ("setting precision to 24 bits\n");
    _controlfp (PC_24, MCW_PC);
    printf ("now FP control word is 0x%05x\n\n", _controlfp(0,0));
#endif
*/

/*
#ifdef WIN32
    printf ("setting precision to 64 bits\n");
    _controlfp (PC_64, MCW_PC);
    printf ("now FP control word is 0x%05x\n\n", _controlfp(0,0));
#endif
*/

/*
#ifdef WIN32
    printf ("enabling all exception traps\n");
    _control87 (0, MCW_EM);
    printf ("now FP control word is 0x%05x\n\n", _controlfp(0,0));
#endif
*/

/*
#ifdef __sparc
    printf ("enabling all exception traps\n");
    fpsetmask (FP_X_INV | FP_X_OFL | FP_X_UFL | FP_X_DZ | FP_X_IMP);
    printf ("now mask is 0x%x\n\n", fpgetmask());
#endif
*/

    printf ("exception mask = 0x%02x\n", GET_EXC_MASK);
    printf (" rounding mode = 0x%02x\n", GET_ROUNDING_MODE);
#ifdef WIN32
    printf ("     precision = 0x%02x\n", GET_PRECISION);
#endif
    printf ("\n");

    l = ~(1<<(8*sizeof(l)-1));
    printf ("largest positive integer is %d(0x%x)\n", l, l);

    n.f = (float) l;
    printf ("largest positive integer as a float is %f(0x%x)\n", n.f, n.l);
    printf ("\n");

    printf ("\n");

    printf ("Test integer conversion for some specific cases ...\n");
    printf ("\n");

    n.l = 0x4f000000;               // this one fails with Visual C++ 5.0
    printf ("n.l = 0x%08x\n", n.l);
    printf ("n.f = %.4f(%.12e)\n", n.f, n.f);
    CLR_EXC_STATUS;
    l = (long) n.f;
    exc = GET_EXC_STATUS;
    err = exc != INVALID_FLAG;
    printf ("int = %d(0x%08x)\n", l, l);
    printf ("exc = 0x%02x --- %s\n", exc, err ? "ERROR HERE !!" : "OK");
    printf ("\n");

    n.l = 0x4effffff;
    printf ("n.l = 0x%08x\n", n.l);
    printf ("n.f = %.4f(%.12e)\n", n.f, n.f);
    CLR_EXC_STATUS;
    l = (long) n.f;
    exc = GET_EXC_STATUS;
    err = exc == INVALID_FLAG;
    printf ("int = %d(0x%08x)\n", l, l);
    printf ("exc = 0x%02x --- %s\n", exc, err ? "ERROR HERE !!" : "OK");
    printf ("\n");

    n.l = 0x5f000000;
    printf ("n.l = 0x%08x\n", n.l);
    printf ("n.f = %.4f(%.12e)\n", n.f, n.f);
    CLR_EXC_STATUS;
    l = (long) n.f;
    exc = GET_EXC_STATUS;
    err = exc != INVALID_FLAG;
    printf ("int = %d(0x%08x)\n", l, l);
    printf ("exc = 0x%02x --- %s\n", exc, err ? "ERROR HERE !!" : "OK");
    printf ("\n");

    n.l = 0x5effffff;               // this one fails with Visual C++ 5.0
    printf ("n.l = 0x%08x\n", n.l);
    printf ("n.f = %.4f(%.12e)\n", n.f, n.f);
    CLR_EXC_STATUS;
    l = (long) n.f;
    exc = GET_EXC_STATUS;
    err = exc != INVALID_FLAG;
    printf ("int = %d(0x%08x)\n", l, l);
    printf ("exc = 0x%02x --- %s\n", exc, err ? "ERROR HERE !!" : "OK");
    printf ("\n");

    printf ("\n");


/*
--
-- IEEE 754 states that:
-- Invalid operation occurs when "7) Conversion of a binary floating-point
-- number to an integer or decimal format when overflow, infinity or NaN
-- precludes a faithful representation in that format and this cannot
-- otherwise be signaled".
--
-- (What does "cannot otherwise be signaled" mean here?)
--
-- My understanding is that this means the invalid exception should be
-- raised, but the result (when the exception is untrapped) is not defined.
-- (This is where the '?' occurs in the Cant vectors.)
--
-- On Solaris, the result is the largest positive integer (0x7fffffff) and
-- the invalid exception. But on Windows, strange things seem to happen
-- for certain values.
--
-- The largest unsigned integer in 32 bits is 2^30-1 (0x7fffffff).
-- As a 32-bit floating point number, this is 0x4f000000.
--
-- On Windows, convert-to-integer on floats with this value upwards as far
-- as 2^(31+32)-1 (0x5effffff) seem to give strange behaviour, i.e. no
-- invalid exception is flagged. Conversions of float values upwards of
-- this (0x5f000000) do correctly flag the invalid exception.
--
-- It is as if the conversion is being done to a 64-bit integer somehow.
--
-- There is a bug report from 1997 concerning the Pentium II and Pentium
-- Pro (see http://www.ddj.com/documents/s=938/ddj9708f/9708f.htm) which
-- seems to describe a very similar behaviour - though not identical as it
-- refers to (a) negative floating-point values, (b) the value zero being
-- written to the result, (c) the inexact exception being flagged, and (d)
-- the problem not occurring with rounding mode DOWN, none of which are
-- consistent with the behaviour I am seeing.
-- The bug was fixed by Intel in 1998. The processor in my laptop is a
-- Pentium III, which should not have the problem AFAIK.
--
-- I think the problem is with the compiler:
-- Having studied the (disassembled) code created by the VC++ compiler, it
-- seems to be doing exactly the above, i.e. the conversion from float to
-- long int is done with a 64-bit destination (2 of the 32-bit general
-- purpose registers in the Pentium), producing the correct value
-- and no exceptions.  But the high-order 32 bits are then thrown away,
-- leaving just whatever happened to be in the low 32 bits as the result.
--
-- In particular, the following instructions
--
--       fistp       qword ptr [ebp-0Ch]
--       mov         eax,dword ptr [ebp-0Ch]
--       mov         edx,dword ptr [ebp-8]
--
-- convert the float in the FPU register ST(0) to a quadword (64 bits)
-- integer in the register EDX:EAX, i.e. the high-order 32 bits in the
-- general purpose register EDX and the low 32 bits in EAX. (EBP is one
-- of the Pentium general registers, used as a pointer to data on the
-- stack. Data bytes are stored little-endian in memory by the Intel CPU.)
-- But the subsequent code only uses the value in EAX, discarding the
-- high-order doubleword (32 bits) in EDX.
--
*/

    printf ("Start at smallest positive FP number ...\n");
    printf ("\n");

    n.l = 0x00000001;
    printf ("n.l = 0x%08x\n", n.l);
    printf ("n.f = %.4f(%.12e)\n", n.f, n.f);

    printf ("\n");

    printf ("... and multiply by 2 until conversion to integer flags invalid operation ...\n");
    printf ("\n");

    CLR_EXC_STATUS;
    l = (long) n.f;
    exc = GET_EXC_STATUS;

    while (exc != INVALID_FLAG)
    {
        n.f *= 2.0f;

        CLR_EXC_STATUS;
        l = (long) n.f;
        exc = GET_EXC_STATUS;
    }

    printf ("n.l = 0x%08x\n", n.l);
    printf ("n.f = %.4f(%.12e)\n", n.f, n.f);
    printf ("int = %d(0x%08x)\n", l, l);
    printf ("exc = 0x%02x\n", exc);
    printf ("\n");
    printf ("\n");

    printf ("Now continue up to the max exponent, and check that invalid exception occurs each time ...\n");

    err = 0;
    while ((n.l & 0x7f800000) < 0x7f800000)
    {
        n.f *= 2.0f;

        CLR_EXC_STATUS;
        l = (long) n.f;
        exc = GET_EXC_STATUS;

        if (exc != INVALID_FLAG)
        {
            printf ("n.l = 0x%08x\n", n.l);
            printf ("n.f = %.4f(%.12e)\n", n.f, n.f);
            printf ("int = %d(0x%08x)\n", l, l);
            printf ("exc = 0x%02x\n", exc);
            printf ("\n");
        }
    }

    printf ("\n");
    printf ("\n");

    printf ("Start at largest positive FP number ...\n");
    printf ("\n");

    n.l = 0x7f7fffff;
    printf ("n.l = 0x%08x\n", n.l);
    printf ("n.f = %.4f(%.12e)\n", n.f, n.f);

    printf ("\n");

    printf ("... and successively divide by 10 until FP value is less than 1 ...\n");
    printf ("\n");

    CLR_EXC_STATUS;
    l = (long) n.f;
    exc = GET_EXC_STATUS;
    printf ("int = %d(0x%08x)\n", l, l);
    printf ("exc = 0x%02x (conversion)\n", exc);
    printf ("\n");

    while (n.f > 1.0f)
    {
        CLR_EXC_STATUS;
        n.f /= 10.0f;
        exc = GET_EXC_STATUS;
        printf ("exc = 0x%02x (division)\n", exc);
        printf ("n.l = 0x%08x\n", n.l);
        printf ("n.f = %.4f(%.12e)\n", n.f, n.f);
        CLR_EXC_STATUS;
        l = (long) n.f;
        exc = GET_EXC_STATUS;
        printf ("int = %d(0x%08x)\n", l, l);
        printf ("exc = 0x%02x (conversion)\n", exc);
        printf ("\n");
    }

    return 0;
}
