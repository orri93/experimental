import board
import digitalio

from agents import Agent, function_tool
from agents.extensions.handoff_prompt import prompt_with_handoff_instructions

_red_led = digitalio.DigitalInOut(board.D18)
_red_led.direction = digitalio.Direction.OUTPUT

_green_led = digitalio.DigitalInOut(board.D23)
_green_led.direction = digitalio.Direction.OUTPUT

_yellow_led = digitalio.DigitalInOut(board.D24)
_yellow_led.direction = digitalio.Direction.OUTPUT

@function_tool
def turn_on_the_red_led():
  """
  Turns on the red LED.
  """
  _red_led.value = True
  print("Red LED turned on.")

@function_tool
def turn_off_the_red_led():
  """
  Turns off the red LED.
  """
  _red_led.value = False
  print("Red LED turned off.")

@function_tool
def turn_on_the_green_led():
  """
  Turns on the green LED.
  """
  _green_led.value = True
  print("Green LED turned on.")

@function_tool
def turn_off_the_green_led(): 
  """
  Turns off the green LED.
  """
  _green_led.value = False
  print("Green LED turned off.")

@function_tool
def turn_on_the_yellow_led():
  """
  Turns on the yellow LED.
  """
  _yellow_led.value = True
  print("Yellow LED turned on.")

@function_tool
def turn_off_the_yellow_led():
  """
  Turns off the yellow LED.
  """
  _yellow_led.value = False
  print("Yellow LED turned off.")

control_agent = Agent(
  name="VoiceControlAgent",
  instructions=prompt_with_handoff_instructions(
    "You are a voice control agent. You're speaking to a human, so be polite and concise. You can control a red, green and yellow LED light. Use the tools provided to turn it on or off."
  ),
  tools=[turn_on_the_red_led, turn_off_the_red_led, turn_on_the_green_led, turn_off_the_green_led, turn_on_the_yellow_led, turn_off_the_yellow_led],
  model="gpt-4o-mini"
)