/*
 * button_header.h
 *
 * Created: 28/09/2022 6:06:53 am
 *  Author: abdo
 */ 


#ifndef BUTTON_HEADER_H_
#define BUTTON_HEADER_H_

//types of button connections : @OPTIONS_BUTTON_TYPES
#define BUTTON_TYPE_PULL_UP			0	
#define BUTTON_TYPE_PULL_DOWN		1

//types of button states	: @OPTIONS_BUTTON_STATES
#define BUTTON_STATE_PRESSED		0
#define BUTTON_STATE_UN_PRESSED		1

//supported internal pull resistor
#define BUTTON_INTERNAL_PULL_UP		0
#define BUTTON_EXTERNAL_PULL_UP		1


//error types of buttons input
typedef enum ES_BUTTON_t{
	BUTTON_OK			= 0,
	BUTTON_INVALID_IN	= 1,
}ES_BUTTON_t;

/************************************************************************/
/* public functions                                                     */
/************************************************************************/
ES_BUTTON_t button_init_Polling(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber, uint8_t args_u8InternOrExternalRes);
ES_BUTTON_t button_init_IT(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber, void (*args_pvCallBack) (void), uint8_t args_u8InternOrExternalRes);
ES_BUTTON_t button_read(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber,uint8_t args_u8pinMode, uint8_t *args_u8Input);
ES_BUTTON_t button_deinit_IT(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber);

#endif /* BUTTON_HEADER_H_ */