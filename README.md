# RFM69HCW-Telemetry-868MHz-
This project implements a long-range wireless RTK correction data link using RFM69HCW RF modules and STM32F103C6T6 (Bluepill) microcontrollers.

The system reads RTK correction data (RTCM) from a GPS base station and transmits it wirelessly using the RFM69 radio module. Multiple receiver nodes can receive the correction data and forward it to GPS receivers or flight controllers via UART.

This setup is useful for:-

RTK drone systems

Robotics navigation

Autonomous vehicles
