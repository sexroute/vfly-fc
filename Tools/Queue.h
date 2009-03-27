#ifndef __QUEUE_H
#define __QUEUE_H

struct QueueObject
{
	uint8* Data;
	uint32 Length,Start,Count;
};
typedef struct QueueObject QueueObj;

void QueueInit( QueueObj* Obj,uint32 Length);
void QueueClear( QueueObj* Obj);
void QueueFree( QueueObj* Obj);
uint8 QueueInsert( QueueObj* Obj,uint8 c);
uint8 QueueFetch( QueueObj* Obj);
uint8 QueueRead( QueueObj* Obj,uint32 Num);

#endif
