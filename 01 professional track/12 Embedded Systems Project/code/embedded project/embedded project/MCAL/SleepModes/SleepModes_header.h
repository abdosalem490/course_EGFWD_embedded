/*
 * SleepModes_header.h
 *
 * Created: 23/09/2022 6:50:38 pm
 *  Author: abdo
 */ 


#ifndef SLEEPMODES_HEADER_H_
#define SLEEPMODES_HEADER_H_

/*
 *	@AVAILABLE_SLEEP_MODES
 *	notes -: SM_STANDBY and SM_EXTENDED_STANDBYy mode are only available with external crystals or resonators
 */
 #define SM_IDLE					0		/*halts clk(CPU) and clk(FLASH), while allowing the other clocks to run*/
 #define SM_ADC_NOISE_REDUCTION		1		/*halts clk(I/O) , clk(CPU) and clk(FLASH), while allowing the other clocks to run*/
 #define SM_POWER_DOWN				2		/*This sleep mode basically halts all generated clocks, allowing operation of asynchronous modules only*/
 #define SM_POWER_SAVE				3		/*This sleep mode basically halts all clocks except clk(ASY)*/
 #define SM_STANDBY					6		/*This mode is identical to Power-down with the exception that the Oscillator is kept running.*/
 #define SM_EXTENDED_STANDBY		7		/*This mode is identical to Power-save mode with the exception that the Oscillator is kept running*/

 //error state for using the function
 typedef enum ES_SLeepMode_t{
	SLEEP_MODE_OK				=	0,
	SLEEP_MODE_INVALID_INPUT	=	1,
 }ES_SLeepMode_t;

 /*
  *@public_functions
  */
  ES_SLeepMode_t enterSleepMode(uint8_t args_u8Mode);

#endif /* SLEEPMODES_HEADER_H_ */