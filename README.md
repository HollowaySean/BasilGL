# Basil Rendering Library

C++ library for simplified setup & management of OpenGL shader-based texture rendering

## Overview

Hello! Welcome to the repository of my newest project, the Basil graphics library. I'm excited to provide more information about it, but this README is currently under construction as I approach the first official release of this project. Check back soon for more information, or feel free to reach out to me.

### Project Goals

The following core tenets comprise Basil's design philosophy.

1. The primary target audience for the library is my future self, who is eager to start writing GLSL as quickly as possible, and has otherwise forgotten nearly everything else that I know now.
2. The secondary audience is any potential employer, peer, or stranger whom I would like to convince that I can write clean & modern C++ code.
3. The library will be supported by an amount of infrastructure that anyone with practical sense would call "excessive".
4. The library will provide feature support for every feature that I have either dropped from projects in the past, or migrated to pre-existing tooling such as the Unity engine in order to use.
5. Design patterns will be consistent and motivated, if highly opinionated.
6. Included examples should be instructive, useful, and impressive.

#### 1. Designing for My Future Self

I have made the following pointed assumptions about my future self, around which the project is designed:
* I will be working on either the latest Ubuntu LTS distribution or latest Windows version, or both, equipped with the latest GCC and/or MSVC releases.
* I will be developing with Visual Studio or Visual Studio Code.
* I will have forgotten everything about how to use OpenGL, or any of the related libraries such as GLEW, GLUT, or GLFW.
* I will have forgotten everything about CMake, or any C++ build system for that matter.
* I will want to get into writing GLSL shader code as quickly as possible.

#### 2. Writing C++ Code That I Don't Mind Showing Off

More than once since starting my foray into software development, a project of mine has attracted a much larger viewing audience than I could have expected. This always starts out as a great ego boost, until some well meaning person with ten times my experience and wisdom dives into the repository and discovers a pile of highly fragrant spaghetti code.

For this project, I want to the code to:
* Display an understanding of the features and use cases of C++11 through C++20 standards.
* Be clean, readable, self-documenting, and materially documented.
* Be at "second pass performant", code that has at least been profiled & examined once for efficiency of memory and speed.
* Provide clear and sensible instruction for a new user to install & use the library in various common C++ workflows.

#### 3. All the Infrastructure I've Ever Wanted

I was either born with, or raised into, the terrible affliction of absolutely loving process and automation. Since this is a personal project, nobody can talk me out of both including and inforcing all the bells and whistles I could ever want.

Major features include:
* Exhaustive testing, including unit tests with 100% line coverage enforced and render tests to check for regression.
* Exhaustive documentation of public classes & methods via Doxygen.
* Full adoption of GitHub features, such as Actions for automated testing, branch management, and publishing documentation, and Issues & Projects for planning.
* Extensive yet beginner friendly CMake build systems.

#### 4. Support for Features I've Previously Needed

In previous OpenGL projects, I have either limited scope or migrated to the Unity game engine due to the effort required to implement more mature features. I hope to eliminate these sharp corners for future projects.

Intended features include:
* Simplified loading of shader code and image files for textures.
* Built-in support for saving rendered content to image files or video.
* Built-in support for common useful shader inputs, such as the current time or the position of the mouse.
* Hot reloading of shader files, for live modification of GLSL code.
* Support for compute shaders.

#### 5. Consistent, Motivated, and Opinionated Design Patterns

One of the simultaneous highs and lows of diving deeper into the world of C++ is discovering that very few things are standardized in terms of recommended file structures, patterns, or toolchains.

Given this reluctant freedom, I have focused on using patterns and structures that make sense to my brain at its current level of experience and exposure to other technologies. To some degree, this is going to be reflective of patterns that I have had a lot of repetition with during my employment, with the project using Java-style file-per-class, or defaulting to a Builder pattern for the "foolproof" version of things.

Other choices will simply come down to taste, but in all cases, my main goal is to be consistent above all else.

#### 6. Instructive, Useful, and Impressive Examples

Adopting a new technology on promise and premise alone is a tough sell. I hope to include examples that really show off the worth of the Basil library, while also serving as an instruction manual, and a showcase. I have some things in mind, such as rewriting old projects using Basil to see how much effort is saved, or mocking up a duplicate of the functionality of Shadertoy, but more concrete plans will be made further down in the line in development, after more features begin to reach maturity.


## Usage

Installation, build, and usage instructions will be fleshed out in upcoming release.
