# qtcreatorplugin

Postfix2InfixNotationConverter QtCreatorPlugin.

Implementation of QtCreator Plugin, which converts opened text file in QtCreator line by line from Postfix Notation (aka Reverse Polish Notation) to Infix Notation (see example below).

Evaluation scope: `basic knowledge of English`, `Git`, `C++`, `STL`, `Qt ecosystem`

### TBD: 
1.    Implement core of postfix to infix convertion in C++ OOP style; 
2.    Implement standalone test application; 
3.    Implement Qt Plugin wrapper;

The choice of type of operating system, toolchain and so on, is up to you, just point it out to us.

Important Notes: You need to build QtCreator from scratch to be able to build plugin properly (we use Ubuntu 16.04, g++-4.9 and QtCreator 4.3.0).

Example:  
 
`    postfix notation	<=>   infix notation`

`    2 2 + 6 * 3 / 8 - 	<=> (2 + 2) * 6 / 3 - 8`

If you have any questions feel free to ask us immediately.
