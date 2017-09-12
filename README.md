# Notation converter plugin

`Copyright (c) 2017 Vitali Parkhomenko`

`This code is licensed under the MIT license (MIT)`

`(http://opensource.org/licenses/MIT)`

### How to compile and deploy

You need to build QtCreator 4.3.1 from scratch to be able to build plugin properly (for build plugin used Windows 7, gcc 5.3.0 and QtCreator 4.3.1).

To compile and deploy this plugin, the project needs access to the Qt Creator sources and build. You should set the QTC_SOURCE and QTC_BUILD environment variables correctly for the plugin's build environment. If these are not defined, it uses the defaults set of author.

### How to install

Don't need install plugin after building. If USE_USER_DESTDIR environment variable are uncommented in notationconverterplugin.pro file, otherwise you should to copy the builded plugin along the path:

` <localappdata>/plugins/<ideversion>`

`    where <localappdata> is e.g.`

`    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later`

`    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux`

`    "~/Library/Application Support/QtProject/Qt Creator" on OS X`

### How to use

To convert Postfix Notation to Infix Notation entire opened text file in QtCreator, you have two ways:
1.  Use shortcut key `Meta+Ctrl+A`;
2.  Use the action in the edit menu `Edit > Notation Converter > Postfix to Infix > Current file`;

To convert Postfix Notation to Infix Notation selected text in an open text file in QtCreator, you have two ways:
1.  Use shortcut key `Meta+Ctrl+S`;
2.  Use the action in the edit menu `Edit > Notation Converter > Postfix to Infix > Selected text`;

If the text in the file does not change, this means that the postfix notation is not valid, otherwise changes in the text can be observed. If you want, you can save the converted text to a file ;)

### What is it for

Implementation of QtCreator Plugin, which converts opened text file in QtCreator line by line from Postfix Notation (aka Reverse Polish Notation) to Infix Notation.
