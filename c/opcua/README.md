# OPC UA Experiments using the [open62641](https://open62541.org/) library

## Build

#### Visual Studio 2019

Set open62541_DIR to the folder containing the open62541Config.cmake file

Example
```json
      "variables": [
        {
          "name": "open62541_DIR",
          "value": "C:/install/lib/open62541/lib/cmake/open62541",
          "type": "PATH"
        }
      ]
```

#### Visual Studio Code

Edit the .vscode/settings.json to configure the CMake with the correct parameters (see above).

Example
```json
{
    "cmake.configureSettings": {
        "open62541_DIR": "C:/install/lib/open62541/lib/cmake/open62541"
    }
}
```
