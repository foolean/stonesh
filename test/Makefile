GCC = gcc -Wall

INC    = -I..
LIBS   = -lGeoIP 
SOURCE = geoip_t.c ../util.c ../geoip.c ../auth_local.c
OBJECT = geoip_t
OS     = $(shell grep "^ID=" /etc/os-release | cut -f2 -d=)

SPLINT = splint -D__gnuc_va_list=va_list +posixlib
VALGRIND = valgrind --leak-check=full --show-reachable=yes --error-exitcode=1


all:
	$(GCC) -o $(OBJECT) $(SOURCE) $(INC) $(LIBS)

splint:
	$(SPLINT)  $(INC) ../auth.c ../auth_ldap.c  ../auth_local.c  ../geoip.c  ../hosts.c  ../log.c  ../readconf.c  ../util.c

t:
	@echo "building tests"
	$(GCC) $(INC) -o test_trim test_trim.c ../util.c
	$(GCC) $(INC) -o test_strtolower test_strtolower.c ../util.c
	$(GCC) $(INC) -o test_strtoupper test_strtoupper.c ../util.c
	$(GCC) $(INC) -o test_getafbyaddr test_getafbyaddr.c ../util.c ../log.c ../readconf.c ../geoip.c -lGeoIP
	$(GCC) $(INC) -o test_is_local test_is_local.c ../util.c ../log.c ../readconf.c ../geoip.c -lGeoIP
	$(GCC) $(INC) -o test_is_local6 test_is_local6.c ../util.c ../log.c ../readconf.c ../geoip.c -lGeoIP
	$(GCC) $(INC) -o test_is_private test_is_private.c ../util.c ../log.c ../readconf.c ../geoip.c -lGeoIP
	$(GCC) $(INC) -o test_is_private6 test_is_private6.c ../util.c ../log.c ../readconf.c ../geoip.c -lGeoIP
	$(GCC) $(INC) -o test_getcountrybyaddr test_getcountrybyaddr.c ../util.c ../log.c ../readconf.c ../geoip.c -lGeoIP
	$(GCC) $(INC) -o test_getcountrybyaddr6 test_getcountrybyaddr6.c ../util.c ../log.c ../readconf.c ../geoip.c -lGeoIP
	$(GCC) $(INC) -o test_is_from_valid_country test_is_from_valid_country.c ../util.c ../log.c ../readconf.c ../geoip.c -lGeoIP
	$(GCC) $(INC) -o test_gethostentry test_gethostentry.c ../hosts.c ../util.c ../log.c
	$(GCC) $(INC) -o test_get_auth_file test_get_auth_file.c ../util.c ../auth_local.c ../log.c
	$(GCC) $(INC) -o test_get_host_filter test_get_host_filter.c ../util.c ../auth_ldap.c ../log.c ../readconf.c -lldap
	$(GCC) $(INC) -o test_get_service_filter test_get_service_filter.c ../util.c ../auth_ldap.c ../log.c ../readconf.c -lldap
	$(GCC) $(INC) -o test_error test_error.c ../log.c
	$(GCC) $(INC) -o test_notice test_notice.c ../log.c
	$(GCC) $(INC) -o test_debug test_debug.c ../log.c
	$(GCC) $(INC) -o test_fatal test_fatal.c ../log.c
	$(GCC) $(INC) -o test_fatal_1024 test_fatal_1024.c ../log.c
	$(GCC) $(INC) -o test_fatal_1038 test_fatal_1038.c ../log.c
       
run_t:
	@echo "running tests"
	./test_trim
	./test_strtolower
	./test_strtoupper
	./test_get_auth_file
	./test_getafbyaddr
	./test_is_local
	./test_is_local6
	./test_is_private
	./test_is_private6
	./test_getcountrybyaddr
	./test_getcountrybyaddr6
	./test_is_from_valid_country
	./test_gethostentry
	./test_get_host_filter
	./test_get_service_filter

valgrind_t:
	if [ "$(OS)" = "ubuntu" ]; then                                 \
		echo "### memory leak checking disabled for '$(OS)'";   \
        else                                                            \
		echo "running tests w/memory leak checking";            \
		$(VALGRIND) ./test_trim;                                \
		$(VALGRIND) ./test_strtolower;                          \
		$(VALGRIND) ./test_strtoupper;                          \
		$(VALGRIND) ./test_get_auth_file;                       \
		$(VALGRIND) ./test_getafbyaddr;                         \
		$(VALGRIND) ./test_is_local;                            \
		$(VALGRIND) ./test_is_local6;                           \
		$(VALGRIND) ./test_is_private;                          \
		$(VALGRIND) ./test_is_private6;                         \
		$(VALGRIND) ./test_getcountrybyaddr;                    \
		$(VALGRIND) ./test_getcountrybyaddr6;                   \
		$(VALGRIND) ./test_is_from_valid_country;               \
		$(VALGRIND) ./test_gethostentry;                        \
		$(VALGRIND) ./test_get_host_filter;                     \
		$(VALGRIND) ./test_get_service_filter;                  \
		$(VALGRIND) ./test_notice;		                \
		$(VALGRIND) ./test_error;		                \
		$(VALGRIND) ./test_debug;		                \
		$(VALGRIND) ./test_fatal.sh test_fatal;		        \
		$(VALGRIND) ./test_fatal.sh test_fatal_1024;		\
		$(VALGRIND) ./test_fatal.sh test_fatal_1038;		\
	fi
 
test: clean splint t run_t valgrind_t
	@echo "" 
	@echo "##############################################################" 
	@echo "###                                                        ###"
	@echo "###                   CONGRATULATIONS!!!                   ###" 
	@echo "###                                                        ###"
	@echo "### It would appear that all configured tests have passed! ###"
	@echo "###                                                        ###"
	@echo "##############################################################" 
	@echo "" 

clean:
	/bin/rm -f test_trim
	/bin/rm -f test_strtolower
	/bin/rm -f test_strtoupper
	/bin/rm -f test_get_auth_file
	/bin/rm -f test_getafbyaddr
	/bin/rm -f test_is_local
	/bin/rm -f test_is_local6
	/bin/rm -f test_is_private
	/bin/rm -f test_is_private6
	/bin/rm -f test_getcountrybyaddr
	/bin/rm -f test_getcountrybyaddr6
	/bin/rm -f test_is_from_valid_country
	/bin/rm -f test_gethostentry
	/bin/rm -f test_get_host_filter
	/bin/rm -f test_get_service_filter
	/bin/rm -f test_notice
	/bin/rm -f test_error
	/bin/rm -f test_debug
	/bin/rm -f test_fatal
	/bin/rm -f test_fatal_1024
	/bin/rm -f test_fatal_1038
