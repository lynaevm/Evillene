# Evillene
Arduino code created for Muny Wiz Evillene, designed for the adafruit Circuit Playground Express.

Code:

There are 2 versions of this code.  Evillene 2 is the version that is currently loaded on the board.  Evillene was the original that I did most of my testing with. The original file was planned to run the 2 strings of lights off a single data pin.  Evillene 2 was an attempt to troubleshoot the problem by running the 2 strands off separate pins.  Either approach is acceptable. Ideally the 2 strands mirror each other as opposed to being treated as one long string.  I also did some tinkering with the Max_power definition in troubleshooting, if that looks different between the versions.

Note that both files in the folder are needed for the code to run, be sure to include the State 2.

The lights have 4 different states and are controlled by the 2 buttons built into the board.  It loads in as case 99, the off position.  The left button cycles between State 1, 2, and 3 and the right button returns you to case 99 at any point.

case99 is the "off" setting.  It was intended to be how it was set backstage when no animation was wanted.  3 neopixels on the board itself were lit up to help her find the correct button.  Three pixels on the strand also blink on and off on a slow delay.  These pixels were hidden under an opaque shrink wrap and meant to be hidden inside the skirt.  Their only purpose is to put the occasional power draw on the battery so it won't go into sleep mode.

State 1 is the low setting for book scenes.  Random individual pixels flash on then slowly dim. Quite subtle.  The neopixels on the board show pink on one side as a "status" indicator, with blue pixels on the other side pointing towards the off button.

State 2 is the animated setting for her song.  The colors in the faces morph between several defined color palettes. The neopixels on the board show cyan on one side as a status indicator, with blue pixels on the other side pointing towards the off button. This code is in the separate file.

State 3 is for the high point of her song.  It is simply 3 racing colorwipes going down each strand.  The neopixels on the board show green on one side as a status indicator, with blue pixels on the other side pointing towards the off button.

This code ran perfectly on each strand independently when the common ground and data wires were alligator clipped to the board.  When I attempted to solder it to the board permanently, I got complete failure or terrible flickering. 

Lights:

There are 2 identical strands of lights.  Nearly all are Adafruit brand Neopixels (WB2812b) at 60 lights per meter.  One segment is the Alitov brand from Amazon, but in my testing I had no compatability problems.  Each strand has 3--30--30--30--30--30. pixels for a total of 153.  They are connected in series with 22gauge stranded wire.  Eash strand is powered independently through a micro usb cable from a standard powerbank which puts out 5v power at 2.4A.  The usb feeds into a breakout board.  A 1000uf capaciter was placed between power and ground and then connected to the lights.  One of the strands has the common ground coming from the breakout board and the other has the common ground coming from the lights -- another troubleshooting attempt.

Theoretically, all that remains to do is thread the 2 ground and 2 data wires through the holes on the case, solder them to the board so that the wires can lay quite close to the back side, tuck the board into the case (there is a small felt padding on the side of the case opposite the opening and 2 more small pieces that go on top of the case so that the board is stable inside the case) put the lid on the case so that the 2 molded nubs hit the 2 buttons built into the board.  The lid does screw onto the case, but I found it worked better to snap the top on as opposed to try to twist it since it tends to catch on the buttons otherwise. The board itself is powered by a smallish lipo battery that I  planned on just taping to the back of the case. The lights on the board can shine through the white ring on the case.

This is the point where everything went wrong.  When I try to solder to the board it all ceases to work.  Much troubleshooting and anguish ocurred here.

After assembly, the case needs to be passed to the outside of the skirt through the small slit on the upper right side so the button is accessable on the outside.  I was planning to tack the case for the battery pack to the inside of the skirt to hold it at the correct place in relation to the  board case.

Things I've tried:
Splitting the strands between 2 pins instead of running both off 1 pin.
Adding a diode between power and the first light: lights ceased to work and the battery no longer stayed awake.
Moving the common ground from the end of the light strip to the ground of the power source: problem seemed to be neither better nor worse.
Remaking my power connectors: micrometer shows good power, didn't solve problem 
Changing power to lipo battery: didn't help.
Lighting up only 30 of the lights to see if the flickering was caused by lack of full amperage: flickering sometimes happens when only first 3 lights are on so doesnt' seem to be an amperage problem
Soldering the wire to the board wrapped several times through the hole.  Soldering the wire to the board with just a single hook in the wire.  Filling in the hole with solder and then soldering on the wire flat.  Soldering to either top of bottom of the board.  Using fairly minimal solder and using a lot of solder. I would alligator each wire to the board and test it, then solder one wire and test again.  I never made it further than 1 wire attached before the terrible flickering started.  Tried with both 63/37 solder and 60/40 kester solder.
Note that I have put new heat shrink tubing in places where I took apart my original connections and redid them but have not yet shrunk them, so that what is happening under them can be seen.
