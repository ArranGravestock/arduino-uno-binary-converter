String buildStr;    //store the string that is built
String buildBin;    //store the binary string that is built

void setup() {
  Serial.begin(9600);
}

/*
 * Read the integer that is input in a range of -128 to 127. Print the
 * returned value from the function decimalToBinary printing "bin=", to
 * get the correct formating. The stored integer in v is passed in to
 * decimalToBinary to get the most recent input value in the serial 
 * montior. The integer value v is also printed.
 */
void loop() {
    Serial.print("Input a number in range -128 to 127 : ");
    int t = readInt();
    
    Serial.print(decimalToBinary("bin=", t) + " ");
    Serial.println(t);
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
  
  return(message + buildBin);
}
