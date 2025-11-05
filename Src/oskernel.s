.syntax unified
.cpu cortex-m4
.thumb
.extern currentThread
.extern period
.globl kernel_schedular_launch
.type kernel_schedular_launch, %function
.globl PendSV_Handler
.type PendSV_Handler, %function
.globl ThreadYield
.type ThreadYield, %function
.globl SysTick_Handler
.type SysTick_Handler, %function


.equ ICSR_BASE,		0xE000ED04 // nvic interrupt controller register
.equ SYST_CVR,		0xE000E018


PendSV_Handler:
	CPSID I //Disable interrupts

	PUSH {R4-R11}

	// R0 = &currentThread;
	LDR R0, =currentThread
	// R1 = currentThread;
	LDR R1, [R0]

	// currentThread.StackPointer = SP
	STR SP, [R1]
	/*
	// Load nextThread
	LDR R1, [R1, #4]
	// unutma r0 in icinde &currentThread yani currentThread adresi var biz  o adrese yaziyoruz
	STR R1, [R0]
	*/

	PUSH {R0, LR}
	BL kernel_schedular
	POP {R0, LR}
	LDR R1, [R0]

	// Yeni stackpointer in ayarlanmasi
	LDR SP, [R1]
	// gerikalanlari bizim yerimize handler itnerrupt halledecek.
	POP {R4-R11}

	CPSIE I

	BX LR // sadece diger registerlari poplasin diye otomatik cikiyoruz

kernel_schedular_launch:
	// R1 = currentThread
	LDR R0, =currentThread
	LDR R2, [R0]

	LDR SP, [R2]

	POP {R4-R11}
	POP {R0-R3}
	POP {R12}

	ADD SP, SP, #4
	POP {LR} // LR = currentThread[PC]
	ADD SP, SP, #4


	CPSIE I
	BX LR


// PendSV_Handler i trigger edicez
SysTick_Handler:
	LDR R0, = ICSR_BASE
	LDR R1, =0X10000000;
	STR R1, [R0]

	BX LR

/*
PendSV tetikliyor
*/

ThreadYield:
    LDR R0, =ICSR_BASE
    LDR R1, =0X10000000
    STR R1, [R0]

    BX LR




