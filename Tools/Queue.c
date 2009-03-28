/*
	Queue.c
	ͨ��ѭ���ж����ݽṹ������������
	BUAA 2008-7-14
	���Գ�
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

//��ʼ���ж�
void QueueInit( QueueObj* Obj,uint32 Length)
{
	Obj->Start=0;
	Obj->Count=0;
	Obj->Length=Length;
	Obj->Data=(uint8*)malloc(Length*sizeof(uint8));
}

//����ж��е�����
void QueueClear( QueueObj* Obj)
{
	Obj->Start=0;
	Obj->Count=0;
}

//�ͷ��ж���ռ�ÿռ�
void QueueFree( QueueObj* Obj)
{
	Obj->Length=0;
	free(Obj->Data);
}

//����ֵ=TRUEʱ˵���ж������²���������Ѽ�ռ���ϵ����ݵ�λ��
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

//ȡ����������ݣ����Ϊ���򷵻�0��
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

//������˳���ȡ���ݣ����ָ��λ�������򷵻�0��
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
