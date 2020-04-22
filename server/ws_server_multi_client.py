#!/usr/bin/env python
# WS server example

import asyncio
import json
import serial
import time
import websockets

#create serial object
ser = serial.Serial('/dev/ttyACM0', 9600)

#clear input serial buffers
ser.reset_input_buffer()

#create a set to store websocket connections
connected = set()

#Receiver runs continuously 
async def hello(websocket, path):
	#add each connection to the list
	connected.add(websocket)
	#use to enable continuous connection to socket
	async for message in websocket:
		y = json.loads(message)
		x = y["Axes"][0]
		x = int(x * 1000)
		print(x)
		ser.write('B{}\n'.format(x).encode())

#Sender is called once and runs in the background
async def world():
	while True:
		#check if data is available in the websocket
		if (ser.in_waiting > 0):
			data = ser.read(ser.in_waiting).decode('ascii')
			print (data)
		#removes unused sockets after they disconnect
		for websocket in connected.copy():
			try:
				await websocket.send(data)
			except: 
				connected.remove(websocket)
		await asyncio.sleep(1)

start_server = websockets.serve(hello, "0.0.0.0", 8765, ping_interval=None)
asyncio.get_event_loop().run_until_complete(start_server)
asyncio.ensure_future(world())
asyncio.get_event_loop().run_forever()
