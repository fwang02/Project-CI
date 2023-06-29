/* Main.c 
 * Processor: PIC18F45K22
 * Compiler:  MPLAB XC8
 */

#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "config.h"
#include "GLCD.h"
#include "splash.h"
#define _XTAL_FREQ 8000000  

#define A PORTAbits.RA3
#define B PORTAbits.RA5
#define C PORTAbits.RA2
#define D PORTAbits.RA4
#define E PORTCbits.RC1
#define F PORTCbits.RC4
#define K PORTCbits.RC0

const char * s1 = "Bru Bernadi\n";
const char * s2 = "Feiyang Wang\n";
const char * s3 = "L7 Projecte\n";
const char * s4 = "Score=\n";
const char * s5 = "You Win!!!\n";
const char * s6 = "Game Over\n";
const char cor = '%';
const char ninot = '#';
const char * TextInici = "Press K button to start";

struct Mur{
   int x;
   int y;
   int espai;
   bool activat;
};
struct Diners{
   int x;
   int y;
   int tipus;
   bool activat;
};
struct Fantasma{
   int x;
   int y;
   int sentit;
   bool activat;
};

int over = 0;
int borra = 0;
int borraSplash = 0;
int punts = 0;
int ant_punts;
int lives = 3;
int en_joc = 0;

int written = 0;

int x = 0;
int y = 3;

int an0;
int an1;

int flanc0 = 0;
int flanc1 = 0;
int flanc2 = 0;
int flanc3 = 0;
int flanc4 = 0;
int flanc5 = 0;
int flanc6 = 0;

int ant_seg = 0;
int seg= 0;
int as = 0;

struct Mur murs[6];
int dist_murs;
int ultim;

struct Diners diners;
int rd_diners;
int a = 0;
int b = 0;
bool ocupat = false;

bool invencible = false;
int premut = 0;		//si està premut el teclat o no		
int dif = 1;		//dificultat


struct Fantasma fantasma;

void writeTxt(byte page, byte y, char * s) {
   int i=0;
   while (*s!='\n' && *s!='\0') 
   {
      putchGLCD(page, y+i, *(s++));
      i++;
   };
}	

void configGLCD() {
   GLCDinit();   	      	// Inicialitzem la pantalla
   clearGLCD(0,7,0,127);      	// Esborrem pantalla
   setStartLine(0);           	// Definim linia d'inici
}

void configPIC() {
   ANSELA = 0x03;
   ANSELB = 0x00;		// PORTB digital
   ANSELC = 0x00;		// PORTC digital
   ANSELD = 0x00;		// PORTD digital
   
   TRISA = 0xFF;
   TRISB = 0x00;		// PORTB output
   TRISC = 0X97;
   TRISD = 0x00;		// PORTD output
   
   PORTA = 0x00;
   PORTB = 0x00;		// Inicialitzem a 0 els ports
   PORTC = 0x00;
   PORTD = 0x00;
   
   IPEN = 0;
   GIE = 1;				

   // Comunicació en sèrie
   BAUDCON1bits.BRG16 = 1;	// 8-bit Baud Rate Generator
   TXSTA1 = 0x24; 		// USART Configuration Register --> TX9 = 0; TXEN = 1; SYNC = 0; BRGH = 1
   TXREG1 = 0;
   SPBRG1 = 16; 		// Set de Baud rate
   TRISCbits.RC6 = 1; 		// Configure TX1 pin for output
   PIE1bits.TX1IE = 1; 		// Disable transmit interrupt
   PIR1bits.TX1IF = 0;		
   RCSTA1bits.SPEN = 1; 	// Enable USART port
   RCSTA1bits.CREN = 1;
   RCSTA1bits.RX9 = 0;
   
   RC1IE = 1;
   RCIE = 1; 
}

int obtenir_valor_an0() {
   ADCON0bits.CHS = 0;
   ADCON0bits.GO = 1;
   while(ADCON0bits.GO);
   return ADRESH*256+ADRESL; //los dos bits de menor peso de ADRESH + los bits de ADRESL
}

