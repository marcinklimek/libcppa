AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I ../m4

noinst_PROGRAMS = unit_tests

unit_tests_SOURCES =    main.cpp \
                        ping_pong.cpp \
                        test__atom.cpp \
                        test__fixed_vector.cpp \
                        test__intrusive_containers.cpp \
                        test__intrusive_ptr.cpp \
                        test__local_group.cpp \
                        test__pattern.cpp \
                        test__primitive_variant.cpp \
                        test__remote_actor.cpp \
                        test__ripemd_160.cpp \
                        test__match.cpp \
                        test__serialization.cpp \
                        test__spawn.cpp \
                        test__tuple.cpp \
                        test__type_list.cpp \
                        test__uniform_type.cpp \
                        test__yield_interface.cpp

unit_tests_DEPENDENCIES = ../.libs/libcppa.la

AM_CPPFLAGS = -I../
unit_tests_CXXFLAGS = --std=c++0x -pedantic -Wall -Wextra
unit_tests_LDADD = -L../.libs/ -lcppa $(BOOST_LDFLAGS) $(BOOST_THREAD_LIB)
