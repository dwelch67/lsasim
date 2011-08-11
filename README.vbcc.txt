
vbcc Dr. Volker Barthelmann´s C Compiler

http://www.compilers.de/vbcc.html

At the bottom of the page there is a link to the current compiler
sources.  Currently the link is:

http://www.ibaug.de/vbcc/vbcc.tar.gz

Download this file and extract in the main lsa directory such that
it overlays the vbcc directory that is already there.

cd vbcc
mkdir bin # should be one, just in case
make bin/vc
#there will be a number of warnings, at the time of this writing these
# are fine
make TARGET=lsa bin/vbcclsa
# when it asks are you building a cross compiler, answer with n for no
# even though we are, just say no, it makes it easier, and it just works.

And that is it you have a C compiler ./bin/vbcclsa.  Note that by default
it does not like // type C comments.  You have to specify -c99.

For the vbcc documentation you need a few more packages installed like
texinfo.  make doc/vbcc.pdf and it will actually make the pdf in the
main vbcc directory.  The command line options are mostly unfamiliar
to folks used to gcc or other compilers so be sure to get the docs from
the vbcc web page or build from sources.

The lsa backend for vbcc is not intended to do everything you can invent
in a C program.  It is intended for educational purposes partly to see
what C compilers produce, also I wanted a C compiler of some sort for
this instruction set and this compiler is very easy to work with.

Vbcc does not force you into any particular parameter passing model.
This particular backend implementation is very register centric.
Parameters are passed in and return value back in registers.  What
would normally be stack used for local variables has been replaced with
registers.  The stack is limited to preserving registers used by
the function on entry and restoring them on exit.

This vbcc backend is designed to generate a single assembler file to be
assembled with lsa-as.  The whole application is in this single assembler
file and the backend adds a few things for you like the reset vector
branches and PUT16/GET16 routines.  Instead of main() implement your
starting code in reset().  See the test.c example in the vbcc directory

