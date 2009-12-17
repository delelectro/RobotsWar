#ifndef _EXTI_H_
#define _EXTI_H_

#include <inttypes.h>
/* Notes:
 *
 * To generate the interrupt, the interrupt line should be configured and
 * enabled. This is done by programming the two trigger registers with the
 * desired edge detection and by enabling the interrupt request by writing a
 * '1' to the corresponding bit in the interrupt mask register.  When the
 * selected edge occurs on the external interrupt line, an interrupt request is
 * generated. The pending bit corresponding to the interrupt line is also set.
 * This request is reset by writing a '1' in the pending register.
 *
 * Hardware interrupt selection:
 * To configure the 20 lines as interrupt sources, use the following procedure:
 * 1) Configure AFIO_EXTIICR[y] to select the source input for EXTIx external 
 *    interrupt
 * 2) Configure the mask bits of the 20 interrupt lines (EXTI_IMR)
 * 3) Configure the trigger selection bits of the interrupt lines (EXTI_RTSR and EXTI_FTSR)
 * 4) Configure the enable and mask bits that control the NVIC_IRQ channel mapped to the External
 * Interrupt Controller (EXTI) so that an inerrupt coming from one of the 20 lines
 * can be correctly acknowledged.
 *
 * AFIO clock must be on.
 *
 * RM0008, page 107: "PD0, PD1 cannot be used for external interrupt/event generation
 * on 36, 48, 64-bin packages."
 * 
 * ----------------------------------------------------------------------------
 * Pin to EXTI Line Mappings:
 * EXTI0          EXTI1          EXTI2           EXTI3             EXTI4  
 * ----------------------------------------------------------------------------
 * D2/PA0         D3/PA1         D1/PA2          D0/A6/PA3         D10/A10/PA4
 * D26/EXT7/PB0   D27/EXT8/PB1   D16/A2/PC2      D17/A3/PC3        D18/A4/PC4
 * D14/A0/PC0     D15/PC1        D25/EXT5/PD2 
 *
 * EXTI5          EXTI6          EXTI7           EXTI8             EXTI9
 * ----------------------------------------------------------------------------
 * D13/A13/PA5    D12/A12/PA6    D11/A11/PA7     D6/PA8            D7/PA9
 * D4/PB5         D5/PB6         D9/PB7          D38/PB8           D23/EXT4/PB9
 * D19/A5/PC5     D34/EXTI15/PC6 D35/EXT16/PC7   D36/PC8           D37/EXT18/PC9
 *
 * EXTI10         EXTI11         EXTI12          EXTI13            EXTI14
 * ----------------------------------------------------------------------------
 * D8/PA10        D29/EXT10/PB11 D30/EXTI1/PB12  D31/EXTI12/PB13   D32/EXT13/PB14
 * D28/PB10                                      D20/EXTI1/PC13    D21/EXT2/PC14
 * D25/PC10
 *
 * EXTI15
 * ----------------------------------------------------------------------------
 * D33/EXTI14/PB15
 * D22/EXT3/PC15
 *
 *
 * The 16 EXTI interrupts are mapped to 7 interrupt handlers. 
 *
 * EXTI Lines to Interrupt Mapping:
 * EXTI0 -> EXTI0
 * EXTI1 -> EXTI1
 * EXTI2 -> EXTI2
 * EXTI3 -> EXTI3
 * EXTI4 -> EXTI4
 * EXTI[5-9] -> EXT9_5
 * EXTI[10-15] -> EXT15_10
 *
 * 
 * */

#define NR_EXTI_CHANNELS         16
#define NR_EXTI_PORTS             4
#define NR_EXTI_MODES             3

#define EXTI_IMR        0x40010400                   // Interrupt mask register
#define EXTI_EMR        (EXTI_IMR + 0x04)            // Event mask register
#define EXTI_RTSR       (EXTI_IMR + 0x08)            // Rising trigger selection register
#define EXTI_FTSR       (EXTI_IMR + 0x0C)            // Falling trigger selection register
#define EXTI_SWIER      (EXTI_IMR + 0x10)            // Software interrupt event register
#define EXTI_PR         (EXTI_IMR + 0x14)            // Pending register

#define AFIO_EVCR       0x40010000
#define AFIO_EXTICR1    (AFIO_EVCR + 0x08)
#define AFIO_EXTICR2    (AFIO_EVCR + 0x0C)
#define AFIO_EXTICR3    (AFIO_EVCR + 0x10)
#define AFIO_EXTICR4    (AFIO_EVCR + 0x14)

#define NVIC_EXTI1_OFFSET   (NVIC_ISER0 + 0x07)
#define NVIC_EXTI9_5_OFFSET (NVIC_ISER0 + 0x17)

/* NVIC Interrupt Enable registers  */
#define NVIC_ISER0          0xE000E100
#define NVIC_ISER1          0xE000E104
#define NVIC_ISER2          0xE000E108
#define NVIC_ISER3          0xE000E10C

/* NVIC Interrupt Clear registers  */
#define NVIC_ICER0          0xE000E180
#define NVIC_ICER1          0xE000E184
#define NVIC_ICER2          0xE000E188
#define NVIC_ICER3          0xE000E18C

#define EXTI_RISING                     0
#define EXTI_FALLING                    1
#define EXTI_RISING_FALLING             2

#define EXTI0  0
#define EXTI1  1
#define EXTI2  2
#define EXTI3  3
#define EXTI4  4
#define EXTI5  5
#define EXTI6  6
#define EXTI7  7
#define EXTI8  8
#define EXTI9  9
#define EXTI10 10
#define EXTI11 11
#define EXTI12 12
#define EXTI13 13
#define EXTI14 14
#define EXTI15 15

#define EXTI_CONFIG_PORTA 0
#define EXTI_CONFIG_PORTB 1
#define EXTI_CONFIG_PORTC 2
#define EXTI_CONFIG_PORTD 3

typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
extern "C"{
#endif

void exti_attach_interrupt(uint8_t, uint8_t, voidFuncPtr, uint8_t);
void exti_detach_interrupt(uint8_t);

#ifdef __cplusplus
} // extern "C"
#endif


#endif

