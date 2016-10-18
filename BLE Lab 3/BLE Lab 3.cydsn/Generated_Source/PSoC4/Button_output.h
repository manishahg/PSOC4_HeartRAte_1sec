/*******************************************************************************
* File Name: Button_output.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Button_output_H) /* Pins Button_output_H */
#define CY_PINS_Button_output_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Button_output_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Button_output_Write(uint8 value) ;
void    Button_output_SetDriveMode(uint8 mode) ;
uint8   Button_output_ReadDataReg(void) ;
uint8   Button_output_Read(void) ;
uint8   Button_output_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Button_output_DRIVE_MODE_BITS        (3)
#define Button_output_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Button_output_DRIVE_MODE_BITS))

#define Button_output_DM_ALG_HIZ         (0x00u)
#define Button_output_DM_DIG_HIZ         (0x01u)
#define Button_output_DM_RES_UP          (0x02u)
#define Button_output_DM_RES_DWN         (0x03u)
#define Button_output_DM_OD_LO           (0x04u)
#define Button_output_DM_OD_HI           (0x05u)
#define Button_output_DM_STRONG          (0x06u)
#define Button_output_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Button_output_MASK               Button_output__MASK
#define Button_output_SHIFT              Button_output__SHIFT
#define Button_output_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Button_output_PS                     (* (reg32 *) Button_output__PS)
/* Port Configuration */
#define Button_output_PC                     (* (reg32 *) Button_output__PC)
/* Data Register */
#define Button_output_DR                     (* (reg32 *) Button_output__DR)
/* Input Buffer Disable Override */
#define Button_output_INP_DIS                (* (reg32 *) Button_output__PC2)


#if defined(Button_output__INTSTAT)  /* Interrupt Registers */

    #define Button_output_INTSTAT                (* (reg32 *) Button_output__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Button_output_DRIVE_MODE_SHIFT       (0x00u)
#define Button_output_DRIVE_MODE_MASK        (0x07u << Button_output_DRIVE_MODE_SHIFT)


#endif /* End Pins Button_output_H */


/* [] END OF FILE */
