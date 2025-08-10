# Wired-BTMouse
Return to [Projects](https://kadeenp.github.io/Projects/)

# Abstract 
This project uses an ESP32 to hijack the PS/2 Signals from the mouse and transmits them via Bluetooth. 

# Preamble
I really liked my old Logitech MX518 mouse. It was comfortable in my hand, had many useless buttons and above all, was racecar red. Except it had a USB cable. I figured I could convert the mouse into a wireless mouse by plugging an ESP32 into it. Little did I know that this thought would turn into a whole thing having me reverse engineer the PCB, and scavenge an old mouse.

# The Project
## My Situation
### The Mouse
I am using a Logitech MX518 circa 2008. The most important thing about this mouse is that its controller, the **CY7C63743-PC** is compatible with both **USB and PS/2**. This versatility is useful since not all ESP32 dev modules support USB. PS/2 is a much older device protocol - one that doesn't natively support a scroll wheel! One other thing to note is that the scroll wheel itself uses a special encoder, which I'll cover in an upcoming section. 

### The Scroll Wheel Encoder

## Schematics
### Controller
The controller uses a [CYC63743-PC]([url](https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/793/CY7C63722,23,43.pdf)) of which the datasheety can be found at Digikey.
