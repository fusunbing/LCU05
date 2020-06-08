/*
+------------------------------------------------------------------------------+
| ******************************** TOOLS.H *********************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Tools for communication                                                      |
+------------------------------------------------------------------------------+
| Author: Jörg Kraemer, started at: 29.09.2005                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __TOOLS_H__
#define __TOOLS_H__

#define VERSION_NUMBER 1

typedef enum _StandardCommunicationErrors
{   
    CommErrors_None=0,
    // masks
    CommErrors_CommunicationLayerMask=    0x80000000,
    CommErrors_RemotingLayerMask=         0x40000000,
    CommErrors_CommonServiceLayerMask=    0x20000000,
    CommErrors_ServiceLayerMask=          0x00000000,

	// communication layer
	CommErrors_ServerAuthenticationFailure=     0x80000001,	// If the device (server) provides wrong authentication to the application (client) then this error code is used at the client side.
	CommErrors_AuthorizationFailure=      0x80000002,

    // remoting layer
    CommErrors_InvalidProtocolVersion=    0x40000001,
    CommErrors_UnknownServiceHandle=      0x40000002,
    CommErrors_UnknownMethodHandle=       0x40000003,
    CommErrors_InvalidCommand=            0x40000004,
    CommErrors_InputStreamEmpty=          0x40000005,
    CommErrors_HeaderXsumError=           0x40000006,

    // common service errors
    CommErrors_OutOfMemory=               0x20000001,
    CommErrors_InvalidHandle=             0x20000002,
    CommErrors_ParameterOutOfRange=       0x20000003,
    CommErrors_InvalidServiceState=       0x20000004,
    CommErrors_OtherMasterActive=         0x20000005,
    CommErrors_NoProgramOnTheDevice=      0x20000006,
    CommErrors_ExecutionError=            0x20000007,
    CommErrors_ReadOnly=                  0x20000008,
	CommErrors_InvalidEclrState=          0x20000009,
    CommErrors_InternalError=             0x2000000A,
	CommErrors_AuthenticationFailure=     0x2000000B,	// BasicSecurityService, SecurityService

    //  These errors are specific for a particular service. 
    //  Every service defines its own set of errors. 
    //  Specific error codes are documented with the service. 

    // DataAccessService
    CommErrors_TempListServiceActive=     0x1,
    CommErrors_NoTempListServiceActive=   0x2,
    CommErrors_CannotReadVariableValues=  0x3,
    CommErrors_CannotWriteVariableValues= 0x4,
    CommErrors_CannotCreateVariableGroup= 0x5,
    CommErrors_CannotAccessClrData=       0x6,

    // DeviceInfoService
    //CommErrors_...=      0x101,

    // ForceService
    CommErrors_CannotAddToForceList=      0x201,
    CommErrors_CannotReadForceList=       0x202,

    // DownloadChanges
    CommErrors_DC_RealtimeViolation=      0x301,
    CommErrors_DC_Prolog=                 0x302,
    CommErrors_DC_OutOfMemory=            0x303,
    CommErrors_DC_ForceList=              0x304,

    // DataAccessService2
    CommErrors_TooManyPreCyclesRecorded=         0x401,
    CommErrors_SubscriptionHandleNotFound=       0x402,
    CommErrors_ClientNotFound=                   0x403,
    CommErrors_SubscriptionCouldNotBeCreated=    0x404,
    CommErrors_DebugValueListError=              0x405,
    CommErrors_InterfaceObjectCouldNotBeCreated= 0x406,
    CommErrors_taskCallbackCouldNotBeRegistered= 0x407,

	// PasswordConfigurationService
    CommErrors_IdentityNameTooLong = 0x501,
    CommErrors_PasswordTooShort = 0x502,
    CommErrors_PasswordTooLong = 0x503,

} StandardCommunicationErrors;

typedef struct coHead
{
    uint16 versionAndFlags;         // lower 8 Bit version of Protokoll = 1, upper 8 Bit for Status: 
    //  (Upload) 0x01 -> not last packet, 0x02 -> not first packet
    uint16 command;                 // 0->GetServiceRequest, 1->GetServiceConfirmation, 2->InvokeRequest,
    // 3->InvokeConfirmation, >3 -> Error  
    uint16 additionalHeaderSize;    // normaly 0, > 0 Size of additional Header
    uint16 commandLength;           // Length of Command (excluding all Heasder length)
    uint16 serviceHandle;           // Handle of service actual invoked
    uint16 methodHandle;            // Handle of method actual invoked
    uint16 invokeID;                // not used in Step 1
    uint16 xsum;                    // header xsum, xsum == 0 : no xsum 
}
CommandHeader;
typedef struct _extendedcommandheader
{
    CommandHeader  head;
    uint32         ext[8];  // 32 bytes header externsion at maximum
} ExtendedCommandHeader;

typedef struct extBufferHead
{
    sint16 additionalArrayLength;   // Incremental length of last transmitted byte array
    sint16 padding;                 // Padding - Must be 0.
    sint32 dividedArrayLengthPos;   // Position of the actual byte[] length in the stream
}
ExtendBufferHeader;

enum ClrStatus
{
    Status_Ok               = 200,
    Status_Error            = 400,
    Status_NotAttached      = 401,
    Status_Illegal          = 500,
    Status_NotFinished      = 600

};

#define COMM_BUFFER_SIZE (250 * sizeof(int))
#define MAX_OUTPUT_LENGTH (COMM_BUFFER_SIZE - sizeof(CommandHeader))
#define IN_DATA_OFFSET (sizeof(CommandHeader))


//#define COMMUNICATIONS_TEST // Schalter um Testmode aus-/einzuschalten


#endif // __TOOLS_H__
