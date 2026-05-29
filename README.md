# IoT Temperature Data Logger to Google Sheets (ESP32)

An ESP32-based IoT project designed to read real-time temperature data from a DS18B20 sensor and timestamp each reading using a DS1302 RTC module. The collected data is automatically and wirelessly transmitted over Wi-Fi to a Google Sheet using the Google Apps Script Web App API.

## Key Features
- **Accurate Time Synchronization:** Utilizes a hardware DS1302 RTC module for standalone, reliable timestamp tracking.
- **Precision Temperature Sensing:** Leverages an industrial DS18B20 sensor communicated via the OneWire protocol.
- **Serverless Cloud Integration:** Transmits data directly to Google Sheets through HTTP GET requests via Google Apps Script (eliminating the need for external MQTT brokers or paid IoT platforms).
- **URL-Safe Encoding:** Automatically structures timestamp character strings into web-safe formats (`%2F` for `/`, `+` for spaces, and `%3A` for `:`) to ensure reliable data transmission across the web API endpoint.

## Hardware Pinout Configuration
| Component | Module Pin | ESP32 GPIO Pin | Type / Function |
|-----------|------------|----------------|-----------------|
| **DS18B20** | Data Pin | GPIO 5 | Input (Internal Pull-Up) |
| **DS1302 RTC** | I/O (DAT) | GPIO 26 | Bi-directional Data Line |
| **DS1302 RTC** | CLK (SCLK) | GPIO 25 | Serial Clock Line |
| **DS1302 RTC** | RST (CE) | GPIO 27 | Chip Enable / Reset Line |

## System Workflow
1. **Initialization:** The ESP32 boots up the hardware peripherals and establishes a connection to the designated local Wi-Fi network.
2. **Data Acquisition:** Once online, the ESP32 fetches the current date and time from the DS1302 and requests a temperature conversion from the DS18B20.
3. **Payload Construction:** The timestamp string is formatted into a URL-encoded payload and appended along with the temperature reading to the Google Apps Script endpoint URL.
4. **Data Transmission (HTTP GET):** The ESP32 executes an HTTP GET request to the Web App API. The script running in the cloud catches the parameters and appends them as a new row in the spreadsheet.
5. **Interval:** The loop repeats continuously every 5 seconds (configurable via the `delay()` function).

## Setup & Deployment Instructions
1. **Google Sheets Setup:**
   - Create a new spreadsheet inside Google Sheets.
   - Go to **Extensions** > **Apps Script**, and insert a script designed to capture `waktu` (time) and `value` parameters from an incoming GET request.
   - Deploy it as a **Web App** and set the access setting to *"Anyone"*. Copy the generated Web App URL.
2. **Arduino Code Configuration:**
   - Modify the `ssid` and `pw` string variables to match your Wi-Fi credentials.
   - Replace the `url` variable string with your unique Google Apps Script Web App URL.
3. **Flashing the Firmware:**
   - Ensure you have the `RtcDS1302`, `DallasTemperature`, and `OneWire` libraries installed in your Arduino IDE or PlatformIO.
   - Compile and flash the program to your ESP32 board.
   - Open the **Serial Monitor** at a baud rate of **115200** to monitor the Wi-Fi connection status and incoming HTTP response codes (e.g., `200` or `302` indicates a successful push).

## License
This project is open-source and distributed under the [MIT License](LICENSE).
