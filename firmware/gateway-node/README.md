# Hardware & Pinout (Gateway Node)

### Required Components
- 1x ESP32 Dev Module (`esp32doit-devkit-v1`)
- 2x DHT22 (AM2302) Temperature & Humidity Sensors
  - *Note: If using bare 4-pin sensors, add a 4.7kΩ–10kΩ pull-up resistor between VCC and DATA. Pre-mounted 3-pin modules have this onboard.*

### Pin Mapping
| Component | Pin / Bus | ESP32 GPIO | Notes |
| :--- | :--- | :--- | :--- |
| **Internal DHT22** | DATA | `GPIO 4` | Inside greenhouse |
| **External DHT22** | DATA | `GPIO 5` | Ambient outdoor reading |
| **Both Sensors** | VCC | `3.3V` | Max current draw ~4mA total |
| **Both Sensors** | GND | `GND` | Common ground |

### Host Interface
- **Connection:** Micro-USB / USB-C to Raspberry Pi host
- **Baud Rate:** `115200`
- **Output:** Single-line newline-delimited JSON (`\n`)