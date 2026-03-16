#include <RFM69.h>
#include <SPI.h>

// UART1 pads for GPS/FC
#define RX_PIN PA10     
#define TX_PIN PA9      
#define BAUD_RATE 9600

// Define the pins for the RFM69
#define RFM69_CS PA4   // NSS pin
#define RFM69_RST PB11  // RESET pin
#define RFM69_IRQ PB10  // IRQ pin

// Built-in LED pin
#define LED_PIN PC13

// RFM69 settings
#define NETWORKID 100
#define MYNODEID 2       // Unique ID for this receiver node (Change for each receiver)
#define FREQUENCY RF69_868MHZ

RFM69 radio(RFM69_CS, RFM69_IRQ, RFM69_RST);

void setup() 
{
  Serial.begin(BAUD_RATE);        // Initialize Serial for debugging
  Serial1.begin(BAUD_RATE);       // Initialize UART1 for (PA9, PA10)
  
  // Initialize SPI and RFM69
  SPI.begin();                    
  radio.initialize(FREQUENCY, MYNODEID, NETWORKID); // Initialize RFM69 radio
  radio.setHighPower();           // Enable high power mode for the RFM69
  radio.encrypt(nullptr);         // Disable encryption

  // Initialize built-in LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Turn off the LED (LED is active low on Bluepill)

  Serial.println("Receiver ready.");
}

void loop() 
{
  if (radio.receiveDone())        // Check if data has been received
  { 
    // Turn on the LED (active low, so LOW turns it on)
    digitalWrite(LED_PIN, LOW);

    // Print received data to Serial and write it to UART1
    for (uint8_t i = 0; i < radio.DATALEN; i++) 
    {
      Serial.print((char)radio.DATA[i]);   // Print received data to Serial for debugging
      Serial1.write((char)radio.DATA[i]);  // Write the received data to UART1
    }

    // Acknowledge receipt of data if acknowledgment is enabled
    if (radio.ACKRequested())
    {
      radio.sendACK();    // Send acknowledgment back to the transmitter
      Serial.println("ACK sent.");
    }

    // Turn off the LED after data is received
    digitalWrite(LED_PIN, HIGH);
  }
}