#include "mvb_event.h"

static struct rt_event mvb_event;
static rt_uint32_t mvb_event_recv = 0;
static rt_err_t mvb_event_init = 0xFF;

void MVB_Send_Read()
{
	if(mvb_event_init == RT_EOK)
	{
		rt_event_send(&mvb_event,MVB_EVENT_READ);
	}
}

void MVB_Send_Write()
{
	if(mvb_event_init == RT_EOK)
	{
		rt_event_send(&mvb_event,MVB_EVENT_WRITE);
	}
}

rt_uint32_t MVB_Check_Event()
{
	rt_event_recv(&mvb_event,
				  (MVB_EVENT_WRITE | MVB_EVENT_READ),
				  (RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR),
				   RT_WAITING_FOREVER,
				   &mvb_event_recv);
	return mvb_event_recv;
}

void MVB_Event_Init()
{
	mvb_event_init = rt_event_init(&mvb_event,"Mvb_Event",RT_IPC_FLAG_FIFO);
}








