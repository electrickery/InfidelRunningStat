# Infidel data statistics demo sketch for Arduino.

These files are the basic Arduino sketch interfacing the Infidel, LCD-display and a simple library implementing running statistics. 
The original library usage of double precision floats works better than the own hacked fixed-precision one I hacked, so I removed it.

The LCD display just shows the actual value, mean value, standard deviation, minimum value, maximum value and the number of measurements. 
The Serial console displays more fractional digits and also values from the original RunningStat library. The Min() and Max() were my addition.

Output of the Serial console looks like this:

    ...
    Diameter: 1.68 mm, Count: 4, Mean: 1.6862, Variance: 0.000012, Stdev: 0.0035, Min: 1.68, Max: 1.69,
    Diameter: 1.68 mm, Count: 5, Mean: 1.6846, Variance: 0.000023, Stdev: 0.0048, Min: 1.68, Max: 1.69,
    Diameter: 1.68 mm, Count: 6, Mean: 1.6833, Variance: 0.000028, Stdev: 0.0053, Min: 1.68, Max: 1.69,
    Diameter: 1.68 mm, Count: 7, Mean: 1.6823, Variance: 0.000031, Stdev: 0.0056, Min: 1.68, Max: 1.69,
    ...

* Source RunningStat library: https://www.johndcook.com/blog/standard_deviation/
* Infidel info: https://www.youmagine.com/designs/infidel-inline-filament-diameter-estimator-lowcost-10-24

ToDo:
* work out why the Arduino IDE doesn't want to work with the library header file. Including source code is not the correct way, but it appears to work.
* Allowing changing the measurement interval, now fixed to 5 seconds. There is a rudimentary command interpreter included, 
 but only commands for starting, pausing, muting, umuting and clearing are implemented.

F.J. Kraan, fjkraan@electrickery.nl

<img src="InfidelV0.3LCD.jpg" alt="Screenshot of the LCD showing actual diameter, mean (Avg), standard deviation (SD), minimum, maximum and measurement count.">
