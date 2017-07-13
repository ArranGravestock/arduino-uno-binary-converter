String buildStr;    //store the string that is built
String buildBin;    //store the binary string that is built
int v;              //store the value that is passed from readInt to atoi()

void setup() {
  Serial.begin(9600);
}

/*
 * Ask for inputs in serial and call the function readInt() to store it,
 * print the value of v and store in appropriate variables BR &
 * AD. Call the trace function passing these values which cause the 
 * debug to print to the serial monitor.
 */
void loop() {
    Serial.print("Input BR in range -32768 to 32767 : ");
    readInt();
    Serial.println(v);
    word BR = v;

    Serial.print("Input AD in range -32768 to 32767 : ");
    readInt();
    Serial.println(v);
    word AD = v;

    Serial.print("Input n in range 1 to 8 : ");
    readInt();
    Serial.println(v);

    Serial.println();
    
    trace(BR, AD, v);

    Serial.println();
    Serial.println();
    
  }

/*
 * Reads the input until a terminator has been declared (\n) which
 * then stores the read input into buildStr. A new char array is created
 * using toCharArray and defined as the length of the string to create
 * a dynamic size array. A char array is used in this case to allow the
 * function atoi (ascii to integer) to be called on this, returning 
 * an integer value only. buildStr is then reset to remove the previous
 * stored value.
 * @param void
 */
int readInt(void) {
  while (!Serial.available()) {}
  
  if (Serial.available() > 0) {
    char charBuf[0];
    
    buildStr = Serial.readStringUntil('\n');
    buildStr.toCharArray(charBuf, buildStr.length()+1);
    
    v = atoi(charBuf);
    
    buildStr = "";
    
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

  Serial.print("n=");
  Serial.print(n);
  Serial.print(" ");
  
  Serial.print(decimalToBinary("D=", lo));
  
  Serial.print(decimalToBinary("B=", hi));

  hi = highByte(AD);
  lo = lowByte(BR);

  Serial.print(decimalToBinary("A=", hi));
  
  Serial.print(decimalToBinary("R=", lo));

}
