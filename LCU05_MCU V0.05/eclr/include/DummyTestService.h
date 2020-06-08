#ifndef __DUMMYTESTSERVICE_H__
#define __DUMMYTESTSERVICE_H__

#include "Services.h"
#include "CallbackBase.h"
#include "math.h"
#include "RemotingServer.h"
#include "libc.h"

  typedef enum _MyDummyTestEnum
  {
      MyDummyTestEnum_Eins=0x1,
      MyDummyTestEnum_Zwei=0x2,
      MyDummyTestEnum_Drei=0x3,
  } MyDummyTestEnum;
  
  typedef struct _MyDummyTestStruct
  {
      sint8 int8Value;
      uint8 uint8Value;
      sint16 int16Value;
      uint16 uint16Value;
      sint32 int32Value;
      uint32 uint32Value;
      double doubleValue;
      float floatValue;
  } MyDummyTestStruct;

class CDummyTestServiceImplBase;
// class for sending callback messages of the Ade.Internal.Remoting.Services.IDummyTestService Service implementation
////////////////////////////////////////////
class CDummyTestServiceImplCallback : public CCallbackBase
{
public:
    CDummyTestServiceImplCallback(CDummyTestServiceImplBase* pServiceBase);
    ~CDummyTestServiceImplCallback();
    void AllFloatTypesAsInputs();
    void AllFloatTypesAsOutput(double doubleOutput, float floatOutput);
    void AllIntegerTypesAsInputs();
    void AllIntegerTypesAsOutput(sint8 int8Output, uint8 uint8Output, sint16 int16Output, uint16 uint16Output, sint32 int32Output, uint32 uint32Output);
    void AnsiUtf8Strings(const char* const ansiOut, const char* const utf8Out);
    void BoolInputs();
    void ByteArrayInput();
    void ByteArrayOutput(uint32 _outputDataLength, uint8* outputData);
    void CreateException();
    void EnumInput();
    void EnumOutput(MyDummyTestEnum enumOutput);
    void FalseOutput(uint8 falseOutput);
    void IntArrayInput();
    void IntArrayOutput(uint32 _intArrayLength, sint32* intArray);
    void IntInputAsReturn(sint32 _returnValue);
    void ObjectArrayInput();
    void ObjectArrayOutput(uint32 _objectArrayLength, uint16* _objectArrayTypeId, uint32* objectArray);
    void ObjectInput();
    void OnlyReturn(sint32 _returnValue);
    void ReadLargeData(uint32 _byteDataLength, uint8* byteData);
    void ReceiveByteArrayPadding(sint16 int16Value, uint32 _byteArrayLength, uint8* byteArray, sint16 int16Value2);
    void ReceiveInt32Padding(uint8 byteValue, sint32 int32Value1, sint16 int16Value, sint32 int32Value2);
    void ReceiveNoPaddingRequired(uint8 trueValue, uint8 byteValue, sint16 int16Value, uint16 uint16Value, sint32 int32Value, float floatValue, double doubleValue);
    void ReceiveStringPadding(uint8 byteValue1, const char* const stringHallo, uint8 byteValue2, const char* const stringTest, uint8 byteValue3, const char* const stringTest2);
    void SendByteArrayPadding();
    void SendInt32Padding();
    void SendLargeData();
    void SendNoData();
    void SendNoPaddingRequired();
    void SendStringPadding();
    void SimpleInput();
    void SimpleOutput(sint32 intOutput);
    void StringArrayInput();
    void StringArrayOutput(uint32 _stringArrayLength, const char* const* stringArray);
    void StringInput();
    void StringOutput(const char* const stringOutput);
    void StructArrayOutput(uint32 _structArrOutputLength, MyDummyTestStruct* structArrOutput);
    void StructInput();
    void StructOutput(MyDummyTestStruct* structOutput);
    void TerminateConnection();
    void TrueOutput(uint8 trueOutput);
};

// Base class for Ade.Internal.Remoting.Services.IDummyTestService Service implementations
////////////////////////////////////////////
class CDummyTestServiceImplBase : public CServiceImplBase
{
public:
    CDummyTestServiceImplBase();
    ~CDummyTestServiceImplBase();
    void Invoke(sint16 methodHandle, char* const pDataBuffer);

protected:
    CDummyTestServiceImplCallback* m_pCallback;
    
    void setBinaryWriter(CBinaryWriter* pBinaryWriter)
    {   if(m_pCallback != NULL)
        {   m_pCallback->m_pBinaryWriter = pBinaryWriter;
        }
   }

   //Service method [1]: void AllFloatTypesAsInputs(Double doubleInput, Single floatInput);
    virtual void AllFloatTypesAsInputs(CBinaryReader* pBinaryReader);
    virtual void AllFloatTypesAsInputs(double doubleInput, float floatInput)=0;

   //Service method [2]: void AllFloatTypesAsOutput(out Double& doubleOutput, out Single& floatOutput);
    virtual void AllFloatTypesAsOutput();

   //Service method [3]: void AllIntegerTypesAsInputs(SByte int8Input, Byte uint8Input, Int16 int16Input, UInt16 uint16Input, Int32 int32Input, UInt32 uint32Input);
    virtual void AllIntegerTypesAsInputs(CBinaryReader* pBinaryReader);
    virtual void AllIntegerTypesAsInputs(sint8 int8Input, uint8 uint8Input, sint16 int16Input, uint16 uint16Input, sint32 int32Input, uint32 uint32Input)=0;

   //Service method [4]: void AllIntegerTypesAsOutput(out SByte& int8Output, out Byte& uint8Output, out Int16& int16Output, out UInt16& uint16Output, out Int32& int32Output, out UInt32& uint32Output);
    virtual void AllIntegerTypesAsOutput();

   //Service method [5]: void AnsiUtf8Strings(String ansi, String utf8, out String& ansiOut, out String& utf8Out);
    virtual void AnsiUtf8Strings(CBinaryReader* pBinaryReader);
    virtual void AnsiUtf8Strings(const char* const ansi, const char* const utf8)=0;

   //Service method [6]: void BoolInputs(Boolean trueInput, Boolean falseInput);
    virtual void BoolInputs(CBinaryReader* pBinaryReader);
    virtual void BoolInputs(uint8 trueInput, uint8 falseInput)=0;

   //Service method [7]: void ByteArrayInput(Byte[] inputData);
    virtual void ByteArrayInput(CBinaryReader* pBinaryReader);
    virtual void ByteArrayInput(uint32 _inputDataLength, uint8* inputData)=0;

   //Service method [8]: void ByteArrayOutput(out Byte[]& outputData);
    virtual void ByteArrayOutput();

   //Service method [9]: void CreateException();
    virtual void CreateException();

   //Service method [10]: void EnumInput(MyDummyTestEnum enumInput);
    virtual void EnumInput(CBinaryReader* pBinaryReader);
    virtual void EnumInput(MyDummyTestEnum enumInput)=0;

   //Service method [11]: void EnumOutput(out MyDummyTestEnum& enumOutput);
    virtual void EnumOutput();

   //Service method [12]: void FalseOutput(out Boolean& falseOutput);
    virtual void FalseOutput();

   //Service method [13]: void IntArrayInput(Int32[] intArray);
    virtual void IntArrayInput(CBinaryReader* pBinaryReader);
    virtual void IntArrayInput(uint32 _intArrayLength, sint32* intArray)=0;

