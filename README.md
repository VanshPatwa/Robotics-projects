# Line follower Robot
This project is a four-wheel autonomous line-following robot developed using an Arduino Uno, an L298N motor driver, and five IR sensors. The robot is designed to detect and follow a line on the track by continuously reading the position of the line through the sensor array and adjusting the speed of the left and right motors accordingly.

The robot uses five IR sensors positioned across the front of the chassis to determine the deviation of the robot from the line. The sensor readings are processed by the Arduino, which calculates the required correction and controls the motors using PWM. A PID-based control approach is implemented to improve tracking stability, reduce oscillations, and allow the robot to negotiate curves and sharper turns more smoothly.

The project also explores the handling of different track conditions, including sharp turns and intersections. Different control strategies were tested and tuned to improve the robot's response and maintain reliable line tracking at higher speeds. The project was developed iteratively by modifying the control algorithm, sensor interpretation, and motor-speed parameters based on the robot's performance on the track.

The hardware consists of an Arduino Uno, an L298N motor driver, five IR sensors, four DC geared motors, a robot chassis, and a battery power supply. The Arduino is responsible for processing the sensor signals and generating the motor-control signals, while the L298N provides the required interface for controlling the DC motors.

The complete Arduino source code is provided in this repository along with photographs and other project documentation. A demonstration video is also provided to show the robot operating on the line-following track.

The robot is an ongoing project, with further improvements possible in areas such as PID tuning, high-speed tracking, intersection detection, and handling of more complex track patterns.

