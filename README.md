# Vulkan Shader Processor (vsp)

Vulkan shader processor is a small library made for automatically locating and compiling GLSL shader files to SPIR-V
and loading them into memory.

# Usage

This library does assume some naming conventions regarding its functionality.
The general syntax is `{filename}.{shadertype}`.


```
---------------------------------------------
Vertex shader                   =>      .vert
---------------------------------------------
Fragment shader                 =>      .frag
---------------------------------------------
Geometry shader                 =>      .geom
---------------------------------------------
Tessellation Control shader     =>      .tesc
---------------------------------------------
Tessellation Evaluation shader  =>      .tese
---------------------------------------------
Compute shader                  =>      .comp
---------------------------------------------
```

e.g.
```
ubershader.vert -> ubershader.vert.spv
ubershader.frag -> ubershader.frag.spv