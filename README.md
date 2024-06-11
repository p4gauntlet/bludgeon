THIS REPOSITORY HAS BEEN MERGED INTO https://github.com/p4lang/p4c/tree/main/backends/p4tools/modules/smith

# [DEPRECATED] Bludgeon: The P4 Random Code Generator
Bludgeon is a tool to fuzz test P4 compilers. It generates [P4-16](https://p4.org/p4-spec/docs/P4-16-v1.2.0) code that compiles for various P4 architecture modules. The primary target of Bludgeon is the P4-16 reference compiler, [P4C](https://github.com/p4lang/p4c). 

## Requirements
Bludgeon is written as an extension to P4C. It has no other dependencies. First P4C must be installed. Instructions can be found [here](https://github.com/p4lang/p4c#dependencies).


## Install
Once P4C has been installed, Bludgeon can be installed as an extension. The following commands provide an example: 

    cd ~/p4c/
    mkdir extensions
    cd extensions
    git clone https://github.com/p4gauntlet/bludgeon
    cd ~/p4c/build
    make
Afterwards, the `p4bludgeon` executable can be found in the `p4c/build`folder. Bludgeon can be installed globally by running `sudo make install`.

## Usage
Generating P4 code with Bludgeon is straightforward. The tool currently supports three different back ends.

 - top: This is simple testing code to test the front end of a P4 compiler. P4C provides the `p4test` binary, which can compile such a program.
 - v1model: Code using the v1model architecture can be compiled with P4C's `p4c` command. It produces code for the [simple switch target](https://github.com/p4lang/behavioral-model/blob/master/docs/simple_switch.md) of the [behavioral model](https://github.com/p4lang/behavioral-model). 
 - tna: This produces code for the proprietary Barefoot Tofino compiler. The code has the same restrictions as the v1model to avoid exposing sensitive restrictions.

Code can be generated by running the command 

    p4c/build/p4bludgeon  --output out.p4 --arch v1model 
`--arch` specifies the target architecture (top, v1model, or tna) and is required.  `--output` specifies the name of the output file. If the flag is not provided, `out.p4` is emitted by default

