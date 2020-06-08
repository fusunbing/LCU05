
#include "rtthread.h"

#include "eclr.h"
//#include "osthread.h"
#include "threadimpl.h"
#include "mscorlib.h"
#include "config.h"

#define pdTRUE		( 1 )
#define pdFALSE		( 0 )
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short
#define portSTACK_TYPE	unsigned portLONG
#define portBASE_TYPE	long
typedef unsigned portLONG portTickType;

#define pdPASS									( 1 )
#define pdFAIL									( 0 )
#define errQUEUE_EMPTY					( 0 )
#define errQUEUE_FULL						( 0 )
typedef void (*pdTASK_CODE)( void * );

CThreadImpl::CThreadImpl()
{
    m_pThreadID = NULL;

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


void TaskWrapper(void* exinf)
{
   CThreadImpl *pThreadImpl = (CThreadImpl*)exinf;
   
   //xSemaphoreTake(m_threadSem, portMAX_DELAY); 
   pThreadImpl->m_proc(pThreadImpl->m_param);  
   
}


bool CThreadImpl::create(const char* name, ThreadProc proc, ThreadParam param, unsigned stackSize)
{
    rt_thread_t result = NULL;
    /*
    vSemaphoreCreateBinary(m_threadSem);
    if (m_threadSem != NULL)
    {
        return false;
    }*/

    m_proc = proc;
    m_param = param;
    
    //vTaskDelay(10); // give time to idle task to delete the task
    //edit by c.d.
    //result = xTaskCreate((pdTASK_CODE)TaskWrapper, 
    //(const signed char *)name, 1204, (void*)this,
    //    tskIDLE_PRIORITY, &m_pThreadID);///
    result = rt_thread_create(name, TaskWrapper, (void*)this, 2048, tskIDLE_PRIORITY, 5);
    ///    result = xTaskCreate((pdTASK_CODE)TaskWrapper, (const signed char *)name, 2048, (void*)this, tskIDLE_PRIORITY, &m_pThreadID);
      
    if (result == NULL)
    {
        return false;
    }
    else
    {
        m_pThreadID = result;
	    //rt_thread_startup(result);
    }
    
    if (m_pThreadID != NULL)
    {
        // assign the OS associated threadId
        rt_thread_startup((rt_thread_t)m_pThreadID);
        rt_thread_suspend((rt_thread_t)m_pThreadID);
        return true;
    }

    return false;
}

bool CThreadImpl::SetPriority(int prioThread)
{
	bool result = false;
    
    rt_uint8_t arg = (rt_uint8_t) prioThread;
    
    if (m_pThreadID != NULL)
    {
        rt_thread_control((rt_thread_t)m_pThreadID, RT_THREAD_CTRL_CHANGE_PRIORITY, (void*)&arg);
        result = true;
    }
    
    return result;  
}

void CThreadImpl::Start()
{
    //Resume();
	//rt_thread_startup((rt_thread_t)m_pThreadID);
    Resume();
}

void CThreadImpl::Resume()
{

	if (m_pThreadID != NULL)
        rt_thread_resume((rt_thread_t)m_pThreadID);
}
void CThreadImpl::Suspend()
{
	if (m_pThreadID != NULL)
        rt_thread_suspend((rt_thread_t)m_pThreadID);
}
void CThreadImpl::Sleep(int ms)
{
	rt_thread_delay(ms);
}

CThreadImpl::ThreadId CThreadImpl::GetThreadId()
{
    return m_pThreadID;
}

CThreadImpl::ThreadId CThreadImpl::GetCurrentThreadId()
{
//    ThreadId tid = (CThreadImpl::ThreadId)xTaskGetCurrentTaskHandle();
//    return tid;
	//2014-10-11  extern struct rt_thread *rt_current_thread;
    ThreadId tid = (CThreadImpl::ThreadId)rt_thread_self();
    return tid;
}

bool CThreadImpl::Abort()
{
	if (m_pThreadID != NULL)
    {
        rt_thread_delete((rt_thread_t)m_pThreadID);
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
//    bool result = false;
//    
//    vSemaphoreCreateBinary(m_pEvent);
//    if (m_pEvent != NULL)
//    {
//        result = true;
//        
//        if (initialstate == false)
//            xSemaphoreTake(m_pEvent, portMAX_DELAY);
//    }
//    
//    return result;
	bool result = false;
    
    //vSemaphoreCreateBinary(m_pEvent);
    //edit by c.d.
    m_pEvent = rt_sem_create("sem", 0, 0);
    rt_sem_release((rt_sem_t)m_pEvent);
    if (m_pEvent != NULL)
    {
        result = true;
        
        if (initialstate == false)
            rt_sem_take((rt_sem_t)m_pEvent, RT_WAITING_FOREVER);
            //xSemaphoreTake(m_pEvent, portMAX_DELAY);
    }
    
    return result;
}


CEventImpl::CEventImpl()
{
    m_pEvent = NULL;
}

void CEventImpl::Release()
{
//    if (m_pEvent != NULL)
//    {
//        vQueueDelete(m_pEvent);
//        m_pEvent = NULL;
//    }
//    
//    delete this;
	if (m_pEvent != NULL)
    {
        rt_sem_delete((rt_sem_t)m_pEvent);
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
//    bool result = true;

//    xSemaphoreGive(m_pEvent);
//    
//    return result;
	 bool result = true;

    rt_sem_release((rt_sem_t)m_pEvent);
    
    return result;
}

bool CEventImpl::Reset()
{
    bool result = true;
    
    return result;
}

bool CEventImpl::WaitOne()
{
//    bool result = false;

//    if (xSemaphoreTake(m_pEvent, portMAX_DELAY) == pdTRUE )
//    {
//        result = true;
//    }
//    
//    return result;
	bool result = false;

    if (rt_sem_take((rt_sem_t)m_pEvent, RT_WAITING_FOREVER) == RT_EOK )
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
//    if (m_pMutex != NULL)
//    {
//        vQueueDelete(m_pMutex);
//        m_pMutex = NULL;
//    }
//    
//    delete this;
	if (m_pMutex != NULL)
    {
        //vQueueDelete(m_pMutex);
        rt_mutex_delete((rt_mutex_t)m_pMutex);
        m_pMutex = NULL;
    }
    
    delete this;
}

void CMutexImpl::Enter()
{
    //xSemaphoreTake(m_pMutex, portMAX_DELAY);
	rt_mutex_take((rt_mutex_t)m_pMutex, RT_WAITING_FOREVER);
}

void CMutexImpl::Leave()
{
    //xSemaphoreGive(m_pMutex);
	rt_mutex_release((rt_mutex_t)m_pMutex);
}

bool CMutexImpl::create()
{
//    bool result = false;
//    
//    m_pMutex = xSemaphoreCreateMutex();
//    if (m_pMutex != NULL)
//        result = true;
//    
//    return result;
	bool result = false;
    
    //m_pMutex = xSemaphoreCreateMutex();
    m_pMutex = rt_mutex_create("mutex", 0);
    if (m_pMutex != NULL)
        result = true;
    
    return result;
}

