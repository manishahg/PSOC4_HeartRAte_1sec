/*******************************************************************************
* File Name: Led_Advertising_Green.h  
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

#if !defined(CY_PINS_Led_Advertising_Green_H) /* Pins Led_Advertising_Green_H */
#define CY_PINS_Led_Advertising_Green_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Led_Advertising_Green_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Led_Advertising_Green_Write(uint8 value) ;
void    Led_Advertising_Green_SetDriveMode(uint8 mode) ;
uint8   Led_Advertising_Green_ReadDataReg(void) ;
uint8   Led_Advertising_Green_Read(void) ;
uint8   Led_Advertising_Green_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Led_Advertising_Green_DRIVE_MODE_BITS        (3)
#define Led_Advertising_Green_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Led_Advertising_Green_DRIVE_MODE_BITS))

#define Led_Advertising_Green_DM_ALG_HIZ         (0x00u)
#define Led_Advertising_Green_DM_DIG_HIZ         (0x01u)
#define Led_Advertising_Green_DM_RES_UP          (0x02u)
#define Led_Advertising_Green_DM_RES_DWN         (0x03u)
#define Led_Advertising_Green_DM_OD_LO           (0x04u)
#define Led_Advertising_Green_DM_OD_HI           (0x05u)
#define Led_Advertising_Green_DM_STRONG          (0x06u)
#define Led_Advertising_Green_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Led_Advertising_Green_MASK               Led_Advertising_Green__MASK
#define Led_Advertising_Green_SHIFT              Led_Advertising_Green__SHIFT
#define Led_Advertising_Green_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Led_Advertising_Green_PS                     (* (reg32 *) Led_Advertising_Green__PS)
/* Port Configuration */
#define Led_Advertising_Green_PC                     (* (reg32 *) Led_Advertising_Green__PC)
/* Data Register */
#define Led_Advertising_Green_DR                     (* (reg32 *) Led_Advertising_Green__DR)
/* Input Buffer Disable Override */
#define Led_Advertising_Green_INP_DIS                (* (reg32 *) Led_Advertising_Green__PC2)


#if defined(Led_Advertising_Green__INTSTAT)  /* Interrupt Registers */

    #define Led_Advertising_Green_INTSTAT                (* (reg32 *) Led_Advertising_Green__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Led_Advertising_Green_DRIVE_MODE_SHIFT       (0x00u)
#define Led_Advertising_Green_DRIVE_MODE_MASK        (0x07u << Led_Advertising_Green_DRIVE_MODE_SHIFT)


#endif /* End Pins Led_Advertising_Green_H */


/* [] END OF FILE */