   //Service method [14]: void IntArrayOutput(out Int32[]& intArray);
    virtual void IntArrayOutput();

   //Service method [15]: Int32 IntInputAsReturn(Int32 intInput);
    virtual void IntInputAsReturn(CBinaryReader* pBinaryReader);
    virtual void IntInputAsReturn(sint32 intInput)=0;

   //Service method [16]: void ObjectArrayInput(Object[] objectArray);
    virtual void ObjectArrayInput(CBinaryReader* pBinaryReader);
    virtual void ObjectArrayInput(uint32 _objectArrayLength, uint16* _objectArrayTypeId, uint32* objectArray)=0;

   //Service method [17]: void ObjectArrayOutput(out Object[]& objectArray);
    virtual void ObjectArrayOutput();

   //Service method [18]: void ObjectInput(Object obj);
    virtual void ObjectInput(CBinaryReader* pBinaryReader);
    virtual void ObjectInput(uint16 _objTypeId, uint32 obj)=0;

   //Service method [19]: Int32 OnlyReturn();
    virtual void OnlyReturn();

   //Service method [20]: void ReadLargeData(out Byte[]& byteData);
    virtual void ReadLargeData();

   //Service method [21]: void ReceiveByteArrayPadding(Boolean even, out Int16& int16Value, out Byte[]& byteArray, out Int16& int16Value2);
    virtual void ReceiveByteArrayPadding(CBinaryReader* pBinaryReader);
    virtual void ReceiveByteArrayPadding(uint8 even)=0;

   //Service method [22]: void ReceiveInt32Padding(out Byte& byteValue, out Int32& int32Value1, out Int16& int16Value, out Int32& int32Value2);
    virtual void ReceiveInt32Padding();

   //Service method [23]: void ReceiveNoPaddingRequired(out Boolean& trueValue, out Byte& byteValue, out Int16& int16Value, out UInt16& uint16Value, out Int32& int32Value, out Single& floatValue, out Double& doubleValue);
    virtual void ReceiveNoPaddingRequired();

   //Service method [24]: void ReceiveStringPadding(out Byte& byteValue1, out String& stringHallo, out Byte& byteValue2, out String& stringTest, out Byte& byteValue3, out String& stringTest2);
    virtual void ReceiveStringPadding();

   //Service method [25]: void SendByteArrayPadding(Int16 int16Value, Byte[] byteArray, Int16 int16Value2);
    virtual void SendByteArrayPadding(CBinaryReader* pBinaryReader);
    virtual void SendByteArrayPadding(sint16 int16Value, uint32 _byteArrayLength, uint8* byteArray, sint16 int16Value2)=0;

   //Service method [26]: void SendInt32Padding(Byte byteValue, Int32 int32Value1, Int16 int16Value, Int32 int32Value);
    virtual void SendInt32Padding(CBinaryReader* pBinaryReader);
    virtual void SendInt32Padding(uint8 byteValue, sint32 int32Value1, sint16 int16Value, sint32 int32Value)=0;

   //Service method [27]: void SendLargeData(Byte[] byteData);
    virtual void SendLargeData(CBinaryReader* pBinaryReader);
    virtual void SendLargeData(uint32 _byteDataLength, uint8* byteData)=0;

   //Service method [28]: void SendNoData();
    virtual void SendNoData();

   //Service method [29]: void SendNoPaddingRequired(Boolean trueValue, Byte byteValue, Int16 int16Value, UInt16 uint16Value, Int32 int32Value, Single floatValue, Double doubleValue);
    virtual void SendNoPaddingRequired(CBinaryReader* pBinaryReader);
    virtual void SendNoPaddingRequired(uint8 trueValue, uint8 byteValue, sint16 int16Value, uint16 uint16Value, sint32 int32Value, float floatValue, double doubleValue)=0;

   //Service method [30]: void SendStringPadding(Byte byteValue1, String stringHallo, Byte byteValue2, String stringTest, Byte byteValue3, String stringTest2);
    virtual void SendStringPadding(CBinaryReader* pBinaryReader);
    virtual void SendStringPadding(uint8 byteValue1, const char* const stringHallo, uint8 byteValue2, const char* const stringTest, uint8 byteValue3, const char* const stringTest2)=0;

   //Service method [31]: void SimpleInput(Int32 intInput);
    virtual void SimpleInput(CBinaryReader* pBinaryReader);
    virtual void SimpleInput(sint32 intInput)=0;

   //Service method [32]: void SimpleOutput(out Int32& intOutput);
    virtual void SimpleOutput();

   //Service method [33]: void StringArrayInput(String[] stringArray);
    virtual void StringArrayInput(CBinaryReader* pBinaryReader);
    virtual void StringArrayInput(uint32 _stringArrayLength, const char* const* stringArray)=0;

   //Service method [34]: void StringArrayOutput(out String[]& stringArray);
    virtual void StringArrayOutput();

   //Service method [35]: void StringInput(String stringInput);
    virtual void StringInput(CBinaryReader* pBinaryReader);
    virtual void StringInput(const char* const stringInput)=0;

   //Service method [36]: void StringOutput(out String& stringOutput);
    virtual void StringOutput();

   //Service method [37]: void StructArrayOutput(out MyDummyTestStruct[]& structArrOutput);
    virtual void StructArrayOutput();

   //Service method [38]: void StructInput(MyDummyTestStruct structInput);
    virtual void StructInput(CBinaryReader* pBinaryReader);
    virtual void StructInput(MyDummyTestStruct* structInput)=0;

   //Service method [39]: void StructOutput(out MyDummyTestStruct& structOutput);
    virtual void StructOutput();

   //Service method [40]: void TerminateConnection();
    virtual void TerminateConnection();

   //Service method [41]: void TrueOutput(out Boolean& trueOutput);
    virtual void TrueOutput();
};



class CDummyTestService : public CDummyTestServiceImplBase
{

public:
    static CServiceImplBase* CreateInstance();



   //Service method: void AllFloatTypesAsInputs(Double doubleInput, Single floatInput);
   void AllFloatTypesAsInputs(double doubleInput, float floatInput)
   {
       if ((fabs(doubleInput-8.8)>0.01)
           ||(fabs(floatInput-4.4)>0.01))
       {
            m_pCallback->m_pBinaryWriter->SendError(42,"Wrong numbers");
       }
       else
       {
            m_pCallback->AllFloatTypesAsInputs();
       }
   };

   //Service method: void AllFloatTypesAsOutput(out Double& doubleOutput, out Single& floatOutput);
   void AllFloatTypesAsOutput()
   {
       m_pCallback->AllFloatTypesAsOutput(8.8, 4.4f);
   };

   //Service method: void AllIntegerTypesAsInputs(SByte int8Input, Byte uint8Input, Int16 int16Input, UInt16 uint16Input, Int32 int32Input, UInt32 uint32Input);
   void AllIntegerTypesAsInputs(sint8 int8Input, uint8 uint8Input, sint16 int16Input, uint16 uint16Input, sint32 int32Input, uint32 uint32Input)
   {
        if ((int8Input!=-0x1)
           ||(uint8Input!=0x1)
           ||(int16Input!=-0x12)
           ||(uint16Input!=0x12)
           ||(int32Input!=-0x1234)
           ||(uint32Input!=0x1234))
       {
           m_pCallback->m_pBinaryWriter->SendError(42,"Wrong numbers");
       }
        else
        {

       m_pCallback->AllIntegerTypesAsInputs();
        }
   };

