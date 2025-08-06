import os
import asyncio

import numpy as np

from agents import Agent, function_tool
from agents.extensions.handoff_prompt import prompt_with_handoff_instructions
from agents.voice import AudioInput, SingleAgentVoiceWorkflow, SingleAgentWorkflowCallbacks, VoicePipeline

from dotenv import load_dotenv

from util import AudioPlayer, record_audio
from agent import control_agent

load_dotenv()

audio_input_device = int(os.getenv("AUDIO_INPUT_DEVICE", 0))
audio_output_device = int(os.getenv("AUDIO_OUTPUT_DEVICE", 0))
audio_input_sample_rate = float(os.getenv("AUDIO_INPUT_SAMPLERATE", 24000))
audio_output_sample_rate = float(os.getenv("AUDIO_OUTPUT_SAMPLERATE", 24000))

class WorkflowCallbacks(SingleAgentWorkflowCallbacks):
  def on_run(self, workflow: SingleAgentVoiceWorkflow, transcription: str) -> None:
    print(f"[debug] on_run called with transcription: {transcription}")

async def main():
  pipeline = VoicePipeline(workflow=SingleAgentVoiceWorkflow(control_agent, callbacks=WorkflowCallbacks()))

  print("Voice Control Agent Started!")
  print("Press Ctrl+C to exit the application.")
  print("=" * 50)

  try:
    while True:
      print("\nReady for voice command...")
      
      # Record audio input
      audio_buffer = record_audio(audio_input_device, audio_input_sample_rate)
      
      # Check if any audio was recorded
      if len(audio_buffer) == 0:
        print("No audio recorded. Try again.")
        continue
      
      audio_input = AudioInput(buffer=audio_buffer)
      
      print("Processing voice command...")
      result = await pipeline.run(audio_input)

      print("Playing response...")
      with AudioPlayer(device=audio_output_device, samplerate=audio_output_sample_rate) as player:
        async for event in result.stream():
          if event.type == "voice_stream_event_audio":
            if event.data is not None:
              player.add_audio(event.data)
          elif event.type == "voice_stream_event_lifecycle":
            print(f"Received lifecycle event: {event.event}")

        # Add 1 second of silence to the end of the stream to avoid cutting off the last audio.
        player.add_audio(np.zeros(24000 * 1, dtype=np.int16))
      
      print("Response completed.")
      
  except KeyboardInterrupt:
    print("\n\nExiting Voice Control Agent. Goodbye!")
  except Exception as e:
    print(f"\nAn error occurred: {e}")
    print("Exiting Voice Control Agent.")

if __name__ == "__main__":
  asyncio.run(main())
