String buildStr; //Store the string that is built

void setup() {
  Serial.begin(9600);
}

/*  
 * Loop is used to ask for an input, printing to the serial monitor
 * Calls the readInt() class to store the value that is input and 
 * print to the serial monitor
 */
void loop() {
  Serial.print("Please input an integer : ");
  Serial.println(readInt());
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
    
    v = atoi(charBuf);
    
    buildStr = "";

    return (v);
  }
}
