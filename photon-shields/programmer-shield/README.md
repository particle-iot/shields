# Installing OpenOCD for Particle Programmer Shield:

### OSX:

- Install all the OpenOCD prerequisites. They have [very good instructions](http://openocd.org/documentation/) on getting it installed on a Mac/Linux/Windows.
- Download the latest copy of the [OpenOCD source.](http://sourceforge.net/projects/openocd/) At the time of writing, v0.9.0 was the latest.
- Since programmer shield is new, it is not yet supported by OpenOCD natively. Hence, you'll need to rebuild and install OpenOCD with the hardware configuration file for the Particle Programmer Shield. You can download the file [here.](https://github.com/spark/photon-shields/blob/master/programmer-shield/particle-ftdi.cfg)
- Save the config file in `openocd-0.9.0/tcl/interface/ftdi/`
- Navigate to the `openocd-0.9.0` folder and rebuild/install OpenOCD by typing in the following commands in the terminal:
    + `./configure --enable-ftdi`
    + `make install`
- To check if the installation was successful, type in:  
`openocd -v`  
which should result in a message like this:  
<pre>
Open On-Chip Debugger 0.9.0 
Licensed under GNU GPL v2
For bug reports, read
http://openocd.org/doc/doxygen/bugs.html
</pre>

Now comes the tricky part. The latest OSX updates have the USB FTDI drivers built-in by default. The Particle Programmer Shield uses FTDI's FT2232 chip which opens up two ports to do: USB-Serial and USB-JTAG simultaneously. Unfortunately, the AppleUSBFTDI recognises both of them as USB-Serial which renders OpenOCD unusable. You could potentially unload the default Apple drivers to overcome this but that would render USB-Serial unusable. [(Original blog post on this modification)](http://alvarop.com/2014/01/using-busblaster-openocd-on-osx-mavericks/)

So the trick is to disable only one of the driver entries by commenting it out in the kext's Info.plist which can be found here:

`/System/Library/Extensions/IOUSBFamily.kext/Contents/PlugIns/AppleUSBFTDI.kext/Contents/Info.plist`

![](https://github.com/spark/photon-shields/blob/master/programmer-shield/kext-modify.png)

You'll need to restart your machine now. I had to shutdown my computer and start it again for the changes to get updated.

If you have previously installed FTDI drivers (most likely this happened automatically in the background when you plugged in your Programmer Shield), make sure to remove them or unload them before running OpenOCD.

`sudo kextunload -bundle com.apple.driver.AppleUSBFTDI`

*Note:* On OS X Yosemite, you will have to unload and load the kext for the serial1 to showup after every restart of the computer.

```
sudo kextunload -bundle com.apple.driver.AppleUSBFTDI 
sudo kextload -bundle com.apple.driver.AppleUSBFTDI
```

Here is a sample command to flash a binary.

`openocd -f interface/ftdi/particle-ftdi.cfg -f target/stm32f2x.cfg -c "program bootloader.elf verify reset exit"`

You can find documentation on OpenOCD commands [here.](http://openocd.org/doc-release/html/Flash-Commands.html#Flash-Commands)

Also a [great 5-step guide](https://medium.com/@jvanier/5-steps-to-setup-and-use-a-debugger-with-the-particle-photon-ad0e0fb43a34) written by one of our awexome community members @monkbroc

##Common Commands

####**Flash a binary using OpenOCD and FTDI shield:**
 
 (This is a stand alone command.)

`openocd -f interface/ftdi/particle-ftdi.cfg -f target/stm32f2x.cfg -c "program bootloader.bin verify reset exit"`

This particular command will flash the bootloader.bin to the Particle device, verify it, and then reset it.

_**NOTE:**_  If the above command fails, it could be due to following reasons:

  - The vanilla Particle device will fail to JTAG when breathing cyan. Put the device into DFU mode to overcome this.
  - The OpenOCD was not closed properly. To resolve this, kill the openocd process by doing `killall -KILL openocd`
 
 
####**Open a GDB port:**

`openocd -f interface/ftdi/particle-ftdi.cfg -f target/stm32f2x.cfg -c "gdb_port 3333"`

This command will open up a gdb port that you can telnet into from another terminal instance using a command like: `telnet localhost 4444` or `telnet 127.0.0.1 4444` 
Once successfully connected, you'll get an output like this:
```
> Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Open On-Chip Debugger
> 
```

---
_**NOTE:**_ All of the following commands can be accessed via telenet or through a arm gdb server.

####**Get flash memory information:**
*Command:* `flash info num`
Print info about flash bank num. The `num` parameter is a value shown by flash banks. This command will first query the hardware, it does not print cached and possibly stale information.

Example:

```
> flash info 0
device id = 0x20036411
flash size = 1024kbytes
#0 : stm32f2x at 0x08000000, size 0x00100000, buswidth 0, chipwidth 0
	#  0: 0x00000000 (0x4000 16kB) not protected
	#  1: 0x00004000 (0x4000 16kB) not protected
	#  2: 0x00008000 (0x4000 16kB) not protected
	#  3: 0x0000c000 (0x4000 16kB) not protected
	#  4: 0x00010000 (0x10000 64kB) not protected
	#  5: 0x00020000 (0x20000 128kB) protected
	#  6: 0x00040000 (0x20000 128kB) protected
	#  7: 0x00060000 (0x20000 128kB) protected
	#  8: 0x00080000 (0x20000 128kB) protected
	#  9: 0x000a0000 (0x20000 128kB) not protected
	# 10: 0x000c0000 (0x20000 128kB) not protected
	# 11: 0x000e0000 (0x20000 128kB) not protected
STM32F2xx - Rev: X
```
Note how it also gives the protection status of each flash sector on the STM32 microcontroller.

Another useful command is `flash list`

```
> flash list
{name stm32f2x base 134217728 size 1048576 bus_width 0 chip_width 0}
>
```

If the device is not in the DFU mode and is running the user code with JTAG enabled, you might have better luck executing flash memory commands by resetting and halting the device by doing: 

```
> reset halt
```

This will reset the device and halt the execution and the very beginning of the code.

---

####**Protect/Unprotect flash memory sectors:**
In order to be able to erase the flash memory, we first have to make sure that the flash sectors are unprotected. You can check the status by using the ```flash info 0``` command as described earlier.

*Command:* `flash protect num first last (on|off)`
Enable (on) or disable (off) protection of flash sectors in flash bank num, starting at sector first and continuing up to and including last. Providing a last sector of last specifies "to the end of the flash bank". The num parameter is a value shown by flash banks.

Example:
This will unprotect ALL of the flash sectors!!

```
> reset halt
> flash protect 0 0 11 off
cleared protection for sectors 0 through 11 on flash bank 0
> 
```

---

####**Erase the Particle Device:**
Once all/necessary flash sectors are unprotected, you can go ahead and do a complete/specific flash erase.

*Command:* `flash erase_sector num first last`
Erase sectors in bank num, starting at sector first up to and including last. Sector numbering starts at 0. Providing a last sector of last specifies "to the end of the flash bank". The num parameter is a value shown by flash banks.

Example:
This will erase ALL of the flash sectors!!

```
> reset halt
> flash erase_sector 0 0 11
erased sectors 0 through 11 on flash bank 0 in 16.078684s
>
```

To check the status of the erase, use `flash erase_check 0`

```
> flash erase_check 0
successfully checked erase state
	#  0: 0x00000000 (0x4000 16kB) erased
	#  1: 0x00004000 (0x4000 16kB) erased
	#  2: 0x00008000 (0x4000 16kB) erased
	#  3: 0x0000c000 (0x4000 16kB) erased
	#  4: 0x00010000 (0x10000 64kB) erased
	#  5: 0x00020000 (0x20000 128kB) erased
	#  6: 0x00040000 (0x20000 128kB) erased
	#  7: 0x00060000 (0x20000 128kB) erased
	#  8: 0x00080000 (0x20000 128kB) erased
	#  9: 0x000a0000 (0x20000 128kB) erased
	# 10: 0x000c0000 (0x20000 128kB) erased
	# 11: 0x000e0000 (0x20000 128kB) erased
> 
```

You can also erase specific addresses within the memory instead of complete sector erases by using the following command:

*Command:* `flash erase_address [pad] [unlock] address length`
Erase sectors starting at address for length bytes. Unless pad is specified, address must begin a flash sector, and address + length - 1 must end a sector. Specifying pad erases extra data at the beginning and/or end of the specified region, as needed to erase only full sectors. The flash bank to use is inferred from the address, and the specified length must stay within that bank. As a special case, when length is zero and address is the start of the bank, the whole flash is erased. If unlock is specified, then the flash is unprotected before erase starts.

