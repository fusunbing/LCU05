#ifndef __CODE_Sim32_H__
#define __CODE_Sim32_H__

class BlockHeap;
class CilSim32 
{
public :
    enum CpuRegister
    {
        Reg0    = 0,
        Reg1    = 1,
        Reg2    = 2,
        Reg3    = 3,
        Reg4    = 4,
        Reg5    = 5,
        Reg6    = 6,
        Reg7    = 7,
        Reg8    = 8,
        Reg9    = 9,
        Reg10   = 10,
        Reg11   = 11,
        Reg12   = 12,
        Reg13   = 13,
        Reg14   = 14,
        Reg15   = 15,
        RegFp   = Reg13,
        RegSp   = Reg14,
        RegRa   = Reg15,
        RegArg1 = Reg0,
        RegArg2 = Reg1,
        RegArg3 = Reg2,
        RegArg4 = Reg3,
        RegArg5 = Reg4,
        RegArg6 = Reg5,
        RegArg7 = Reg6,
        RegRet1 = Reg0,
        RegRet2 = Reg1,
        RegAmount = 16
    };

    enum Sim32Op
    {
        Sim32iLdb    = 0x00,
        Sim32iLdbx   = 0x01,
        Sim32iLdbu   = 0x02,
        Sim32iLdbux  = 0x03,
        Sim32iLdh    = 0x04,
        Sim32iLdhx   = 0x05,
        Sim32iLdhu   = 0x06,
        Sim32iLdhux  = 0x07,
        Sim32iLdw    = 0x08,
        Sim32iLdwx   = 0x09,
        Sim32iStb    = 0x0a,
        Sim32iStbx   = 0x0b,
        Sim32iSth    = 0x0c,
        Sim32iSthx   = 0x0d,
        Sim32iStw    = 0x0e,
        Sim32iStwx   = 0x0f,
        Sim32iMovi   = 0x10,
        Sim32iMovix  = 0x11,
        Sim32iAddi   = 0x12,
        Sim32iMuli   = 0x13,
        Sim32iDivi   = 0x14,
        Sim32iAndi   = 0x15,
        Sim32iOri    = 0x16,
        Sim32iXori   = 0x17,
        Sim32iBr     = 0x18,
        Sim32iBrx    = 0x19,
        Sim32iBge    = 0x1a,
        Sim32iBgex   = 0x1b,
        Sim32iBlt    = 0x1c,
        Sim32iBltx   = 0x1d,
        Sim32iBne    = 0x1e,
        Sim32iBnex   = 0x1f,
        Sim32iBeq    = 0x20,
        Sim32iBeqx   = 0x21,
        Sim32iBgeu   = 0x22,
        Sim32iBgeux  = 0x23,
        Sim32iBltu   = 0x24,
        Sim32iBltux  = 0x25,
        Sim32iBtrue  = 0x26,
        Sim32iBtruex = 0x27,
        Sim32iBfalse = 0x28,
        Sim32iBfalsex= 0x29,
        Sim32iNop    = 0x2a,
        // sim extended instructions
        Sim32jCallxm = 0x2b,
        Sim32jCallxu = 0x2c,
        Sim32jCallxus= 0x2d,
        Sim32jCallrx = 0x2e,
        Sim32jCallrxs= 0x2f,
        Sim32jBreak  = 0x30,
        // r-code
        Sim32rRet    = 0x31,
        Sim32rJmp    = 0x32,
        Sim32rMov    = 0x33,
        Sim32rNor    = 0x34,
        Sim32rAnd    = 0x35,
        Sim32rOr     = 0x36,
        Sim32rXor    = 0x37,
        Sim32rCmpeq  = 0x38,
        Sim32rCmpne  = 0x39,
        Sim32rCmpge  = 0x3a,
        Sim32rCmpgeu = 0x3b,
        Sim32rCmplt  = 0x3c,
        Sim32rCmpltu = 0x3d,
        Sim32rSll    = 0x3e,
        Sim32rSlli   = 0x3f,
        Sim32rSrl    = 0x40,
        Sim32rSrli   = 0x41,
        Sim32rSra    = 0x42,
        Sim32rSrai   = 0x43,
        Sim32rAdd    = 0x44,
        Sim32rSub    = 0x45,
        Sim32rMul    = 0x46,
        Sim32rMulu   = 0x47,
        Sim32rDiv    = 0x48,
        Sim32rDivu   = 0x49,

        Sim32Illegal = 0xFF,       
        
        Sim32_end    = 0x00
    };

    typedef struct _jtypebits
    { 
        uint32
        // 0 .. 15 
        op	    :   7,
        regargs	:   3, // amount of reg32 args
        retsize	:   3, // return type size
        maxsize :   3, // maximum arg type size
        // 16 ..31
        imm16	:   16;
    } JTypeBits;

    typedef struct _jtypebitslow
    { 
        uint16
            op	    :   7,
            regargs	:   3, // amount of reg32 args
            retsize	:   3, // return type size
            maxsize :   3; // maximum arg type size
    } JTypeBitsLow;
    typedef struct _jtypebitshigh
    { 
        uint16  
            imm16 : 16;
    } JTypeBitsHigh;

    typedef struct _itypebits
    { 
        uint32
        // 0 .. 15 
        op	    :   7,
        b	    :   4,
        a	    :   4,
        empty   :   1,
        // 16 ..31
        imm16	:   16;
    } ITypeBits;
    typedef struct _itypebitslow
    { 
        uint16
        op	    :   7,
        b	    :   4,
        a	    :   4,
        empty   :   1;
    } ITypeBitsLow;
    typedef struct _itypebitshigh
    { 
        uint16
        // 16 ..31
        imm16	:   16;
    } ITypeBitsHigh;

