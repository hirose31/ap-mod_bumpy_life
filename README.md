ABSTRACT
================

mod_bumpy_life is Apache 2.2 module for setting random value to
MaxRequestsPerChild. You can specify range of MaxRequestsPerChild by
new directive MaxRequestsPerChildMin and MaxRequestsPerChildMax.


INSTALLATION
================

download from http://github.com/hirose31/ap-mod_bumpy_life

    # apxs -c -i mod_bumpy_life.c

or debug build:

    # apxs -c -Wc,-DDEBUG -i mod_bumpy_life.c


DOCUMENTATION
================

    Description: set the name minimum value of MaxRequestsPerChild
    Syntax:      MaxRequestsPerChildMin number
    Context:     server config
    Module:      mod_bumpy_life

    Description: set the name maximum value of MaxRequestsPerChild
    Syntax:      MaxRequestsPerChildMax number
    Context:     server config
    Module:      mod_bumpy_life

Example
----------------

    LoadModule bumpy_life_module   modules/mod_bumpy_life.so
    
    <IfModule bumpy_life_module>
    MaxRequestsPerChildMin  150
    MaxRequestsPerChildMax  200
    </IfModule>


COPYRIGHT & LICENSE
================

Copyright 2009 HIROSE, Masaaki

Apache License, Version 2.0

