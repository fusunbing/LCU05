#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "eclr.h"
#include "osthread.h"
#include "mscorlib.h"

#include "eclr_port.h"

void setPriorityScheme()
{
    // only one prio level available
    //mscorlib::PriorityClass::SetLowest(10, 10);
  mscorlib::PriorityClass::SetLowest(TASK_PRO_ECLR_LOWEST, TASK_PRO_ECLR_LOWEST);
    // 12 level avalable
    //mscorlib::PriorityClass::SetNormal(11, 19);
  mscorlib::PriorityClass::SetNormal(TASK_PRO_ECLR_NORMAL_BEGIN, TASK_PRO_ECLR_NORMAL_END);
    // only one prio level avalable
    //mscorlib::PriorityClass::SetHighest(21, 21);
  mscorlib::PriorityClass::SetHighest(TASK_PRO_ECLR_HIGHEST, TASK_PRO_ECLR_HIGHEST);
}

void TaskWrapper(void* exinf)
{

   CThreadImpl *pThreadImpl = (CThreadImpl*)exinf;
   
   //xSemaphoreTake(m_threadSem, portMAX_DELAY); 
   
   pThreadImpl->m_proc(pThreadImpl->m_param);  
   
   vTaskDelete(NULL);
}

CThreadImpl::CThreadImpl()
{
    m_pThreadID = NULL;
    //m_threadSem = NULL;
}

CThreadImpl::~CThreadImpl()
{
    m_pThreadID = NULL;
}

void CThreadImpl::Release()
{
    delete this;
}

CThreadImpl* CThreadImpl::Create(const char* name, ThreadProc proc, ThreadParam param, unsigned stackSize)
{
    CThreadImpl* result = new CThreadImpl;
    if (result != NULL)
    {
        if (result->create(name, proc, param, stackSize) == true)
        {

        }
        else
        {
            delete result;
            result = NULL;
        }

    }

    return result;
}

bool CThreadImpl::create(const char* name, ThreadProc proc, ThreadParam param, unsigned stackSize)
{
    portBASE_TYPE result = pdFALSE;
    /*
    vSemaphoreCreateBinary(m_threadSem);
    if (m_threadSem != NULL)
    {
        return false;
    }*/

    m_proc = proc;
    m_param = param;
    
    //vTaskDelay(10); // give time to idle task to delete the task
       result = xTaskCreate((pdTASK_CODE)TaskWrapper, (const signed char *)name, 1204, (void*)this, tskIDLE_PRIORITY, &m_pThreadID);///
 ///    result = xTaskCreate((pdTASK_CODE)TaskWrapper, (const signed char *)name, 2048, (void*)this, tskIDLE_PRIORITY, &m_pThreadID);
      
    if (result != pdPASS)
    {
        return false;
    }
    
    if (m_pThreadID != NULL)
    {
        // assign the OS associated threadId
        vTaskSuspend(m_pThreadID);
        return true;
    }

    return false;
}

bool CThreadImpl::SetPriority(int prioThread)
{
    bool result = false;
    
    if (m_pThreadID != NULL)
    {
        vTaskPrioritySet(m_pThreadID, (unsigned portBASE_TYPE)prioThread);
        result = true;
    }
    
    return result;  
}

void CThreadImpl::Start()
{
    Resume();
}
void CThreadImpl::Resume()
{
    if (m_pThreadID != NULL)
        vTaskResume(m_pThreadID);
}
void CThreadImpl::Suspend()
{
    if (m_pThreadID != NULL)
        vTaskSuspend(m_pThreadID);
}
void CThreadImpl::Sleep(int ms)
{
    vTaskDelay(configTICK_RATE_HZ * ms / 1000);
}

CThreadImpl::ThreadId CThreadImpl::GetThreadId()
{
    return m_pThreadID;
}

CThreadImpl::ThreadId CThreadImpl::GetCurrentThreadId()
{
    ThreadId tid = (CThreadImpl::ThreadId)xTaskGetCurrentTaskHandle();
    return tid;
}

bool CThreadImpl::Abort()
{
    if (m_pThreadID != NULL)
    {
        vTaskDelete(m_pThreadID);
        m_pThreadID = NULL;
    }
    
    return true;
}

// AutoResetEvent
CEventImpl* CEventImpl::Create(bool initialstate, bool isAutoReset)
{
    CEventImpl* result = new CEventImpl();
    if (result != NULL)
    {
        if (result->create(initialstate, isAutoReset) == false)
        {
            delete result;
            result = NULL;
        }

    }
    return result;
}


bool CEventImpl::create(bool initialstate, bool isAutoReset)
{
    bool result = false;
    
    vSemaphoreCreateBinary(m_pEvent);
    if (m_pEvent != NULL)
    {
        result = true;
        
        if (initialstate == false)
            xSemaphoreTake(m_pEvent, portMAX_DELAY);
    }
    
    return result;
}


CEventImpl::CEventImpl()
{
    m_pEvent = NULL;
}

void CEventImpl::Release()
{
    if (m_pEvent != NULL)
    {
        vQueueDelete(m_pEvent);
        m_pEvent = NULL;
    }
    
    delete this;
}

CEventImpl::~CEventImpl()
{
    m_pEvent = NULL;
}

bool CEventImpl::Set()
{
    bool result = true;

    xSemaphoreGive(m_pEvent);
    
    return result;
}

bool CEventImpl::Reset()
{
    bool result = true;
    
    return result;
}

bool CEventImpl::WaitOne()
{
    bool result = false;

    if (xSemaphoreTake(m_pEvent, portMAX_DELAY) == pdTRUE )
    {
        result = true;
    }
    
    return result;
}

// crititical section implementation
CMutexImpl::CMutexImpl()
{
    m_pMutex = NULL;
}

CMutexImpl::~CMutexImpl()
{
    m_pMutex = NULL;
}

CMutexImpl* CMutexImpl::Create()
{
    CMutexImpl* result = new CMutexImpl();
    if (result != NULL)
    {
        if (result->create() == false)
        {
            delete result;
            result = NULL;
        }
    }
    
    return result;
}

void CMutexImpl::Release()
{
    if (m_pMutex != NULL)
    {
        vQueueDelete(m_pMutex);
        m_pMutex = NULL;
    }
    
    delete this;
}

void CMutexImpl::Enter()
{
    xSemaphoreTake(m_pMutex, portMAX_DELAY);
}

void CMutexImpl::Leave()
{
    xSemaphoreGive(m_pMutex);
}

bool CMutexImpl::create()
{
    bool result = false;
    
    m_pMutex = xSemaphoreCreateMutex();
    if (m_pMutex != NULL)
        result = true;
    
    return result;
}

