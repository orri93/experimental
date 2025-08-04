import os
import asyncio

import numpy as np

from agents import Agent, function_tool
from agents.extensions.handoff_prompt import prompt_with_handoff_instructions
from agents.voice import AudioInput, SingleAgentVoiceWorkflow, SingleAgentWorkflowCallbacks, VoicePipeline

from dotenv import load_dotenv

from util import AudioPlayer, record_audio

load_dotenv()

audio_input_device = int(os.getenv("AUDIO_INPUT_DEVICE", 0))
audio_output_device = int(os.getenv("AUDIO_OUTPUT_DEVICE", 0))
samplerate = float(os.getenv("SAMPLERATE", 44100))

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

agent = Agent(
  name="VoiceControlAgent",
  instructions=prompt_with_handoff_instructions(
    "You're speaking to a human, so be polite and concise. You can control a red LED light. Use the tools provided to turn it on or off."
  ),
  tools=[turn_on_the_red_led, turn_off_the_red_led],
  model="gpt-4o-mini"
)

class WorkflowCallbacks(SingleAgentWorkflowCallbacks):
  def on_run(self, workflow: SingleAgentVoiceWorkflow, transcription: str) -> None:
    print(f"[debug] on_run called with transcription: {transcription}")

async def main():
  pipeline = VoicePipeline(workflow=SingleAgentVoiceWorkflow(agent, callbacks=WorkflowCallbacks()))

  audio_input = AudioInput(buffer=record_audio(audio_input_device, samplerate))

  result = await pipeline.run(audio_input)

  with AudioPlayer(device=audio_output_device, samplerate=samplerate) as player:
    async for event in result.stream():
      if event.type == "voice_stream_event_audio":
        player.add_audio(event.data)
        print("Received audio")
      elif event.type == "voice_stream_event_lifecycle":
        print(f"Received lifecycle event: {event.event}")

    # Add 1 second of silence to the end of the stream to avoid cutting off the last audio.
    player.add_audio(np.zeros(24000 * 1, dtype=np.int16))

if __name__ == "__main__":
  asyncio.run(main())
