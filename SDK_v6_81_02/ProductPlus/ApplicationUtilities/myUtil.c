/******************************* myUtil.c  *******************************
 *           #######
 *           ##  ##
 *           #  ##    ####   #####    #####  ##  ##   #####
 *             ##    ##  ##  ##  ##  ##      ##  ##  ##
 *            ##  #  ######  ##  ##   ####   ##  ##   ####
 *           ##  ##  ##      ##  ##      ##   #####      ##
 *          #######   ####   ##  ##  #####       ##  #####
 *                                           #####
 *          Z-Wave, the wireless language.
 *
 *              Copyright (c) 2001
 *              Zensys A/S
 *              Denmark
 *
 *              All Rights Reserved
 *
 *    This source file is subject to the terms and conditions of the
 *    Zensys Software License Agreement which restricts the manner
 *    in which it may be used.
 *
 *---------------------------------------------------------------------------
 *
 * Description: Button module for development kit controller board.
 *
 * Author:   Henrik Holm
 *
 * Last Changed By:  $Author: efh $
 * Revision:         $Revision: 23679 $
 * Last Changed:     $Date: 2012-11-09 13:38:48 +0200 (Пт, 09 ноя 2012) $
 *
 ****************************************************************************/


/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include "config_app.h"
#include <ZW050x.h>
#include <io_zdp03a.h>
#include <ZW_uart_api.h>
#include <ZW_nvr_api.h>
#include <appl_timer_api.h>
#include <key_driver.h>
#include <gpio_driver.h>
#include <misc.h>
#include <ota_util.h>
#include <CommandClassVersion.h>
#include <ZW_string.h>
#include <ZW_security_api.h>
#include <ZW_TransportSecProtocol.h>

#ifdef TEST_INTERFACE_SUPPORT
#include <ZW_test_interface.h>
#endif /*TEST_INTERFACE_SUPPORT*/

#include <ZW_sysdefs.h>
#include <ZW_typedefs.h>
#include <ZW_pindefs.h>
#include <ZW_evaldefs.h>
#include <ZW_timer_api.h>
#include <ZW_uart_api.h>

/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/

#define LED_FLICK_TIME 30          //300ms flick LED

#define MULTIMAX 4          //300ms flick LED

/**
 * Application events for AppStateManager(..)
 */
typedef enum _EVENT_APP_
{
  EVENT_EMPTY = DEFINE_EVENT_APP_NBR,
  EVENT_APP_INIT,
  EVENT_APP_REFRESH_MMI,
  EVENT_APP_NEXT_EVENT_JOB,
  EVENT_APP_FINISH_EVENT_JOB,
  EVENT_APP_BATT_LOW,
  EVENT_APP_IS_POWERING_DOWN,
  EVENT_APP_BASIC_STOP_JOB,
  EVENT_APP_BASIC_START_JOB,
  EVENT_APP_NOTIFICATION_START_JOB,
  EVENT_APP_NOTIFICATION_STOP_JOB,
  EVENT_APP_START_TIMER_EVENTJOB_STOP
}
EVENT_APP;

/**
 * Application states. Function AppStateManager(..) includes the state
 * event machine.
 */
typedef enum _STATE_APP_
{
  STATE_APP_STARTUP,
  STATE_APP_IDLE,
  STATE_APP_LEARN_MODE,
  STATE_APP_WATCHDOG_RESET,
  STATE_APP_OTA,
  STATE_APP_TRANSMIT_DATA,
  STATE_APP_POWERDOWN,
  STATE_APP_BASIC_SET_TIMEOUT_WAIT
}
STATE_APP;

extern BYTE basicValue;
void ChangeState( STATE_APP newState);
/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/

// BYTE buttonOncePressed;

