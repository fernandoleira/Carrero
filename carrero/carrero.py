# ---------------------------------------------------------------------------------------
#
# carrero.py
# Author: Fernando Leira
#
# ---------------------------------------------------------------------------------------


import RPi.GPIO as GPIO
from picamera import PiCamera
import time
import cv2

# Functions for controlling the motors direction
def move_forward(delay):
	GPIO.output(pins["motorL_pos"], True)
	GPIO.output(pins["motorL_neg"], False)
	GPIO.output(pins["motorR_pos"], True)
	GPIO.output(pins["motorR_neg"], False)
	time.sleep(delay)
	
def move_backwards(delay):
	GPIO.output(pins["motorL_pos"], False)
	GPIO.output(pins["motorL_neg"], True)
	GPIO.output(pins["motorR_pos"], False)
	GPIO.output(pins["motorR_neg"], True)
	time.sleep(delay)
	
def move_right(delay):
	GPIO.output(pins["motorL_pos"], False)
	GPIO.output(pins["motorL_neg"], True)
	GPIO.output(pins["motorR_pos"], True)
	GPIO.output(pins["motorR_neg"], False)
	time.sleep(delay)
	
def move_left(delay):
	GPIO.output(pins["motorL_pos"], True)
	GPIO.output(pins["motorL_neg"], False)
	GPIO.output(pins["motorR_pos"], False)
	GPIO.output(pins["motorR_neg"], True)
	time.sleep(delay)

def stop_motors(delay):
    GPIO.output(pins["motorL_pos"], False)
    GPIO.output(pins["motorL_neg"], False)
    GPIO.output(pins["motorR_pos"], False)
    GPIO.output(pins["motorR_neg"], False)
    time.sleep(delay)

# Declare pins on the Raspberry PI
pins = {
	"motorL_pos": 17,
	"motorL_neg": 22,
	"motorR_neg": 23,
	"motorR_pos": 24,
}

# Setup for pinouts
GPIO.setmode(GPIO.BCM)
for key in pins.keys():
	GPIO.setup(pins[key], GPIO.OUT)
print("Pins on")

camera = PiCamera()
camera.resolution = (2500, 1900)
camera.framerate = 15
camera.brightness = 60
print("Camera on")

# Start recording
cv2.namedWindow("Preview")
camera.start_preview()

key = cv2.waitKey(0)
while key != 27: # ESC
	# Move Forward
	if key == 65362:
		print("Forward")
		move_forward(1)
    # Move Backwards
	elif key == 65364:
		print("Back")
		move_backwards(1)
    # Move Left
	elif key == 65361:
		print("Left")
		move_left(1)
    # Move Right
	elif key == 65363:
		print("Back")
		move_right(1)
	else:
		print(key)
		stop_motors(1)
		
	key = cv2.waitKey(0)

camera.stop_preview()

# GPIO cleanup
GPIO.cleanup()
cv2.destroyAllWindows()
