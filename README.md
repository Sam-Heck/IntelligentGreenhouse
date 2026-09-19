# IntelligentGreenhouse

An edge-native greenhouse monitoring and automation platform engineered for low power consumption, resilient local offline operation, and remote management.

**Live Demo (Coming Soon):** [https://intelligentgreenhouse.com](https://intelligentgreenhouse.com) *(Read-only guest access)*  
**Architecture Spec:** See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for full circuit schematics, protocol trade-offs, and control flow diagrams.

---

## Highlights

- **Ultra-Low-Power Field Telemetry:** Peripheral ESP32 sensor nodes leverage connectionless **ESP-NOW** and deep sleep cycles to maximize battery life in damp soil beds.
- **Offline-First Edge Gateway:** Raspberry Pi and a dedicated USB-connected ESP32 gateway handle local sensor ingestion, database persistence, and automation routines with zero dependence on external cloud connectivity.
- **Bi-Directional State & Control:** Real-time push updates via WebSockets paired with an authenticated command-and-acknowledgment loop for actuators (relays/servos).
- **Secure Remote Access:** Exposes an isolated, read-only guest dashboard over **Cloudflare Tunnel**, protecting physical controls behind session-based admin authentication without opening inbound router ports.

---

## System Architecture

```text
[ Battery Sensor Nodes ]  (ESP32 + Capacitive Soil Probes)
           │
           │  ESP-NOW (2.4 GHz packet, <150ms wake)
           ▼
[ Gateway Bridge Node ]   (ESP32 + BME280 Ambient Sensor)
           │
           │  Serial over USB (JSON framing)
           ▼
[ Raspberry Pi Edge Host ]
   ├── Ingestion Service (Node.js + WebSockets)
   ├── Local Database    (SQLite via Prisma ORM)
   └── Web Dashboard     (Next.js + Tailwind CSS)
           ▲
           │  Cloudflare Tunnel (TLS)
     [ Remote Web / Phone ]
```

*For complete message sequence diagrams and failure handling, review the [Actuator Control & State Acknowledgment Flow in docs/ARCHITECTURE.md](docs/ARCHITECTURE.md#actuator-control--state-acknowledgment-flow).*

---

## Tech Stack

| Domain | Technology |
| :--- | :--- |
| **Field Sensing** | ESP32-C3, Capacitive Soil Moisture, 18650 Li-ion |
| **Wireless Protocol** | ESP-NOW (Node-to-Gateway peer-to-peer) |
| **Edge Hardware** | Raspberry Pi 4/5 (4GB), Gateway ESP32, BME280 |
| **Ingestion & Server** | Node.js, WebSockets (`ws`), Prisma ORM |
| **Frontend** | Next.js (App Router), Tailwind CSS |
| **Storage** | SQLite |
| **Networking & Security** | Cloudflare Tunnel, RBAC (Public Guest vs. Admin) |
| **Firmware Tooling** | PlatformIO |

---

## Project Structure

```text
├── apps/
│   ├── frontend/        # Next.js real-time web dashboard
│   └── server/          # Node.js serial ingestion, WebSocket emitter, & Prisma models
├── firmware/
│   ├── gateway/         # ESP-NOW receiver & USB serial bridge sketch
│   └── sensor-node/     # Deep-sleep battery sensor sketch
├── docs/
│   ├── ARCHITECTURE.md  # Deep dive into hardware, state flow, and power budget
│   ├── ROADMAP.md       # Phased engineering milestones
│   └── devlog/          # Debugging notes and hardware bench tests
└── docker/
    └── docker-compose.yml
```

---

## Hardware Requirements (Bench Prototype)

- **1x Raspberry Pi 4 or 5 (4GB)** running Raspberry Pi OS.
- **1x ESP32 Dev Board** (Gateway node, plugged via USB into the Pi).
- **1x or more ESP32-C3 / DevKit boards** (Battery field sensor nodes).
- **Sensors:** BME280 (I2C ambient temp/humidity) and analog capacitive soil moisture probes.
- **Power:** 18650 lithium cells (or 3x AA battery packs) with high-value resistor dividers (e.g., 2x 100kΩ) for ADC battery monitoring.

---

## Security & Access Model

- **Public / Guest:** Default unauthenticated state. Visitors can inspect live sensor telemetry, ambient metrics, and historical graphs in real time. Actuators and calibration inputs are disabled.
- **Admin:** Authenticated via secure session cookie. Unlocks physical control triggers (relays, exhaust fans, servos) and threshold overrides.

---

## Contact

- **Author:** Samuel Heck
- **Email:** [heck.sam@gmail.com](mailto:heck.sam@gmail.com)
- **GitHub:** [@Sam-Heck](https://github.com/Sam-Heck)
- **LinkedIn:** [linkedin.com/in/samuelheck](https://linkedin.com/in/samuelheck)