   //Service method: void AllIntegerTypesAsOutput(out SByte& int8Output, out Byte& uint8Output, out Int16& int16Output, out UInt16& uint16Output, out Int32& int32Output, out UInt32& uint32Output);
   void AllIntegerTypesAsOutput()
   {
       m_pCallback->AllIntegerTypesAsOutput(-0x1,0x1,-0x12,0x12,-0x1234,0x1234);
   };

   void AnsiUtf8Strings(const char* const ansi, const char* const utf8)
   {
       if (0!=strncmp(ansi, "Jörg",4))
       {
            m_pCallback->m_pBinaryWriter->SendError(1,"Ansi string is not Jörg");
            return;
       }
       if (5 != strlen(utf8))
       {
            m_pCallback->m_pBinaryWriter->SendError(2,"UTF8 string 'Jörg' shuld be 5 bytes.");
            return;
       }
       uint8 utf8JoergBytes[]={74,195,182,114,103,0};
       const char* const utf8Joerg = (const char* const)utf8JoergBytes;
       for(int i=0;i<5;i++)
       {
           if (utf8Joerg[i] != utf8[i])
           {
                m_pCallback->m_pBinaryWriter->SendError(3,"UTF8 string 'Jörg' shuld be 5 bytes.");
                return;
           }
       }
       m_pCallback->AnsiUtf8Strings("Jörg", utf8Joerg);
   }

   void IntArrayInput(uint32 intArrayLength, sint32* intArray)
   {
       if ((intArrayLength != 5)
           || (intArray[0] != -2)
           || (intArray[1] != -1)
           || (intArray[2] != 0)
           || (intArray[3] != 1)
           || (intArray[4] != 2))
       {
           m_pCallback->m_pBinaryWriter->SendError(42,"Invalid Array!");
       }
       else
       {
            m_pCallback->IntArrayInput();
       }
   };

   //Service method: void ArrayOutput(out Int32[]& intArray);
   void IntArrayOutput()
   {
       sint32 arr[]={-2,-1,0,1,2};

        m_pCallback->IntArrayOutput(5,(sint32*)arr);
   };

   //Service method: void BoolInputs(Boolean trueInput, Boolean falseInput);
   void BoolInputs(uint8 trueInput, uint8 falseInput)
   {
       if (!trueInput || falseInput)
           m_pCallback->m_pBinaryWriter->SendError(42,"invalid values");
        else
            m_pCallback->BoolInputs();
   };

   //Service method: void ByteArrayInput(Byte[] inputData);
   void ByteArrayInput(uint32 inputDataLength, uint8* inputData)
   {
       if ((inputDataLength != 5)
           || (inputData[0] != 1)
           || (inputData[1] != 2)
           || (inputData[2] != 3)
           || (inputData[3] != 4)
           || (inputData[4] != 5))
       {
           m_pCallback->m_pBinaryWriter->SendError(42,"Invalid Array!");
       }
       else
       {
            m_pCallback->ByteArrayInput();
       }

   };

   //Service method: void ByteArrayOutput(out Byte[]& outputData);
   void ByteArrayOutput()
   {
       char buff[]={1,2,3,4,5};

        m_pCallback->ByteArrayOutput(5,(unsigned char*)buff);
   };

   //Service method: void CreateException();
   void CreateException()
   {
       m_pCallback->m_pBinaryWriter->SendError(42,"Sample Error");
   };

   //Service method: void EnumInput(MyDummyTestEnum enumInput);
   void EnumInput(MyDummyTestEnum enumInput)
   {
        if (enumInput == MyDummyTestEnum_Zwei)
           m_pCallback->EnumInput();
        else
            m_pCallback->m_pBinaryWriter->SendError(42,"Invalid Enum value.");
   };

   //Service method: void EnumOutput(out MyDummyTestEnum& enumOutput);
   void EnumOutput()
   {
       m_pCallback->EnumOutput(MyDummyTestEnum_Zwei);
   };

   //Service method: void FalseOutput(out Boolean& falseOutput);
   void FalseOutput()
   {
       m_pCallback->FalseOutput(false);
   };

   //Service method: Int32 IntInputAsReturn(Int32 intInput);
   void IntInputAsReturn(sint32 intInput)
   {
        m_pCallback->IntInputAsReturn(intInput);
   };

   void SendNoPaddingRequired(uint8 trueValue, uint8 byteValue, 
       sint16 int16Value, uint16 uint16Value, sint32 int32Value, float floatValue, double doubleValue)
   {
        if ((trueValue==0)
           ||(byteValue!=0x1)
           ||(int16Value!=(sint16)-0x12)
           ||(uint16Value!=0x12)
           ||(int32Value!=-0x1234)
           ||(fabs(floatValue-4.4f)>0.01)
           ||(fabs(doubleValue-8.8)>0.01))
       {
               m_pCallback->m_pBinaryWriter->SendError(42,"Wrong padding?");
       }
        else
        {
            m_pCallback->SendNoPaddingRequired();
        }
   };

    void ReceiveNoPaddingRequired()
    {
        m_pCallback->ReceiveNoPaddingRequired(true,0x1,-0x12,0x12,-0x1234,4.4f,8.8);
    };

    void SendInt32Padding(uint8 byteValue, sint32 int32Value1, sint16 int16Value, sint32 int32Value2)
   {
        if ((byteValue!=0x1)
           ||(int32Value1!=-0x1234)
           ||(int16Value!=(sint16)-0x12)
           ||(int32Value2!=-0x1234))
       {
           m_pCallback->m_pBinaryWriter->SendError(42,"Wrong padding?");
       }
        else
        {
            m_pCallback->SendInt32Padding();
        }
   };

    void SendStringPadding(uint8 byteValue1, const char* const stringHallo, uint8 byteValue2, const char* const stringTest, uint8 byteValue3, const char* const stringTest2)
    {
        char* stringTestX = (char*)stringTest2;
        stringTestX = stringTestX;

        if ((byteValue1!=0x1)
           ||(0!=strcmp("Hallo", stringHallo))
           ||(byteValue2!=0x1)
           ||(0!=strcmp("Test", stringTest))
           ||(byteValue3!=0x1)
           ||(0!=strcmp("Test", stringTest)))
       {
           m_pCallback->m_pBinaryWriter->SendError(42,"Wrong padding?");
       }
        else
        {
            m_pCallback->SendStringPadding();
        }
    }

    void ReceiveStringPadding()
    {
        m_pCallback->ReceiveStringPadding(0x1, "Hallo", 0x1, "Test", 0x1, "Test");
    }

    void ReceiveInt32Padding()
    {
        m_pCallback->ReceiveInt32Padding(0x1,-0x1234,-0x12,-0x1234);
    };

   //Service method: void ObjectInput(Object objectInput);
   virtual void ObjectInput(uint16  objTypeId, uint32 obj)
   {
       if ((objTypeId>>8)==TypeInt32 && obj == 42)
       {
            m_pCallback->ObjectInput();
            return;
       }
       if ((objTypeId>>8)==TypeString && (0==strcmp(((char*)obj),"Hallo")))
       {
            m_pCallback->ObjectInput();
            return;
       }

        m_pCallback->m_pBinaryWriter->SendError(42,"ObjectInput only accepts 42 as Int32 and 'Hallo' as string!");
   }