static int ledFlickCount = 0;
static BYTE ledFlickCountUp = 1;
BYTE ledFlickTimes = 0;
WORD ledOnTimes = 0;
int sleepAfterTimes = 0;
int JobTimes = 0;
extern BYTE ledEnable;
extern BYTE currentState;
extern BYTE ledUse;
extern BYTE myEvent,eventTime;
extern void TimerJob();
extern void ZCB_KeyAction(KEY_NAME_T keyName, KEY_EVENT_T keyEvent, BYTE holdCount);
BYTE PortOldValue[4],PortSteadValue[4],PortCount[4],PortChange[4];
extern BYTE PinOutList[];
extern SW_WAKEUP wakeupReason;

#ifdef MULTISWITCH
  extern BYTE onOffState;
#endif

void debugWave(BYTE b1,BYTE b2,BYTE b3,BYTE b4)
{
    uint8_t pTestClass[0x30];
         static TRANSMIT_OPTIONS_TYPE sTxOptions;

          sTxOptions.destNode = 0xff;
	  sTxOptions.txOptions = TRANSMIT_OPTION_NO_ROUTE;
	  sTxOptions.txSecOptions = 0x00; 
	  sTxOptions.securityKey = 0x00;

          pTestClass[0x00]=0x20;        //basic command class
  	  pTestClass[0x01]=0x01;        // basic Set
  	  pTestClass[0x02]=b1;        // basic Set
  	  pTestClass[0x03]=b2;        // basic Set
  	  pTestClass[0x04]=b3;        // basic Set
  	  pTestClass[0x05]=b4;        // basic Set
	   ZW_SendDataEx(pTestClass, 
	   	sizeof(pTestClass),
	                       &sTxOptions,
	                       NULL);  
  return;
}

void Led_Toggle(BYTE ledUse)
{
	switch (ledUse)
	{
		case 0:
		    break;
		case 2 : 
			PIN_TOGGLE(P10);
			break;
		case 5 : 
			PIN_TOGGLE(P37);
			break;
		case 10 : 
			PIN_TOGGLE(P07);
			break;
		case 11 : 
			PIN_TOGGLE(P15);
			PIN_TOGGLE(P16);
			break;
		default :
			PIN_TOGGLE(P36);
	}
}

void Led_On(BYTE ledUse)
{
	switch (ledUse)
	{
		case 0:
		    break;
		case 2 : 
			PIN_ON(P10);
			break;
		case 5 : 
			PIN_ON(P37);
			break;
		case 10 : 
			PIN_ON(P07);
			break;
		case 11 : 
			PIN_OFF(P15);
			PIN_OFF(P16);
			break;
		default :
			PIN_ON(P36);
	}
}

void Led_Off(BYTE ledUse)
{
	switch (ledUse)
	{
		case 0:
		    break;
		case 2 : 
			PIN_OFF(P10);
			break;
		case 5 : 
			PIN_OFF(P37);
			break;
		case 10 : 
			PIN_OFF(P07);
			break;
		case 11 : 
			PIN_ON(P15);
			PIN_ON(P16);
			break;
		default :
			PIN_OFF(P36);
	}
}

/****************************************************************************/
/*                           EXPORTED FUNCTIONS                             */
/****************************************************************************/
void ZCB_TimerMyUtil(void);
code const void (code * ZCB_TimerMyUtil_p)(void) = &ZCB_TimerMyUtil;
/*============================   TimerMyUtil   ======================
**    Function description
**    This function checks and updates the status of the Push buttons.
**    It is run every 10ms
**    Side effects:
**
**--------------------------------------------------------------------------*/
BOOL
my_GetPinChange(BYTE pin)
{
  BYTE port = pin>>4;
  BYTE portPin = pin&0x0F;
  BYTE tmpVal;

  tmpVal = (PortChange[port] & (1 << portPin)) ? TRUE : FALSE;
  PortChange[port] &= (0xff ^ (1 << portPin));
  return tmpVal;
}

BOOL
my_GetPinVal(BYTE pin)
{
  BYTE port = pin>>4;
  BYTE portPin = pin&0x0F;

  return (PortOldValue[port] & (1 << portPin)) ? TRUE : FALSE;
}

