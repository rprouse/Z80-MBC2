// ***********************************************************************************************
// Blink_MBC2.c - Blink test with IRQ disabled
//
//
// REQUIRED: IOS S220718-R290823 (and following revisions unless stated otherwise)
//
// REQUIRED: S290923_Z80-MBC2.rel compiled with: sdcc -c -mz80 S290923_Z80-MBC2.c
//           (IRQ support disabled)
//
//
// -----------------------------------------------------------------------------------------------
//
// HOW TO COMPILE: 
//
// Use the SDCC compiler (https://sdcc.sourceforge.net/):
// 
//  sdcc -mz80 Blink_MBC2.c S290923_Z80-MBC2.rel -o out.hex
//
// This will create the output executable file out.hex (Intel-Hex format).
// Please note that this file need further processing before using iLoad to load it, to sort it in 
// ascending address order.
// To do this use the srec_cat utility (https://srecord.sourceforge.net/) with the command:
//
//  srec_cat -disable-sequence-warnings out.hex -Intel -o load.hex -Intel
//
// This will create the load.hex file (Intel-Hex) ready to be loaded and executed on the Z80-MBC2 
// using iLoad.
//
// -----------------------------------------------------------------------------------------------
//
// Tested with SDCC 4.0.2
//
// ***********************************************************************************************
//
// Z80-MBC2 - IOS S220718-R290823
//
// IOS Ports definitions:
//
// EXECUTE_READ_OPCODE      0x00  (read)            IOS Execute Opcode read/write port
// SERIALPORT_RX            0x01  (read)            IOS Serial Rx read only port
// EXECUTE_WRITE_OPCODE     0x00  (write)           IOS Execute Opcode read/write port
// STORE_OPCODE             0x01  (write)           IOS Store Opcode write only port
//
//
// Currently defined IOS Opcodes for I/O write operations:
//
//   Opcode     Name            Exchanged bytes
// -------------------------------------------------
// Opcode 0x00  USER LED        1
// Opcode 0x01  SERIAL TX       1
// Opcode 0x03  GPIOA Write     1
// Opcode 0x04  GPIOB Write     1
// Opcode 0x05  IODIRA Write    1
// Opcode 0x06  IODIRB Write    1
// Opcode 0x07  GPPUA Write     1
// Opcode 0x08  GPPUB Write     1
// Opcode 0x09  SELDISK         1
// Opcode 0x0A  SELTRACK        2
// Opcode 0x0B  SELSECT         1  
// Opcode 0x0C  WRITESECT       512
// Opcode 0x0D  SETBANK         1
// Opcode 0x0E  SETIRQ          1
// Opcode 0x0F  SETTICK         1
// Opcode 0x10  SETOPT          1
// Opcode 0x11  SETSPP          1
// Opcode 0x12  WRSPP           1
// Opcode 0xFF  No operation    1
//
//
// Currently defined IOS Opcodes for I/O read operations:
//
//   Opcode     Name            Exchanged bytes
// -------------------------------------------------
// Opcode 0x80  USER KEY        1
// Opcode 0x81  GPIOA Read      1
// Opcode 0x82  GPIOB Read      1
// Opcode 0x83  SYSFLAGS        1
// Opcode 0x84  DATETIME        7
// Opcode 0x85  ERRDISK         1
// Opcode 0x86  READSECT        512
// Opcode 0x87  SDMOUNT         1
// Opcode 0x88  ATXBUFF         1
// Opcode 0x89  SYSIRQ          1
// Opcode 0x8A  GETSPP          1
// Opcode 0xFF  No operation    1
//
// ***********************************************************************************************

#include <stdio.h>

// Z80-MBC2 IOS Opcodes definitions
#define         USRLED      0x00
#define         SERIALTX    0x01
#define         GPIOAW      0x03
#define         GPIOBW      0x04
#define         IODIRAW     0x05
#define         IODIRBW     0x06
#define         GPPUAW      0x07
#define         GPPUBW      0x08
#define         SELDISK     0x09
#define         SELTRACK    0x0A
#define         SELSECT     0x0B
#define         WRITESECT   0x0C
#define         SETBANK     0x0D
#define         SETIRQ      0x0E
#define         SETTICK     0x0F
#define         SETOPT      0x10
#define         SETSPP      0x11
#define         WRSPP       0x12
#define         USRKEY      0x80
#define         GPIOAR      0x81
#define         GPIOBR      0x82
#define         SYSFLAGS    0x83
#define         DATETIME    0x84
#define         ERRDISK     0x85
#define         READSECT    0x86
#define         SDMOUNT     0x87
#define         ATXBUFF     0x88
#define         SYSIRQ      0x89 
#define         GETSPP      0x8A

// Z80-MBC2 IOS I/O ports definitions
__sfr __at 0x00 EXEC_OPCODE;                        // IOS Execute Opcode read/write port
__sfr __at 0x01 STORE_OPCODE;                       // IOS Store Opcode write only port
__sfr __at 0x01 SER_RX;                             // IOS Serial Rx read only port

char userKey();
void userLed(char c);
void delay(unsigned short d);

char    c, message[] = "Hello Folk!";
int     i;

char userKey()
{
    STORE_OPCODE = USRKEY;
    c = EXEC_OPCODE;
    return c;
}

void userLed(char c)
{
    STORE_OPCODE = USRLED;
    EXEC_OPCODE = c;
}

void delay(unsigned short d)
{
    unsigned long long i, j;
    j= (unsigned long long) d * 1000;
    for (i=0; i < j; i++ );
}
     
void main(void) { 
    printf("\n");
    printf(message);
    printf("\n");
    printf("\nJust some counts...\n\n");
    for (i=0; i<11; i++) printf("I=%d\n", i);

    printf("\nPress USER key to continue...");
    while (userKey() == 0);
    
    printf("\nPress 'b' to start blinking >");
    do
        c = getchar();
    while ((c != 'b') && (c != 'B'));
    putchar('b');
    
    printf("\n\nAt last start blinking...");
    delay(10);
    
    printf ("%c[2J",27);                            // Clear screen
    printf ("%c[?25l",27);                          // Cursor invisible
    while (1)
    {
        userLed(1);                                 // User led on
        printf("\r");
        printf ("%c[15;30H",27);
        printf ("%c[7m * * LED on  * * ",27);       // Print reverse
        delay(12);
        printf ("%c[m",27);                         // Reset all attributes
        printf ("%c[15;30H",27);
        printf (" * * LED off * * ");
        userLed(0);                                 // User led off
        delay(12);
    }
} 