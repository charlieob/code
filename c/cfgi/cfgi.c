/*---------------------------------------------------------------------------*\
|
|   Configuration Data - Check Imported Configuration Constant Definitions
|
\*---------------------------------------------------------------------------*/

#if (! defined CFG_PHASE_R2_0 && ! defined CFG_PHASE_R2_1) ||\
    (defined CFG_PHASE_R2_0 && defined CFG_PHASE_R2_1)
    #error BB badly configured
#endif