void Deal_PortChange( BYTE port, BYTE chgval, BYTE nowval);

void ZCB_TimerMyUtil(void)
{
#ifdef MULTISWITCH
  static multival;
  BYTE tmpVal;
#endif

  BYTE fPinState,myPort,i,j,k;
  
  for (i=0;i<4;i++) {
	myPort=0;
	for (j=0;j<8;j++) {
		for (k=0;k<PINOUTNO && PinOutList[k]!=((i<<4)|j);k++);
		if (k<PINOUTNO) {
			if (gpio_GetPinBool(PinOutList[k], &fPinState)==TRUE) {
				if (fPinState==TRUE) myPort |= (1 << j);
			}
		}
	}
//	if (myPort!=0xff) 
	{
		if (PortSteadValue[i]==myPort) PortCount[i]++;
		else { PortSteadValue[i]=myPort; PortCount[i]=0; }
	
		if (PortCount[i]>5) { 
			PortCount[i]=0;
//			if (PortOldValue[i]==0) PortOldValue[i] = PortSteadValue[i];
//			else 
			{
				PortChange[i]= PortOldValue[i] ^ PortSteadValue[i];
				PortOldValue[i] = PortSteadValue[i]; 
				if (PortChange[i]>0) Deal_PortChange( i, PortChange[i], PortOldValue[i]);
			}
		}
	}
  }

  if (ledFlickTimes > 0)
  {
    if (ledFlickCountUp ==1) ledFlickCount++;
	else ledFlickCount--;
	
	if (ledFlickCount>LED_FLICK_TIME) { ledFlickCountUp = 0; if (ledEnable != 0) Led_Toggle(ledUse); }
	if (ledFlickCount<0) { ledFlickCountUp = 1; if (ledEnable != 0) Led_Toggle(ledUse); ledFlickTimes--; }
    if (ledFlickTimes == 0)  Led_On(ledUse); 
  }

  if (ledOnTimes > 0)
  {
    if (ledOnTimes == 100) debugWave(5,currentState,ledOnTimes,IE);
	if (ledEnable != 0) Led_Off(ledUse);
//    PIN_OFF(LED1);
	ledOnTimes--;
	if (ledOnTimes == 0)  Led_On(ledUse);
  }
//  if (ledOnTimes == 0 && ledFlickTimes == 0) Led_On(ledUse);
  if (sleepAfterTimes > 0) sleepAfterTimes--;
  if (JobTimes > 0) {
	  JobTimes--;
	  if (JobTimes == 0) TimerJob();
  }
  
#ifdef MULTISWITCH
  if (onOffState>100) tmpVal = MULTIMAX;
  else tmpVal = onOffState*MULTIMAX/100;
  multival++;
  if (multival<=tmpVal) { PIN_ON(P10); } 
  else { PIN_OFF(P10); }
  if (multival>=MULTIMAX) multival = 0;
#endif
}


BYTE initMyUtil(void)
{
  BYTE i;
  
  for (i=0;i<4;i++) { PortSteadValue[i]=0; PortCount[i]=0; }
  return TimerStart(ZCB_TimerMyUtil, 1, TIMER_FOREVER);
}



void SetMyPinIn( BYTE port, KEY_NAME_T key, BOOL pullUp)
{
  UNUSED(key);
//  if (key==KEY01) KeyDriverRegisterCallback(key, port, BITFIELD_KEY_DOWN | BITFIELD_KEY_UP | BITFIELD_KEY_PRESS |BITFIELD_KEY_HOLD |BITFIELD_KEY_TRIPLE_PRESS,  ZCB_KeyAction);
//  else KeyDriverRegisterCallback(key, port, BITFIELD_KEY_DOWN | BITFIELD_KEY_UP | BITFIELD_KEY_PRESS |BITFIELD_KEY_HOLD, ZCB_KeyAction);
  gpio_SetPinIn(port, pullUp);
}
