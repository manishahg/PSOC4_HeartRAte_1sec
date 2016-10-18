/*******************************************************************************
* File Name: Led_Connected_Blue.h  
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

#if !defined(CY_PINS_Led_Connected_Blue_H) /* Pins Led_Connected_Blue_H */
#define CY_PINS_Led_Connected_Blue_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Led_Connected_Blue_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Led_Connected_Blue_Write(uint8 value) ;
void    Led_Connected_Blue_SetDriveMode(uint8 mode) ;
uint8   Led_Connected_Blue_ReadDataReg(void) ;
uint8   Led_Connected_Blue_Read(void) ;
uint8   Led_Connected_Blue_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Led_Connected_Blue_DRIVE_MODE_BITS        (3)
#define Led_Connected_Blue_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Led_Connected_Blue_DRIVE_MODE_BITS))

#define Led_Connected_Blue_DM_ALG_HIZ         (0x00u)
#define Led_Connected_Blue_DM_DIG_HIZ         (0x01u)
#define Led_Connected_Blue_DM_RES_UP          (0x02u)
#define Led_Connected_Blue_DM_RES_DWN         (0x03u)
#define Led_Connected_Blue_DM_OD_LO           (0x04u)
#define Led_Connected_Blue_DM_OD_HI           (0x05u)
#define Led_Connected_Blue_DM_STRONG          (0x06u)
#define Led_Connected_Blue_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Led_Connected_Blue_MASK               Led_Connected_Blue__MASK
#define Led_Connected_Blue_SHIFT              Led_Connected_Blue__SHIFT
#define Led_Connected_Blue_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Led_Connected_Blue_PS                     (* (reg32 *) Led_Connected_Blue__PS)
/* Port Configuration */
#define Led_Connected_Blue_PC                     (* (reg32 *) Led_Connected_Blue__PC)
/* Data Register */
#define Led_Connected_Blue_DR                     (* (reg32 *) Led_Connected_Blue__DR)
/* Input Buffer Disable Override */
#define Led_Connected_Blue_INP_DIS                (* (reg32 *) Led_Connected_Blue__PC2)


#if defined(Led_Connected_Blue__INTSTAT)  /* Interrupt Registers */

    #define Led_Connected_Blue_INTSTAT                (* (reg32 *) Led_Connected_Blue__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Led_Connected_Blue_DRIVE_MODE_SHIFT       (0x00u)
#define Led_Connected_Blue_DRIVE_MODE_MASK        (0x07u << Led_Connected_Blue_DRIVE_MODE_SHIFT)


#endif /* End Pins Led_Connected_Blue_H */


/* [] END OF FILE */