   //Service method: Int32 OnlyReturn();
   void OnlyReturn()
   {
       m_pCallback->OnlyReturn(-0x1234);
   };

   //Service method: void ReadLargeData(out Byte[]& byteData);
   void ReadLargeData()
   {
        Byte* pData = new Byte[10000];
        for(int i=0;i<10000;i++)
            pData[i]=(Byte)i;
        m_pCallback->m_pBinaryWriter->writeBytes((const char* const)pData, 10000,true);
   };


   void ReceiveByteArrayPadding(uint8 even)
   {
       char buff[]={1,2,3,4,5,6,7,8,9,10};

       if (even)
           m_pCallback->ReceiveByteArrayPadding(-0x12, 10,(unsigned char*)buff,-0x12);
       else
           m_pCallback->ReceiveByteArrayPadding(-0x12, 5,(unsigned char*)buff,-0x12);
   };


   void SendByteArrayPadding(sint16 int16Value, uint32  byteArrayLength, uint8* byteArray, sint16 int16Value2)
   {
       if (( (byteArrayLength != 5) && (byteArrayLength != 10))
           ||(int16Value != -0x12)
           ||(int16Value2 != -0x12))
       {
          m_pCallback->m_pBinaryWriter->SendError(42,"Invalid Data - padding error?");
          return;
       }
       if ((byteArray && 0x7)  != 0)
       {
          m_pCallback->m_pBinaryWriter->SendError(42,"Byte array not aligned to 8 Bytes.");
          return;
       }

       m_pCallback->SendByteArrayPadding();
   };

   //Service method: void SendLargeData(Byte[] byteData);
   void SendLargeData(uint32 byteDataLength, uint8* byteData)
   {
       if (byteDataLength != 10000)
       {
            m_pCallback->m_pBinaryWriter->SendError(42,"SendLargeData expects 10000.");
            return;
       }
       for(int i = 0; i<10000; i++)
       {
           if (((Byte)i)!=byteData[i])
           {
                m_pCallback->m_pBinaryWriter->SendError(42,"Invalid Data in large byte array.");
                return;
           }
       }
        m_pCallback->SendLargeData();
    };

   //Service method: void SendNoData();
   void SendNoData()
   {
       m_pCallback->SendNoData();
   };

   //Service method: void SimpleInput(Int32 intInput);
   void SimpleInput(sint32 intInput)
   {
       if (intInput != -0x1234)
           m_pCallback->m_pBinaryWriter->SendError(42,"Invalid Parameter!");
       else
           m_pCallback->SimpleInput();
   };

   //Service method: void SimpleOutput(out Int32& intOutput);
   void SimpleOutput()
   {
       m_pCallback->SimpleOutput(-0x1234);
   };

   //Service method: void StringInput(String stringInput);
   void StringInput(const char* const stringInput)
   {
       if (0 != Libc_stricmp(stringInput, "Test"))
       {
           m_pCallback->m_pBinaryWriter->SendError(42,"Wrong padding?");
       }
       else
       {
            m_pCallback->StringInput();
       }
   };

   //Service method: void StringOutput(out String& stringOutput);
   void StringOutput()
   {
        m_pCallback->StringOutput("Test");
   };

//   //Service method: void StructArrayInput(MyDummyTestStruct[] structArrInput);
//   void StructArrayInput(uint32 structArrInputLength, MyDummyTestStruct** structArrInput)
//   {
//       structArrInputLength;
//       structArrInput;
//        m_pCallback->SendError(42,"Test not implemented Yet");
//   };

   //Service method: void StructArrayOutput(out MyDummyTestStruct[]& structArrOutput);
   void StructArrayOutput()
   {
        m_pCallback->m_pBinaryWriter->SendError(42,"Test not implemented Yet");
   };

   //Service method: void StructInput(MyDummyTestStruct structInput);
   void StructInput(MyDummyTestStruct* structInput)
   {
       structInput = structInput;
        m_pCallback->m_pBinaryWriter->SendError(42,"Test not implemented Yet");
   };

   //Service method: void StructOutput(out MyDummyTestStruct& structOutput);
   void StructOutput()
   {
        m_pCallback->m_pBinaryWriter->SendError(42,"Test not implemented Yet");
   };

   //Service method: void TerminateConnection();
   void TerminateConnection()
   {
        m_pCallback->m_pBinaryWriter->SendError(42,"Test not implemented Yet");
   };

   //Service method: void TrueOutput(out Boolean& trueOutput);
   void TrueOutput()
   {
       m_pCallback->TrueOutput(true);
   };

    void StringArrayInput(uint32 stringArrayLength, const char* const* stringArray)
    {
       if ((stringArrayLength != 2)
           ||(0!=strcmp("Hallo", stringArray[0]))
           ||(0!=strcmp("Test", stringArray[1])))
       {
           m_pCallback->m_pBinaryWriter->SendError(42,"Invalid Array!");
       }
       else
       {
            m_pCallback->StringArrayInput();
       }
    }

    void StringArrayOutput()
    {
        char** arr = new char*[2];
        arr[0]= "Hallo";
        arr[1]= "Test";
        m_pCallback->StringArrayOutput(2, arr);
    }

    void ObjectArrayInput(uint32 objectArrayLength, uint16* objectArrayTypeId, uint32* objectArray)
    {
       if ((objectArrayLength != 5)
           || ((objectArrayTypeId[0]>>8) != TypeInt32)
           || ((objectArrayTypeId[1]>>8) != TypeString)
           || ((objectArrayTypeId[2]>>8) != TypeVoid)
           || ((objectArrayTypeId[3]>>8) != TypeString)
           || ((objectArrayTypeId[4]>>8) != TypeInt32)
           || (objectArray[0] != (uint32)-2)
           || (0!=strcmp("Hallo", (char*)objectArray[1]))
           || (objectArray[2] != 0)
           || (0!=strcmp("Test", (char*)objectArray[3]))
           || (objectArray[4] != 2))
       {
           m_pCallback->m_pBinaryWriter->SendError(42,"Invalid Array!");
       }
       else
       {
            m_pCallback->ObjectArrayInput();
       }
       delete [] objectArrayTypeId;
       delete [] objectArray;
    }

    uint16* objectArrayTypeId;
    uint32* objectArray;
    void ObjectArrayOutput()
    {
        objectArrayTypeId = new uint16[5];
        objectArray = new uint32[5];
        objectArrayTypeId[0]=TypeInt32;
        objectArrayTypeId[1]=TypeString;
        objectArrayTypeId[2]=TypeVoid;
        objectArrayTypeId[3]=TypeString;
        objectArrayTypeId[4]=TypeInt32;
        objectArray[0]=(uint32)-2;
        objectArray[1]=(uint32)"Hallo";
        objectArray[2]=0;
        objectArray[3]=(uint32)"Test";
        objectArray[4]=2;
        m_pCallback->ObjectArrayOutput(5, objectArrayTypeId, objectArray);
        delete [] objectArrayTypeId;
        delete [] objectArray;
    }
};

