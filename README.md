# SMUE5
Since this is gaining some traction on Twitter, I've decided to write this readme to explain what's going on and what needs to be done.
And especially since I did this as an experiment for myself to make a plugin that works with a 3rd party library, I don't really have any intentions to finish it.

Feel free to fork it and improve it though!

TL;DR this is a very simple experiment that aimed to put [libsm64](https://github.com/libsm64/libsm64) into Unreal Engine 5.

## Features as they are right now
 - Mario's atlas texture is made into a UTexture2D that can be referenced by a material.
 - Mario's model is made and created on SMUE5Character's Procedural Mesh Component. UE5 introduced some new ways to create procedural meshes that may be much better than what I've used (and was comfortable with at the time).
 - Mario can move around and the camera will follow.
 - Mario's position and vertex positions are transformed from the original's right-handed Y up space to Unreal's left-handed Z up space.

## Issues and things it doesn't have
 - Framerate of [libsm64](https://github.com/libsm64/libsm64) (at least the version I've used) is locked at 30, and it doesn't look like it has been changed since. So you may be better off limiting the UE game's framerate to 30 so that it doesn't feel weird.
 - No collision. I've got no clue how the get collision for the entire world in Unreal. Feel free to modify the [ULibSM64Component::SM64_BuildCollision](https://github.com/TheEnbyWitch/SMUE5/blob/master/Plugins/LibSM64UE/Source/LibSM64UE/Private/LibSM64Component.cpp#L51) function to do it properly. Keep in mind that you'll have to convert Unreal coordinates to Mario64's coordinate system!
 - No velocity vectors. This will not work properly with TAA or Motion Blur. Disable both of these.
 - [libsm64](https://github.com/libsm64/libsm64) has a hard limit for world coordinates (16-bits on each axis: from -32768 to 32767).
 - Absolutely no interaction or events you can hook up to with the Unreal game world. That one may need modifications made to [libsm64](https://github.com/libsm64/libsm64) itself.

## Frequently Asked Questions
### Is this Mario's movement reimplemented in Blueprints?
No. All of that handled is within the [libsm64](https://github.com/libsm64/libsm64) library. Would be a fun thing to try though (but not for me).
### Where do I put the ROM?
Content folder of the project (`/Content/`). It also needs to be named `sm64.z64`, and it needs to be the US ROM specifically (as per requirements of [libsm64](https://github.com/libsm64/libsm64)).
### Will you finish it?
No.

## Credits
- Me (Vivi aka TheEnbyWitch or ViviTheEnbyWitch) - Putting it in Unreal for fun.
- [libsm64](https://github.com/libsm64/libsm64) - For actually drawing the rest of the owl. (aka putting the mario movement into a library that can be used anywhere)
