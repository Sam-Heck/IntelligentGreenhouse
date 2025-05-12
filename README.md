# IntelligentGreenhouse

## Overview

**IntelligentGreenhouse** is a full-stack, containerized greenhouse monitoring and control system built for remote access, automation, and extensibility. It features a real-time dashboard powered by MQTT-connected ESP32 devices, allowing users to view temperature and humidity data and, when authenticated, control relays and servos in the greenhouse environment.

This is a personal passion project demonstrating skills in embedded systems, backend architecture, CI/CD, and modern web development.  
**Live Demo (Coming Soon):** [https://intelligentgreenhouse.com](https://intelligentgreenhouse.com) (Read-only access)

---

## Features

-   Real-time temperature and humidity monitoring
-   Device control (relay/servo) for authenticated users
-   Data persistence with MySQL
-   Public read-only dashboard access
-   Responsive Next.js frontend
-   Secure public access via Cloudflare Tunnel
-   Modular and extensible architecture

---

## Tech Stack

| Layer           | Technology                             |
| --------------- | -------------------------------------- |
| Microcontroller | ESP32 with DHT22 sensors               |
| Messaging       | MQTT (Mosquitto broker in Docker)      |
| Backend         | Node.js data collector, Prisma ORM     |
| Frontend        | Next.js web dashboard                  |
| Database        | MySQL (Docker)                         |
| CI/CD           | GitHub Actions with self-hosted runner |
| Deployment      | Docker Compose on home server          |
| Public Access   | Cloudflare Tunnel with custom domain   |

---

## Project Structure

    apps/
      frontend/         → Next.js web dashboard
      data-collector/   → Node.js backend storing MQTT data

    firmware/
      sensor-node/      → ESP32 sketch for sensor publishing
      control-node/     → ESP32 sketch for control commands

    docker/
      docker-compose.yml → All containers (broker, db, frontend, backend)

    .github/
      workflows/        → CI/CD pipelines

---

## Getting Started

### Requirements

-   Docker and Docker Compose
-   ESP32 board (e.g., DevKit V1)
-   DHT22 sensor
-   PlatformIO

### Quick Setup

1. **Clone the repository:**

    ```bash
    git clone https://github.com/samheck/intelligent-greenhouse.git
    cd intelligent-greenhouse
    ```

2. **Set up environment variables:**

    Create a `.env` file in the root directory:

    ```env
    DATABASE_URL=mysql://user:password@mysql/intelligent_greenhouse
    NODE_ENV=production
    LANDING_PORT=3000
    FRONTEND_PORT=3001
    ```

3. **Build and run containers:**

    ```bash
    docker-compose up --build
    ```

4. **Upload firmware to ESP32:**

    - Update Wi-Fi and MQTT broker credentials in PlatformIO project folder env file (manually create)
    - Upload to the ESP32 board with PlatformIO
    - Must open the project in VS Code with PlatformIO project folder as the root for proper board recognition by PlatformIO.

---

## Usage

-   Open the browser and visit: [https://intelligentgreenhouse.com](https://intelligentgreenhouse.com)
-   Public users can view live sensor data
-   Authenticated users can send control commands to connected devices

---

## Security and Access

### Public Users

-   Can view live data on the dashboard
-   Cannot interact with control components

### Authenticated Users

-   Can toggle relays or adjust servos via MQTT
-   May have the ability to register their own hardware in future updates

---

## Roadmap

-   Add historical data graphing (charts)
-   Add threshold-based alerting via (email, SMS)
-   Add basic automation from thresholds adjustable via client(e.g., close vents if temp > X)
-   Support multiple user accounts and device registration

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Acknowledgments

Thanks to the open-source community and key technologies that made this possible:

-   [Mosquitto MQTT Broker](https://mosquitto.org/)
-   [Prisma ORM](https://www.prisma.io/)
-   [Next.js](https://nextjs.org/)
-   [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)

---

## Contact

-   Email: [heck.sam@gmail.com](mailto:heck.sam@gmail.com)
-   GitHub: [https://github.com/Sam-Heck](https://github.com/Sam-Heck)
-   LinkedIn: [https://linkedin.com/in/samuelheck](https://linkedin.com/in/samuelheck)

---

## Why This Project?

This project merges a practical real-world application—greenhouse management—with modern software and hardware. It demonstrates my ability to design and implement secure, extensible, full-stack systems that interact with real-time sensor networks.
