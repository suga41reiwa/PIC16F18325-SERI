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
        Device            :  PIC16F18326
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
#include "stdio.h"
#include "math.h"

/*
                         Main application
 */


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
 323 ,327 ,332 ,336 ,341 ,345 ,350 ,355 ,359 ,364 ,
 369 ,374 ,379 ,384 ,389 ,395 ,400 ,405 ,411 ,416 ,
 422 ,428 ,434 ,439 ,445 ,451 ,457 ,464 ,470 ,476 ,
 483 ,489 ,496 ,502 ,509 ,516 ,523 ,530 ,537 ,545 ,
 552 ,559 ,567 ,575 ,582 ,590 ,598 ,606 ,614 ,623 ,
 631 ,640 ,648 ,657 ,666 ,675 ,684 ,693 ,703 ,712 ,
 722 ,731 ,741 ,751 ,761 ,772 ,782 ,793 ,803 ,814 ,
 825 ,836 ,848 ,859 ,871 ,883 ,894 ,907 ,919 ,931 ,
 944 ,957 ,969 ,983 ,996 ,1009,1023,1037,1051,1065,
1079,1094,1109,1124,1139,1154,1170,1185,1201,1218,
1234,1251,1268,1285,1302,1320,1338,1356,1374,1392,
1411,1430,1450,1469,1489,1509,1530,1550,1571,1592,
1614,1636,1658,1680,1703,1726,1749,1773,1797,1821,
1845,1870,1896,1921,1947,1973,2000,2027,2055,2082,
2110,2139,2168,2197,2227,2257,2287,2318,2349,2381,
2413,2446,2479,2512,2546,2581,2616,2651,2687,2723,
2760,2797,2835,2873,2912,2951,2991,3031,3072,3114,
3156,3198,3242,3285,3330,3375,3420,3466,3513,3561,
3609,3658,3707,3757,3808,3859,3911,3964,4018,4072,
4127,4182,4239,4296,4354,4413,4473,4533,4594,4656,
4719,4783,4847,4913,4979,5046,5115,5184,5254,5325,
5396,5469,5543,5618,5694,5771,5849,5928,6008,6089,
6171,6254,6339,6424,6511,6599,6688,6779,6870,6963,
7057,7152,7249,7347,7446,7546,7648,7751,7856,7962,
8070,8179,8289,8401,8515,8629,8746,8864,8984,9105,
9228,9353,9479,9607,9737,9868,10001,10136,10273,10412,
10553,10695,10840,10986,11134,11285,11437,11591,11748,
11907,12067,
};

#define FVR_VOLT    4.048514    // 4.096

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
        tSys.fco2v = fco2 * (FVR_VOLT/fvr20);
        
        n =  (3.27-tSys.fco2v)/(2.5E-3);
        if(n<0) n= 0;
        else if( n>= (sizeof(co2_table)/2))n=sizeof(co2_table)/2-1;
        sprintf(s,"%d,%5.3fV\r\n",co2_table[n],tSys.fco2v);
        p = s;
        while(*p){ 
            EUSART_Write(*p);
            p++;
        }
        __delay_ms(1000);
    }
}

/**
 End of File
*/