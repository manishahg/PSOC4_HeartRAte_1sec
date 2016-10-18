/*******************************************************************************
* File Name: Heart_Rate_input.c  
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
#include "Heart_Rate_input.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        Heart_Rate_input_PC =   (Heart_Rate_input_PC & \
                                (uint32)(~(uint32)(Heart_Rate_input_DRIVE_MODE_IND_MASK << (Heart_Rate_input_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (Heart_Rate_input_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: Heart_Rate_input_Write
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
void Heart_Rate_input_Write(uint8 value) 
{
    uint8 drVal = (uint8)(Heart_Rate_input_DR & (uint8)(~Heart_Rate_input_MASK));
    drVal = (drVal | ((uint8)(value << Heart_Rate_input_SHIFT) & Heart_Rate_input_MASK));
    Heart_Rate_input_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: Heart_Rate_input_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Heart_Rate_input_DM_STRONG     Strong Drive 
*  Heart_Rate_input_DM_OD_HI      Open Drain, Drives High 
*  Heart_Rate_input_DM_OD_LO      Open Drain, Drives Low 
*  Heart_Rate_input_DM_RES_UP     Resistive Pull Up 
*  Heart_Rate_input_DM_RES_DWN    Resistive Pull Down 
*  Heart_Rate_input_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Heart_Rate_input_DM_DIG_HIZ    High Impedance Digital 
*  Heart_Rate_input_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Heart_Rate_input_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(Heart_Rate_input__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: Heart_Rate_input_Read
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
*  Macro Heart_Rate_input_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Heart_Rate_input_Read(void) 
{
    return (uint8)((Heart_Rate_input_PS & Heart_Rate_input_MASK) >> Heart_Rate_input_SHIFT);
}


/*******************************************************************************
* Function Name: Heart_Rate_input_ReadDataReg
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
uint8 Heart_Rate_input_ReadDataReg(void) 
{
    return (uint8)((Heart_Rate_input_DR & Heart_Rate_input_MASK) >> Heart_Rate_input_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Heart_Rate_input_INTSTAT) 

    /*******************************************************************************
    * Function Name: Heart_Rate_input_ClearInterrupt
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
    uint8 Heart_Rate_input_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(Heart_Rate_input_INTSTAT & Heart_Rate_input_MASK);
		Heart_Rate_input_INTSTAT = maskedStatus;
        return maskedStatus >> Heart_Rate_input_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
