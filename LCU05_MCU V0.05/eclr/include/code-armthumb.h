#ifndef __CODE_ARMTHUMB_H__
#define __CODE_ARMTHUMB_H__

#define DATA_BLOCK_MARKER 0xDDDD			// use an opcode that is not an instruction

enum Arm7Register
{
    _r0=0,  _r1,    _r2,    _r3,    _r4,    _r5,    _r6,    _r7,
    _r8,    _r9,    _r10,   _fp,    _ip,    _sp,    _lr,    _pc
};


#define OPBITS( a, b, c, d ) ( ( a << 3 ) | ( b << 2 ) | ( c << 1 ) | ( d ) )

#define SBZ        0x0000                    // SBZ = Should be zero
#define ignore     0xFFFFFFFF

#define MASK_CONDITION        0xF0000000

enum ArmThumbConditions
{
    ArmThumbEqual               =    OPBITS( 0, 0, 0, 0 ),        // Equal
    ArmThumbNotEqual            =    OPBITS( 0, 0, 0, 1 ),        // Not equal
    ArmThumbGreaterEqualU       =    OPBITS( 0, 0, 1, 0 ),        // Carry Set/unsigned higher or same
    ArmThumbLessU               =    OPBITS( 0, 0, 1, 1 ),        // Carry Clear/unsigned lower
    
    ArmThumbNegative            =    OPBITS( 0, 1, 0, 0 ),        // Minus/Negative
    ArmThumbPositiveZero        =    OPBITS( 0, 1, 0, 1 ),        // Plus/Positive or zero
    ArmThumbOverflow            =    OPBITS( 0, 1, 1, 0 ),        // Overflow
    ArmThumbNoOverflow          =    OPBITS( 0, 1, 1, 1 ),        // No overflow
    
    ArmThumbGreaterU            =    OPBITS( 1, 0, 0, 0 ),        // Unsigned higher
    ArmThumbLessEqualU          =    OPBITS( 1, 0, 0, 1 ),        // Unsigned lower or same
    ArmThumbGreaterEqual        =    OPBITS( 1, 0, 1, 0 ),        // Signed greater than or equeal
    ArmThumbLess                =    OPBITS( 1, 0, 1, 1 ),        // Signed less than

    ArmThumbGreater             =    OPBITS( 1, 1, 0, 0 ),        // Signed greater than
    ArmThumbLessEqual           =    OPBITS( 1, 1, 0, 1 ),        // Signed less than or equal
    ArmThumbAlways              =    OPBITS( 1, 1, 1, 0 ),        // Always
    ArmThumbNever               =    OPBITS( 1, 1, 1, 1 )         // Never - Dont Use!
};

enum ArmThumbITBlock
{
	IT							=    OPBITS( 1, 0, 0, 0 ),
    IT_T						=    OPBITS( 0, 1, 0, 0 ),
    IT_E						=    OPBITS( 1, 1, 0, 0 ),
    IT_TT						=    OPBITS( 0, 0, 1, 0 ),
    IT_ET						=    OPBITS( 1, 0, 1, 0 ),
    IT_TE						=    OPBITS( 0, 1, 1, 0 ),
	IT_EE						=    OPBITS( 1, 1, 1, 0 ),

	IT_TTT						=    OPBITS( 0, 0, 0, 1 ),
	IT_ETT						=    OPBITS( 1, 0, 0, 1 ),
	IT_TET						=    OPBITS( 0, 1, 0, 1 ),
	IT_EET						=    OPBITS( 1, 1, 0, 1 ),
	IT_TTE						=    OPBITS( 0, 0, 1, 1 ),
	IT_ETE						=    OPBITS( 1, 0, 1, 1 ),
	IT_TEE						=    OPBITS( 0, 1, 1, 1 ),
	IT_EEE						=    OPBITS( 1, 1, 1, 1 ),
};

enum ArmThumbPushPop
{
	PP_Reg0						=    0x0001,
    PP_Reg1						=    0x0002,
    PP_Reg2						=    0x0004,
    PP_Reg3						=    0x0008,
    PP_Reg4						=    0x0010,
    PP_Reg5						=    0x0020,
	PP_Reg6						=    0x0040,
	PP_Reg7						=    0x0080,
	PP_Reg8						=    0x0100,
    PP_Reg9						=    0x0200,
    PP_Reg10					=    0x0400,
    PP_Reg11					=    0x0800,
    PP_Reg12					=    0x1000,
};


typedef struct _cilopnames
{
    int opcode;
    char text[32];
} CILOpNames;

// -------------------------------------------------------------------------------------

inline bool is12BitDistance(int dist)
{
    bool result = false;
    if (dist >= -2048 && dist <= 2047)
    {
        return true;
    }
    return result;
}

#endif //#ifndef __CODE_ArmThumb_H__
