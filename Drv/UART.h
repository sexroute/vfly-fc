#ifndef __UART_H 
#define __UART_H

#define IER_RBR		0x01
#define IER_THRE		0x02
#define IER_RLS		0x04

#define IIR_PEND		0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE		0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE		0x80

#define UART0QueueSize		300
#define UART1QueueSize		100
#define UART2QueueSize		100
#define UART3QueueSize		100

uint32 UARTInit(uint32 PortNum,uint32 Baudrate);
void __irq UART0Handler(void);
void __irq UART1Handler(void);
void __irq UART2Handler(void);
void __irq UART3Handler(void);
void UARTSend(uint32 PortNum,uint8 *BufferPtr,uint32 Length);
void UARTSendChar(uint32 PortNum,uint8 C);
void UARTBufferSend(uint32 PortNum,uint8 *BufferPtr,uint32 Length);

#endif
