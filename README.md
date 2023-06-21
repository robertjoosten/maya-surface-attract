# maya-surface-attract
Maya surface attract node will pull the vertices of the meshes towards the 
closest point on the surface based on the `attract` and `bulge` attributes. 

## Installation
* Extract the content of the .rar file anywhere on disk.
* Build the plugin for a specific Maya version.
* Drag the surface-attract.mel file in Maya to permanently install the script.

## Compiling
Building the plugin using cmake will place the plugin in the plug-ins folder 
with a maya version divider. This will ensure the plug-in is compatible with 
the launched version of Maya.

1. Open Terminal
```
cd <PATH_TO_MODULE>
mkdir build/<MAYA_VERSION>
cd build/<MAYA_VERSION>
cmake -A x64 -T v141 -DMAYA_VERSION=<MAYA_VERSION> ../../
cmake --build . --target install --config Release
```

## Usage
Once the plug-in is build and loaded a new `surfaceAttract` node can be 
created by selecting the meshes and running the 
`cmds.deformer(type="surfaceAttract")` command. 

<p align="center"><img src="icons/surface-attract-network-example.png?raw=true"></p>

After this the surface needs to be manually connected into the `inputSurface` 
attribute on the node.

<p align="center"><img src="icons/surface-attract-scene-example.png?raw=true"></p>

The `attractRampU` and `attractRampV` ramp is used to generate multiplier 
values for the parameter of the closest point on the surface. This can 
come in handy to generate a zipper effect. The `attractRamp` ramp is used 
to generate multiplier values for the falloff allowing for a smoother transition.

The `bulgeDistance` and `bulgeStrength` values are used to control the overall 
bulge effect with `bulgeRamp` being used to control the falloff.

<p align="center"><img src="icons/surface-attract-attribute-example.png?raw=true"></p>

Smoothing operations are in place to smooth vertices along the distance. For 
this the `smoothingStep` and `smoothingIterations` attributes can be used.