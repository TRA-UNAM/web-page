// NATIONAL AUTONOMOUS UNIVERSITY OF MEXICO
// SCHOOL OF ENGINEERING
// MECHATRONICS DEPARTMENT

// AUTHORS:    M. Eng. ULISES M. PEÑUELAS RIVAS
//             M. Eng. YUKIHIRO MINAMI KOYAMA

// NOTE: THIS LIBRARY FILE WAS CREATED AS FREE EDUCATIONAL MATERIAL

// Version 2020-2

/****************************************************************
This library file is debugged to work with PIC16F887, PIC16F1939 
and PIC18F4550 microcontrollers

There are certain pins with special configurations in these PICs, 
for example, in the DIP-40 packaging, the following considerations 
must be observed:

   - Pin 1 is Master Clear (Reset by hardware)
   - Pin 11 and pin 32 power input, voltage
   - Pin 12 and pin 31 power input, ground

Port A: A0-A7 Inputs
Port B: B0-B7 Inputs
Port C: C0-C7 Outputs
Port D: D0-D7 Outputs
Port E: E0-E2 Outputs

Reset hardware must be connected on pin E3.

---Functions---
The library has the following functions:
   - PLD_INI()
      PIC inicialization as GAL
      It must be placed at the begin of main() function
      
   - PLD_555(value)
      Value - Is an unsigned integer number. It select the frequency ouput in Hz
      The function generates clock pulses in a 1-bit variable named OUT_555
      The function must be placed inside the main () function after pld_ini ()
          
      Example: 

         ...
         pld_ini();   //Initialize the microcontroller in PLD mode
         pld_555(10); //Generates a square signal with a frequency of 10 Hz
                      //in the 1 bit internal variable OUT_555
         ...
         ck = out_555;   //ck loads the value of the variable out_555
                        
****************************************************************/
   
#ignore_warnings 203
#if getenv("DEVICE")=="PIC16F887"
   #fuses INTRC_IO,NOWDT,NOBROWNOUT,NOLVP
#endif
#if getenv("DEVICE")=="PIC16F1939"
   #fuses INTRC_IO,NOWDT,NOLVP
 #endif
#if getenv("DEVICE")=="PIC18F4550"
   #fuses NOWDT
   #use delay(internal=8MHz)
 #endif
#use delay(internal=8MHz, CLOCK=8MHz)
#byte trisa = getenv("SFR:TRISA")
#byte trisb = getenv("SFR:TRISB")
#byte trisc = getenv("SFR:TRISC")
#byte trisd = getenv("SFR:TRISD")
#byte trise = getenv("SFR:TRISE")
#byte porta = getenv("SFR:PORTA")
#byte portb = getenv("SFR:PORTB")
#byte portc = getenv("SFR:PORTC")
#byte portd = getenv("SFR:PORTD")
#byte porte = getenv("SFR:PORTE")
//PORTA
#bit A0 = PORTA.0
#bit A1 = PORTA.1
#bit A2 = PORTA.2
#bit A3 = PORTA.3
#bit A4 = PORTA.4
#bit A5 = PORTA.5
#bit A6 = PORTA.6
#if !(getenv("DEVICE")=="PIC18F4550")
#bit A7 = PORTA.7
#endif
//PORTB
#bit B0 = PORTB.0
#bit B1 = PORTB.1
#bit B2 = PORTB.2
#bit B3 = PORTB.3
#bit B4 = PORTB.4
#bit B5 = PORTB.5
#bit B6 = PORTB.6
#bit B7 = PORTB.7
//PORTC
#bit C0 = PORTC.0
#bit C1 = PORTC.1
#bit C2 = PORTC.2
#bit C3 = PORTC.3
#bit C4 = PORTC.4
#bit C5 = PORTC.5
#bit C6 = PORTC.6
#bit C7 = PORTC.7
//PORTD
#bit D0 = PORTD.0
#bit D1 = PORTD.1
#bit D2 = PORTD.2
#bit D3 = PORTD.3
#bit D4 = PORTD.4
#bit D5 = PORTD.5
#bit D6 = PORTD.6
#bit D7 = PORTD.7
//PORTE
#bit E0 = PORTE.0
#bit E1 = PORTE.1
#bit E2 = PORTE.2

void PLD_555(long pld_fre);
void PLD_FLIP(short pld_opt);

unsigned long pld_cA=0;
unsigned long pld_max=0;
short out_555=0;

#define tmr1_val 64549

#int_TIMER1
void pld_tiempo()
{
   set_timer1(tmr1_val);
   pld_cA++;
   if(pld_cA >= pld_max)   { out_555 = !out_555; pld_cA=0; }
}

void pld_ini()
{
   setup_adc_ports(NO_ANALOGS);
   set_tris_A(0xFF);
   set_tris_B(0xFF);
   set_tris_C(0x00);
   set_tris_D(0x00);//0x03);
   set_tris_E(0x00);
   PORTA=0;
   PORTB=0;
   PORTC=0;
   PORTD=0;
   PORTE=0;
   
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
   clear_interrupt(INT_TIMER1);
   
}

void pld_555(unsigned long pld_fre)
{
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
   set_timer1(tmr1_val);
   pld_max = 1000./pld_fre;
}
