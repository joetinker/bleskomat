# Bleskomat

The world's simplest Lightning Network ATM.

![](images/logo.png)

* [Overview](#overview)
* [Requirements](#requirements)
	* [Hardware Requirements](#hardware-requirements)
	* [Software Requirements](#software-requirements)
* [Setup](#setup)
	* [Building the Hardware Device](#building-the-hardware-device)
		* [Wiring Diagram](#wiring-diagram)
		* [Wiring the Power Supply](#wiring-the-power-supply)
		* [Wiring the TFT Display](#wiring-the-tft-display)
		* [Wiring the Coin Acceptor](#wiring-the-coin-acceptor)
	* [Training the Coin Acceptor](#training-the-coin-acceptor)
	* [Installing Libraries and Dependencies](#installing-libraries-and-dependencies)
* [Compiling and Uploading to Device](#compiling-and-uploading-to-device)
* [Running the HTTP Server](#running-the-http-server)
* [Remote Tunneling](#remote-tunneling)
	* [Using SSH and a VPS](#using-ssh-and-a-vps)
	* [Using ngrok](#using-ngrok)
	* [Using TOR](#using-tor)
* [Generate and Test Signed LNURLs](#generate-and-test-signed-lnurls)
* [Notes](#notes)
* [License](#license)


## Overview

Key features of the Bleskomat ATM include:
* Works offline - no WiFi required
* Inexpensive, easily-sourced components
* Easily hackable and extendible
* Plug it in and it's ready to accept fiat in seconds

![](images/Bleskomaty.jpg)

## [Video presentation (CZ)](https://www.youtube.com/watch?v=SmE7AxKkGUo)

The project consists of two parts:
* __HTTP Server__ - supports [lnurl-withdraw](https://github.com/btcontract/lnurl-rfc/blob/master/lnurl-withdraw.md) with additional request handlers for fiat-currency -> satoshi conversion and request signing.
* __Physical Device (ATM)__ - user inserts coins, device generates a signed LNURL and displays as QR code, user's app (which supports lnurl-withdraw) scans QR code and makes request to HTTP server, withdraw process is completed and the user has successfully bought satoshis with fiat coins.

## Requirements

This section includes information about the requirements (software + hardware) that you will need to build your own Bleskomat ATM.


### Hardware Requirements

To build the physical device, you will need the following hardware components:
* [NodeMCU-32s](images/NodeMCU-32s-img.png) [ESP-WROOM-32](https://www.espressif.com/en/products/modules/esp-wroom-32/overview) by espressif
* [TFT Display ST7735s - 1.8 inch (128x160 pixel)](images/bezel-ST7735s.jpg)
* Coin Acceptor - "Model HX-616"
* 12V DC power adaptor with \~1A
* [Step-down converter DC/DC](images/dc-dc_6-28V_5V.png)
* Installation enclosure CP5001 (210x280x130)
* Standard USB to micro USB cable

It is strongly recommended to use a multimeter while wiring the device.


### Software Requirements

* [make](https://www.gnu.org/software/make/) - used as a meta task runner
* [PlatformIO Core (CLI)](https://docs.platformio.org/en/latest/core/) - used to compile and upload the firmware to the ESP32
* [nodejs](https://nodejs.org/) - used to run the HTTP server and as a task runner
	* On Linux and Mac install node via [nvm](https://github.com/creationix/nvm)


## Setup

Step-by-step setup process including both hardware and software.


### Building the Hardware Device

Before proceeding, be sure that you have all the project's [hardware requirements](#hardware-requirements).


#### Wiring Diagram

Here is a wiring diagram for the Bleskomat ATM:

![](images/Bleskomat-v1.sch.png)


#### Device and the Power Supply

![](images/PowerSupply_PCB.jpg)

Use a standard USB to micro USB cable to connect the ESP32.


#### PCB

![](images/PCB.jpg)

#### DC/DC module

![](images/dc-dc_6-28V_5V.png)

### Training the Coin Acceptor

Be sure that you've already finished wiring the power supply to all the components before continuing with this step.

Plug-in the power supply so that all the electronic components have power.

An instruction sheet is included with the coin acceptor that will guide you through the training process:

![](images/coin-acceptor-hx-616-instructions-side-1.jpg)

| PANEL  | Meaning                                     |
|--------|---------------------------------------------|
| E      | number of coin values                       |
| Hn     | sampling quantity of coin n                 |
| Pn     | signal output value of coin n               |
| Fn     | recognition accuracy of coin n (8 advised)  |
| A      | training the acceptor                       |
| An     | start inserting coins for coin n            |

Press ADD + MINUS to get to the setting menu for Hn, Pn, Fn. Then press SET to swap between possibilities. To change the option press ADD or MINUS to select the desired value and confirm with SET.

Press SET to enter the training menu (A appears on the panel). After pressing SET again you can start inserting the coins.


### Installing Libraries and Dependencies

Before proceeding, be sure that you have all the project's [software requirements](#software-requirements).

Use make to install libraries and dependencies for both the device firmware and HTTP server:
```bash
make install
```
* The server's dependencies are defined in its package.json file located at `./server/package.json`
* The firmware's dependencies are defined in its platformio.ini file located at `./device/platformio.ini`

If while developing you need to install a new library for the device firmware, use the following as a guide:
```bash
cd ./device
platformio lib install --save LIBRARY_NAME[@VERSION]
```
The `--save` flag tells platformio to add the library to the project's `platformio.ini` file.

You can find PlatformIO's libraries repository [here](https://platformio.org/lib).

If you need to add dependencies to the server:
```bash
cd ./server
npm install --save LIBRARY_NAME[@VERSION]
```


## Compiling and Uploading to Device

Be sure that you've already [installed libraries and dependencies](#installing-libraries-and-dependencies) before continuing here.

To compile the firmware (without uploading to a device):
```bash
make compile
```

To compile and upload to your device:
```bash
make upload
```
If you receive a "Permission denied" error about `/dev/ttyUSB0` then you will need to set permissions for that file on your system:
```bash
sudo chown $USER:$USER /dev/ttyUSB0
```

To open the serial monitor:
```bash
make monitor
```


## Running the HTTP Server

Be sure that you've already [installed libraries and dependencies](#installing-libraries-and-dependencies) before continuing here.

To start the server:
```bash
make server
```

To run the server while printing debug messages:
```bash
DEBUG=lnurl* make server
```
Example output as follows:
```
lnurl:info Creating web server... +0ms
lnurl:mock:c-lightning:info JSON-RPC API listening at /path/to/this/project/server/c-lightning.sock +0ms
lnurl:mock:c-lightning:info Listening for TCP connections at 127.0.0.1:9735 +0ms
lnurl:info Web server listening at http://127.0.0.1:3000/ +5ms
```

A configuration file is automatically generated for you to help get you started. The auto-generated config file is located at `./server/config.json`.

By default the server uses a mock c-lightning node. Once you're ready to connect your own Lightning Network node, have a look at the [lnurl-node](https://github.com/chill117/lnurl-node#lightning-backend-configuration-options) documentation for how to configure the server's lightning backend.

## Generate and Test Signed LNURLs

There is a helper script included in this project that can generate signed LNURLs that are valid for your server. Run the script as follows:
```bash
make signedLnurl AMOUNT=1.00
```
The output is an unencoded URL - for example:
```
https://94fb07c3f0620eu.ngrok.io/u?id=ngNmBZs%3D&n=ead7b208dc1f6901&t=withdrawRequest&f=CZK&pn=1.00&px=1.00&pd=&s=2faf6595e2df24837ac1b655accd7a172901be216fac33d6643f3135bb4067cb
```
Query the URL with cURL:
```bash
curl "https://94fb07c3f0620eu.ngrok.io/u?id=ngNmBZs%3D&n=ead7b208dc1f6901&t=withdrawRequest&f=CZK&pn=1.00&px=1.00&pd=&s=2faf6595e2df24837ac1b655accd7a172901be216fac33d6643f3135bb4067cb"
```
The response will look something like this:
```
{"minWithdrawable":401000,"maxWithdrawable":401000,"defaultDescription":"","callback":"https://94fb07c3f0620eu.eu.ngrok.io/u","k1":"826fc889dd20128d11fbf9d07447254cc3b9f4db9f347aa05f2ca2e88ea046e7","tag":"withdrawRequest"}
```
Generate a signed LNURL and encode it as a QR code:
```bash
make signedLnurl AMOUNT=1.00 | ./server/node_modules/.bin/lnurl encode | qr
```
Note that this requires the [qr](https://github.com/lincolnloop/python-qrcode/) CLI utility.

If you prefer not to use the `qr` utility, then print the encoded LNURL:
```bash
make signedLnurl AMOUNT=1.00 | ./server/node_modules/.bin/lnurl encode
```
And copy the result into the QR encoder of your choice.

Once you've got a sample QR code, scan it with your mobile wallet app. The whole flow should work even if your server is still running the mock c-lightning, but obviously the LN payment will never be completed.

If you don't already have a mobile wallet app that supports LNURL, you can check out the [Introduction to Bleskomat](https://www.bleskomat.com/intro) page which has a list of mobile wallet apps that work with this project.


## Notes

* Exchange rates are queried using Coinbase's `/v2/exchange-rates` API end-point. The function that does this is defined in `./server/utils.js`. If you prefer to use a different provider, that is the place to make your changes.


## License

The device firmware source code is licensed under the [GNU General Public License v3 (GPL-3)](https://tldrlegal.com/license/gnu-general-public-license-v3-(gpl-3)):
> You may copy, distribute and modify the software as long as you track changes/dates in source files. Any modifications to or software including (via compiler) GPL-licensed code must also be made available under the GPL along with build & install instructions.

The server source code is licensed under the [GNU Affero General Public License v3 (AGPL-3.0)](https://tldrlegal.com/license/gnu-affero-general-public-license-v3-(agpl-3.0)):
> The AGPL license differs from the other GNU licenses in that it was built for network software. You can distribute modified versions if you keep track of the changes and the date you made them. As per usual with GNU licenses, you must license derivatives under AGPL. It provides the same restrictions and freedoms as the GPLv3 but with an additional clause which makes it so that source code must be distributed along with web publication. Since web sites and services are never distributed in the traditional sense, the AGPL is the GPL of the web.