class CDummyTestServiceImplBase2;
class CDummyTestServiceImplCallback2 : public CCallbackBase
{
public:
    CBinaryWriter2* m_pBinaryWriter2;
    CDummyTestServiceImplCallback2(CDummyTestServiceImplBase2* pServiceBase);
    ~CDummyTestServiceImplCallback2();
    void AllFloatTypesAsInputs();
    void AllFloatTypesAsOutput(double doubleOutput, float floatOutput);
    void AllIntegerTypesAsInputs();
    void AllIntegerTypesAsOutput(sint8 int8Output, uint8 uint8Output, sint16 int16Output, uint16 uint16Output, sint32 int32Output, uint32 uint32Output);
    void AnsiUtf8Strings(const char* const ansiOut, const char* const utf8Out);
    void BoolInputs();
    void ByteArrayInput();
    void ByteArrayOutput(uint32 _outputDataLength, uint8* outputData);
    void CreateException();
    void EnumInput();
    void EnumOutput(MyDummyTestEnum enumOutput);
    void FalseOutput(uint8 falseOutput);
    void IntArrayInput();
    void IntArrayOutput(uint32 _intArrayLength, sint32* intArray);
    void IntInputAsReturn(sint32 _returnValue);
    void ObjectArrayInput();
    void ObjectArrayOutput(uint32 _objectArrayLength, uint16* _objectArrayTypeId, uint32* objectArray);
    void ObjectInput();
    void OnlyReturn(sint32 _returnValue);
    void ReadLargeData(uint32 _byteDataLength, uint8* byteData);
    void ReceiveByteArrayPadding(sint16 int16Value, uint32 _byteArrayLength, uint8* byteArray, sint16 int16Value2);
    void ReceiveInt32Padding(uint8 byteValue, sint32 int32Value1, sint16 int16Value, sint32 int32Value2);
    void ReceiveNoPaddingRequired(uint8 trueValue, uint8 byteValue, sint16 int16Value, uint16 uint16Value, sint32 int32Value, float floatValue, double doubleValue);
    void ReceiveStringPadding(uint8 byteValue1, const char* const stringHallo, uint8 byteValue2, const char* const stringTest, uint8 byteValue3, const char* const stringTest2);
    void SendByteArrayPadding();
    void SendInt32Padding();
    void SendLargeData();
    void SendNoData();
    void SendNoPaddingRequired();
    void SendStringPadding();
    void SimpleInput();
    void SimpleOutput(sint32 intOutput);
    void StringArrayInput();
    void StringArrayOutput(uint32 _stringArrayLength, const char* const* stringArray);
    void StringInput();
    void StringOutput(const char* const stringOutput);
    void StructArrayOutput(uint32 _structArrOutputLength, MyDummyTestStruct* structArrOutput);
    void StructInput();
    void StructOutput(MyDummyTestStruct* structOutput);
    void TerminateConnection();
    void TrueOutput(uint8 trueOutput);
};

class CDummyTestServiceImplBase2 : public CServiceImplBase
{
public:
    CDummyTestServiceImplBase2();
    ~CDummyTestServiceImplBase2();
    void Invoke(sint16 methodHandle, char* const pDataBuffer);

protected:
    CDummyTestServiceImplCallback2* m_pCallback;
    
    void setBinaryWriter(CBinaryWriter* pBinaryWriter)
    {   if(m_pCallback != NULL)
        {   m_pCallback->m_pBinaryWriter = pBinaryWriter;
        }
   }

   //Service method [1]: void AllFloatTypesAsInputs(Double doubleInput, Single floatInput);
    virtual void AllFloatTypesAsInputs(CBinaryReader2* pBinaryReader);
    virtual void AllFloatTypesAsInputs(double doubleInput, float floatInput)=0;

   //Service method [2]: void AllFloatTypesAsOutput(out Double& doubleOutput, out Single& floatOutput);
    virtual void AllFloatTypesAsOutput();

   //Service method [3]: void AllIntegerTypesAsInputs(SByte int8Input, Byte uint8Input, Int16 int16Input, UInt16 uint16Input, Int32 int32Input, UInt32 uint32Input);
    virtual void AllIntegerTypesAsInputs(CBinaryReader2* pBinaryReader);
    virtual void AllIntegerTypesAsInputs(sint8 int8Input, uint8 uint8Input, sint16 int16Input, uint16 uint16Input, sint32 int32Input, uint32 uint32Input)=0;

   //Service method [4]: void AllIntegerTypesAsOutput(out SByte& int8Output, out Byte& uint8Output, out Int16& int16Output, out UInt16& uint16Output, out Int32& int32Output, out UInt32& uint32Output);
    virtual void AllIntegerTypesAsOutput();

   //Service method [5]: void AnsiUtf8Strings(String ansi, String utf8, out String& ansiOut, out String& utf8Out);
    virtual void AnsiUtf8Strings(CBinaryReader2* pBinaryReader);
    virtual void AnsiUtf8Strings(const char* const ansi, const char* const utf8)=0;

   //Service method [6]: void BoolInputs(Boolean trueInput, Boolean falseInput);
    virtual void BoolInputs(CBinaryReader2* pBinaryReader);
    virtual void BoolInputs(uint8 trueInput, uint8 falseInput)=0;

   //Service method [7]: void ByteArrayInput(Byte[] inputData);
    virtual void ByteArrayInput(CBinaryReader2* pBinaryReader);
    virtual void ByteArrayInput(uint32 _inputDataLength, uint8* inputData)=0;

   //Service method [8]: void ByteArrayOutput(out Byte[]& outputData);
    virtual void ByteArrayOutput();

   //Service method [9]: void CreateException();
    virtual void CreateException();

   //Service method [10]: void EnumInput(MyDummyTestEnum enumInput);
    virtual void EnumInput(CBinaryReader2* pBinaryReader);
    virtual void EnumInput(MyDummyTestEnum enumInput)=0;

   //Service method [11]: void EnumOutput(out MyDummyTestEnum& enumOutput);
    virtual void EnumOutput();

   //Service method [12]: void FalseOutput(out Boolean& falseOutput);
    virtual void FalseOutput();

   //Service method [13]: void IntArrayInput(Int32[] intArray);
    virtual void IntArrayInput(CBinaryReader2* pBinaryReader);
    virtual void IntArrayInput(uint32 _intArrayLength, sint32* intArray)=0;

   //Service method [14]: void IntArrayOutput(out Int32[]& intArray);
    virtual void IntArrayOutput();

   //Service method [15]: Int32 IntInputAsReturn(Int32 intInput);
    virtual void IntInputAsReturn(CBinaryReader2* pBinaryReader);
    virtual void IntInputAsReturn(sint32 intInput)=0;

   //Service method [16]: void ObjectArrayInput(Object[] objectArray);
    virtual void ObjectArrayInput(CBinaryReader2* pBinaryReader);
    virtual void ObjectArrayInput(uint32 _objectArrayLength, uint16* _objectArrayTypeId, uint32* objectArray)=0;

   //Service method [17]: void ObjectArrayOutput(out Object[]& objectArray);
    virtual void ObjectArrayOutput();

   //Service method [18]: void ObjectInput(Object obj);
    virtual void ObjectInput(CBinaryReader2* pBinaryReader);
    virtual void ObjectInput(uint16 _objTypeId, uint32 obj)=0;

