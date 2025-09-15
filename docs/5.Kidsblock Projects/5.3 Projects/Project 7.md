## 7. School Smart Access Control

Let’s make a smart access control system with RFID module and servo. By scanning card, it can recognize identities and automatically control the door lock opening and closing, so that we can experience a safe and convenient smart school life!

### 7.1 RFID Card Module

The RFID module is a non-contact card reading device based on radio frequency identification. It can quickly scan the identity information in IC cards or electronic tags so is widely used in access control, attendance and payment systems.

![KS6065](../../img/KS6065.png)



#### Parameters

Operating voltage: DC 5V

Operating current: 13 ~ 100 mA /DC 5V 

Idle current: 10 ~ 13 mA /DC 5V

Sleep current: < 80 uA

Peak current: < 100 mA

Operating frequency: 13.56 MHz

Maximum power: 0.5W

Data transmission rate: Maximum 10Mbit/s

Operating temperature: -10°C ~ +50°C

Dimensions: 48 x 24 x 8 mm

Positioning hole: Diameter of 4.8mm

Interface: 4-pin curved pin interface spacing 2.54mm



#### Principle

**Workflow**:

![7101](../../img/7101.jpg)

**(1) Energy transmission**

- The antenna of the card reader emits an electromagnetic field → provide energy for passive RFID cards (without batteries).

**(2) Data interaction**

- After the card enters the magnetic field, it is activated → the data within the card (such as UID) is transmitted back to the card reader through load modulation.

**(3) Identity verification**

- The card reader decodes the data → it is compared with the system database for authentication.



#### Test Code

![7_1](../../img/7_1.png)



#### Code Explanation

![7106](../../img/7106.png)

- Initialize the RFID module.

- Initialize the serial port.

![7107](../../img/7107.png)

- Detect whether there is a card entering the radio frequency field. If yes, activate the card and read the UID, and print it on the serial monitor.
- Refresh the output every 0.1 seconds.



#### Test Result

After uploading the code, place the RFID card within the magnetic field for detection. The card reader will print the UID in hexadecimal on the serial monitor.

![7102](../../img/7102.png)

==放一张卡片在磁场范围的图片==

![7110](../../img/7110.png)

#### FAQ

1. **The card cannot be read**
	- Check whether the I2C address is correct.
	- Check the power supply voltage (5V) and the card type.
2. **The version displays `0x00` or `0xFF`**
	- Check whether the I2C(SDA/SCL) is connected in reverse.
	- Ensure the stability of the power supply voltage.

---



### 7.2 Servo

A servo is a motor that precisely controls the rotation angle by receiving control signals.

![servo](../../img/servo.png)

The servo we use comes with three external wires. The brown one is for ground, the red is power positive, and the orange is for signal.

![7201](../../img/7201.png)



#### Parameters

Operating voltage: DC 3.3 ~ 5V 

Operating temperature: -10°C ~ +50°C

Dimensions: 32.25 x 12.25 x 30.42mm

Interface: 3-pin female interface spacing 2.54mm



#### Principle

**1. Working principle of servo**

A servo is a closed-loop controlled position servo motor. The ESP32 controls its angle by **PWM (Pulse Width Modulation) signals**. Core working principle is as follows:

**PWM signal input**:

- ESP32 generates a 50Hz (period 20ms) PWM signal

- **The pulse width (high-level time) determines the angle**:

	- **0.5ms(500μs)→ 0°**

	- **1.5ms(1500μs)→ 90°** (Middle position)

	- **2.5ms(2500μs)→ 180°**

![341301](../../img/341301.png)

**Pin restriction**:

- Avoid using the following pins (with special functions) :
	- GPIO0(Download Mode)
	- GPIO2(Internal pull up)
	- GPIO12(Level sensitive at startup)



#### Test Code

![7_2](../../img/7_2.png)



#### Code Explanation

1. Initialize the servo (GPIO32 pin)
2. Loop execution
	- rotate to 180° → delay 1s
	- rotate to 135° → delay 1s
	- rotate to 90° → delay 1s
	- Repeat the loop



#### Test Result

After uploading the code, the servo will rotate according to the following rules:

1. **rotate to 180° immediately** (door closes) → maintain the state for 1s
2. **rotate to 135°** (door is half open and half closed) → maintain the state for 1s
3. **rotate to 90°** (door opens) → maintain the state for 1s
4. **Repeat these actions** (180°→135°→90°→180°...)

==动图==

![7104](../../img/7104.png)

---



### 7.3 Smart Access Control

In the previous sections, we have mastered how to recognize identity by RFID module and how servo works. Now, we will integrate them to create an access control system with automatic card (un)locking function. It not only enhances school security but also showcases the charm of technology.

In the system, it can identify the RFID cards of authorized personnel and open/close the door by the servo. Now let’s build this stable and reliable system starting from flowchart and program writing. Are you ready? Start right now!



#### Code Flow

```mermaid
flowchart TD
    A([START]) --> B[Initialize the RFID and servo]
    B --> C{RFID card detected?}
    C -- YES --> D[Read card UID]
    D --> E{UID matches authorization list?}
    E -- YES --> F[open the door for 2s] --> G[close the door]
    E -- NO --> H[Prompt: Unauthorized]
    C -- NO --> C
    G --> C
    H --> C
```



#### Test Code

<span style="color: rgb(200, 70, 100);">Please ensure that the servo has been initialized before assembly; otherwise, it may get stuck and be damaged.</span>

![7_3](../../img/7_3.png)



#### Code Explanation

<span style="color: rgb(200, 70, 100);">**The UID needs to be replaced with your own RFID card number before uploading code.**</span>

![7108](../../img/7108.png)

- Initialize the RFID module.

![7109](../../img/7109.png)

- Check if there are any new cards approaching. If yes, read the UID of the card, and compare whether this UID matches the one you set.
  - Match → Open the door (`90°`), and close the door after 2 seconds (`180°`)
  - Mismatch → No operation (the door remains closed).



#### Test Result

<span style="color: rgb(200, 70, 100);">The servo must be initialized before installation. After that, you can upload the code.</span>

After uploading the code, the access control system begins to circulate:

- Card available → Read UID → Match succeeded → Open the door → Delay 2s → Close the door
- Card available → Read UID → Match failed → Prompt unauthorized
- No card → Continue the detection

==动图==

![7105](../../img/7105.png)



#### FAQ

1. The card cannot be detected
	- Check if the I2C address is incorrect and if the wiring is loose
2. The servo does not rotate
	- Check the power supply voltage and connect an external power supply
	- Make sure the servo has been initialized before assembly
3. The serial port outputs messy codes
	- Make sure the serial monitor is set to 115200