import board
import digitalio

button = digitalio.DigitalInOut(board.D17)
button.direction = digitalio.Direction.INPUT
button.pull = digitalio.Pull.UP