int obtenir_valor_an1() {
   ADCON0bits.CHS = 1;
   ADCON0bits.GO = 1;
   while(ADCON0bits.GO);
   return ADRESH*256+ADRESL; //los dos bits de menor peso de ADRESH + los bits de ADRESL
}

void splash() {
   int pos_splash = 0;
   for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 128; ++j) {
	 int draw = bitmap[pos_splash];
	 writeByte(i,j,draw);
	 ++pos_splash;
      }
   }
}

void noms(){
   for(int i = 0; i < 8; ++i){
      if(i > 0) clearGLCD(i-1,i-1,0,127);
      writeTxt(i, 7, s1);
      __delay_ms(200);
   }
   for(int i = 0; i < 6; ++i){
      if(i > 0) clearGLCD(i-1,i-1,0,127);
      writeTxt(i, 7, s2);
      __delay_ms(200);
   }
   for(int i = 0; i < 4; ++i){
      if(i > 0) clearGLCD(i-1,i-1,0,127);
      writeTxt(i, 7, s3);
      __delay_ms(200);
   } 
}

void generar_murs(int n) {
   int pos = murs[n].espai+murs[n].y+1;
   for (int j = 1; j <= murs[n].y; ++j) {
      writeTxt(j,murs[n].x,"@ ");
   }
   for (int i = murs[n].y+1; i < pos; ++i) putchGLCD(i,murs[n].x,' ');
   for (int k = pos; k <= 6; ++k) {
      writeTxt(k,murs[n].x,"@ ");
   }
}

void you_win() {

   for(int i = 0; i < 4; ++i){
      if(i > 0) clearGLCD(i-1,i-1,0,127);
      writeTxt(i, 8, s5);
      __delay_ms(200);
   }
   over = 2;
}

void game_over(){
   for(int i = 0; i < 4; ++i){
      if(i > 0) clearGLCD(i-1,i-1,0,127);
      writeTxt(i, 9, s6);
      __delay_ms(200);
   }
   over = 2;
}

void putc_usart1 (char xc){
   while (!PIR1bits.TX1IF);
   TXREG1 = xc;
};

void puts_usart1(char * cptr){
   while(*cptr != '\n' && *cptr != '\0') {
      putc_usart1 (*cptr++);
   }
   putc_usart1('\n');
};

void Barra_horizontal() {
   for (int i = as*1.67; i <= seg*1.67;++i) {
      SetDot(59,i+11);
      SetDot(60,i+11);
      SetDot(62,i+11);
      SetDot(63,i+11);
   }
}

void interrupt high_RSI(void) {
   if (TMR0IE && TMR0IF) {
      as = seg;
      ++seg;
      Barra_horizontal();
      if (seg == 60 && lives > 0) {en_joc=0; over = 1;}
      TMR0IF = 0;
   }
   if (!premut && RC1IE && RC1IF) {
      char c = RCREG1;
      if (c == 'w') {
	 if (y > 1) {
	    --y;
	    putchGLCD (y+1,x,' ');
	    puts_usart1("Up\r");
	 }
      }
      else if (c == 'a') {
	 if (x > 1) {
	    --x;
	    putchGLCD (y,x+1,' ');
	    puts_usart1("Left\r");
	 }
      }
      else if (c == 's') {
	 if (y<6) {
	    ++y;
	    putchGLCD (y-1,x,' ');
	    puts_usart1("Down\r");
	 }
      }
      else if (c == 'd') {
	 if (x < 22) {
	    ++x;
	    putchGLCD (y,x-1,' ');
	    puts_usart1("Right\r");
	 }
      }
      premut = 1;
   }
}

// Config. per llegir en sèrie
void configLS(){
   OSCCONbits.IRCF = 0b101;
   TRISCbits.TRISC7 = 1;
   TRISCbits.TRISC6 = 1;
   SPBRG1 = 16; 
   SPBRGH1 = 0;
   TXSTA1bits.BRGH = 1;
   BAUDCON1bits.BRG16 = 1;
   RCSTA1bits.SPEN = 1;
   RCSTA1bits.CREN1 = 1;
};

