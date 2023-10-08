# Program Structure

We created a file called macqueen.cpp that handles interfacing with the motors, ultrasonic sensors, and patrol (line/light) sensors. 
This was based on the macqueen.ts file in the example.

Our main.cpp file handles the rest of the logic. We have used #define directives to if the robot goes forward at intersections,
or alternates between left and right, or follows a maze.

# Line Following Algorithm

## Following a Line

We have decided to get the robot to track the edge of the line. 
When it is turning left, it tracks the left hand side of the line.
When it is turning right, it tracks the right hand side of the line.

it alternates between turning left and right. When both the patrol sensors are not detecting anything,
it turns off the line. When the outer patrol sensor fires (detecting white), it turns back towards
the line. 
If the inner patrol sensor fires and the outer patrol sensor isn't detecting anything, then that
is a signal that we have hit an intersection, as the intersections have a white square in the center.
We use different routines in this case to get it to turn right or left.
If both patrol sensors fire at the same time, then that is a signal that we have reached a dead end,
or the track may have turned really quickly, So we veer back towards where the line was.
(at a dead end this results in us spinning in a circle until we find the other side of the track going back.)


## Debugging - using the display

To make it easier to debug, we use the 5x5 matrix on the microbit to display if the microbit is currently 
trying to follow the left or right side, or if it has found an intersection.


# Advanced Features

## Compass 
We use the compass to detect if it is turning left or right. When the microbit needs to turn left or right, we read 
the current heading (direction angle) from the compass. Then we try to turn until the compass heading has changed by
90 degrees. sometimes this breaks, or the compass is inaccurate. So we have a min and max turning time as well, which
is based on the rotation speed of the wheels. This is our fallback for if the compass is inaccurate and doesn't provide a
reasonable angle.

## Ultrasonic sensor

The ultrasonic sensor takes some time to wait for the ping to come back, so we don't poll it as much as we do the 
patrol sensors. However, we still check frequently and when the ultrasonic sensor detects and obstacle we wait for a 
while until the obstacle moves.

## Maze Following

When you enable the "follow_maze" flag, the robot will turn left at every intersection. this is a classic maze following Algorithm
(hug the left wall) and allows it to get to the end of the maze.