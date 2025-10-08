# Frequently Asked Questions

## General Questions
### What is this?
This is a first-of-its-kind reverse engineering project aiming to produce a functionally equivalent decompilation of Dance Central 3 for the Xbox 360. This process involves writing C++ that, when compiled, produces assembly that is as close as possible to the original assembly, effectively giving us the code used for the built game.

### Is this like that Sonic Unleashed PC port but for Dance Central 3?
No, the Sonic Unleashed project was a **re**comp, not a **de**comp.

A **re**comp entails taking the original assembly for the game, and recompiling it for a different platform (i.e. modern PCs). This is great if you just want to port the game, but it comes with the massive downside of human-unreadable code, which greatly limits the ability to truly understand what the game code does, or even modify it.

A **de**comp entails writing C++ code from the original assembly. While this approach takes significantly longer than a recomp, the main benefit is human-readable code that enables options/modifications that go far beyond what is already possible on the script side.

Since we are interested in learning more about how the game works (especially the song data format) and want to explore possible modifications in the future, a decomp is the best approach for us.

### An Xbox 360 decomp? How is this even possible?
[jeff](https://github.com/rjkiv/jeff) was actually created for the very purpose of parsing and splitting Xbox 360 games like Dance Central 3. While a decomp is actually not advised for the vast majority of Xbox 360 games due to them being built with [Link Time Optimization](https://en.wikipedia.org/wiki/Interprocedural_optimization), we got very lucky with Dance Central 3, since the build we are decompiling is debug, meaning no LTO. Plus, the build came with a map file, so we got function symbols too!

### What's the progress like so far?
You can view the progress [here](https://decomp.dev/rjkiv/dc3-decomp). You will see the overall matched %, as well as each individual file's %. You can even compare this decomp to other active decomps!

## Technical Questions
### How can I contribute?
Please see [the guide found here](CONTRIBUTING.md). We are always looking for contributors to push this project forward.

### Why do so many class names start with "Ham"?
Dance Central's internal codename was "Project Hammer" - "Ham" was an abbreviation of that name. Unfortunately, there is no known relation between this game and the meat.

### Will this project let me make my own custom, mocapped routines?
*In theory*, once the appropriate parts of the game are decompiled and fully matched, you can use this codebase to learn what kind of data the game expects, make your own motion capture, and try to integrate it into the game.

### Are there plans for a PC port?
Let's preface this by saying that decompilation projects are *NOT* ports. With that being said, this is a very rare case where the people working on the decompilation project are also those interested in porting the codebase to more platforms.

Keep in mind this process takes an *extremely* long time, and we will not be making any attempt at a port until the game is fully decompiled. If you really want a port to be made faster and you have the technical know-how (or are willing to learn), you should consider contributing to the project.

### What about Kinect integration?
The idea is that the eventual:tm: PC port would add support for more cameras than the 360 Kinect. However...I'm gonna be real, that's a bridge we'll cross when we get there.
