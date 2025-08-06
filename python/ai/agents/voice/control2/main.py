import os
import asyncio

import numpy as np

import digitalio
import board

from agents.voice import AudioInput, SingleAgentVoiceWorkflow, SingleAgentWorkflowCallbacks, VoicePipeline

from dotenv import load_dotenv

from util import AudioPlayer, record_audio
from agent import control_agent
from input import button

load_dotenv()

audio_input_device = int(os.getenv("AUDIO_INPUT_DEVICE", 0))
audio_output_device = int(os.getenv("AUDIO_OUTPUT_DEVICE", 0))
audio_input_sample_rate = float(os.getenv("AUDIO_INPUT_SAMPLERATE", 24000))
audio_output_sample_rate = float(os.getenv("AUDIO_OUTPUT_SAMPLERATE", 24000))

button = digitalio.DigitalInOut(board.D4)
button.direction = digitalio.Direction.INPUT
button.pull = digitalio.Pull.UP

class WorkflowCallbacks(SingleAgentWorkflowCallbacks):
  def __init__(self):
    self.should_exit = False

  def on_run(self, workflow: SingleAgentVoiceWorkflow, transcription: str) -> None:
    print(f"[debug] on_run called with transcription: {transcription}")
    
    # Check if user wants to exit
    exit_phrases = ["exit", "quit", "stop", "goodbye", "bye"]
    if any(phrase in transcription.lower() for phrase in exit_phrases):
      print("Exit command detected. Preparing to exit...")
      self.should_exit = True

async def main():
  callbacks = WorkflowCallbacks()
  pipeline = VoicePipeline(workflow=SingleAgentVoiceWorkflow(control_agent, callbacks=callbacks))

  print("Voice control agent started. Press button to give commands.")
  print("Say 'exit', 'quit', 'stop', 'goodbye', or 'bye' to exit, or press Ctrl+C.")
  
  try:
    while True:
      print("\nWaiting for command...")
      
      # Record audio input
      audio_buffer = record_audio(button, audio_input_device, audio_input_sample_rate)
      
      # Check if we got any audio
      if len(audio_buffer) == 0:
        print("No audio recorded, continuing...")
        continue
      
      audio_input = AudioInput(buffer=audio_buffer)
      result = await pipeline.run(audio_input)

      # Play the response
      with AudioPlayer(device=audio_output_device, samplerate=audio_output_sample_rate) as player:
        async for event in result.stream():
          if event.type == "voice_stream_event_audio":
            # Convert audio data to int16 format if needed
            audio_data = event.data
            if audio_data is not None:
              # Always ensure we have int16 data
              if audio_data.dtype == np.float32:
                # Convert float32 to int16
                converted_audio = np.array(audio_data * 32767, dtype=np.int16)
              else:
                # Convert any other type to int16
                converted_audio = np.array(audio_data, dtype=np.int16)
              player.add_audio(converted_audio)
              print("Received audio")
          elif event.type == "voice_stream_event_lifecycle":
            print(f"Received lifecycle event: {event.event}")

        # Add 1 second of silence to the end of the stream to avoid cutting off the last audio.
        player.add_audio(np.zeros(24000 * 1, dtype=np.int16))
      
      # Check if user wants to exit
      if callbacks.should_exit:
        print("Exiting voice control agent...")
        break
        
  except KeyboardInterrupt:
    print("\nKeyboard interrupt received. Exiting voice control agent...")
  except Exception as e:
    print(f"An error occurred: {e}")
    print("Exiting voice control agent...")

if __name__ == "__main__":
  asyncio.run(main())
