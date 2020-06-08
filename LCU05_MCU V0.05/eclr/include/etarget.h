#ifndef __TARGET_H__
#define __TARGET_H__

// MUST be defined here !!!!
// compiler dependent declarations
typedef unsigned long long  uint64;
typedef long long           sint64;
#define PLATFORM_LONGLONG_SUPPORT

typedef unsigned int        uint32;
typedef signed   int        sint32;
typedef unsigned short int  uint16;
typedef signed   short int  sint16;
typedef unsigned char       uint8;
typedef signed   char       sint8;

#ifdef PLATFORM_INTERPRETER_SUPPORT
// target (platform/cpu) dependent declarations
#define PLATFORM_PROCFAMILY     CpuType_FamSim  
#define PLATFORM_PROCTYPE       CpuType_Sim32    
#define PLATFORM_PROGMODEL      ProgModel_Sim1 
typedef uint32                  TargetInstructionType;

#else
// target (platform/cpu) dependent declarations
#define PLATFORM_PROCFAMILY     CpuType_FamArmThumb  
#define PLATFORM_PROCFAMILY_ARMTHUMB  // necessary for special treatment within eCLr kernel
#define PLATFORM_PROCTYPE       CpuType_ArmThumb2    
#define PLATFORM_PROGMODEL      ProgModel_Arm 
typedef uint16                  TargetInstructionType;

#endif


#define PLATFORM_DATAMODEL          PLATFORM_DATAMODEL_ILP32
#define PLATFORM_STACKPOLICY        TARGET_STACKPOLICY_DOWNWARD


// macro to get the 'real' function pointer of a class member
// thiscall_p -> c++ member funtion poiner
typedef void(*__fp__)();                             // void f(void)
inline __fp__ get_class_method(void* p)
{
    return *((__fp__*)(p));
}
inline void set_class_method(void* p, __fp__ f)
{
    *((__fp__*)(p)) = f;
}

#endif //#ifndef __TARGET_H__
