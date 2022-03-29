# Wave Fractal

## Compiling with Visual Studio C/C++

```
cl -D_USE_MATH_DEFINES wf.c
```

## Create the output

```
wf 1000 0.028374 > wf.tsv
```

## Plotting the output with GNUPlot

```
plot 'wf.tsv' w l
```
