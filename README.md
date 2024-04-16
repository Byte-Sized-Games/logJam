# Logger

(Un)apologetically Canadian rhythm/puzzle/adventure game for your arcade cabinet/smart fridge/laptop. Written w/ RayLib.

## Features

### 🦾 Code

- c++ 20 with descriptive variable names and comments if you feel like it 🤓
- [conventional commits](https://www.conventionalcommits.org/en/v1.0.0/) to describe changes in git
  
### ⚙️ Technologies

- CMake as build system
- [RayLib](https://www.raylib.com/) for graphics
- [SQLite](https://www.sqlite.org/) for the song & score database

## Building this project

First, clone the repo, and then proceed to setup.

**NOTE:**
This project does not use a particular build system, instead choosing to work through a system of scripts.
Please follow the following instructions carefully.

### Setup

This project is built with a just script, and thus needs the [just](https://just.systems/) program to compile.

> This program can also be compiled manually, but it takes a lot of work.
> Just is system agnostic, and speeds up & simplifies the overall build process.

As well, being a c++ program, the project requires a c++ compiler as well.
It defaults to clang, but can be changed to use gcc as well.
Please follow the install instructions for your operating system of choice to install it.
Also, different libraries are needed for different operating systems and are included in the [lib/](lib/) folder.
When running the just script be sure to specify which operating system you use, so that the right libraries are linked at compile time.

### Building

Once setup is complete, run `just build` in the root of the repository.
If all works well, a binary output for your system will be found in `bin/out`.

**Note:** This project was made on linux and macos.
We can not promise compatibility with other systems out of the box.
Sorry.
We'll help if an issue is posted, but without access to other operating systems, there is no promise that we'll fully support operating systems outside of unix based ones.

More build documentation at a future date...

Binaries will be made available on the releases tab and an itch.io page once the game nears completion.
If you are willing to wait, this is the better option, since there will not really be much game to play until v1.
