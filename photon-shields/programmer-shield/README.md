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

Here is a sample command to flash a binary.

`openocd -f interface/ftdi/particle-ftdi.cfg -f target/stm32f2x.cfg -c "program bootloader.elf verify reset exit"`

You can find documentation on OpenOCD commands [here.](http://openocd.org/doc-release/html/Flash-Commands.html#Flash-Commands)

Also a [great 5-step guide](https://medium.com/@jvanier/5-steps-to-setup-and-use-a-debugger-with-the-particle-photon-ad0e0fb43a34) written by one of our awexome community members @monkbroc
