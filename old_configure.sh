#!/bin/bash

# some neede variables to create 'Makefile.rules'
HEADERS=""
SOURCES=""
NLINE="\\n"
BSLASH="\\\\"
# appends *.hpp from $1 to $HEADERS
function append_hpp_from()
{
	for i in "$1"/*.hpp ; do
		HEADERS="$HEADERS ${BSLASH}${NLINE}          $i"
	done
}
# appends *.cpp from $1 to $SOURCES
function append_cpp_from()
{
	for i in "$1/"*.cpp ; do
		SOURCES="$SOURCES ${BSLASH}${NLINE}          $i"
	done
}
# default flags
gcc_flags="-std=c++0x -pedantic -Wall -Wextra -g -O0 -I/opt/local/include/ -fpermissive -Wno-deprecated-declarations"
# get all g++ binaries
gcc_found=$(find /bin/ /usr/bin /usr/local/bin /opt/bin /opt/local/bin -regex "^.*/g\+\+.*$" 2>/dev/null)
# holds the finally selected g++ binary
gcc_selected=""
# checks if g++ binary $1 is able to compile cpp0x_test.cpp
function compatibility_test()
{
	if $1 $gcc_flags -o variadic_templates_test variadic_templates_test.cpp &>/dev/null ; then
		if test "x$(./variadic_templates_test)" "!=" "xyes" ; then
			return -1
		else
			return 0
		fi
	fi
}
# iterates over all found g++ binaries until a suitable binary is found
for i in $gcc_found ; do
	version=$($i -v 2>&1 | grep -oE "gcc version [0-9](\.[0-9]){2}" | grep -oE "[0-9](\.[0-9]){2}")
	if test "!" $version "<" 4.6.0 ; then
		if compatibility_test "$i" ; then
			gcc_selected=$i
			break
		fi
	fi
done
# did we found a suitable g++ binary?
if test -z "$gcc_selected" ; then
	echo "no GCC >= 4.6.0 found ... quit"
	exit
fi
echo "chosen g++ binary: $gcc_selected"
# ok, write makefiles now
echo "build Makefiles..."
# link file descriptor #6 to stdout
exec 6>&1
# redirect stdout to Makefile.rules
exec >Makefile.rules
printf "%b\n" "CXX = $gcc_selected"
printf "%b\n" "CXXFLAGS = $gcc_flags"
if test $(uname) = "Darwin" ; then
    printf "%b\n" "LIBS = -L/opt/local/lib -lboost_thread-mt"
else
    printf "%b\n" "LIBS = -lboost_thread"
fi
# redirect stdout to libcppa.Makefile
exec >libcppa.Makefile
append_hpp_from "cppa"
append_hpp_from "cppa/detail"
append_hpp_from "cppa/util"
append_cpp_from "src"
printf "%b\n" "include Makefile.rules"
printf "%b\n" "INCLUDE_FLAGS = \$(INCLUDES) -I./"
printf "\n"
printf "%b\n" "HEADERS =$HEADERS"
printf "\n"
printf "%b\n" "SOURCES =$SOURCES"
printf "\n"
printf "%b\n" "OBJECTS = \$(SOURCES:.cpp=.o)"
printf "\n"
printf "%b\n" "LIB_NAME = $LIB_NAME"
printf "\n"
printf "%b\n" "%.o : %.cpp \$(HEADERS)"
printf "%b\n" "\t\$(CXX) \$(CXXFLAGS) \$(INCLUDE_FLAGS) -fPIC -c \$< -o \$@"
printf "\n"
printf "%b\n" "libcppa.so.0.0.0 : \$(OBJECTS) \$(HEADERS)"
if test "$(uname)" "=" "Darwin" ; then
	printf "%b\n" "\t\$(CXX) \$(LIBS) -dynamiclib -o libcppa.dylib \$(OBJECTS)"
else
	printf "%b\n" "\t\$(CXX) \$(LIBS) -shared -Wl,-soname,libcppa.so.0 -o libcppa.so.0.0.0 \$(OBJECTS)"
	printf "%b\n" "\tln -s libcppa.so.0.0.0 libcppa.so.0.0"
	printf "%b\n" "\tln -s libcppa.so.0.0.0 libcppa.so.0"
	printf "%b\n" "\tln -s libcppa.so.0.0.0 libcppa.so"
fi
printf "\n"
printf "%b\n" "all : libcppa.so.0.0.0 \$(OBJECTS)"
printf "\n"
printf "%b\n" "clean:"
printf "%b\n" "\trm -f libcppa.so libcppa.so.0 libcppa.so.0.0 libcppa.so.0.0.0 \$(OBJECTS)"
# redirect stdout to Makefile
exec >Makefile
printf "%b\n" "all:"
printf "%b\n" "\tmake -f libcppa.Makefile"
printf "%b\n" "\tmake -C unit_testing"
printf "%b\n" "\tmake -C queue_performances"
printf "\n"
printf "%b\n" "clean:"
printf "%b\n" "\tmake -f libcppa.Makefile clean"
printf "%b\n" "\tmake -C unit_testing clean"
printf "%b\n" "\tmake -C queue_performances clean"

# restore stdout
exec 1>&6

echo "done"
