/*
 * atmega32_uPU_reg.h
 *
 * Created: 20/09/2022 10:09:40 pm
 *  Author: abdo
 */ 

/*include necessary header files*/
#include "../../utils/standard_int.h"

#ifndef ATMEGA32_UPU_REG_H_
#define ATMEGA32_UPU_REG_H_



/*micro processor internal registers addresses*/
#define R0				*((uint8_t volatile * const)0x00)
#define R1				*((uint8_t volatile * const)0x01)
#define R2				*((uint8_t volatile * const)0x02)
#define R3				*((uint8_t volatile * const)0x03)
#define R4				*((uint8_t volatile * const)0x04)
#define R5				*((uint8_t volatile * const)0x05)
#define R6				*((uint8_t volatile * const)0x06)
#define R7				*((uint8_t volatile * const)0x07)
#define R8				*((uint8_t volatile * const)0x08)
#define R9				*((uint8_t volatile * const)0x09)
#define R10				*((uint8_t volatile * const)0x0A)
#define R11				*((uint8_t volatile * const)0x0B)
#define R12				*((uint8_t volatile * const)0x0C)
#define R13				*((uint8_t volatile * const)0x0D)
#define R14				*((uint8_t volatile * const)0x0E)
#define R15				*((uint8_t volatile * const)0x0F)
#define R16				*((uint8_t volatile * const)0x10)
#define R17				*((uint8_t volatile * const)0x11)
#define R18				*((uint8_t volatile * const)0x12)
#define R19				*((uint8_t volatile * const)0x13)
#define R20				*((uint8_t volatile * const)0x14)
#define R21				*((uint8_t volatile * const)0x15)
#define R22				*((uint8_t volatile * const)0x16)
#define R23				*((uint8_t volatile * const)0x17)
#define R24				*((uint8_t volatile * const)0x18)
#define R25				*((uint8_t volatile * const)0x19)
#define R26				*((uint8_t volatile * const)0x1A)
#define R27				*((uint8_t volatile * const)0x1B)
#define R28				*((uint8_t volatile * const)0x1C)
#define R29				*((uint8_t volatile * const)0x1D)
#define R30				*((uint8_t volatile * const)0x1E)
#define R31				*((uint8_t volatile * const)0x1F)

/*status register address*/
#define SREG			*((uint8_t volatile * const)0x5F)

/*stack pointer register*/
#define SP				*((uint8_t volatile * const)0x5D)


#endif /* ATMEGA32_UPU_REG_H_ */