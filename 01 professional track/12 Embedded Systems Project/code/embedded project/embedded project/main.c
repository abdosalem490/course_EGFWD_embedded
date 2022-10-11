/*
 * embedded project.c
 *
 * Created: 20/09/2022 6:28:01 pm
 * Author : abdo
 */ 

 #include "app/application.h"

int __attribute__((noreturn)) main(void)
{
	appSetup();
	appStart();

    while (1); 

}


