Extremely Small Automatic Code Generator using PHP.
This program is under GPLv3 License.

Below Command will convert 
    *.[file_extension].php file to
    *.[file_extension]

./phpCodeGen [Directory]

Directory must be given as No-trailing slash
, without any special characters.
    ex )    /A/B/C      -> O
            /A/B/C/     -> X
            ./B/C       -> O
            ./B/C/      -> X
            "./B/C/"    -> X

Used PH7 library for embeddin thread-safe PHP5.3 into this program.
Special thanks to PH7 developers.