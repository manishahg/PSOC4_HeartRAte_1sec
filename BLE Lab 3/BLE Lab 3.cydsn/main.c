/*****************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
* This is the top level file for the PSoC 4 BLE Lab 3.
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
#define TIME_SINCE_CONNECTED_MS         (5000)


/*****************************************************************************
* Global variables
*****************************************************************************/
static CYBLE_GAP_CONN_UPDATE_PARAM_T hrmConnectionParam =
{
    800,         /* Minimum connection interval of 1000 ms */ //manisha
    800,         /* Maximum connection interval of 1000 ms */
    49,         /* Slave latency of 49 */
    500         /* Supervision timeout of 5 seconds */
};


/*****************************************************************************
* Static function definitions
*****************************************************************************/

/*****************************************************************************
* Function Name: InitializeSystem
******************************************************************************
* Summary:
* Initializes all the blocks of the system.
*
* Parameters:
* None
*
* Return:
* None
*
* Theory:
* The function enables the Opamp and ADC for the heart rate measurement, and 
* setups the BLE component. It also starts the watchdog timer and ensures that 
* all the status LEDs are off at system startup. 
*
* Side Effects:
* None
*
*****************************************************************************/
static void InitializeSystem(void)
{
    #if (RGB_LED_IN_PROJECT)
        /* Turn off all LEDs */
        Led_Advertising_Green_Write(1);
        Led_Connected_Blue_Write(1);
    #endif  /* #if (RGB_LED_IN_PROJECT) */

    /* Enabling Global interrupts */
    CyGlobalIntEnable; 
	
    /* Start Opamp and ADC components */
	Opamp_Start();
    ADC_Start();
	
    /* Start BLE component */
    CyBle_Start(GeneralEventHandler);
    
    
    /* Register the Heart Rate Service event handler callback. The function
     * to be registered is HrsEventHandler().
     */
	CyBle_HrsRegisterAttrCallback(HrsEventHandler);
    
    
    /* Start the Watchdog Timer */
	WatchdogTimer_Start();
}


/*****************************************************************************
* Public function definitions
*****************************************************************************/

