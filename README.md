# IntelligentGreenhouse

### **Overview**

IntelligentGreenhouse is a remote monitoring and control application designed for greenhouse systems. This app allows users to:

- Monitor temperature and humidity in real-time.
- Control devices such as heaters, fans, and lights remotely.
- Receive periodic updates on environmental conditions.

This project demonstrates my skills in embedded systems, full-stack development, and IoT technologies.

---

### **Features**

- **Real-Time Monitoring:** View live temperature and humidity readings via a web-based dashboard.
- **Device Control:** Turn devices on/off remotely with the click of a button.
- **Data Logging:** Maintain a historical log of environmental data for analysis.
- **Responsive UI:** The front-end interface is optimized for desktop and mobile devices.
- **Wi-Fi Enabled:** Uses an ESP8266 microcontroller for data transmission over Wi-Fi.
- **Expandable Design:** Easily integrate additional sensors or devices.

---

### **Tech Stack**

- **Embedded Systems:** NodeMCU ESP8266
- **Sensors:** DHT22 for temperature and humidity
- **Back-End:**
  - Node.js with Express (API server)
  - MQTT for real-time communication
- **Front-End:**
  - React
- **Database:** 
  - MySQL
- **Deployment:**
  - Local: Raspberry Pi (Optional to host Express.js server)

---

### **Getting Started**

#### Prerequisites

- Node.js and npm are installed on your machine.
- MySQL or another database server running locally or in the cloud.
- A NodeMCU ESP8266 microcontroller with a DHT22 sensor.

#### Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/Sam-Heck/IntelligentGreenhouse
   cd IntelligentGreenhouse
   ```

2. **Set up the back end:**

   - Navigate to the `server` folder:
     ```bash
     cd server
     ```
   - Install dependencies:
     ```bash
     npm install
     ```
   - Configure `.env` file with:
     ```
     DB_URI=<your-database-connection-string>
     PORT=3000
     ```

3. **Set up the front end:**

   - Navigate to the `client` folder:
     ```bash
     cd ../client
     ```
   - Install dependencies:
     ```bash
     npm install
     ```
   - Start the development server:
     ```bash
     npm start
     ```

4. **Program the ESP8266:**

   - Use the Arduino IDE to upload the `firmware.ino` script from the `firmware` directory to your NodeMCU.
   - Update Wi-Fi credentials in the script:
     ```cpp
     const char* ssid = "your-SSID";
     const char* password = "your-PASSWORD";
     ```

---

### **Usage**

1. **Start the back end:**

   ```bash
   cd backend
   npm start
   ```

2. **Access the front end:**

   - Open your browser and go to `http://localhost:3000`.

3. **Monitor and Control:**

   - View real-time temperature and humidity readings.
   - Use the controls to turn devices on or off.

---

### **Screenshots**

#### Dashboard



#### Historical Data



---

### **Roadmap**

Future enhancements for IntelligentGreenhouse include:

- Adding support for additional sensors (e.g., soil moisture, CO2 levels).
- Incorporating machine learning to predict optimal greenhouse conditions.
- Enabling notification alerts for threshold breaches (e.g., temperature too high).

---

### **License**

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

### **Acknowledgments**

- Special thanks to the open-source community for providing the tools and libraries used in this project.
- Inspiration from modern IoT and smart farming solutions.

---

### **Contact**

For questions, suggestions, or collaboration opportunities, feel free to reach out to me:

- **Email:** [heck.sam@gmail.com](mailto\:heck.sam@gmail.com)
- **LinkedIn:** [linkedin.com/in/samuelheck/](https://www.linkedin.com/in/samuelheck/)
- **GitHub:** [github.com/Sam-Heck](https://github.com/Sam-Heck)

---

### **Why This Project?**

I built IntelligentGreenhouse as a passion project to combine my love for technology and practical problem-solving. It demonstrates my ability to design and develop a system that bridges hardware and software to address real-world challenges.

