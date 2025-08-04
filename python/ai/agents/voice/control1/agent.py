from agents import Agent, function_tool
from agents.extensions.handoff_prompt import prompt_with_handoff_instructions

@function_tool
def turn_on_the_red_led():
  """
  Turns on the red LED.
  """
  print("Red LED turned on.")

@function_tool
def turn_off_the_red_led():
  """
  Turns off the red LED.
  """
  print("Red LED turned off.")

@function_tool
def turn_on_the_green_led():
  """
  Turns on the green LED.
  """
  print("Green LED turned on.")

@function_tool
def turn_off_the_green_led(): 
  """
  Turns off the green LED.
  """
  print("Green LED turned off.")

@function_tool
def turn_on_the_yellow_led():
  """
  Turns on the yellow LED.
  """
  print("Yellow LED turned on.")

@function_tool
def turn_off_the_yellow_led():
  """
  Turns off the yellow LED.
  """
  print("Yellow LED turned off.")

control_agent = Agent(
  name="VoiceControlAgent",
  instructions=prompt_with_handoff_instructions(
    "You are a voice control agent. You're speaking to a human, so be polite and concise. You can control a red, green and yellow LED light. Use the tools provided to turn it on or off."
  ),
  tools=[turn_on_the_red_led, turn_off_the_red_led, turn_on_the_green_led, turn_off_the_green_led, turn_on_the_yellow_led, turn_off_the_yellow_led],
  model="gpt-4o-mini"
)
