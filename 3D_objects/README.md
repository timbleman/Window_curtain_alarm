# 3D objects and mounting

This subfolder stores 3D objects used to mount and build the curtain closer alarm.
The objects have been designed using Fusion360 and saved as STLs. Step files can be provided if desired.

## Project structure
These directories contain the 3D models:
* /electronics_mount/: Mounts for the board and the motor. Also includes the pulley and belt components.
* /motor_and_belt/: Components to install the Nema 17 and gt2 belt.
* /mount_curved_window/: Mounting brackets for a curved window. Will need to be adapted to your setup.
* /mount_rail/: Basic mounts for the rail. Includes end stop mount.

## Installation
### Needed components
You will need metric m4 screws of the following lengths: 10mm, 16mm, and 30mm. M3x6mm screws are also needed. You will also need square m4 nuts with dimensions of 7x7x2mm. The curtain rail has a diameter of 12mm. The flat u-shaped rail to mount all the components can be found in most German hardware stores. A 6mm wide gt2 belt and 684zz bearings are needed to move the curtain. Please look up the required electronic components in the wiring diagram.

### Printing
As many of the components are under constant (but not very big) load, I would go with PETG instead of PLA, due to possible creep. The optimal print orientation should be obvious for most parts. Supports should only be needed for the belt connector. An end cap can be printed using TPU to prevent the metal curtain rail from directly hitting the wall. The beam holders require the m4 nuts to be inserted while printing. Remember to add pauses in the slicer!

### Installing everything
Cut the rails to the appropriate length and mount printed components using the m4 screws and nuts. Insert the curtain.
Mount the motor, the pulley, the roller with inserted bearings, and the gt2 belt. Tighten the belt either using the belt connector or the sliding motor mount. Install all electronic components.
