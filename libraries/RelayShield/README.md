# RelayShield

The RelayShield library makes it easy to use the Photon RelayShield. It provides functions to turn relays on and off, all the relays on and off at the same time, and to check whether a relay is currently on or off.

## Usage

Plug a Photon, Electron, or compatible device into a [Particle Relay Shield](https://store.particle.io) and you're ready to go!

```
//Create an instance of the RelayShield library, so we have something to talk to
RelayShield myRelays;

void setup() {
    //.begin() sets up a couple of things and is necessary to use the rest of the functions
    myRelays.begin();
}

void loop() {
    // If relay 1 is on, then turn it off

    if(myRelays.isOn(1)){    // myRelays.isOn(1) will return TRUE if relay 1 on and FALSE if it's off
        myRelays.off(1);     // myRelays.off(#) will switch relay # off
    }

    // If it wasn't already on, then turn it on
    else{
        myRelays.on(1);      // myRelays.on(#) will switch relay # on
    }

    // Change it once a second
    delay(1000);
}
```

## Reference

### `RelayShield`

`RelayShield relays();`

Creates a relay object that is used to act on the library and stores the current relay states.

### `begin`

`relays.begin();`

Sets the appropriate pins on the device to OUTPUT. D3,D4,D5,D6 are connected to the relays.

### `on`

`relays.on(relayNumber);`

Turns the specified relay on. Valid inputs are integers 1,2,3,4.

### `off`

`relays.off(relayNumber);`

Turns the specified relay off. Valid inputs are integers 1,2,3,4.

### `allOn`

`relays.allOn();`

Turns all the relays on.

### `allOff`

`relays.allOff();`

Turns all the relays off.

### `isOn`

`bool relayState = relays.isOn(relayNumber);`

Returns `true` if a relay is currently set to on, and `false` otherwise.

## Examples

1. __Blink a Relay__ - Turning relays on and off, one at a time
2. __Control all Relays__ - Turning all the relays on and off
3. __Internet Relays__ - Setting up functions so you can call relays over the internet!

## License
Copyright 2016 Richard Whitney

Released under LGPL
