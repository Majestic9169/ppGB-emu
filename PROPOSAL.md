<a id="readme-top"></a>

#  ppGB (plus plus Game Boy Emulator)

Proposal for Software Engineering Project by Tegan Jain, Spring 2025.

<details>
<summary>Table of Contents</summary>

1. [Title](#title)
2. [Abstract](#abstract)
3. [Technical Details](#technical-details)
4. [Schedule of Deliverables](#schedule-of-deliverables)
   - [Preparation Period](#preparation-period)
   - [Week 1](#week-1)
   - [Week 2](#week-2)
   - [Week 3](#week-3)
   - [Week 4](#week-5)
   - [Final Week](#final-week)
5. [About Me](#about-me)
   - [Development Experience](#development-experience)
   - [Other Experiences](#other-experiences)
   - [Why this project?](#why-this-project)
6. [Appendix](#appendix)

</details>

## Abstract

ppGB (plus plus GameBoy emulator) is a GameBoy Emulator written in C++. The GameBoy is a handheld game console developed by Nintendo in 1989. 
It was a revolutionary piece of hardware at the time, and its legacy still lives on today impacting the childhood of children across the world.
Over the years it has been thoroughly reverse engineered and studied, and today many people use it as a learning project to introduce themselves to computer architecture and hardware emulation.

The GameBoy Emulator is a multi-faceted project involving implementing essentially a tiny computer from scratch. One has to implement the memory, cpu, instruction codes, display, audio, and everything else required for the use to play the games they want. It should also be easy to configure and allow for remapping of keybinds. 

Due to the many moving parts and complexity of this project, proceeding with good systems in place is necessary so as not to overly complicate the project, therefore this is also a fantastic project to learn good design principles. 

This project aims to implement a basic featured Gameboy Emulator that will allow the user to play simple games like Tetris at their convenience, while exhibiting good software engineering practices like version control, build systems, documentation, etc.


<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Technical Details

The essential aspects of the emulator are the 

1. Memory: 8KiB RAM, 10 registers
1. CPU: 8-bit, 499 opcodes
1. Input/Output (Display, controls, audio etc)

- [Technical Specs](https://gbdev.io/pandocs/Specifications.html)
- [Instruction Codes](https://gbdev.io/gb-opcodes/optables/)

To implement Input/Output the use of a library will be necessary. I intend to use SDL3 (Simple DirectMedia Layer) for this

- [SDL3 Wiki](https://wiki.libsdl.org/SDL3/Tutorials/FrontPage)
- [Helpful Guide to getting started](https://github.com/t0wbo2t/sdl3_foundations)

On the software engineering aspect
- Git will be used for the version control system, following conventional commits as accurately as possible
- CMake will be used for the build system
- CI/CD for github releases
- Testing will be done with test roms from 
  - [Gekkio](https://gekkio.fi/files/mooneye-test-suite/)
  - [blargg](https://gbdev.gg8.se/files/roms/blargg-gb-tests/)
  - I will try to writeup github actions for CI tests using these if feasible
- Profiling reports will be generated using
  - prof
  - gcov
  - valgrind

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Schedule of Deliverables

**Deliverables**:
- Working memory and registers (MBCs will be implemented)
- Greater than 80% opcodes will be implemented (tested by opcode count and test roms)
- Functioning graphics
- Functioning user input
- ? Audio Output (Extra Feature if time permits)

Below is a tentative timeline of implementing these deliverables


<p align="right">(<a href="#readme-top">back to top</a>)</p>

### **Preparation Period**

I will be reading more about the GameBoy specifications. Side-by-side I will be planning out my project structure and setup my build tools. 

**Major achievements**: 
- [x] README.md template
- [x] Project structure decided
- [x] CMake implemented
- [x] Libraries installed
  - SDL3
- [ ] Pipelines implemented
  - for CI/CD of releases
  - for conventional commits


### **Week 1**

This will be spent implementing the memory and starting work on the CPU and instruction codes

**Major achievements**: 
- [x] Implement memory
  - choose between vector and array or any other option
  - implement registers in an easy to access manner[^1]
  - work on memory bank controllers
- [x] Begin implementing the CPU
  - implement around 20% of the opcodes
  - test against tetris rom

### **Week 2**

Will begin implementing the display and user input. Will expand the opcodes and memory functionality (OAM tables etc)

**Major Achievements**: 
- [ ] Functioning display and user input
  - using SDL3
  - basic games might be playable now
- [ ] Release v0.1.0
  - provided basic games are playable
  - around 60% of the opcodes should be implemented

### **Week 3**

Will expand all features so far, Implement audio functionality

**Major Achievements**
- [x] Progress in CPU
  - around 80% of the opcodes implemented
- [ ] Game audio should be working
- [ ] More games should be playable
  - Super Mario Land should function
  - Legend of Zelda should work

### **Week 4**

Testing of code, refactoring, updating docs, and other chores. 

### **Final Week**

Finish up the project, implement any final changes, and generate final technical reports. Finally submit and present the project.

## About Me


| Project Idea        | Gameboy Emulator                                |
| ------------------- | ------------------------------------------------|
| Name                | Tegan Jain                                      |
| Roll No.            | 23CS30065                                       |
| Github Profile Link | [Majestic9169](https://github.com/Majestic9169/)|

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Development Experience

I am experienced with Javascript and frontend frameworks like ReactJS and NextJS. I have built many websites using these frameworks, either alone or collaboratively

- [DAAO.ai](https://daao.ai)
  - Frontend for trading blockchain coins
  - NextJS
- [Livescore](https://github.com/KGP-Speedcubers/live-results/)
  - Website to display live results at unofficial speedcubing competitions
  - Not deployed, I just expose it on the local network when we need it
  - ReactJS

Besides frontend I am comfortabe with Linux and the shell, and have an interest in lower-level systems programming. Here are some related repositories

- [Dotfiles](https://github.com/Majestic9169/koss-selection-task)
  - My (old) configurations for my Arch Linux setup
  - With an accompanying [presentation](https://majestic9169.github.io/koss-selection-task/)
- [Chip-8 Emulator](https://github.com/Majestic9169/chip8)
  - Basic chip8 emulator written in C
  - Custom ROM written in the [Octo IDE](https://johnearnest.github.io/Octo/)

### Other Experiences

I am also involved in Cybersecurity, and often participate in CTF competitions. We recently conducted a CTF workshop you can find [here](https://github.com/kossiitkgp/KOSSCTF-2025/)

### Why this project?

Nintendo games have been a large part of my childhood, I have played many GameBoy Advance games on an emulator. I also have an interest in computer architecture. We use computers so often in our daily life, but can we really say we understand them until we code one?

I also wanted a project that would give me the opportunity to utilise C++ features that we have learnt in our Software Engineering and Systems Programming Lab course. 

## Appendix

**GameBoy Resources**
1. [GBdev.io](https://gbdev.io/)
1. [Pan Docs](https://gbdev.io/pandocs/About.html)
1. [GameBoy CPU Manual](http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf)
1. [GameBoy Instruction Set](https://gbdev.io/gb-opcodes/optables/)
1. [Imran Nazar: GameBoy Emulation in Javascript](https://imrannazar.com/gameboy-Emulation-in-JavaScript)
1. [Cinoop](https://cturt.github.io/cinoop.html)
1. [Jeremy Banks: Learning Rust by writing a GameBoy emulator](https://jeremybanks.github.io/0dmg/2018/05/23/getting-started.html)
1. [Coffee-GB](https://blog.rekawek.eu/2017/02/09/coffee-gb/)

**Software Engineering Resources**
1. [Semantic Versioning](https://semver.org/)
1. [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/)
1. [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
1. [CMake](https://cmake.org/getting-started/)

**Misc**
1. [Git-Animate](https://github.com/mitxela/git-animate)
1. [GameBoy GIF](https://codepen.io/heero/pen/DpBZzz)
1. [Numfocus GSoC Proposal Template](https://github.com/numfocus/gsoc/blob/master/templates/proposal.md)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- Some resources -->

[^1]: [Creative use of unions in C](https://cturt.github.io/cinoop.html) 
