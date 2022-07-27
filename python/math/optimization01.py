import PySimpleGUI as sg

width = 1200
height = 800

top_margin = 10
bottom_margin = 10
left_margin = 10
right_margin = 10

x = left_margin
y = bottom_margin

timeout = 5000

layout = [
  [
    sg.Graph(
      canvas_size=(width, height),
      graph_bottom_left=(0, 0),
      graph_top_right=(width, height),
      key="graph"
    )
  ]
]

window = sg.Window("rect on image", layout)
window.Finalize()

graph = window.Element("graph")
graph.DrawRectangle(
  (left_margin, bottom_margin),
  (width - left_margin - right_margin, height - top_margin - bottom_margin),
  line_color="blue")
graph.DrawImage(filename="foo.png", location=(0, 400))


cid = None

while True:
  event, values = window.Read(timeout=timeout)
  if event == "__TIMEOUT__":
    if cid is not None:
      graph.DeleteFigure(cid)
    cid = graph.DrawCircle((x, y), 10, fill_color="red")
    x += 10
    y += 10
  if event is None or event == sg.WIN_CLOSED:
    break