   //Service method [19]: Int32 OnlyReturn();
    virtual void OnlyReturn();

   //Service method [20]: void ReadLargeData(out Byte[]& byteData);
    virtual void ReadLargeData();

   //Service method [21]: void ReceiveByteArrayPadding(Boolean even, out Int16& int16Value, out Byte[]& byteArray, out Int16& int16Value2);
    virtual void ReceiveByteArrayPadding(CBinaryReader2* pBinaryReader);
    virtual void ReceiveByteArrayPadding(uint8 even)=0;

   //Service method [22]: void ReceiveInt32Padding(out Byte& byteValue, out Int32& int32Value1, out Int16& int16Value, out Int32& int32Value2);
    virtual void ReceiveInt32Padding();

   //Service method [23]: void ReceiveNoPaddingRequired(out Boolean& trueValue, out Byte& byteValue, out Int16& int16Value, out UInt16& uint16Value, out Int32& int32Value, out Single& floatValue, out Double& doubleValue);
    virtual void ReceiveNoPaddingRequired();

   //Service method [24]: void ReceiveStringPadding(out Byte& byteValue1, out String& stringHallo, out Byte& byteValue2, out String& stringTest, out Byte& byteValue3, out String& stringTest2);
    virtual void ReceiveStringPadding();

   //Service method [25]: void SendByteArrayPadding(Int16 int16Value, Byte[] byteArray, Int16 int16Value2);
    virtual void SendByteArrayPadding(CBinaryReader2* pBinaryReader);
    virtual void SendByteArrayPadding(sint16 int16Value, uint32 _byteArrayLength, uint8* byteArray, sint16 int16Value2)=0;

   //Service method [26]: void SendInt32Padding(Byte byteValue, Int32 int32Value1, Int16 int16Value, Int32 int32Value);
    virtual void SendInt32Padding(CBinaryReader2* pBinaryReader);
    virtual void SendInt32Padding(uint8 byteValue, sint32 int32Value1, sint16 int16Value, sint32 int32Value)=0;

   //Service method [27]: void SendLargeData(Byte[] byteData);
    virtual void SendLargeData(CBinaryReader2* pBinaryReader);
    virtual void SendLargeData(uint32 _byteDataLength, uint8* byteData)=0;

   //Service method [28]: void SendNoData();
    virtual void SendNoData();

   //Service method [29]: void SendNoPaddingRequired(Boolean trueValue, Byte byteValue, Int16 int16Value, UInt16 uint16Value, Int32 int32Value, Single floatValue, Double doubleValue);
    virtual void SendNoPaddingRequired(CBinaryReader2* pBinaryReader);
    virtual void SendNoPaddingRequired(uint8 trueValue, uint8 byteValue, sint16 int16Value, uint16 uint16Value, sint32 int32Value, float floatValue, double doubleValue)=0;

   //Service method [30]: void SendStringPadding(Byte byteValue1, String stringHallo, Byte byteValue2, String stringTest, Byte byteValue3, String stringTest2);
    virtual void SendStringPadding(CBinaryReader2* pBinaryReader);
    virtual void SendStringPadding(uint8 byteValue1, const char* const stringHallo, uint8 byteValue2, const char* const stringTest, uint8 byteValue3, const char* const stringTest2)=0;

   //Service method [31]: void SimpleInput(Int32 intInput);
    virtual void SimpleInput(CBinaryReader2* pBinaryReader);
    virtual void SimpleInput(sint32 intInput)=0;

   //Service method [32]: void SimpleOutput(out Int32& intOutput);
    virtual void SimpleOutput();

   //Service method [33]: void StringArrayInput(String[] stringArray);
    virtual void StringArrayInput(CBinaryReader2* pBinaryReader);
    virtual void StringArrayInput(uint32 _stringArrayLength, const char* const* stringArray)=0;

   //Service method [34]: void StringArrayOutput(out String[]& stringArray);
    virtual void StringArrayOutput();

   //Service method [35]: void StringInput(String stringInput);
    virtual void StringInput(CBinaryReader2* pBinaryReader);
    virtual void StringInput(const char* const stringInput)=0;

   //Service method [36]: void StringOutput(out String& stringOutput);
    virtual void StringOutput();

   //Service method [37]: void StructArrayOutput(out MyDummyTestStruct[]& structArrOutput);
    virtual void StructArrayOutput();

   //Service method [38]: void StructInput(MyDummyTestStruct structInput);
    virtual void StructInput(CBinaryReader2* pBinaryReader);
    virtual void StructInput(MyDummyTestStruct* structInput)=0;

   //Service method [39]: void StructOutput(out MyDummyTestStruct& structOutput);
    virtual void StructOutput();

   //Service method [40]: void TerminateConnection();
    virtual void TerminateConnection();

   //Service method [41]: void TrueOutput(out Boolean& trueOutput);
    virtual void TrueOutput();
};

class CDummyTestService2 : public CDummyTestServiceImplBase2
{

public:
    static CServiceImplBase* CreateInstance();


   //Service method: void AllFloatTypesAsInputs(Double doubleInput, Single floatInput);
   void AllFloatTypesAsInputs(double doubleInput, float floatInput)
   {
       if ((fabs(doubleInput-8.8)>0.01)
           ||(fabs(floatInput-4.4)>0.01))
       {
           m_pBinaryWriter2->WriteConfirmation(42,"Wrong numbers");
       }
       else
       {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->AllFloatTypesAsInputs();
       }
   };

   //Service method: void AllFloatTypesAsOutput(out Double& doubleOutput, out Single& floatOutput);
   void AllFloatTypesAsOutput()
   {
       m_pBinaryWriter2->WriteConfirmation();
       m_pCallback->AllFloatTypesAsOutput(8.8, 4.4f);
   };

   //Service method: void AllIntegerTypesAsInputs(SByte int8Input, Byte uint8Input, Int16 int16Input, UInt16 uint16Input, Int32 int32Input, UInt32 uint32Input);
   void AllIntegerTypesAsInputs(sint8 int8Input, uint8 uint8Input, sint16 int16Input, uint16 uint16Input, sint32 int32Input, uint32 uint32Input)
   {
        if ((int8Input!=-0x1)
           ||(uint8Input!=0x1)
           ||(int16Input!=-0x12)
           ||(uint16Input!=0x12)
           ||(int32Input!=-0x1234)
           ||(uint32Input!=0x1234))
       {
           m_pBinaryWriter2->WriteConfirmation(42,"Wrong numbers");
       }
        else
        {
            m_pBinaryWriter2->WriteConfirmation();

            m_pCallback->AllIntegerTypesAsInputs();
        }
   };

   //Service method: void AllIntegerTypesAsOutput(out SByte& int8Output, out Byte& uint8Output, out Int16& int16Output, out UInt16& uint16Output, out Int32& int32Output, out UInt32& uint32Output);
   void AllIntegerTypesAsOutput()
   {
            m_pBinaryWriter2->WriteConfirmation();
       m_pCallback->AllIntegerTypesAsOutput(-0x1,0x1,-0x12,0x12,-0x1234,0x1234);
   };

