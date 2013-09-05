Erlang S-Lang Bindings
======================

This is slang, an erlang interface to the amazing highly portable tty
interface that gave us such nice tty applications as mutt and slrn

It's distributed as an erlang application (without a start mod) which makes
it possible to integrate into a larger build environment.
We use this at bluetail to have a terminal application onto our
actual target machine. It can be used to to anything that's possible
to do with the slang lib itself.

It's know to compile and run with slang version 2.2.4.
The API is one-to-one withe the normal C-api to slanglib.

Install Prerequisites
---------------------

    $ sudo apt-get install libslang2-pic

NOTE: make sure you build eslang_drv.so linking against static libslang_pic.a

Credits
-------

* Claes Wikstrom
* David N. Welton
* Robin Haberkorn
* Maxim Sokhatsky
* Vitaly Takmazov

OM A HUM
