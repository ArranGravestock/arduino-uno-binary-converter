////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CI209 Lab Activity 2 - 2016-2017                           (c) Alan Thomas 2016
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Function prototypes (signatures)
int readInt(void);                                            // (from program 1) reads an integer from the keyboard (serial port)
String decimalToBinary(String message ,byte n);               // (from program 2) converts an 8 bit number to binary
extern void trace(word BR, word AD, byte n) asm ("TRACE");    // (from program 3) dump the register contents for trace purposes
int signedMult(char multiplier, char multiplicand);           // Assembly language implementation of 4 register multiply alg

String buildStr;    //store the string that is built
String buildBin;    //store the binary string that is built

/*
 * Reads the input until a terminator has been declared (\n) which
 * then stores the read input into buildStr. A new char array is created
 * using toCharArray and defined as the length of the string to create
 * a dynamic size array. A char array is used in this case to allow the
 * function atoi (ascii to integer) to be called on this, returning 
 * an integer value only. buildStr is then reset to remove the previous
 * stored value.
 * @param void
 * @return v
 */
int readInt(void) {
  while (!Serial.available()) {}
  
  if (Serial.available() > 0) {
    char charBuf[0];
    
    buildStr = Serial.readStringUntil('\n');
    buildStr.toCharArray(charBuf, buildStr.length()+1);
    
    int v = atoi(charBuf);
    
    buildStr = "";

    return (v);
  }
}

/*
 * Resets the binary string to null. Uses a for loop which iterates from
 * 7 to 0, starting from the MSB and reading the value using bitRead.
 * Every time the bitRead function is called, the bit is shifted and read,
 * storing in the buildBin string variable.
 * @param message
 * @param n
 * @return message + buildBin
 */
String decimalToBinary(String message, byte n) {

  buildBin = "";
  for (int i = 7; i >= 0; i--) {
    buildBin += bitRead(n, i);
  }
  
  return(message + buildBin + " ");
}

/*
 * Uses the function highByte and lowByte to seperate the 16 bits into
 * two seperate 8 bit variables. Stores them in 2 different variables
 * as they are not needed to be stored or used as it is for debugging
 * purposes only (serial prints). Prints to serial monitor in order
 * of formatting so that 4 variables do not need to be stored.
 * @param BR
 * @param AD
 * @param n
 */
void trace(word BR, word AD, byte n) {
  byte hi, lo;
  hi = highByte(BR);
  lo = lowByte(AD);
  
  Serial.print(decimalToBinary("D=", lo));
  
  Serial.print(decimalToBinary("B=", hi));

  hi = highByte(AD);
  lo = lowByte(BR);

  Serial.print(decimalToBinary("A=", hi));
  
  Serial.print(decimalToBinary("R=", lo));

}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print("Input multiplicand : ");
  char multiplicand = Serial.print(decimalToBinary("", readInt()));
  Serial.println(multiplicand);

  Serial.print("Input multiplier : ");
  char multiplier = Serial.print(decimalToBinary("", readInt()));
  Serial.println(multiplier);

  Serial.println(signedMult(multiplier, multiplicand));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program 4 - Signed Multiplication using 4 registers with correction factor
// 
// Input parameters are automatically passed in the following registers:
//    r24 - multiplier          alias (R)
//    r22 - multiplicand        alias (D)
//
// IMPORTANT - use the following machine registers for your other varialbles:
//    r25 - Product high byte   alias (B)
//    r23 - Product low  byte   alias (A)
//    r20 - Interation counter  alias (n)
// 
// The contents of registers r25 and r24 are returned from the function
// You must ensure that you copy the product to these if necessary i.e. B->r25 A->r24
//
// to find out more about C function parameter passing, see https://gcc.gnu.org/wiki/avr-gcc#Register_Layout
//  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int signedMult(char multiplier, char multiplicand)
{
    asm volatile(
"initalise:                       ;                                   \n"
"            ldi r25, 0           ;  load immediate 0 into r25        \n"
"            ldi r23, 0           ;  load immediate 0 into r23        \n"
"            ldi r20, 8           ;  load immediate 8 into r20        \n"
"            call dumpRegs        ;  call the function dumpRegs       \n"
"reiterate:  sbrc r24, 0          ;  skip next instruction if 0       \n"
"            call add             ;  call add routine                 \n"
"            call shift           ;  call the shift routine           \n"
"            dec r20              ;  decrement r20 by 1               \n"
"            cpi r20, 0           ;  copy immediate 0 into r20        \n"
"            brne reiterate       ;  branch if not equal to top       \n"
"            mov 24, r23          ;  move contents r23 into 24        \n"
    );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global inline assembly language subroutines
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

asm volatile(
//-----------------------------------------------------------------------
// Wrapper for the C trace function, which saves and restores
// registers and flags, so we can basically do anything we want in it ;)
//-----------------------------------------------------------------------
"dumpRegs:   push r20           ; save the loop count n               \n"
"            push r22           ; save the multiplicand, Yreg         \n"
"            push r23           ; save the loop count, Nreg           \n"
"            push r24           ; save the product lo byte Pl         \n"
"            push r25           ; save the product hi byte Ph         \n"  
"            in r16, __SREG__   ; save the status register SREG       \n"
"            call TRACE         ; call the C function trace()         \n"
"            out __SREG__, r16  ; restore the status register, SREG   \n"
"            pop r25            ; restore working registers....       \n"
"            pop r24            ; note that these must be popped off  \n"
"            pop r23            ; the stack in reverse order to that  \n"
"            pop r22            ; in which they were pushed           \n"
"            pop r20            ; in which they were pushed           \n"
"            ret                ; return back to multiplication       \n"     

// put your add and shift subroutines here
"add:                           ;                                   \n"
"            adc r22, r25       ; add carry r25 to r22              \n"
"            call dumpRegs      ; call the dumpRegs routine         \n"
"            ret                ; return back                       \n"

"shift:                         ;                                   \n"
"            asr r24            ; shift bit right r24               \n"
"            asr r25            ; shift bit right r25               \n"
"            ror r23            ; rotate right through carry r23    \n"
"            call dumpRegs      ; call the dumpRegs routine         \n"
"            ret                ; return back                       \n"
);
