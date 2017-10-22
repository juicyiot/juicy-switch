# Juicy Switch :electric_plug::tangerine:
A **RESTful** power **switch**.
The Juicy Switch enables you to turn a 230VAC device **on** and **off** using **HTTP**. E.g. you can switch a :bulb: on by calling `http://juicy.local/on`
and turn it off with `http://juicy.local/off`.

## :warning: Disclaimer :zap:
Mains can kill or severly injure people! Please read about electrical saftey first.
We designed the PCB to the best of our knowledge to be safe (see [Safety](#safety) and [Enclosure](#enclosure)), however **use it at your own risk**, we are no electrical engineers!

## Design goals
- DIY (do it yourself) :hammer:
- Affordable :dollar:
- Easy to setup and to use (your mom can use it :wink:)
- Simple yet powerful :rocket:

The Juicy Switch can be self assembled and is affordable, see [Bill of Materials](#bill-of-materials). The setup is a friction less and quick process. After the device is powered up, it will create a WIFI hotspot called `juicy_switch`. Connect with your Smartphone/Notebook and proceed with the one time [Setup](#setup). Eventhough the switch is very simple to use, many *smart* use cases can be built on top, see [Use Case Ideas](#use-case-ideas) to get inspired.

## Hardware

![OSH](https://i2.wp.com/www.oshwa.org/wp-content/uploads/2014/03/oshw-logo-100-px.png "Open Source Hardware")

### Bill of Materials

The cost per unit is roughly 10$.

| Marking    | Item             | Package           | Value         |
------------ | ---------------- |------------------ | ------------- |
|            | µC-ESP826 ESP-07 | -                 | -             |
|            | HLK-PM01         | -                 | -             |
| R1,R2,R6,R8| resistor         | SMD 0815          | 10kΩ          |
| R3,R4      | resistor         | SMD 0815          | 1kΩ           |
| R5         | resistor         | SMD 0815          | 120Ω          |
| R7         | varistor         | THT               | 250VAC/2500A  |
| C1         | capacitor        | SMD 0815          | 22µF          |
| C2,C4      | capacitor        | SMD 0815          | 10µF          |
| C3         | capacitor        | SMD               | 100µF         |
| C5,C6      | capactior        | SMD 0815          | 100nF         |
| J3,J5      | screw terminal   | THT               | 5m            |
| D1,D3      | LED              | THT               | 3mm           |
| D2         | diode 1n4148     | THT               | 200mA/100V/1V |
| F1         | fuse (slow blow) | glass tube 5x20mm | 0.3A          |
| F2         | fuse (fast blow) | glass tube 5x20mm | 6.3A          |
| F1,F2      | fuse holder      | THT 5x20mm        | -             |
| Q1         | transistor npn   | SMD SOT23-3       | 0.5A/40V      |
| K1         | relay            | -                 | 10A/250VAC    |
| U1         | voltage regulator| SOT23-3           | 3.3V          |
| S1         | tactile switch   | SMD (4x4x1.5mm)   | -             |
| J1,J2,J4,J6 | Headers         | THT               | -             |

We bought most parts from Aliexpress (various suppliers).

### PCB

#### Schematic
![](hardware/schematic.png "Gerber view top")

#### Board
Seeedstudio manufactured our board (2 layer and 1oz of copper).

![](hardware/gerber-top.png "Gerber view top")
![](hardware/gerber-bottom.png "Gerber view bottom")

## Progress/Status
October 2017: PCB got produced and has been shipped.