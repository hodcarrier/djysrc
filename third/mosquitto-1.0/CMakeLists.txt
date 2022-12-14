# This is a cmake script.
# It is intended to be the default means of building mosquitto, but isn't
# quite ready for all cases yet.

set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project(mosquitto)

cmake_minimum_required(VERSION 2.6)

set (VERSION 1.0)

if (WIN32)
	execute_process(COMMAND cmd /c echo %DATE% %TIME% OUTPUT_VARIABLE TIMESTAMP
			OUTPUT_STRIP_TRAILING_WHITESPACE)
else (WIN32)
	execute_process(COMMAND date "+%F %T%z" OUTPUT_VARIABLE TIMESTAMP
			OUTPUT_STRIP_TRAILING_WHITESPACE)
endif (WIN32)

add_definitions (-DCMAKE -DVERSION=\"${VERSION}\" -DTIMESTAMP=\"${TIMESTAMP}\")

if (WIN32)
	set (BINDIR .)
	set (SBINDIR .)
	set (SYSCONFDIR .)
	set (LIBDIR .)
	set (INCLUDEDIR include)
	set (DATAROOTDIR share)
	set (MANDIR man)
    set (SHAREDEST .)
    add_definitions("-D_CRT_SECURE_NO_WARNINGS")
else (WIN32)
	set (BINDIR bin)
	set (SBINDIR sbin)
	set (SYSCONFDIR etc/mosquitto)
	set (LIBDIR lib${LIB_SUFFIX})
	set (INCLUDEDIR include)
	set (DATAROOTDIR share)
	set (MANDIR ${DATAROOTDIR}/man)
	set (SHAREDIR ${DATAROOTDIR}/mosquitto)
endif (WIN32)

option(WITH_TLS
	"Include SSL/TLS support?" ON)
if (${WITH_TLS} STREQUAL ON)
	find_package(OpenSSL REQUIRED)
	add_definitions("-DWITH_TLS")
else (${WITH_TLS} STREQUAL ON)
	set (OPENSSL_INCLUDE_DIR "")
endif (${WITH_TLS} STREQUAL ON)

# ========================================
# Include projects
# ========================================

add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(src)
add_subdirectory(man)

# ========================================
# Install config file
# ========================================

install(FILES mosquitto.conf aclfile.example pwfile.example DESTINATION ${SYSCONFDIR})
