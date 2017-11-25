# Juicy Switch Firmware 🔌 🍊 💻

⚠️ **This is work in progress!** ⚠️

The firmware powering Juicy's *esp8266* chip. It let's you configure the WiFi that Juicy should connect to and provides a RESTful API for controlling the chip's LED. Later, when Juicy is fully assembled, you will be able to power the socket on and off via that API.

## Features

The following is a rough outline of features we want to implement. Already implemented features are marked accordingly.

- [x] Easy configuration of Juicy's WiFi connection
- [x] Configuration via a captive portal (tested on iOS)
- [x] Persisted WiFi credentials
- [x] Auto connect if correct WiFi credentials are available
- [x] LED feedback on successful or failed WiFi configuration
- [x] MDNS hostname for the API endpoint
- [ ] Automatic reconnection if WiFi connection is lost
- [ ] Nice looking configuration interface
- [ ] Unique API hostname
- [ ] ...

## Getting Started

### Development & Installation

The easiest way to get the Juicy firmware up and running on your esp8266 chip is using the [*PlatformIO IDE*](http://platformio.org/get-started/ide) or [*PlatformIO Core*](http://platformio.org/get-started/cli).

[Here](http://docs.platformio.org/en/latest/ide/atom.html#quick-start) you can find instructions on how to setup PlatformIO IDE for [Atom](https://atom.io). Just import Juicy's `/firmware` directory as a project and you're ready to go.

### Usage

`// TODO`



 