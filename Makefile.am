AUTOMAKE_OPTIONS = no-dependencies

SPLINT = splint -D__gnuc_va_list=va_list -warnposixheaders
CPPCHECK = cppcheck --enable=all -v
VALGRIND = valgrind --leak-check=full --show-reachable=yes --error-exitcode=1 --track-origins=yes

bin_PROGRAMS = stonesh
sbin_SCRIPTS = stonewho

AM_CXXFLAGS = $(INTI_CFLAGS)
stonesh_SOURCES = stonesh.c geoip.c geoip.h log.c log.h auth.c auth.h auth_local.c auth_local.h auth_ldap.c auth_ldap.h util.c util.h hosts.c hosts.h stonesh.h readconf.c readconf.h
stonesh_LDADD = $(DEPS_LIBS) 
sysconf_DATA = stonesh.conf
sysconf_HOSTS = hosts.lst
sysconf_USERS = users.lst
sysconfdir = /etc/stonesh

DEFAULT_INCLUDES =

man1_MAN = stonesh.1 stonewho.1
man5_MAN = hosts.lst.5 users.lst.5 stonesh.conf.5
man_MANS = $(man1_MAN) $(man5_MAN) 

EXTRA_DIST = 		 \
	$(man1_MAN) 	 \
	$(man5_MAN) 	 \
	$(sbin_SCRIPTS)  \
	$(sysconf_HOSTS) \
	$(sysconf_USERS) \
	$(sysconf_DATA)  \
	contrib 	 \
	stonesh.pl	 \
	test

CLEANFILES = 				\
	$(man_MANS) 			\
	test/test_trim 			\
	test/test_strtolower 		\
	test/test_strtoupper 		\
	test/test_get_auth_file 	\
	test/test_getafbyaddr 		\
	test/test_is_local 		\
	test/test_is_local6 		\
	test/test_is_private 		\
	test/test_is_private6 		\
	test/test_getcountrybyaddr 	\
	test/test_getcountrybyaddr6 	\
	test/test_is_from_valid_country \
	test/test_get_host_filter 	\
	test/test_get_service_filter 	\
	test/test_gethostentry		\
	test/test_debug			\
	test/test_error			\
	test/test_fatal			\
	test/test_fatal_1024		\
	test/test_fatal_1038		\
	test/test_notice		\
	test/test_parse_token		\
	test/test_process_config_line	\
	test/test_fill_default_options	\
	test/test_strdelim		\
	test/test_read_config_file_01	\
	test/test_read_config_file_02	\
	test/test_read_config_file_03	\
	test/test_get_ldap_uri		\
	test/test_get_ldap_base		\
	debian/stonesh.debhelper.log  	\
	debian/stonesh.substvars        \
        debian/files

CLEANDIRS =                     \
	packaging/rpm/BUILD     \
	packaging/rpm/BUILDROOT \
	packaging/rpm/SOURCES   \
	packaging/rpm/SRPMS     \
	packaging/rpm/RPMS      \
	debian/stonesh

DISTCLEANFILES = $(distdir).tar.gz

install-sysconfDATA:
	$(mkinstalldirs) $(DESTDIR)$(sysconfdir)
	@if [ ! -f "$(DESTDIR)$(sysconfdir)/$(sysconf_HOSTS)" ]; then \
		$(INSTALL) -m 0644 $(sysconf_HOSTS) $(DESTDIR)$(sysconfdir)/$(sysconf_HOSTS); \
	else \
		echo "$(DESTDIR)$(sysconfdir)/$(sysconf_HOSTS) already exists, install will not overwrite"; \
	fi
	@if [ ! -f "$(DESTDIR)$(sysconfdir)/$(sysconf_USERS)" ]; then \
		$(INSTALL) -m 0644 $(sysconf_USERS) $(DESTDIR)$(sysconfdir)/$(sysconf_USERS); \
	else \
		echo "$(DESTDIR)$(sysconfdir)/$(sysconf_USERS) already exists, install will not overwrite"; \
	fi
	@if [ ! -f "$(DESTDIR)$(sysconfdir)/$(sysconf_DATA)" ]; then \
		$(INSTALL) -m 0644 $(sysconf_DATA) $(DESTDIR)$(sysconfdir)/$(sysconf_DATA); \
	else \
		echo "$(DESTDIR)$(sysconfdir)/$(sysconf_DATA) already exists, install will not overwrite"; \
	fi

install-sbinSCRIPTS:
	$(mkinstalldirs) $(DESTDIR)/usr/sbin
	$(INSTALL) -m 0755 $(sbin_SCRIPTS) $(DESTDIR)/usr/sbin/$(sbin_SCRIPTS)
         
stonesh.1:
	$(POD2MAN) --release=$(VERSION) --center=STONESH --section=1 stonesh.pod > stonesh.1

stonewho.1:
	$(POD2MAN) --release=$(VERSION) --center=STONESH --section=1 stonewho.pod > stonewho.1

stonesh.conf.5:
	$(POD2MAN) --release=$(VERSION) --center=STONESH --section=5 stonesh.conf.pod > stonesh.conf.5

hosts.lst.5:
	$(POD2MAN) --release=$(VERSION) --center=STONESH --section=5 hosts.lst.pod > hosts.lst.5

users.lst.5:
	$(POD2MAN) --release=$(VERSION) --center=STONESH --section=5 users.lst.pod > users.lst.5

stonesh.conf:
	@echo -n ""

stonewho:
	@echo -n ""

splint:
	$(SPLINT) *.c

cppcheck:
	$(CPPCHECK) .

valgrind:
	$(VALGRIND) ./$(bin_PROGRAMS)

test: force_look
	cd test; make test
	cd ..

force_look:
	true

debian-package: debian/rules
	@if test x$(HAVE_DPKG_BUILDPACKAGE) = xyes ; then 		\
		debian/rules build && debian/rules binary ;             \
	else 								\
		echo "Error: Debian packaging tools not found." ; 	\
		echo "Package build abandoned." ; 			\
	fi

rpm_topdir=`cd $(top_builddir) && pwd`/packaging/rpm
rpm-package: dist
	@if test x$(HAVE_RPMBUILD) = xyes ; then                                  \
		cd $(rpm_topdir) ;                                                \
		mkdir -p BUILD RPMS SOURCES SRPMS;                                \
		cd BUILD;                                                         \
		ln -sf ../../../$(distdir).tar.gz ;                               \
		cd .. ;                                                           \
		/bin/tar -C "SOURCES" --strip-components=1                        \
			-xzf "BUILD/$(distdir).tar.gz";                           \
		rpmbuild --define "_topdir `cd . && pwd`" --define "_prefix /usr" \
			--buildroot="`cd . && pwd`/BUILDROOT/$(distdir)"          \
			-ba SPECS/stonesh.spec &&                                 \
			echo "Package successfully built in `pwd`/RPMS." &&       \
			rm -f BUILD/$(distdir).tar.gz $(distdir).tar.gz ;         \
	else                                                                      \
		echo "Error: RPM executable and/or source directory not found." ; \
		false;                                                            \
	fi

deb: debian-package
	true

rpm: rpm-package
	true

clean-local:
	rm -rf $(CLEANDIRS)
