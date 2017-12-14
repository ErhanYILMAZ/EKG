/*
Author: Erhan YILMAZ
Date: 14.12.17
Description: MSP430 software to send ECG signal to computer via uart to monitoring

*/

#include <msp430.h>		// MSP430 baþlýk dosyasý

unsigned short wADCHam; 
unsigned int i,j;
void main(void) {

  WDTCTL  = WDTPW | WDTHOLD;	// Watchdog timeri durdur.
  DCOCTL = 0;					// Dahili osilatör ayarlarýný en düþük yap
  BCSCTL1 = CALBC1_1MHZ;		// Dahili osilatörü 1MHz'e ayarla
  DCOCTL = CALDCO_1MHZ;			// Dahili osilatörü 1MHz'e ayarla


  P1SEL |= BIT1 + BIT2 ;			// P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 + BIT2 ;		// P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;			// UART Ayarlarý, Saat kaynaðý SMCLK
  UCA0BR0 = 104;				// 1MHz 9600 bud ayarý
  UCA0BR1 = 0;					// 1MHz 9600 baud ayarý
  UCA0MCTL = UCBRS0;			// UART Baud hassas ayar
  UCA0CTL1 &= ~UCSWRST;			// USCI birimini hazýrla


    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC ayarlarý, kesme aktif
    ADC10CTL1 = INCH_0;					// A0 giriþini seç
    ADC10AE0 |= 0x1;					// PA.0 ADC özelliðini aktif et
    _BIS_SR(GIE);						// Kesmeleri aç
    while(1){							// Sonsuz döngü
    ADC10CTL0 |= ENC + ADC10SC;         // AD çevrimi baþlat
    _BIS_SR(LPM0_bits);					// Uykuya gir
    wADCHam = ADC10MEM>>3;					// Çevrim sonucunu kaydet.
    while (!(IFG2&UCA0TXIFG));		// USCI_A0 TX tamaponu hazýr olana kadar bekle
    UCA0TXBUF = wADCHam;			// Alýnan karakteri geri gönder
    while (!(IFG2&UCA0TXIFG));		// USCI_A0 TX tamaponu hazýr olana kadar bekle
    for(i=0;i<30;i++)
    for(j=0;j<100;j++);


    }
}


// ADC10 kesme vektörü
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Ýþlemciyi uykudan uyandýr.
}
