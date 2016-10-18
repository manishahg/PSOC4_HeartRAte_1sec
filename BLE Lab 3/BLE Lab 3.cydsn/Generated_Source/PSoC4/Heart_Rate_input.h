/*******************************************************************************
* File Name: Heart_Rate_input.h  
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

#if !defined(CY_PINS_Heart_Rate_input_H) /* Pins Heart_Rate_input_H */
#define CY_PINS_Heart_Rate_input_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Heart_Rate_input_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Heart_Rate_input_Write(uint8 value) ;
void    Heart_Rate_input_SetDriveMode(uint8 mode) ;
uint8   Heart_Rate_input_ReadDataReg(void) ;
uint8   Heart_Rate_input_Read(void) ;
uint8   Heart_Rate_input_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Heart_Rate_input_DRIVE_MODE_BITS        (3)
#define Heart_Rate_input_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Heart_Rate_input_DRIVE_MODE_BITS))

#define Heart_Rate_input_DM_ALG_HIZ         (0x00u)
#define Heart_Rate_input_DM_DIG_HIZ         (0x01u)
#define Heart_Rate_input_DM_RES_UP          (0x02u)
#define Heart_Rate_input_DM_RES_DWN         (0x03u)
#define Heart_Rate_input_DM_OD_LO           (0x04u)
#define Heart_Rate_input_DM_OD_HI           (0x05u)
#define Heart_Rate_input_DM_STRONG          (0x06u)
#define Heart_Rate_input_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Heart_Rate_input_MASK               Heart_Rate_input__MASK
#define Heart_Rate_input_SHIFT              Heart_Rate_input__SHIFT
#define Heart_Rate_input_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Heart_Rate_input_PS                     (* (reg32 *) Heart_Rate_input__PS)
/* Port Configuration */
#define Heart_Rate_input_PC                     (* (reg32 *) Heart_Rate_input__PC)
/* Data Register */
#define Heart_Rate_input_DR                     (* (reg32 *) Heart_Rate_input__DR)
/* Input Buffer Disable Override */
#define Heart_Rate_input_INP_DIS                (* (reg32 *) Heart_Rate_input__PC2)


#if defined(Heart_Rate_input__INTSTAT)  /* Interrupt Registers */

    #define Heart_Rate_input_INTSTAT                (* (reg32 *) Heart_Rate_input__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Heart_Rate_input_DRIVE_MODE_SHIFT       (0x00u)
#define Heart_Rate_input_DRIVE_MODE_MASK        (0x07u << Heart_Rate_input_DRIVE_MODE_SHIFT)


#endif /* End Pins Heart_Rate_input_H */


/* [] END OF FILE */
