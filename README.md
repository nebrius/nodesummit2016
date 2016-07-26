# nodesummit2016

Hardware Day Zero information. Today we will walk through getting everyone started, and then you will be doing one of two builds!

# Getting Started

## Connecting the particle

1. Go to: [Particle build site](https://build.particle.io/) and create and account.  Keep this page open once an account is created as we will be using it to do all of our development
2. Go to the [Particle.io docs](https://docs.particle.io/guide/getting-started/intro/photon/) and click on **SETUP MY PHOTON** and follow step 1
3. Navigate to [Connecting over USB](https://docs.particle.io/guide/getting-started/connect/photon/) and follow the instructions for your operating system.
4. Plug your Photon into your computer and make sure it is blinking blue. If it is node, hold down the "mode" button for 5 seconds until it blinks blue.
5. Run the command `particle setup`. A series of questions will pop up:
    - **Would you like to continue with this one?** Yes
    - **Found "Photon-****". Would you like to perform setup on this one now?** Yes
    - **Would you like to manually enter your Wi-Fi network configuration?** No
    - **Please select the network to which your Photon should connect:** //pick your network
    - **Please enter your network password:** //type the password
    - **Would you like to continue with the information shown above?**  Yes

Once connected, you should see a color change on the photon from flashing blue to breathing cyan.

### Hello World

Next, let's get the DotStar LED strips to light up.

1. Set up the circuit as such:
![Photon Connections](https://theoreticalideations.com/static/fritzing_basic.png)
2. Go back to the [Particle build page](https://build.particle.io/build/new)
3. You should see your Photon listed under the "Devices" section
4. Go to the "Libraries" section and search for **dotstar** under "Community Libraries"
5. Select "Use This Example"
6. Modify line 47 so that it says:

```c
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS);
```

7. On the upper left hand corner, click on flash
8. You're pixels should now be flashing!

# Builds

Choose one of the two builds below, or do both if you're ambitious!

To make calls to the Particle API, first install the SDK:

```sh
npm install particle-api-js@5.3.0
```

Then call it with:

```
const DEVICE_ID = 'Your Device ID';
const ACCESS_TOKEN = 'Your Access Token';

const Particle = require('particle-api-js');
const particle = new Particle();

var fnPr = particle.callFunction({
  deviceId: DEVICE_ID,
  name: 'set_color',
  argument: '00255000255',
  auth: ACCESS_TOKEN
}).then(
  (data) => {
    console.log('Function called succesfully:', data);
  },
  (err) => {
    console.log('An error occurred:', err);
  }
);
```

## Game Scoreboard

For this build, we will be creating a visual scoreboard. The scoreboard will keep track of points in two player games in the form of a bar graph.

To start, load your photon with the [scoreboard firmware](firmware/scoreboard.c) by copying the code into your Particle Build window (overwriting the Hello World sample), and flashing it to your Photon.

Each player will be represented by a different color and assigned to one side of the DotStar Strip. As players gain points, another dot will light up, starting from their side of the strip and moving toward the center. The first player to get to the center wins!

This basic scorekeeping can be used for a variety of games implemented in the browser. The first game you will implement is Tic-Tac-Toe to keep things simple. When a player wins, the browser will send an AJAX request to a Node.js server.

This Node.js server will use the [Particle Javascript SDK](https://docs.particle.io/reference/javascript/) to call a function named `set_color`. This method takes a specially formatted string of the form `LLRRRGGGBBB`, where `LL` is the number of the LED you want to light up, `RRR` is the red color channel, `GGG` is the green color channel, and `BBB` is the blue color channel. All of these numbers are in decimal.

**Stretch goals**

- Add support for multiple games that can be switched between using a button wired up to the Photon. The button can be wired up as:
![Button Wiring](https://theoreticalideations.com/static/fritzing_button.png)
- Modify the firmware to add a new Particle API method called `set_flash` that will flash the lights at a set rate.

## Animation Control

For this build, we will be created an animated light stick that can be controlled remotely.

To start, load your photon with the [animation firmware](firmware/animation.c) by copying the code into your Particle Build window (overwriting the Hello World sample), and flashing it to your Photon.

This firmware comes preloaded with two animations: "flash" and "pulse". These can be switched between by calling an Particle API endpoint.

For this build, we will create a Node.js app that switches between animations at a preset interval.

This Node.js app will use the [Particle Javascript SDK](https://docs.particle.io/reference/javascript/) to call a function named `preset`. This method takes a single number that corresponds to the preset to use. `1` is the "fade" preset and `2` is the "pulse" preset. All of these numbers are in decimal.

**Stretch Goals*

- In the firmware there is a method called `set_value` that allows you to control various aspects of the animations. This method takes two numbers, the first is which value to set, and the second is what to set the value to, like a key-value pair in a JavaScript object. See the constants under `Settable Values` in the firmware for details. Expose this method to the Particle cloude so it can be called remotely.
- Add your own animation! Take a look at the "fade" animation for an example
- Add a button to switch between animations. Wire up the button as such:
![Button Wiring](https://theoreticalideations.com/static/fritzing_button.png)
- Create a web frontend to control the animation

## License

The MIT License (MIT)

Copyright (c) 2016 Bryan Hughes <bryan@nebri.us>, Alex Glow <alex@hackster.io>,
Nadine Hachouche <nadine@polybit.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.