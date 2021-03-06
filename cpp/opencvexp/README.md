# GOS OpenCV Experiments

## Build

### Visual Studio 2019

Set Qt6_DIR, Qt6CoreTools_DIR, Qt6GuiTools_DIR and Qt6WidgetsTools_DIR to the CMake library folders of the Qt6 library.

Example
```json
    {
      "name": "Qt6_DIR",
      "value": "C:/Qt/6.0.2/msvc2019_64/lib/cmake/Qt6",
      "type": "PATH"
    },
    {
      "name": "Qt6CoreTools_DIR",
      "value": "C:/Qt/6.0.2/msvc2019_64/lib/cmake/Qt6CoreTools",
      "type": "PATH"
    },
    {
      "name": "Qt6GuiTools_DIR",
      "value": "C:/Qt/6.0.2/msvc2019_64/lib/cmake/Qt6GuiTools",
      "type": "PATH"
    },
    {
      "name": "Qt6WidgetsTools_DIR",
      "value": "C:/Qt/6.0.2/msvc2019_64/lib/cmake/Qt6WidgetsTools",
      "type": "PATH"
    }
```

Set OpenCV_DIR to the CMake library folder of the OpenCV library.

Example
```json
    {
      "name": "OpenCV_DIR",
      "value": "C:/lib/opencv/build/x64/vc15/lib",
      "type": "PATH"
    }
```
## Debug

### Visual Studio 2019

Set the PATH environmental variable to the directory containing the Dynamic-link libraries of the Qt6 and OpenCV libraries in the Debug and launch setting file launch.vs.json.

Example
```json
{
  "version": "0.2.1",
  "defaults": {},
  "configurations": [
    {
      "type": "default",
      "project": "CMakeLists.txt",
      "projectTarget": "face.exe (src\\face\\face.exe)",
      "name": "face.exe (src\\face\\face.exe)",
      "args": [ "0" ],
      "env": {
        "PATH": "C:\\Qt\\6.0.2\\msvc2019_64\\bin;C:\\lib\\opencv\\build\\x64\\vc15\\bin"
      }
    }
  ]
}
```

To specify Video Capture devices 