// Configura AD
void configAD() {
   ADCON1bits.TRIGSEL = 0; 	// Selects CCP5
   ADCON1bits.PVCFG = 0b00; 	// +VREF -> VDD
   ADCON1bits.NVCFG = 0b00; 	// -VREF -> VSS
   ADCON2bits.ADFM = 1;    	// Right justified
   ADCON2bits.ACQT= 0b101 ; 	// 12 TAD
   ADCON2bits.ADCS = 0b001; 	// Fosc/8
   ADCON0bits.ADON = 1;         // Enable ADC
}

void configTimers() {
   T08BIT = 0;	        	// Timer0 configurat com a timer de 16 bits
   T0CS = 0;			// Seleccionar Fosc/4 -> Instruction Cycle Clock
   PSA = 0;			// Timer0 té prescaler assignat
   T0CONbits.T0PS = 0b101; 	// prescaler a 1:64
   TMR0 = 34285;
   TMR0ON = 1;			// Inicialitzem el timer aturat
   TMR0IF = 0;			// Flag no ha fet overflow
   TMR0IE = 1;			// Enable l'interrupt de TMR0
   
   TRISCbits.RC2 = 0;
   T2CONbits.T2OUTPS = 0b1111;
   T2CONbits.TMR2ON = 1;
   T2CONbits.T2CKPS = 1;
   
   CCPTMRS0bits.C1TSEL = 0b00;
   CCP1CONbits.DC1B = 0b00;
   CCP1CONbits.CCP1M = 0b1100;
   
   PR2 = 0;
   CCPR1L = 0b01111101;
   TMR2IF = 0;
   TMR2IE = 1;
}



void configMurs() {
   for (int i = 0; i < 6; ++i) {
      murs[i].x = 20;
      murs[i].y = rand()%3+1;
      murs[i].espai = rand()%2+1;
      murs[i].activat = false;
   }
   murs[0].activat = true;
   dist_murs = rand()%10+6;
   ultim = 0;
}

void configDiners() {
   diners.x = 20;
   diners.y = rand()%6+1;
   diners.activat = false;
   diners.tipus = rand()%2;
   a=0;
}

void configFantasma() {
   fantasma.x = 20;
   fantasma.y = 1;
   fantasma.sentit = 0;
   fantasma.activat = false;
}

void Cors(){
   if (lives == 3) {
      putchGLCD(0, 0, cor);
      putchGLCD(0, 2, cor);
      putchGLCD(0, 4, cor);
   }
   else if (lives == 2){
      putchGLCD(0, 0, cor);
      putchGLCD(0, 2, cor);
      putchGLCD(0, 4, ' ');
   }
   else if(lives == 1){
      putchGLCD(0, 0, cor);
      putchGLCD(0, 2, ' ');
      putchGLCD(0, 4, ' ');
   } 
}

void perd_vida() {
   --lives;
   if (lives == 0) {en_joc = 0; over=1;}
   else {
      PR2 = 125;
      putchGLCD(y,x,' ');
      x=0;
      y=3;
      invencible = true;
      ant_seg = seg;
      Cors();
   
   }
}

void TaulaResultats(){
   Cors();
   writeTxt(0, 14, s4);
   writeNum(0, 20, punts);
   writeTxt(0, 6, "MEDIUM");
   putchGLCD(0,24,'|');
}

void Linies(){
      for (int i = 11; i <= 111; ++i){ 
	 SetDot(61, i);
      }
      for (int i = 8; i <= 60; ++i){ 
	 SetDot(i, 122);
      }
}

void Barra_vertical() {
   for (int i = punts*0.104; i >= ant_punts*0.104;--i) {
      SetDot(60-i,120);
      SetDot(60-i,121);
      SetDot(60-i,123);
      SetDot(60-i,124);
   }
}

