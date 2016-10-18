/*****************************************************************************
* File Name: HeartRateProcessing.c
*
* Version: 1.0
*
* Description:
* This file implements the heart rate measurement capability in the the PSoC 4 
* BLE Lab 3.
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
#include "WatchdogTimer.h"


/*****************************************************************************
* Macros 
*****************************************************************************/
#define HEART_RATE_CHANNEL			        (0)
#define ADC_THRESHOLD 			            (0x06A0)
#define SEC_IN_MIN							(60)
#define MS_TO_SECOND                        (1000)


/*****************************************************************************
* Public variables 
*****************************************************************************/
uint8 heartRate = 0;


/*****************************************************************************
* Public function definitions
*****************************************************************************/

/*****************************************************************************
* Function Name: ProcessHeartRateSignal
******************************************************************************
* Summary:
* Measures the heart rate of the user.
*
* Parameters:
* None
*
* Return:
* None
*
* Theory:
* This function acts as a simple model for heart rate measurement.
* The function takes the ADC sampled output and compares it to a threshold.
* If the ADC output is more than the threshold then it is considered as a 
* valid beat (R peak). The rising edge of this R peak is identified and the 
* corresponding system timestamp is noted. The RR-interval between two 
* peaks is then calculated and converted to a heart rate value in beats per
* minute. The RR-interval period is calculated over a rolling window.
*
* Side Effects:
* None
*
*****************************************************************************/
void ProcessHeartRateSignal(void)
{
    static bool newBeat = true;
    static bool firstTime = true;
    static uint32 previousBeatTime = 0;
    static uint32 newBeatTime = 0;
    int16 adcOut;
    uint32 twoSampleTime = 0;

    /* Get the ADC output */
    ADC_StartConvert();
    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
    adcOut = ADC_GetResult16(HEART_RATE_CHANNEL);
    
    /* If the ADC output is more than a fixed threshold, consider that a 
     * valid R peak */
    if (adcOut > ADC_THRESHOLD)
    {
        /* Check if the R peak just started - i.e. identify the rising 
         * edge of the R peak */
        if(newBeat)
        {
            /* Check if this is the first R-peak seen by the device yet.
             * If that is the case, we cannot calculate a heart rate value 
             * yet since a minimum of two peak time interval is required. 
             * Just note the timestamp of this peak.
             */
    		if(firstTime == true)
    		{
    			firstTime = false;
    			previousBeatTime = WatchdogTimer_GetTimestamp();
    		}
    		else
    		{
                /* Rolling window of two samples. Note the timestamp of 
                 * the new peak and subtract the timestamp of the previous
                 * to obtain the RR-interval. Extrapolate it to get a heart
                 * beat value in beats per minute.
                 */
    			newBeatTime = WatchdogTimer_GetTimestamp();
    			twoSampleTime = newBeatTime - previousBeatTime;
                
                if(twoSampleTime != 0)
                {
                    heartRate = (uint32)SEC_IN_MIN * MS_TO_SECOND / twoSampleTime;
                }
                
                previousBeatTime = newBeatTime;
    		}
        }
        
        /* Clear the flag to indicate next time that this R peak has already
         * been accounted for and we need to wait for the next peak. 
         */
        newBeat = false;
    }
    else
    {
        /* Set the flag to indicate that there is no R-peak going on right now.
         * So it is expected that the next time the ADC output is more than 
         * the threshold, it will be a new peak. 
         */
        newBeat = true;
    }
}


/* [] END OF FILE */
