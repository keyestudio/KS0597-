## 13. Web Page Remote Monitor Air Quality

In smart school era, environment perception and data interconnection are becoming important symbols of its modernization. In this project, with the theme of Remote Monitoring of Air Quality, we will guide you to deeply explore the innovative application of Internet of Things in school monitoring.

From now on, let’s protect the school environment with technology, build a smart learning environment with innovation, and jointly explore the infinite possibilities of Internet of Things in education!



#### Principle

1. Data collection

	ENS160 sensor → ESP32 (via I2C)

2. Data transmission

	ESP32 → Router → Mobile phone/Computer

3. Data display

	Browser request → Server response → Update web page



#### Code Flow

```mermaid
flowchart TD
    A[Initialization] --> B[Network connection]
    B --> C[Server starts]
    C --> D{Request processing}
    D --> E[Web page]
    D --> F[Sensor data]
    E --> G[Real-time monitoring display]
    F --> G
    G --> H[Application completed]
```



#### Test Code

![14](../../img/14.png)



#### Code Explanation

**Here covers extracurricular knowledge of HTML, CSS, and JS, so we only provide a brief introduction.**

Click ![add](../../img/add.png) to choose the extension. Search the following extension to load it.

![1304](../../img/1304.png)

Back to the editing area after it is loaded.

![line1](../../img/line1.png)

![1302](../../img/1302.png)

- Initialize the OLED and serial port

- Set the WiFi name and password, and connect to WiFi. Then print the IP address on the OLED and the serial monitor.

	<span style="color: rgb(200, 70, 100);">Please replace the WiFi name and password in the code with yours.</span>

![1402](../../img/1402.png)

- There are three components on the page: **AQI**, **TVOC** and **CO2**
  - AQI: Display the comprehensive air quality index from levels 1 to 5 in real time
  - TVOC: Display the volatile organic compound concentration in real time
  - CO2: Display the equivalent carbon dioxide value in real time
- Update the data every 5 seconds.



#### Test Result

1. After uploading the code, open the serial monitor and set the baud rate to 115200. You can see the printed IP information:

   ![1207](../../img/1207.png)

   The IP address will also be printed on the OLED at the same time.

   ![1208](../../img/1208.png)

2. Enter this IP address in the browser of your mobile phone or computer to access the air quality monitoring page.

   Data is obtained immediately when the page is opened, and it is refreshed every 5 seconds.

   <span style="color: rgb(200, 70, 100);">Note: Make sure your mobile phone/computer and ESP32 are connected to the same WiFi.</span>

   ![1403](../../img/1403.png)



#### FAQ

1. If nothing is printed on the serial monitor, please press the reset button on the board.

   ![RESET](../../img/RESET.png)

2. If the ESP32 has not been able to obtain an IP address, it is usually because the WiFi connection has failed. Solutions:

   - Make sure that the WiFi name and password in the code have been replaced with yours.
   - Make sure your WiFi network is 2.4GHz. ESP32 does not support 5GHz WiFi.

3. If there is no page when entering the IP address,

	- Make sure the IP address is entered correctly.
	- Check whether your mobile phone/computer is on the same network as the ESP32.