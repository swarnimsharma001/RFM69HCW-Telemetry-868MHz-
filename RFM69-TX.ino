#include <RFM69.h>
#include <SPI.h>

// Define the size of the buffer
const int BUFFER_SIZE = 1024; 

// Ring buffer to store incoming data
char ringBuffer[BUFFER_SIZE]; 

// Temporary buffer to store data for transmission
char transmitBuffer[57]; 
int transmitIndex = 0; // Index to track the number of bytes in the transmit buffer

// Index to keep track of where to write new data
int writeIndex = 0;    

// Index to keep track of where to read data from
int readIndex = 0;   

// Define the pins for the GPS
#define RX_PIN PA10  // GPS RX pin (STM32F103 RX pin)
#define TX_PIN PA9   // GPS TX pin (STM32F103 TX pin)
#define BAUD_RATE 9600

// Define the pins for the RFM69
#define RFM69_CS PA4   // NSS pin
#define RFM69_RST PB11  // RESET pin
#define RFM69_IRQ PC15  // IRQ pin

// Built-in LED pin
#define LED_PIN PC13

// RFM69 settings
#define NETWORKID 100
#define MYNODEID 1
#define TONODEID 2
#define FREQUENCY RF69_868MHZ

RFM69 radio(RFM69_CS, RFM69_IRQ, RFM69_RST);

void setup() 
{
  // Initialize Serial for debugging and GPS communication
  Serial.begin(BAUD_RATE);
  Serial1.begin(BAUD_RATE); // Use Serial1 for GPS communication (PA9 and PA10 on STM32)

  // Initialize SPI and RFM69
  SPI.begin();
  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower();
  radio.encrypt(nullptr); // No encryption

  // Initialize built-in LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Turn off the LED (LED is active low on Bluepill)

  Serial.write("GPS and RFM69 initialization complete.");
}

void loop() 
{
  while (Serial1.available())  
  {
    // Read one character from Serial1 (GPS)
    char incomingChar = Serial1.read();
    
    // Store the character in the ring buffer
    ringBuffer[writeIndex] = incomingChar;
   
    // Update the write index to point to the next position in the buffer
    writeIndex = (writeIndex + 1) % BUFFER_SIZE;
  }

  // Read from the buffer, and send it via RFM69
  while (readIndex != writeIndex) 
  {
    // Read one character from the ring buffer
    char outgoingChar = ringBuffer[readIndex];
    Serial.print(outgoingChar); 

    // Store the character in the transmit buffer
    transmitBuffer[transmitIndex++] = outgoingChar;

    // If transmit buffer is full, send the data
    if (transmitIndex >= 57) 
    { 
      // Turn on the LED (active low, so LOW turns it on)
      digitalWrite(LED_PIN, LOW);
      
      // Transmit data
      radio.send(TONODEID, transmitBuffer, 57);
      
      // Turn off the LED after transmission
      digitalWrite(LED_PIN, HIGH);

      transmitIndex = 0; // Reset the transmit buffer index after sending
    }

    // Update the read index to point to the next position in the buffer
    readIndex = (readIndex + 1) % BUFFER_SIZE;
  } 
}