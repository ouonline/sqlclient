# This Makefile is generated by omake: https://github.com/ouonline/omake.git

CXX := g++

ifeq ($(debug), y)
	CXXFLAGS += -g
else
	CXXFLAGS += -O2 -DNDEBUG
endif

TARGET := test_mysql

.PHONY: all clean distclean

all: $(TARGET)

.PHONY: omake_phony_0
omake_phony_0:
	$(MAKE) debug=$(debug) libmysql_client_static.a -C ..

omake_dep_0_INCLUDE := -I../..

omake_dep_0.test_mysql_client.cpp.o: test_mysql_client.cpp
	$(CXX) $(CXXFLAGS) -Wall -Werror -Wextra $(omake_dep_0_INCLUDE) -c $< -o $@

test_mysql_OBJS := omake_dep_0.test_mysql_client.cpp.o

test_mysql_LIBS := ../libmysql_client_static.a -lmysqlclient

test_mysql: $(test_mysql_OBJS) | omake_phony_0
	$(CXX) $(CXXFLAGS) -Wall -Werror -Wextra -o $@ $^ $(test_mysql_LIBS)

clean:
	rm -f $(TARGET) $(test_mysql_OBJS)

distclean:
	$(MAKE) clean
	$(MAKE) distclean -C ..