void start() {
   while (!K) {
      if (!written) {
	 writeTxt(6, 1, TextInici);
      }
      written = 1;
   }
   flanc6=1;
   clearGLCD(6,6,0,120);
   putchGLCD(0,24,'}');
   en_joc = 1;
 
}

void pinta_ninot() {
   an0 = obtenir_valor_an0();
   an1 = obtenir_valor_an1();
   if (!invencible) putchGLCD(y,x,ninot);
   else {
      if (!b) {
	 putchGLCD(y,x,' ');
	 b = 1;
      }
      else {
	 putchGLCD(y,x,ninot);
	 b = 0;
      }
   }
   if ((A && !flanc0)||an1 > 511) {
     putchGLCD(y,x,' ');
     if (y > 1) --y;
     putchGLCD(y,x,ninot);
     flanc0 = 1;        
   } 
   if (!A && flanc0) flanc0 = 0;
   if ((C && !flanc1)||an1 < 511) { 
     putchGLCD(y,x,' ');
     if (y < 6) ++y;
     putchGLCD(y,x,ninot);
     flanc1 = 1;        
   } 
   if (!C && flanc1) flanc1 = 0;
   if ((D && !flanc2) || an0 > 511) { 
     putchGLCD(y,x,' ');
     if (x > 0) --x;
     putchGLCD(y,x,ninot);
     flanc2 = 1;        
   } 
   if (!D && flanc2) flanc2 = 0;
   if ((B && !flanc3) || an0 < 511) {
     putchGLCD(y,x,' ');
     if (x < 22) ++x;
     putchGLCD(y,x,ninot);
     flanc3 = 1;        
   } 
   if (!B && flanc3) flanc3 = 0;
   premut = 0;
}

void pinta_murs() {
   for (int i = 0; i < 6; ++i) {
	 if (murs[i].activat) {
	    if (murs[i].x != -1) {
	       generar_murs(i);
	       --murs[i].x;
	    }
	    else {
	       murs[i].activat = false;
	       for (int i = 1; i <= 6;++i) {
		  putchGLCD(i,0,' ');
	       }
	    }
	 }
      }
   if (murs[ultim].x == 19) ocupat = true;
   if (murs[ultim].x != 19 && ocupat) ocupat = false;
   
   if (murs[ultim].x == dist_murs && ultim<5) {
	 ++ultim;
	 murs[ultim].activat = true;
	 dist_murs = rand()%10+6;
	 }
   if(murs[5].x==-1) {
      configMurs();
      for (int i = 1; i <= 6;++i) putchGLCD(i,0,' ');
   }
}

void pinta_diners() {
   if (rd_diners == rand()%4 && !ocupat) diners.activat = true;
   if (diners.activat) {
      if (diners.x != -1) {
	 if (murs[ultim].x == diners.x) a = 1;
	 if (diners.tipus==0) {   
	    if (a==0) writeTxt(diners.y,diners.x,"$ ");
	    else putchGLCD(diners.y,diners.x,'$');
	 }
	 else {
	    if (a==0) writeTxt(diners.y,diners.x,"> ");
	    else putchGLCD(diners.y,diners.x,'>');
	 }
	 --diners.x;
      }
      else {
	 putchGLCD(diners.y,0,' ');
	 configDiners();
      } 
   }
}

void pinta_fantasma() {
   if (murs[2].activat && murs[2].x == 18) fantasma.activat = true;
   if(fantasma.activat) {
      if (fantasma.x != -1) {
	 putchGLCD(fantasma.y,fantasma.x,';');
	 if (fantasma.sentit==0) {
	    putchGLCD(2,fantasma.x+1,' ');
	    if (fantasma.y>1) putchGLCD(fantasma.y-1,fantasma.x+1,' ');
	    ++fantasma.y;
	 }
	 else {
	    putchGLCD(5,fantasma.x+1,' ');
	    if (fantasma.y<6) putchGLCD(fantasma.y+1,fantasma.x+1,' ');
	    --fantasma.y;
	 }
	 if (fantasma.y==6) {
	    
	    fantasma.sentit = 1;
	    }
	 if (fantasma.y==1) {
	    
	    fantasma.sentit = 0;
	    }
	 --fantasma.x;
      }
      else {
	 fantasma.activat = false;
	 if (fantasma.sentit) putchGLCD(fantasma.y+1,0,' ');
	 else putchGLCD(fantasma.y-1,0,' ');
	 configFantasma();
      }
   }
}

