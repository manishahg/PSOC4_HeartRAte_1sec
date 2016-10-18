/*******************************************************************************
* File Name: Led_Connected_Blue.c  
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
#include "Led_Connected_Blue.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        Led_Connected_Blue_PC =   (Led_Connected_Blue_PC & \
                                (uint32)(~(uint32)(Led_Connected_Blue_DRIVE_MODE_IND_MASK << (Led_Connected_Blue_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (Led_Connected_Blue_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: Led_Connected_Blue_Write
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
void Led_Connected_Blue_Write(uint8 value) 
{
    uint8 drVal = (uint8)(Led_Connected_Blue_DR & (uint8)(~Led_Connected_Blue_MASK));
    drVal = (drVal | ((uint8)(value << Led_Connected_Blue_SHIFT) & Led_Connected_Blue_MASK));
    Led_Connected_Blue_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: Led_Connected_Blue_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Led_Connected_Blue_DM_STRONG     Strong Drive 
*  Led_Connected_Blue_DM_OD_HI      Open Drain, Drives High 
*  Led_Connected_Blue_DM_OD_LO      Open Drain, Drives Low 
*  Led_Connected_Blue_DM_RES_UP     Resistive Pull Up 
*  Led_Connected_Blue_DM_RES_DWN    Resistive Pull Down 
*  Led_Connected_Blue_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Led_Connected_Blue_DM_DIG_HIZ    High Impedance Digital 
*  Led_Connected_Blue_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Led_Connected_Blue_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(Led_Connected_Blue__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: Led_Connected_Blue_Read
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
*  Macro Led_Connected_Blue_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Led_Connected_Blue_Read(void) 
{
    return (uint8)((Led_Connected_Blue_PS & Led_Connected_Blue_MASK) >> Led_Connected_Blue_SHIFT);
}


/*******************************************************************************
* Function Name: Led_Connected_Blue_ReadDataReg
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
uint8 Led_Connected_Blue_ReadDataReg(void) 
{
    return (uint8)((Led_Connected_Blue_DR & Led_Connected_Blue_MASK) >> Led_Connected_Blue_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Led_Connected_Blue_INTSTAT) 

    /*******************************************************************************
    * Function Name: Led_Connected_Blue_ClearInterrupt
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
    uint8 Led_Connected_Blue_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(Led_Connected_Blue_INTSTAT & Led_Connected_Blue_MASK);
		Led_Connected_Blue_INTSTAT = maskedStatus;
        return maskedStatus >> Led_Connected_Blue_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