    typedef struct _rtypebits
    { 
        uint32
        // 0 .. 15 
        op      :   7,
        imm6	:   6,
        empty1  :   3,
        // 16 ..31
        c	    :   4,
        b	    :   4,
        a	    :   4,
        empty2  :   4;
    } RTypeBits;
    typedef struct _rtypebitslow
    { 
        uint16
        // 0 .. 15 
        op      :   7,
        imm6	:   6,
        empty1  :   3;
    } RTypeBitsLow;
    typedef struct _rtypebitshigh
    { 
        uint16
        c	    :   4,
        b	    :   4,
        a	    :   4,
        empty2  :   4;
    } RTypeBitsHigh;

    typedef union _sim32code
    {
        uint32 code;
        ITypeBits i;
        JTypeBits j;
        RTypeBits r;
    } Sim32Code;

    typedef uint32 RegType;
    typedef sint32 RegTypeS;
    //typedef uint64 RegType;
    class CilArgs
    {
    public:
        RegType     m_arg1;
        RegType     m_arg2;
        RegType     m_arg3;
        RegType     m_arg4;
        RegType     m_ret1;
        RegType     m_ret2;
        uint32*     m_entry;
        RegType*    m_pStackTop;   
        RegType*    m_pStackEnd;
        void*       m_pCodeStart;
        void*       m_pCodeEnd;
        BlockHeap*  m_pMeta;
    };

    CilSim32();
    void Init(void* pStart, uint32 size, BlockHeap* pMeta);
    int Invoke(void (*pCode)());
    int Invoke(void (*pCode)(int p0), int p0);
    int Invoke(void* (*pInvoke)(), void*& result);
    int Invoke(void* (*pInvoke)(int p0), int p0, void*& result);

    void InvokeUnsafe(Object* pObj, void (*pCode)());
    int  InvokeUnsafe(Object* pThis, int (*pInvoke)(Object* p0), Object* p0, int& result);
    static inline int get_op(uint32 instr)
    {
        return (int)(instr & 0x7F);
    }
    static inline int get_i_b(uint32 instr)
    {
        return (int)((instr >> 7) & 0x0F);
    }
    static inline int get_i_a(uint32 instr)
    {
        return (int)((instr >> 11) & 0x0F);
    }
    static inline int get_i_imm16(uint32 instr)
    {
        return (int)((sint16)((instr >> 16) & 0xFFFF));
    }
    static inline int get_retsize(uint32 instr)
    {
        return (int)((instr >> 10) & 0x07);
    }
    static inline int get_regargs(uint32 instr)
    {
        return (int)((instr >> 7) & 0x07);
    }
    static inline int get_maxsize(uint32 instr)
    {
        return (int)((instr >> 13) & 0x07);
    }
    static inline int get_r_c(uint32 instr)
    {
        return (int)((instr >> 16) & 0x0F);
    }
    static inline int get_r_b(uint32 instr)
    {
        return (int)((instr >> 20) & 0x0F);
    }
    static inline int get_r_a(uint32 instr)
    {
        return (int)((instr >> 24) & 0x0F);
    }
    static inline int get_r_imm6(uint32 instr)
    {
        return (int)((instr >> 7) & 0x3F);
    }

private:
    void invokeDummy1();
    int invokeDummy2(Object*);
    static int exec(CilArgs* pArgs);
    static void invokeUnmanagedStatic(uint32 c,  void (*pMethod)(), RegType* pRegs);
    static void invokeUnmanaged(uint32 c, CilSim32* pThis, void (*pMethod)(), RegType* pRegs);
    static void stackCheck(void* p);
    static void dumpMethodInfo(void (*pMethod)(), BlockHeap* pMeta);
    int  invoke(uint32* pEntry, RegType p0, RegType& result);

    uint32 invoke_r32_0(){ return 0; }
    uint32 invoke_r32_1(uint32 ){ return 0; }
    uint32 invoke_r32_2(uint32 ,uint32){ return 0; }
    uint32 invoke_r32_3(uint32,uint32,uint32){ return 0; }
    uint32 invoke_r32_4(uint32,uint32,uint32,uint32){ return 0; }
    uint32 invoke_r32_5(uint32,uint32,uint32,uint32,uint32){ return 0; }
    uint32 invoke_r32_6(uint32,uint32,uint32,uint32,uint32,uint32){ return 0; }
    uint32 invoke_r32_7(uint32,uint32,uint32,uint32,uint32,uint32,uint32){ return 0; }
    uint32 invoke_r32_8(uint32,uint32,uint32,uint32,uint32,uint32,uint32,uint32){ return 0; }
    uint32 invoke_r64_0(){ return 0; }
    uint64 invoke_r64_1(uint32){ return 0; }
    uint64 invoke_r64_2(uint32,uint32){ return 0; }
    uint64 invoke_r64_3(uint32,uint32,uint32){ return 0; }
    uint64 invoke_r64_4(uint32,uint32,uint32,uint32){ return 0; }
    uint64 invoke_r64_5(uint32,uint32,uint32,uint32,uint32){ return 0; }
    uint64 invoke_r64_6(uint32,uint32,uint32,uint32,uint32,uint32){ return 0; }
    uint64 invoke_r64_7(uint32,uint32,uint32,uint32,uint32,uint32,uint32){ return 0; }
    uint64 invoke_r64_8(uint32,uint32,uint32,uint32,uint32,uint32,uint32,uint32){ return 0; }

    void* m_pCodeStart;
    void* m_pCodeEnd;
    BlockHeap* m_pMeta;
    static bool isLogging;
};

#endif //#ifndef __CODE_Sim32_H__
