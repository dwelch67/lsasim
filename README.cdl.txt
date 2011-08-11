http://cyclicity-cdl.sourceforge.net/


<instructions for building cdl>
svn co https://cyclicity-cdl.svn.sourceforge.net/svnroot/cyclicity-cdl
copy and modify the makefile.config
cd bin
export CYCLICITY_ROOT
make
<instructions for building cdl>



apt-get install subversion
svn co https://cyclicity-cdl.svn.sourceforge.net/svnroot/cyclicity-cdl

probably need to apt-get a number of other things, I think these packages
are required:

libpcap-dev
python-dev
bison
flex
php5-cli


copy and modify the makefile.config
cd cyclicity-cdl/trunk/scripts
cp makefile.config.fc8 makefile.config
Change these to a no
DEBUG_BUILD := no
BUILD_PYTHON := no
BUILD_VPI := no

cd ../bin

export CYCLICITY_ROOT=/path/to/cyclicity-cdl/trunk

make

put bin/linux/ in the path or copy or symlink to bin/linux/cdl


Using cdl as an hdl and outputting verilog is straightforward, the cdl binary
is all that is required.  it can be as simple as this:

cdl --verilog lsa.v lsa.cdl

If going straight to hardware or some other simulation environment (verilator, icarus verilog,
modelsim, cadence, etc) that is as far as you need to go in this README.

Using the cdl simulation environment requires a few more files.  Looking in the cdlsim
directory there are a couple subdirectories:

src/
build/linux/

And a handful of files
build/Makefile
Makefile
src/model_list

And some cdl script language files to drive the cdl sim environment
test.batch
test.hwex
test.th

test.batch has a line like this:
  step 2000
Which is the number of cycles to simulate.  If you want to simulate more
or less then modify this line.

csram.cpp needs a file named sram.h which lsa-as generates when it
assembles.
Copy the sram.h file to the cdlsim directory,

touch csram.cpp
make
./build/linux/sim test.batch

using gtkwave or other .vcd waveform viewer examine the test.vcd file.

