# qtcreatorplugin

Postfix2InfixNotationConverter QtCreatorPlugin.

Needs implementation of QtCreator Plugin, which convert opened text file in QtCreator editor line by line from Postfix Notation (aka Reverse Polish Notation) to Infix Notation (see example below).

Evaluations scope: basic knowledges of English, git, std C++, Qt ecosystem.

ToBeDone (expected): C++ class which implement core of postfix to infix convertion, function `main` (application) which test it and Qt Plugin wrapper which use it.
Choose of work environment (kind of and version of operating system, toolchain and so on) is up to you, just point it to us.

Important Notes: needs build QtCreator from scratch to build plugin properlly.

Example:
    postfix notation	<=>   infix notation
    2 2 + 6 * 3 / 8 - 	<=> (2 + 2) * 6 / 3 - 8
