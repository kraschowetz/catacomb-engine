# Catacomb Shading Language
```
# Table of Contents
1. What is CSL?
2. CSL under the hood
```
---
# 1. What is CSL?
**C**atacomb **S**hading **L**anguage is a shader preprocessing system used to implement `ubershaders` into `Catacomb Engine`.

CSL works as a wrapper for GLSL, where the engine holds the standard shaders (wich do all matrix, vector lighting calculations and other things as such) and allows the end-user to implement their own custom methods via the function `hooks`.

Because CSL is just a wrapper for GLSL, the end-user just need to be aware of all the variables that the standard shaders provide and, of course, should know how to implement the CSL hooks as a GLSL method.

CSL variables, methods and hook signatures are present in the engine documentation and, to some extent, in the engine source code.

# 2. CSL under the hood
under the hood, CSL is just a wrapper for GLSL, and roughly the GLSL processing processing consist of the following steps:

1. Read source .CSL file
2. Append .CSL contents to basic shader
3. Resolve all missing hooks
4. compile the full resolved file

## .CSL files
The .CSL file consists of all the user-implemented custom hooks. A file should also contain the **section delimeters**, wich, roughly, tell the CSL processor wich hooks belong to each shader;

A section delimeter, just like all (future) CSL directives is predecessed by an **@** symbol.

A basic CSL file should look like this:
```
// section delimeter
@section FRAGMENT

// the CSL processor will attach this method to its hook
// so the user just need to implement the function body
void post_process_hook()
{
	// sets all output color to red
	output_color = vec4(1, 0, 0, 1);
}
```
## the basic shaders
Catacomb has its basic shaders, that handle things like the MVP matrices, light calculation and other low-level-ish stuff.

Each basic shader is split in three parts: **preamble**, **vertex** and **fragment**.
The preamble is the most important part of the shader. It declares all hooks as glsl methods, declare all CSL variables and all CSL structs.

The vertex part handles all basic steps regarding the vertices that will be rendered, such as the MVP matrices calculation.

The fragment part handles all steps regarding the fragments (for simplicity, let's say each fragment is a pixel) that will be rendered, such as applying textures and calculating lights.

## hook resolving
After loading the CSL file and attaching it to the basic shader, the CSL processor looks for any un-implemented hook and adds a default no-operation function body to it.
without this hook resolving, every CSL file would need to implement every single hook, or else, the shader program would not compile.

after all theese steps, the shader is compiled normally.
