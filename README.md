Computer engineers are often responsible for designing and writing such software, including tools known as assemblers, which translate assembly-language programs into machine code. The machine code program can then be loaded into the machine’s memory for subsequent execution.

In those situations where the new machine is unable to run the assembler (for example, it is still under development), a cross-assembler is required.1 The cross-assembler runs on a second machine, producing a program that can be loaded and executed on the target machine or an emulator of the target machine.

This assignment requires the design, implementation, and testing of a two-pass cross-assembler for the Texas Instrument’s MSP-430 microcomputer.2 The design is to be implemented in a high-level language (either C or C++). It can run on the machine of your choice. All examples in class will be in C.

The assembler is to read MSP-430 assembly-language files containing assembly-language records and produce executable modules containing loadable MSP-430 machine code. It is to operate in two passes: the first pass builds the symbol table, while the second generates the executable module.

An assembly-language file consists of one or more records containing instructions and data for the assembler to translate into machine-readable records. The record is to be free-format, meaning that it has no fixed fields. All records have the same format, defined as follows:
Record = (Label) + ([Instruction | Directive]) + (Operand) + (; Comment)
Label = Alphabetic + 0 {Alphanumeric} 30
Instruction = * An instruction mnemonic, see section 2.1.2 *
Directive = * An assembler directive, described in section 2.1.3 *
Operand = * The operands associated with the Instruction or Directive *
Comment = * Text associated with the record – ignored by the assembler *
Alphabetic = [A..Z | a..z | _ ]
Alphanumeric = [A..Z | a..z | 0..9 | _ ]

Instructions and Directives should be treated as case insensitive (that is, the value can be upper case, lower case, or some combination thereof). However, a Label, if it exists, is case sensitive, meaning that the label Alpha is not the same as the label ALPHA.
The following are not supported by the assembler:
 External references (i.e., references to Labels in other files)
 Include files (i.e., external files to be “copied into” the program for assembly)
 In-line arithmetic expressions (i.e., operands containing arithmetic operators)

