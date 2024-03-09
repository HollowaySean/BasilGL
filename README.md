# Basil Rendering Library

C++ library for simplified setup & management of OpenGL shader-based texture rendering

## Usage

Installation, build, and usage instructions will be fleshed out in upcoming release.

## Release Plan

0. ~~Basic window, support for simple ray tracer~~
1. First "usable" build, with a couple examples
   - Support for a pared down clone of "shadertoy"
      - Including various default inputs
   - ~~Ease of use for example setup~~
      - Specifically builders for public facing classes
   - Fill in gaps from P0
      - ~~Revisit Frame directory to fix and simplify~~
      - GLTexture extension
      - ~~Better constructors for GL* classes~~
      - ~~Thread-independent logging test utilities~~
      - Turn thread-dependent variable into pattern
      - Consolidate compile-time defaults and test directives
   - Hot reload shader pane?
   - Data model for setting uniforms & textures?
   - One or two actual examples
2. CI/CD and build system improvements
   - This phase is essentially the "v1.0" release, with user-friendly documentation and build systems
   - Wishlist:
      - A real README
      - Multi platform support
      - Installation
      - Auto releases with tags, binaries, and Doxygen pages
      - More Github Actions automations, such as checking code coverage and updating dependencies
      - Compiler flags
      - Streamlined dependency intake
      - Visual Studio support
      - Github issues & projects
      - Package manager distribution
      - Separable tests
      - Complete VS Code support
3. Output system
   - The ability to save screenshots and video
4. Compute shader support
5. Builders
   - This phase completes a sort of "v1.1", of the features I really want to be sure to get in
6. Emscripten support
7. Metaprogramming system
   - These later phases are conditional on if I can figure out how to implement them, and are sort of stretch goals