void collisio() {
   if (!invencible) {
      for (int i = 0; i <= ultim; ++i) {
	 if (murs[i].activat) {
	    if (x == murs[i].x+1 && (y <= murs[i].y || y > murs[i].y+murs[i].espai)) perd_vida();
	 }
      }   
      if (x == fantasma.x+1 && y == fantasma.y && fantasma.activat) perd_vida();
   }
   else {
      if (seg - ant_seg == 2) invencible = false;
   }
   if (x == diners.x+1 && y == diners.y) {
      if (diners.tipus == 0) {punts += 50; PR2 = 135;}
      else {punts += 75; PR2 = 150;}
      if (punts >= 500) {en_joc = 0; over = 1;}
      else {
	 writeNum(0, 20, punts);
	 Barra_vertical();
	 putchGLCD(diners.y,diners.x+1,' ');
	 configDiners();
      }
      
   }
}

void dificultat() {
   if (E && flanc4==0 && dif < 3) {
      ++dif;
      if (dif == 3) writeTxt(0,6,"INSANE");
      else if (dif == 2) writeTxt(0,6,"HARD  ");
      else if (dif == 1) writeTxt(0,6,"MEDIUM");
      flanc4 = 1;
   }
   if (!E && flanc4==1) flanc4 = 0;
   
   if (F && flanc5==0 && dif > 0) {
      --dif;
      if (dif == 2) writeTxt(0,6,"HARD  ");
      else if (dif == 1) writeTxt(0,6,"MEDIUM");
      else if (dif == 0) writeTxt(0,6,"EASY  ");
      flanc5 = 1;
   }
   if (!F && flanc5==1) flanc5 = 0;
   if (dif == 3) __delay_ms(200);
   else if (dif == 2) __delay_ms(300);
   else if (dif == 1) __delay_ms(400);
   else if (dif == 0) __delay_ms(500);
   
   if (K == 1 && flanc6 == 0) {
      en_joc = 0;
      T0IE = 0;
      putchGLCD(0,24,'|');
      flanc6 = 1;
   }
   if (K == 0 && flanc6 == 1) flanc6 = 0;
}


void main(void){
   configPIC();
   configGLCD();
   configLS();
   configAD();
   noms();
   configMurs();
   configDiners();
   configFantasma();
   puts_usart1("Bru Bernadi\r");
   puts_usart1("Feiyang Wang\r");
   puts_usart1("L7 Projecte\r");
   __delay_ms(1000);
   splash();
   __delay_ms(1000);
   clearGLCD(0,7,0,127);
   TaulaResultats();
   Linies();
   __delay_ms(1000);
   start();
   configTimers();
   
   while(1){
      if (en_joc == 1) {
	 PR2 = 0;
	 pinta_murs();
	 pinta_diners();
	 pinta_fantasma();
	 pinta_ninot();
	 collisio();
	 dificultat();
      }
      else {
	 if (over == 0) {
	    if (K==1 && flanc6 == 0) {
	       en_joc = 1;
	       T0IE=1;
	       putchGLCD(0,24,'}');
	       flanc6 = 1;
	    }
	    if (K == 0 && flanc6 == 1) flanc6 = 0;
	 }
	 else if (over == 1){
	    T0IE = 0;
	    clearGLCD(0,7,0,127);
	    if (lives > 0 || punts >= 500) you_win();
	    else game_over();
	 }
      }
   }
}

	
   

