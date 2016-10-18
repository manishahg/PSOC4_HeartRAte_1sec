/*******************************************************************************
* File Name: Led_Advertising_Green.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Led_Advertising_Green.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        Led_Advertising_Green_PC =   (Led_Advertising_Green_PC & \
                                (uint32)(~(uint32)(Led_Advertising_Green_DRIVE_MODE_IND_MASK << (Led_Advertising_Green_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (Led_Advertising_Green_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: Led_Advertising_Green_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void Led_Advertising_Green_Write(uint8 value) 
{
    uint8 drVal = (uint8)(Led_Advertising_Green_DR & (uint8)(~Led_Advertising_Green_MASK));
    drVal = (drVal | ((uint8)(value << Led_Advertising_Green_SHIFT) & Led_Advertising_Green_MASK));
    Led_Advertising_Green_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: Led_Advertising_Green_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Led_Advertising_Green_DM_STRONG     Strong Drive 
*  Led_Advertising_Green_DM_OD_HI      Open Drain, Drives High 
*  Led_Advertising_Green_DM_OD_LO      Open Drain, Drives Low 
*  Led_Advertising_Green_DM_RES_UP     Resistive Pull Up 
*  Led_Advertising_Green_DM_RES_DWN    Resistive Pull Down 
*  Led_Advertising_Green_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Led_Advertising_Green_DM_DIG_HIZ    High Impedance Digital 
*  Led_Advertising_Green_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Led_Advertising_Green_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(Led_Advertising_Green__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: Led_Advertising_Green_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro Led_Advertising_Green_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Led_Advertising_Green_Read(void) 
{
    return (uint8)((Led_Advertising_Green_PS & Led_Advertising_Green_MASK) >> Led_Advertising_Green_SHIFT);
}


/*******************************************************************************
* Function Name: Led_Advertising_Green_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 Led_Advertising_Green_ReadDataReg(void) 
{
    return (uint8)((Led_Advertising_Green_DR & Led_Advertising_Green_MASK) >> Led_Advertising_Green_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Led_Advertising_Green_INTSTAT) 

    /*******************************************************************************
    * Function Name: Led_Advertising_Green_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 Led_Advertising_Green_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(Led_Advertising_Green_INTSTAT & Led_Advertising_Green_MASK);
		Led_Advertising_Green_INTSTAT = maskedStatus;
        return maskedStatus >> Led_Advertising_Green_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