/*****************************************************************************
* Function Name: main
******************************************************************************
* Summary:
* The main function for the project.
*
* Parameters:
* None
*
* Return:
* None
*
* Theory:
* The main function first calls the initialization function to start the 
* system, and then enters a loop to run forever. In the main loop, it scans
* the heart rate first, then sends a notification packet every second to a 
* BLE connected device. It then enters low power (deep sleep) state, waiting
* for the periodic wakeup interrupt from watchdog timer.
* When the device is disconnected or when advertisement timeout happens, 
* the device enters Hibernate mode, waiting for the SW2 switch press to wakeup.
*
* Side Effects:
* None
*
*****************************************************************************/
int main()
{
    static uint32 previousTimestamp = 0;
    static uint32 currentTimestamp = 0;
    CYBLE_LP_MODE_T bleMode;
    uint8 interruptStatus;
    
    /* Initialize all blocks of the system */
	InitializeSystem();
    
    /* Run forever */
    for(;;)
    {
        /* Wake up ADC from low power mode */
        ADC_Wakeup();
        
        /* Analog Front End. 
         * Detects the input signal and measures Heart Rate 
         */
        ProcessHeartRateSignal();

        /* Put ADC in low power mode */
        ADC_Sleep();
        
        /* Measure the current system timestamp from watchdog timer */
        currentTimestamp = WatchdogTimer_GetTimestamp();        

//        /* Update BLE connection parameters a few seconds after connection */ //manisha
       if((CyBle_GetState() == CYBLE_STATE_CONNECTED) && 
           (connParamRequestState == CONN_PARAM_REQUEST_NOT_SENT))
        {
            if((currentTimestamp - timestampWhenConnected) > TIME_SINCE_CONNECTED_MS)
            {
                CyBle_L2capLeConnectionParamUpdateRequest(cyBle_connHandle.bdHandle, &hrmConnectionParam);
                connParamRequestState = CONN_PARAM_REQUEST_SENT;
            }
        }
        
        
        /* Send Heart Rate notification over BLE every second.
         * Check if the current timestamp minus previous exceeds 1000 ms.
         */
        if((currentTimestamp - previousTimestamp) >= 1000)
        {
            /* Call API defined in BleProcessing.c to send 
             * notification over BLE.
             */
            SendHeartRateOverBLE();
            
            /* Update the previous timestamp with the current timestamp. */
            previousTimestamp = currentTimestamp;
        }

        /* Try to stay in low power mode until the next watchdog interrupt */
        while(WatchdogTimer_GetTimestamp() == currentTimestamp)
        {
            /* Process any pending BLE events */
            CyBle_ProcessEvents();
            
            /* The idea of low power operation is to first request the BLE 
             * block go to Deep Sleep, and then check whether it actually
             * entered Deep Sleep. This is important because the BLE block
             * runs asynchronous to the rest of the application and thus
             * could be busy/idle independent of the application state. 
             * 
             * Once the BLE block is in Deep Sleep, only then the system 
             * can enter Deep Sleep. This is important to maintain the BLE 
             * connection alive while being in Deep Sleep.
             */

            
            /* Request the BLE block to enter Deep Sleep */
            bleMode = CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);

            
            /* Check if the BLE block entered Deep Sleep and if so, then the 
             * system can enter Deep Sleep. This is done inside a Critical 
             * Section (where global interrupts are disabled) to avoid a 
             * race condition between application main (that wants to go to 
             * Deep Sleep) and other interrupts (which keep the device from 
             * going to Deep Sleep). 
             */
            interruptStatus = CyEnterCriticalSection();
            
            /* Check if the BLE block entered Deep Sleep */
            if(CYBLE_BLESS_DEEPSLEEP == bleMode)
            {
                /* Check the current state of BLE - System can enter Deep Sleep
                 * only when the BLE block is starting the ECO (during 
                 * pre-processing for a new connection event) or when it is 
                 * idle.
                 */
                if((CyBle_GetBleSsState() == CYBLE_BLESS_STATE_ECO_ON) ||
                   (CyBle_GetBleSsState() == CYBLE_BLESS_STATE_DEEPSLEEP))
                {
                    CySysPmDeepSleep();
                }
            }
            /* The else condition signifies that the BLE block cannot enter 
             * Deep Sleep and is in Active mode.  
             */
            else
            {
                /* At this point, the CPU can enter Sleep, but Deep Sleep is not
                 * allowed. 
                 * There is one exception - at a connection event, when the BLE 
                 * Rx/Tx has just finished, and the post processing for the 
                 * connection event is ongoing, the CPU cannot go to sleep.
                 * The CPU should wait in Active mode until the post processing 
                 * is complete while continuously polling the BLE low power 
                 * entry. As soon as post processing is complete, the BLE block 
                 * would enter Deep Sleep (because of the polling) and the 
                 * system Deep Sleep would then be entered. Deep Sleep is the 
                 * preferred low power mode since it takes much lesser current.
                 */
                if(CyBle_GetBleSsState() != CYBLE_BLESS_STATE_EVENT_CLOSE)
                {
                    CySysPmSleep();
                }
            }
            
            /* Exit Critical section - Global interrupts are enabled again */
            CyExitCriticalSection(interruptStatus);
        }

        /* Hibernate entry point - Hibernate is entered upon a BLE disconnect
         * event or advertisement timeout. Wakeup happens via SW2 switch press, 
         * upon which the execution starts from the first line of code. 
         * The I/O state, RAM and UDBs are retained during Hibernate.
         */
        if(enterHibernateFlag)
        {
            /* Stop the BLE component */
            CyBle_Stop();
            
            /* Enable the Hibernate wakeup functionality */
            SW2_Switch_ClearInterrupt();
            Wakeup_ISR_Start();
            
            #if (RGB_LED_IN_PROJECT)
                /* Turn off Green and Blue LEDs to indicate Hibernate */
                Led_Advertising_Green_Write(1);
                Led_Connected_Blue_Write(1);
                
                /* Change the GPIO state to High-Z */
                Led_Advertising_Green_SetDriveMode(Led_Advertising_Green_DM_ALG_HIZ);
                Led_Connected_Blue_SetDriveMode(Led_Connected_Blue_DM_ALG_HIZ);
            #endif  /* #if (RGB_LED_IN_PROJECT) */
            
            /* Enter hibernate mode */
            CySysPmHibernate();
        }
    }
}


/* [] END OF FILE */
