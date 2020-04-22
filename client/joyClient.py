#!/usr/bin/env python

import asyncio
import pygame
import websockets
import json
import time

#initialize pygame
pygame.init()
#initialize joystick
pygame.joystick.init()

#create and initialize one joystick object
stick = pygame.joystick.Joystick(0)
stick.init()

uri = "ws://192.168.1.9:8765"
#interval must be set to none or the client dies without a regular response from server
websocket = websockets.connect(uri, ping_interval=None)

async def joy(): 

	global websocket
	async with websocket as websocket:
		while True:
		#request update of joystick status
				pygame.event.pump()
				axes = stick.get_numaxes()
				axis_values = [0] * axes
				for i in range(axes):
					axis_values[i] = stick.get_axis(i)
				#send all four axis value packaged as json
				await websocket.send(json.dumps({"Axes": axis_values}))
				#delay necessary to prevent overload of receiver, adjust as necessary
				time.sleep(.1)

asyncio.get_event_loop().run_until_complete(joy())
pygame.joystick.quit()
