

#include "rtthread.h"
#include "EventRecorder.h"

#define EVENT_RECORDER_OBJ_COUNTS (6)

static Event_DatasetObjectTypeDef edo[EVENT_RECORDER_OBJ_COUNTS];
static __IO uint32_t edo_regCnt;

static void record(Event_DatasetObjectTypeDef* _me, void* cell);


void EventRecd_Init(void)
{
    uint16_t i;

    for (i = 0; i < EVENT_RECORDER_OBJ_COUNTS; i++)
    {
        edo[i].ctrlFlag.value  = 0;
        edo[i].cellSize        = 0;
        edo[i].loopPoint       = 0;
        edo[i].loopSize        = 0;
        edo[i].loopBuf1        = RT_NULL;
        edo[i].loopBuf2        = RT_NULL;
        edo[i].pBufWrite       = edo[i].loopBuf1;
        edo[i].pBufRead        = edo[i].loopBuf1;
        edo[i].Record          = RT_NULL;
    }

    edo_regCnt = 0;
}


Event_DatasetObjectTypeDef* EventRecd_CreateObject(uint32_t cellsize, uint32_t loopsize)
{
    void* memCreate;

    if( (cellsize == 0) || (loopsize == 0) || ((edo_regCnt + 1) >= EVENT_RECORDER_OBJ_COUNTS))
    {
        return RT_NULL;
    }

    memCreate = rt_malloc(cellsize * loopsize * 2);

    if (memCreate == RT_NULL)
    {
        return RT_NULL;
    }

    edo[edo_regCnt].ctrlFlag.bits.isCtreate = 1;
    edo[edo_regCnt].ctrlFlag.bits.canRead   = 0;

    edo[edo_regCnt].cellSize  = cellsize;
    edo[edo_regCnt].loopSize  = loopsize;
    edo[edo_regCnt].loopBuf1  = (uint8_t*)memCreate;
    edo[edo_regCnt].loopBuf2  = edo[edo_regCnt].loopBuf1 + (cellsize * loopsize);
    edo[edo_regCnt].pBufWrite = edo[edo_regCnt].loopBuf1;
    edo[edo_regCnt].pBufRead  = edo[edo_regCnt].loopBuf1;
    edo[edo_regCnt].loopPoint = 0;
    edo[edo_regCnt].Record    = record;

    return &edo[edo_regCnt++];
}


static void record(Event_DatasetObjectTypeDef* _me, void* cell)
{
    void* src;
    void* dest;

    src  = cell;
    dest = (void*)(_me->pBufWrite + (_me->loopPoint * _me->cellSize));

    rt_memcpy(dest, src, _me->cellSize);

    _me->loopPoint++;
    if (_me->loopPoint >= _me->loopSize)
    {
        _me->loopPoint = 0;

        // _me->loopPoint = 0 should change writePoint
        // and update readPoint [6/20/2014 life-3550]
        if (_me->pBufWrite == _me->loopBuf1)
        {
            _me->pBufWrite = _me->loopBuf2;
            _me->pBufRead  = _me->loopBuf1;
            _me->ctrlFlag.bits.canRead = 1;
        }
        else
        {
            _me->pBufWrite = _me->loopBuf1;
            _me->pBufRead  = _me->loopBuf2;
            _me->ctrlFlag.bits.canRead = 1;
        }
        
        //2015-7-28 更换完毕当前的写指针后，清空此缓冲区，
        //目的：去除里面的历史数据
        rt_memset(_me->pBufWrite, 0 , (_me->cellSize * _me->loopSize));
    }
}

