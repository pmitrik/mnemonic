# mnemonic
example mnemonic task

NOTE: This was based on another article that I read from the Barr Group. I decided to make it more task like, meaning loop forever until turned 'off'. In this case, 'exit' or ctrl-c out of the loop. In a true embedded system, this would not happen nor would it be the only task. This was for educational purposes only.

This routine just explores the 3-letter mnemonic for command and control. There are currently only 3 commands with FRQ being the only one that really does something. The other two just return strings that indicate either a query or command action.

The idea behind this is to have all mnemonics be the same length and separated by a space. The total character count for this would be 4. By knowing the width of the mnemonic means that it can be broken out and each mnemonic can be read. This also means that by using an array function pointer, the intended command can be found and processed. The down side is that each command, in this example, has to have the same inputs and output.

Some nice todo's would be to break this out in a more object like manner. Instead of using the fgets function, create your own processing abilities. Add in more tasks. Experiement with having multiple groups of varying lengths of commands, i.e. 3-letter, 4-letter and 5-letter command groups. Port this over to a real embedded project.