   void AnsiUtf8Strings(const char* const ansi, const char* const utf8)
   {
       if (0!=strncmp(ansi, "Jörg",4))
       {
            m_pBinaryWriter2->WriteConfirmation(1,"Ansi string is not Jörg");
            return;
       }
       if (5 != strlen(utf8))
       {
            m_pBinaryWriter2->WriteConfirmation(2,"UTF8 string 'Jörg' shuld be 5 bytes.");
            return;
       }
       uint8 utf8JoergBytes[]={74,195,182,114,103,0};
       const char* const utf8Joerg = (const char* const)utf8JoergBytes;
       for(int i=0;i<5;i++)
       {
           if (utf8Joerg[i] != utf8[i])
           {
                m_pBinaryWriter2->WriteConfirmation(3,"UTF8 string 'Jörg' shuld be 5 bytes.");
                return;
           }
       }
            m_pBinaryWriter2->WriteConfirmation();
       m_pCallback->AnsiUtf8Strings("Jörg", utf8Joerg);
   }

   void IntArrayInput(uint32 intArrayLength, sint32* intArray)
   {
       if ((intArrayLength != 5)
           || (intArray[0] != -2)
           || (intArray[1] != -1)
           || (intArray[2] != 0)
           || (intArray[3] != 1)
           || (intArray[4] != 2))
       {
           m_pBinaryWriter2->WriteConfirmation(42,"Invalid Array!");
       }
       else
       {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->IntArrayInput();
       }
   };

   //Service method: void ArrayOutput(out Int32[]& intArray);
   void IntArrayOutput()
   {
        sint32 arr[]={-2,-1,0,1,2};

        m_pBinaryWriter2->WriteConfirmation();
        m_pCallback->IntArrayOutput(5,(sint32*)arr);
   };

   //Service method: void BoolInputs(Boolean trueInput, Boolean falseInput);
   void BoolInputs(uint8 trueInput, uint8 falseInput)
   {
       if (!trueInput || falseInput)
           m_pBinaryWriter2->WriteConfirmation(42,"invalid values");
        else
        {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->BoolInputs();
       }
   };

   //Service method: void ByteArrayInput(Byte[] inputData);
   void ByteArrayInput(uint32 inputDataLength, uint8* inputData)
   {
       if ((inputDataLength != 5)
           || (inputData[0] != 1)
           || (inputData[1] != 2)
           || (inputData[2] != 3)
           || (inputData[3] != 4)
           || (inputData[4] != 5))
       {
           m_pBinaryWriter2->WriteConfirmation(42,"Invalid Array!");
       }
       else
       {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->ByteArrayInput();
       }

   };

   //Service method: void ByteArrayOutput(out Byte[]& outputData);
   void ByteArrayOutput()
   {
       char buff[]={1,2,3,4,5};

            m_pBinaryWriter2->WriteConfirmation();
        m_pCallback->ByteArrayOutput(5,(unsigned char*)buff);
   };

   //Service method: void CreateException();
   void CreateException()
   {
       m_pBinaryWriter2->WriteConfirmation(42,"Sample Error");
   };

   //Service method: void EnumInput(MyDummyTestEnum enumInput);
   void EnumInput(MyDummyTestEnum enumInput)
   {
        if (enumInput == MyDummyTestEnum_Zwei)
        {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->EnumInput();
        }
        else
            m_pBinaryWriter2->WriteConfirmation(42,"Invalid Enum value.");
   };

   //Service method: void EnumOutput(out MyDummyTestEnum& enumOutput);
   void EnumOutput()
   {
        m_pBinaryWriter2->WriteConfirmation();
       m_pCallback->EnumOutput(MyDummyTestEnum_Zwei);
   };

   //Service method: void FalseOutput(out Boolean& falseOutput);
   void FalseOutput()
   {
        m_pBinaryWriter2->WriteConfirmation();
       m_pCallback->FalseOutput(false);
   };

   //Service method: Int32 IntInputAsReturn(Int32 intInput);
   void IntInputAsReturn(sint32 intInput)
   {
            m_pBinaryWriter2->WriteConfirmation();
        m_pCallback->IntInputAsReturn(intInput);
   };

   void SendNoPaddingRequired(uint8 trueValue, uint8 byteValue, 
       sint16 int16Value, uint16 uint16Value, sint32 int32Value, float floatValue, double doubleValue)
   {
        if ((trueValue==0)
           ||(byteValue!=0x1)
           ||(int16Value!=(sint16)-0x12)
           ||(uint16Value!=0x12)
           ||(int32Value!=-0x1234)
           ||(fabs(floatValue-4.4f)>0.01)
           ||(fabs(doubleValue-8.8)>0.01))
       {
               m_pBinaryWriter2->WriteConfirmation(42,"Wrong padding?");
       }
        else
        {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->SendNoPaddingRequired();
        }
   };

    void ReceiveNoPaddingRequired()
    {
            m_pBinaryWriter2->WriteConfirmation();
        m_pCallback->ReceiveNoPaddingRequired(true,0x1,-0x12,0x12,-0x1234,4.4f,8.8);
    };

    void SendInt32Padding(uint8 byteValue, sint32 int32Value1, sint16 int16Value, sint32 int32Value2)
   {
        if ((byteValue!=0x1)
           ||(int32Value1!=-0x1234)
           ||(int16Value!=(sint16)-0x12)
           ||(int32Value2!=-0x1234))
       {
           m_pBinaryWriter2->WriteConfirmation(42,"Wrong padding?");
       }
        else
        {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->SendInt32Padding();
        }
   };

    void SendStringPadding(uint8 byteValue1, const char* const stringHallo, uint8 byteValue2, const char* const stringTest, uint8 byteValue3, const char* const stringTest2)
    {
        char* stringTestX = (char*)stringTest2;
        stringTestX = stringTestX;

        if ((byteValue1!=0x1)
           ||(0!=strcmp("Hallo", stringHallo))
           ||(byteValue2!=0x1)
           ||(0!=strcmp("Test", stringTest))
           ||(byteValue3!=0x1)
           ||(0!=strcmp("Test", stringTest)))
       {
           m_pBinaryWriter2->WriteConfirmation(42,"Wrong padding?");
       }
        else
        {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->SendStringPadding();
        }
    }

    void ReceiveStringPadding()
    {
            m_pBinaryWriter2->WriteConfirmation();
        m_pCallback->ReceiveStringPadding(0x1, "Hallo", 0x1, "Test", 0x1, "Test");
    }

    void ReceiveInt32Padding()
    {
            m_pBinaryWriter2->WriteConfirmation();
        m_pCallback->ReceiveInt32Padding(0x1,-0x1234,-0x12,-0x1234);
    };

   //Service method: void ObjectInput(Object objectInput);
   virtual void ObjectInput(uint16  objTypeId, uint32 obj)
   {
       if ((objTypeId>>8)==TypeInt32 && obj == 42)
       {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->ObjectInput();
            return;
       }
       if ((objTypeId>>8)==TypeString && (0==strcmp(((char*)obj),"Hallo")))
       {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->ObjectInput();
            return;
       }

        m_pBinaryWriter2->WriteConfirmation(42,"ObjectInput only accepts 42 as Int32 and 'Hallo' as string!");
   }

   //Service method: Int32 OnlyReturn();
   void OnlyReturn()
   {
            m_pBinaryWriter2->WriteConfirmation();
       m_pCallback->OnlyReturn(-0x1234);
   };

