/*****************************************************************************
* File Name: BleProcessing.c
*
* Version: 1.0
*
* Description:
* This file implements the BLE capability in the PSoC 4 BLE Lab 3.
*
* Hardware Dependency:
* CY8CKIT-042 BLE Pioneer Kit
*
******************************************************************************
* Copyright (2014), Cypress Semiconductor Corporation.
******************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*****************************************************************************/


/*****************************************************************************
* Included headers
*****************************************************************************/
#include <project.h>
#include <stdbool.h>
#include "main.h"
#include "HeartRateProcessing.h"
#include "BleProcessing.h"
#include "WatchdogTimer.h"

/*****************************************************************************
* Macros 
*****************************************************************************/
#define HEART_RATE_DATA_LEN					(2)
#define HRM_FLAG                            (0)


/*****************************************************************************
* Static variables 
*****************************************************************************/
static uint8 hrsNotification = false;


/*****************************************************************************
* Public variables 
*****************************************************************************/
bool enterHibernateFlag = false;
uint32 timestampWhenConnected = 0;
CONN_PARAM_REQUEST_STATE connParamRequestState = CONN_PARAM_REQUEST_NOT_SENT;


/*****************************************************************************
* Public function definitions
*****************************************************************************/

/*****************************************************************************
* Function Name: SendHeartRateOverBLE
******************************************************************************
* Summary:
* Creates and sends the Heart Rate Measurement characteristic notification 
* packet.
*
* Parameters:
* None
*
* Return:
* None
*
* Theory:
* The function caches the value of the Heart Rate Measurement characteristic 
* and updates the heart rate value in it. It then calls the BLE HRS service 
* API to send this data via notification.
*
* Side Effects:
* None
*
*****************************************************************************/
void SendHeartRateOverBLE(void)
{
    /* Two byte packet for heart rate notification since the heart rate 
     * measurement value is 8-bit (this is sufficient since our heart rate
     * does not cross 255) and we are not using the Energy Expended 
     * and RR-interval fields.
     */
	uint8 heartRatePacket[2];
	
	if(hrsNotification)
	{
        /* Read the existing characteristic value into a 
         * two-byte array, by using CyBle_HrssGetCharacteristicValue(). 
         */
        CyBle_HrssGetCharacteristicValue(CYBLE_HRS_HRM, HEART_RATE_DATA_LEN, heartRatePacket);

        
        /* Update the second byte with the actual heart rate value. */
        heartRatePacket[1] = heartRate;
		
        
        /* Call the BLE component API to send notification */
		CyBle_HrssSendNotification(cyBle_connHandle, CYBLE_HRS_HRM, HEART_RATE_DATA_LEN, heartRatePacket);
    }
}


/*****************************************************************************
* Function Name: HrsEventHandler
******************************************************************************
* Summary:
* Event handler for the Heart Rate Service specific events.
*
* Parameters:
* event: An enumerated value to be checked and accordingly some action to 
*        be taken. The list of events is defined in the CYBLE_EVT_T enum.
*
* eventParam: The parameter associated with the event. The type of parameter
*             can vary depending on the event. For more details refer the 
*             BLE component datasheet.
*
* Return:
* None
*
* Theory:
* The function implements a switch statement to handle the notification
* enable and notification disable events for the Heart Rate Service.
*
* Side Effects:
* None
*
*****************************************************************************/
void HrsEventHandler(uint32 event, void *eventParam)
{
    /* Event handler switch statement for the HRS service specific events. */
     
    
    /* Set the hrsNotification variable on the notification 
     * enabled event, and clear it on the disabled event.
     */
    switch(event)
	{
		case CYBLE_EVT_HRSS_NOTIFICATION_ENABLED:
			hrsNotification = true;	
		    break;
		
		case CYBLE_EVT_HRSS_NOTIFICATION_DISABLED:
			hrsNotification = false;
	    	break;
		
		default:
    		break;
	}
}


/*****************************************************************************
* Function Name: GeneralEventHandler
******************************************************************************
* Summary:
* Event handler for generic BLE events.
*
* Parameters:
* event: An enumerated value to be checked and accordingly some action to 
*        be taken. The list of events is defined in the CYBLE_EVENT_T enum.
*
* eventParam: The parameter associated with the event. The type of parameter
*             can vary depending on the event. For more details refer the 
*             BLE component datasheet.
*
* Return:
* None
*
* Theory:
* The function implements a switch case to handle different events for BLE
* advertisement, connection and disconnection.
*
* Side Effects:
* None
*
*****************************************************************************/
void GeneralEventHandler(uint32 event, void *eventParam)
{
    /* Handle various events for a general BLE connection */
	switch(event)
	{
		case CYBLE_EVT_STACK_ON:
			/* Start the fast advertisement upon BLE initialization. */
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);

            #if (RGB_LED_IN_PROJECT)
                /* Turn ON Green LED to indicate advertisement state */
                Led_Advertising_Green_Write(0);
            #endif  /* #if (RGB_LED_IN_PROJECT) */
    		break;
            
        case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
            /* If advertisement finished, then enter Hibernate mode. */
            if(CYBLE_STATE_DISCONNECTED == CyBle_GetState())
            {
                enterHibernateFlag = true;
            }
            break;
            
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            /* Enter hibernate mode upon disconnect */
			enterHibernateFlag = true;
            
            /* Reset the connection parameter update request status */
            connParamRequestState = CONN_PARAM_REQUEST_NOT_SENT;
            break;
			
		case CYBLE_EVT_GATT_CONNECT_IND:
            /* Note the time when connected */
            timestampWhenConnected = WatchdogTimer_GetTimestamp();

            #if (RGB_LED_IN_PROJECT)
                /* Turn OFF Green LED; Turn ON Blue LED to indicate Connection */
                Led_Advertising_Green_Write(1);
                Led_Connected_Blue_Write(0);
            #endif  /* #if (RGB_LED_IN_PROJECT) */
			break;
			
		case CYBLE_EVT_GATT_DISCONNECT_IND:
            /* Clear the HRS notification flag and the device connected flag */
			hrsNotification = false;
			break;
		
        case CYBLE_EVT_L2CAP_CONN_PARAM_UPDATE_RSP:
            /* Got a response for the connection parameter update request */
            if(*(uint16 *)eventParam == CYBLE_L2CAP_CONN_PARAM_ACCEPTED)
            {
                connParamRequestState = CONN_PARAM_REQUEST_ACCEPTED;
            }
            else
            {
                connParamRequestState = CONN_PARAM_REQUEST_REJECTED;
            }
            
		default:
    		break;
	}
}



/* [] END OF FILE */
