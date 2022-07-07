/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F18325
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include <stdio.h>

#define ADCCH_FVR 0b111111
#define ADCCH_ANA2 0b000010     

typedef struct{
    adc_result_t co2val;
    adc_result_t fvr20;
    adc_result_t fvrdebug[100];
    float fco2v;
    float co2ppm ;

}T_SYSTEM;

T_SYSTEM tSys;

const uint16_t co2_table[]={
 400, 411, 422, 434, 445, 457, 470, 483, 496, 509,
 523, 537, 552, 567, 582, 598, 614, 631, 648, 666,
 684, 703, 722, 741, 761, 782, 803, 825, 848, 871,
 894, 919, 944, 969, 996,1023,1051,1079,1109,1139,
1170,1201,1234,1268,1302,1338,1374,1411,1450,1489,
1530,1571,1614,1658,1703,1749,1797,1845,1896,1947,
2000,2055,2110,2168,2227,2287,2349,2413,2479,2546,
2616,2687,2760,2835,2912,2991,3072,3156,3242,3330,
3420,3513,3609,3707,3808,3911,4018,4127,4239,4354,
4473,4594,4719,4847,4979,5115,5254,5396,5543,5694,
5849,6008,6171

};




/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    
    while (1)
    {
        char s[20];
        char *p;
        float fco2,fvr20;
        float e;
        int n;
        // Add your application code
        ADC_SelectChannel(ADCCH_ANA2);  __delay_us(20);
        tSys.co2val =  ADC_GetConversion(ADCCH_ANA2);
        
        ADC_SelectChannel(ADCCH_FVR);   __delay_us(20);
        tSys.fvr20 =  ADC_GetConversion(ADCCH_FVR);
        fvr20 = tSys.fvr20;
        fco2 = tSys.co2val;
        tSys.fco2v = fco2 * (2.048/fvr20);
        
        n =  (3.23-tSys.fco2v)/(5E-3);
        if(n<0) n= 0;
        else if( n>= (sizeof(co2_table)/2))n=sizeof(co2_table)/2-1;
        sprintf(s,"%d,%4.2fV\r\n",co2_table[n],tSys.fco2v);
        p = s;
        while(*p){ 
            EUSART_Write(*p);
            p++;
        }
        __delay_ms(1000);
//        printf("%f\r\n",tSys.fco2v);
   //     printf("%d",(int)fco2);
    //   tSys.co2ppm = 400.0* pow(10.0 , 1);//MG812_K*e);
    }
}

/*
int putch(int c)
{
    EUSART_Write((char)c);
    return c;
}*/
/**
 End of File
*/