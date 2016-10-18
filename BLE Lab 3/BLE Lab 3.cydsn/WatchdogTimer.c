/*****************************************************************************
* File Name: WatchdogTimer.c
*
* Version: 1.0
*
* Description:
* This file defines the watchdog timer functionality for this lab session in 
* the PSoC 4 BLE Lab 3.
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


/*****************************************************************************
* Macros and constants
*****************************************************************************/
#define WDT_PERIOD_MS               (10)
#define WDT_TICKS_PER_MS            (32)
#define WDT_TICKS                   (WDT_PERIOD_MS * WDT_TICKS_PER_MS)
#define WDT_INTERRUPT_NUM           (8)


/*****************************************************************************
* Static variables
*****************************************************************************/
static uint32 watchdogTimestamp = 0;


/*****************************************************************************
* Public function definitions
*****************************************************************************/

/*****************************************************************************
* Function Name: WatchdogTimer_Isr
******************************************************************************
* Summary:
* Interrupt service routine for the watchdog timer.
*
* Parameters:
* None
*
* Return:
* None
*
* Theory:
* The ISR increments the system timestamp by the watchdog timer period. It 
* then clears the WDT interrupt.
*
* Side Effects:
* None
*
*****************************************************************************/
CY_ISR(WatchdogTimer_Isr)
{
    /* Update the system timestamp - the watchdog period time has elapsed
     * since the last interrupt.
     */
    watchdogTimestamp += WDT_PERIOD_MS;
    
    /* Clear WDT interrupt */
    CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
}


/*****************************************************************************
* Function Name: WatchdogTimer_Start
******************************************************************************
* Summary:
* Starts the watchdog timer WDT0 to be used as the system timer. Define a 
* system timestamp variable to be updated on every watchdog interrupt.
*
* Parameters:
* None
*
* Return:
* None
*
* Theory:
* The function uses the watchdog timer WDT0 of the chip. It configures the 
* timer to fire an interrupt upon match. The periodic interrupt updates the 
* system timestamp variable which is used to keep track of the system activity.
* The timer is configured for clear on match i.e. the WDT counter is reset to
* zero upon a match event. The timer is continuously run.
*
* To change the watchdog timer settings, the function needs to unlock the 
* WDT first, and then lock it after the modification is complete.
*
* Side Effects:
* None
*
*****************************************************************************/
void WatchdogTimer_Start(void)
{
    /* Set the WDT ISR */
    CyIntSetVector(WDT_INTERRUPT_NUM, &WatchdogTimer_Isr);
    
    /* Unlock the sytem watchdog timer to be able to change settings */
	CySysWdtUnlock();
    
    /* Configure the watchdog timer 0 (WDT0) to fire an interrupt upon match 
     * i.e. when the count register value equals the match register value.
     */
    CySysWdtWriteMode(0, CY_SYS_WDT_MODE_INT);
    
    /* WDT0 counter to be cleared upon a match event and then begin again.
     * The timer is to be run continuously.
     */
	CySysWdtWriteClearOnMatch(0, 1);
    
    /* Set the value of the match register. Since the count starts from zero, 
     * the actual value is the (intended - 1). 
     * The match register value set here is for 10 ms interval. This is 
     * because we want to run a main loop of 10 ms with a scan happening at 
     * the beginning of the loop and stay in deep sleep for the rest of the
     * time. With a scan happening every 10 ms, we can properly detect a 
     * heart rate without missing a beat.
     */
    CySysWdtWriteMatch(0, WDT_TICKS - 1);
    
    /* Enable the WDT0 */
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
    
    /* Enable interrupt */
    CyIntEnable(WDT_INTERRUPT_NUM);
    
    /* Lock the watchdog timer to prevent future modification */
	CySysWdtLock();
}


/*****************************************************************************
* Function Name: WatchdogTimer_GetTimestamp
******************************************************************************
* Summary:
* Returns the system timestamp value.
*
* Parameters:
* None
*
* Return:
* uint32: Current system timestamp 
*
* Theory:
* The function returns the watchdog timestamp.
*
* Side Effects:
* None
*
*****************************************************************************/
uint32 WatchdogTimer_GetTimestamp(void)
{
    return watchdogTimestamp;
}


/* [] END OF FILE */
