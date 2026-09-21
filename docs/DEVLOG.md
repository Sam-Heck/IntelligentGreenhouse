### 9/19/2026:
- Update system architecture to prefer an Edge Gateway Architecture pattern with an offline-first design. This prioritizes system functionality locally, while still allowing remote access and control. The switch in design was made to add resilience to the system in case of network outages and to provide a single UI that works both on-premise in the greenhouse and online as a remote interface. 

### 9/21/2026:
- Defined unified flexible data transfer shapes so that additional remote nodes of various functions (ie: soil moisture, heater relay, window servos) can be added without writing custom firmware for each type. This also eliminates adjusting db schema when adding new sensor types. 
- Actuator sensors will store their current and targeted state a seperate table that has 1 record per device to keep hardware boundaries clean
- Write firmware for gateway node with a non blocking loop reading local DHT sensors and an ESP-NOW radio message listener that interrupts and runs callback to format and send recieved data to Node.js ingestion service. 
- Remove code for old architecure and organize repo as a multi-project workspace to improve dev experience.