   //Service method: void ReadLargeData(out Byte[]& byteData);
   void ReadLargeData()
   {
        Byte* pData = new Byte[10000];
        for(int i=0;i<10000;i++)
            pData[i]=(Byte)i;

        m_pBinaryWriter2->Write(pData, 10000);
   };


   void ReceiveByteArrayPadding(uint8 even)
   {
       char buff[]={1,2,3,4,5,6,7,8,9,10};

            m_pBinaryWriter2->WriteConfirmation();
       if (even)
           m_pCallback->ReceiveByteArrayPadding(-0x12, 10,(unsigned char*)buff,-0x12);
       else
           m_pCallback->ReceiveByteArrayPadding(-0x12, 5,(unsigned char*)buff,-0x12);
   };


   void SendByteArrayPadding(sint16 int16Value, uint32  byteArrayLength, uint8* byteArray, sint16 int16Value2)
   {
       if (( (byteArrayLength != 5) && (byteArrayLength != 10))
           ||(int16Value != -0x12)
           ||(int16Value2 != -0x12))
       {
          m_pBinaryWriter2->WriteConfirmation(42,"Invalid Data - padding error?");
          return;
       }
       if ((byteArray && 0x7)  != 0)
       {
          m_pBinaryWriter2->WriteConfirmation(42,"Byte array not aligned to 8 Bytes.");
          return;
       }

            m_pBinaryWriter2->WriteConfirmation();
       m_pCallback->SendByteArrayPadding();
   };

   //Service method: void SendLargeData(Byte[] byteData);
   void SendLargeData(uint32 byteDataLength, uint8* byteData)
   {
       if (byteDataLength != 10000)
       {
            m_pBinaryWriter2->WriteConfirmation(42,"SendLargeData expects 10000.");
            return;
       }
       for(int i = 0; i<10000; i++)
       {
           if (((Byte)i)!=byteData[i])
           {
                m_pBinaryWriter2->WriteConfirmation(42,"Invalid Data in large byte array.");
                return;
           }
       }
            m_pBinaryWriter2->WriteConfirmation();
        m_pCallback->SendLargeData();
    };

   //Service method: void SendNoData();
   void SendNoData()
   {
            m_pBinaryWriter2->WriteConfirmation();
       m_pCallback->SendNoData();
   };

   //Service method: void SimpleInput(Int32 intInput);
   void SimpleInput(sint32 intInput)
   {
       if (intInput != -0x1234)
           m_pBinaryWriter2->WriteConfirmation(42,"Invalid Parameter!");
       else
       {
            m_pBinaryWriter2->WriteConfirmation();
           m_pCallback->SimpleInput();
       }
   };

   //Service method: void SimpleOutput(out Int32& intOutput);
   void SimpleOutput()
   {
            m_pBinaryWriter2->WriteConfirmation();
       m_pCallback->SimpleOutput(-0x1234);
   };

   //Service method: void StringInput(String stringInput);
   void StringInput(const char* const stringInput)
   {
       if (0 != Libc_stricmp(stringInput, "Test"))
       {
           m_pBinaryWriter2->WriteConfirmation(42,"Wrong padding?");
       }
       else
       {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->StringInput();
       }
   };

   //Service method: void StringOutput(out String& stringOutput);
   void StringOutput()
   {
            m_pBinaryWriter2->WriteConfirmation();
        m_pCallback->StringOutput("Test");
   };

//   //Service method: void StructArrayInput(MyDummyTestStruct[] structArrInput);
//   void StructArrayInput(uint32 structArrInputLength, MyDummyTestStruct** structArrInput)
//   {
//       structArrInputLength;
//       structArrInput;
            //m_pBinaryWriter2->WriteConfirmation();
//        m_pBinaryWriter2->WriteConfirmation(42,"Test not implemented Yet");
//   };

   //Service method: void StructArrayOutput(out MyDummyTestStruct[]& structArrOutput);
   void StructArrayOutput()
   {
        m_pBinaryWriter2->WriteConfirmation(42,"Test not implemented Yet");
   };

   //Service method: void StructInput(MyDummyTestStruct structInput);
   void StructInput(MyDummyTestStruct* structInput)
   {
       structInput = structInput;
        m_pBinaryWriter2->WriteConfirmation(42,"Test not implemented Yet");
   };

   //Service method: void StructOutput(out MyDummyTestStruct& structOutput);
   void StructOutput()
   {
        m_pBinaryWriter2->WriteConfirmation(42,"Test not implemented Yet");
   };

   //Service method: void TerminateConnection();
   void TerminateConnection()
   {
        m_pBinaryWriter2->WriteConfirmation(42,"Test not implemented Yet");
   };

   //Service method: void TrueOutput(out Boolean& trueOutput);
   void TrueOutput()
   {
            m_pBinaryWriter2->WriteConfirmation();
       m_pCallback->TrueOutput(true);
   };

    void StringArrayInput(uint32 stringArrayLength, const char* const* stringArray)
    {
       if ((stringArrayLength != 2)
           ||(0!=strcmp("Hallo", stringArray[0]))
           ||(0!=strcmp("Test", stringArray[1])))
       {
           m_pBinaryWriter2->WriteConfirmation(42,"Invalid Array!");
       }
       else
       {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->StringArrayInput();
       }
    }

    void StringArrayOutput()
    {
        char** arr = new char*[2];
        arr[0]= "Hallo";
        arr[1]= "Test";
            m_pBinaryWriter2->WriteConfirmation();
        m_pCallback->StringArrayOutput(2, arr);
    }

    void ObjectArrayInput(uint32 objectArrayLength, uint16* objectArrayTypeId, uint32* objectArray)
    {
       if ((objectArrayLength != 5)
           || ((objectArrayTypeId[0]>>8) != TypeInt32)
           || ((objectArrayTypeId[1]>>8) != TypeString)
           || ((objectArrayTypeId[2]>>8) != TypeVoid)
           || ((objectArrayTypeId[3]>>8) != TypeString)
           || ((objectArrayTypeId[4]>>8) != TypeInt32)
           || (objectArray[0] != (uint32)-2)
           || (0!=strcmp("Hallo", (char*)objectArray[1]))
           || (objectArray[2] != 0)
           || (0!=strcmp("Test", (char*)objectArray[3]))
           || (objectArray[4] != 2))
       {
           m_pBinaryWriter2->WriteConfirmation(42,"Invalid Array!");
       }
       else
       {
            m_pBinaryWriter2->WriteConfirmation();
            m_pCallback->ObjectArrayInput();
       }
       delete [] objectArrayTypeId;
       delete [] objectArray;
    }

    uint16* objectArrayTypeId;
    uint32* objectArray;
    void ObjectArrayOutput()
    {
        m_pBinaryWriter2->WriteConfirmation();
        m_pBinaryWriter2->WriteArrayLength(5);
        m_pBinaryWriter2->WriteObject(TypeInt32, (uint32)-2);
        m_pBinaryWriter2->WriteObjectString(CBinaryReader2::Ansi, "Hallo", 6);
        m_pBinaryWriter2->WriteObject(TypeVoid, 0);
        m_pBinaryWriter2->WriteObjectString(CBinaryReader2::Ansi, "Test", 5);
        m_pBinaryWriter2->WriteObject(TypeInt32, 2);
    }
};

#endif // __DUMMYTESTSERVICE_H__

