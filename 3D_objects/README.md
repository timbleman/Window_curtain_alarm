# 3D objects and mounting

This subfolder stores 3D objects used to mount and build the curtain closer alarm.
The objects have been designed using Fusion360 and saved as STLs. Step files are also included.

## Project structure
These directories contain the 3D models:
* /electronics_mount/: Mounts for the board and the motor. Also includes the pulley and belt components.
* /motor_and_belt/: Components to install the Nema 17 and gt2 belt.
* /mount_curved_window/: Mounting brackets for a curved window. Will need to be adapted to your setup.
* /mount_rail/: Basic mounts for the rail. Includes end stop mount.

## Installation
### Needed components
You will need metric m4 screws of the following lengths: 10mm, 16mm, and 30mm. M3x6mm screws are also needed. You will also need square m4 nuts with dimensions of 7x7x2mm. The curtain rail has a diameter of 12mm. The flat u-shaped rail to mount all the components can be found in most German hardware stores. A 6mm wide gt2 belt and 684zz bearings are needed to move the curtain. Please look up the required electronic components in the wiring diagram. Have a look at the bill of meterials below, to find needed components.  

#### BOM
| Part name               | Quantity | Price [€] |
|-------------------------|----------|-----------|
| **Electronics**         |          |           |
| ESP8266 NodeMCU ESP-12F | 1        | 5         |
| NEMA17 (max depth 34mm) | 1        | 7         |
| TMC2209 V2              | 1        | 3         |
| Board                   | 1        | 2         |
| R 100 Ohm               | 1        | 0.01      |
| R 1 kOhm                | 3        | 0.01      |
| R 10 kOhm               | 2        | 0.01      |
| C 100 uF                | 1        | 0.01      |
| C 220 uF                | 1        | 0.01      |
| C 100 nF                | 2        | 0.01      |
| LED 5mm                 | 1        | 0.01      |
| Button 12x12            | 1        | 0.01      |
| Pin headers m/f         | Several  | 0.2       |
| Micro USB Cable         | 1        | 5         |
| USB wall plug           | 1        | 5         |
| **3D Printing**         |          |           |
| Filament (PLA/PETG)     | 200g     | 4         |
| **Screws**              |          |           |
| M3x6                    | 4        | 0.1       |
| M4x4 grub screw         | 1        | 0.1       |
| M4x10                   | 22       | 0.3       |
| M4x16                   | 4        | 0.1       |
| M4x30                   | 3        | 0.1       |
| M4 7x7x2 square nuts    | 29       | 0.2       |
| M4 washers              | 25       | 0.2       |
| **Mounting**            |          |           |
| GT2 belt 6mm            | 3m       | 3         |
| 684zz bearings          | 2        | 2         |
| Curtain rod 12mm        | 1.5m     | 5         |
| Rail 10001              | 1.5m     | 6         |
| **Total**               |          | ~50€      |

### Printing
As many of the components are under constant (but not very big) load, I would go with PETG instead of PLA, due to possible creep. The optimal print orientation should be obvious for most parts. Supports should only be needed for the belt connector. An end cap can be printed using TPU to prevent the metal curtain rail from directly hitting the wall. The beam holders require the m4 nuts to be inserted while printing. Remember to add pauses in the slicer! About 200g of Filament are needed.

### Installing everything
Cut the rails to the appropriate length and mount printed components using the m4 screws and nuts. Insert the curtain.
Mount the motor, the pulley, the roller with inserted bearings, and the gt2 belt. Tighten the belt either using the belt connector or the sliding motor mount. Solder components to the board. Install all electronic components. Mount the rail to your desired location and connect a micro USB cord to the NodeMCU.
