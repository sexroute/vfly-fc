/*
	Queue.c
	通用循环列队数据结构及操作函数集
	BUAA 2008-7-14
	张以成
*/

#include "config.h"
#include "Queue.h"

void StartAdd( QueueObj* Obj)
{
    Obj->Start++;
    if (Obj->Start>=Obj->Length)
    {
        Obj->Start-=Obj->Length;
    }
}

//初始化列队
void QueueInit( QueueObj* Obj,uint32 Length)
{
	Obj->Start=0;
	Obj->Count=0;
	Obj->Length=Length;
	Obj->Data=(uint8*)malloc(Length*sizeof(uint8));
}

//清除列队中的数据
void QueueClear( QueueObj* Obj)
{
	Obj->Start=0;
	Obj->Count=0;
}

//释放列队所占用空间
void QueueFree( QueueObj* Obj)
{
	Obj->Length=0;
	free(Obj->Data);
}

//返回值=TRUE时说明列队已满新插入的数据已挤占最老的数据的位置
uint8 QueueInsert( QueueObj* Obj,uint8 c)
{
    uint32 t=Obj->Start+Obj->Count;
    if (t>=Obj->Length)
    {
        t-=Obj->Length;
    }
    Obj->Data[t]=c;
    if (Obj->Count==Obj->Length)
    {
        StartAdd(Obj);
        return TRUE;
    }
    else
    {
        Obj->Count+=1;
        return FALSE;
    }
}

//取出最早的数据，如果为空则返回0。
uint8 QueueFetch( QueueObj* Obj)
{
    if (Obj->Count>0)
    {
        uint8 c=Obj->Data[Obj->Start];
        StartAdd(Obj);
        Obj->Count--;
        return c;
    }
    else
    {
        return 0;
    }
}

//按插入顺序读取数据，如果指定位不存在则返回0。
uint8 QueueRead( QueueObj* Obj,uint32 Num)
{
    if (Num<Obj->Count)
    {
        uint32 t=Obj->Start+Num;
        uint8 c;
		//cout<<"t="<<t<<endl;
        if (t>=Obj->Length)
        {
            t-=Obj->Length;
        }
		//cout<<"t="<<t<<endl;
		c=Obj->Data[t];
		//cout<<"c="<<(uint32)c<<endl;
        return c;
    }
    return 0;
}
