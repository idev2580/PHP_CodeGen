# PHP_CodeGen
Extremely Small Automatic Code Generator using PHP.
This program is under GPLv3 License.

## How to Use
`./phpCodeGen [Directory]`
This command will Convert all "~~~.[extension].php" files to "~~~.[extension]".

## Argument(Directory) Rules
Directory must be given as No-trailing slash, without any special characters.
- /A/B/C      -> O
- /A/B/C/     -> X
- ./B/C       -> O
- ./B/C/      -> X
- "./B/C/"    -> X

## Used Library
Used PH7 library for embedding thread-safe PHP5.3 into this program.
Special thanks to PH7 developers.

https://github.com/symisc/PH7
(PH7 is under symisc public license, which allows redistribution in any form with its copyright and redistribution condition notice)

