/*!
    \file    main.c
    \brief   led spark with systick
    
    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "gd32f450i_eval.h"

#if   defined ( __CC_ARM )          /* Arm Compiler 4/5 */
    __attribute__((section("no_init1"), zero_init)) uint16_t data;

#elif defined (__ARMCC_VERSION)     /* Arm Compiler 6 */
    __attribute__((section(".bss.no.init1"))) uint16_t data;
#endif

/*!
    \brief    main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);

    gd_eval_com_init(EVAL_COM0);
    systick_config();
    
    if(0U != (RCU_RSTSCK & RCU_RSTSCK_PORRSTF)){
        data = 0;
        rcu_all_reset_flag_clear();
    }

    data += 1;
    printf("%d\r\n", data);

    while (1){
        /* turn on LED1, turn off LED2 and LED3 */
        gd_eval_led_on(LED1);
        gd_eval_led_off(LED2);
        gd_eval_led_off(LED3);
        delay_1ms(500);

        /* turn on LED2, turn off LED1 and LED3 */
        gd_eval_led_on(LED2);
        gd_eval_led_off(LED1);
        gd_eval_led_off(LED3);
        delay_1ms(500);

        /* turn on LED3, turn off LED1 and LED2 */
        gd_eval_led_on(LED3);
        gd_eval_led_off(LED1);
        gd_eval_led_off(LED2);
        delay_1ms(500);
    }
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
