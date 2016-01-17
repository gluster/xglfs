cmake_minimum_required(VERSION 2.6.4)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Release' as none was specified.")
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	message("This is DEBUG build")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

include(FindPkgConfig)
if (PKG_CONFIG_FOUND)
	pkg_check_modules(GLUSTERFS-API glusterfs-api)
	pkg_check_modules(FUSE fuse)
	if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
		pkg_check_modules(LIBTCMALLOC_MINIMAL libtcmalloc_minimal)
		if(LIBTCMALLOC_MINIMAL_FOUND EQUAL 1)
			set(GB_LD_EXTRA "${LIBTCMALLOC_MINIMAL_LIBRARIES}")
		endif (LIBTCMALLOC_MINIMAL_FOUND EQUAL 1)
	endif(NOT CMAKE_BUILD_TYPE MATCHES Debug)
endif (PKG_CONFIG_FOUND)

set(CMAKE_C_FLAGS
	"-std=c99 -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26 -W -Wall -Wextra -pedantic -Wwrite-strings -Winit-self -Wcast-qual -Wpointer-arith -Wstrict-aliasing -Wformat=2 -Wmissing-declarations -Wmissing-include-dirs -Wno-unused-parameter -Wuninitialized -Wold-style-definition -Wstrict-prototypes -Wmissing-prototypes")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g -ggdb -DMODE_DEBUG")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -w3 -wd2552 -wd981")
endif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")

set(CMAKE_C_FLAGS_RELEASE "-O3 -march=native -DMODE_NORMAL")

add_subdirectory(pfcquirks)
add_subdirectory(pfglfsquirks)

include_directories(.
	pfcquirks
	pfglfsquirks)

add_executable(xglfs
	xglfs.c
	xglfs_access.c
	xglfs_chmod.c
	xglfs_chown.c
	xglfs_create.c
	xglfs_destroy.c
	xglfs_fgetattr.c
	xglfs_flush.c
	xglfs_fsync.c
	xglfs_fsyncdir.c
	xglfs_ftruncate.c
	xglfs_getattr.c
	xglfs_getxattr.c
	xglfs_init.c
	xglfs_link.c
	xglfs_listxattr.c
	xglfs_lock.c
	xglfs_mkdir.c
	xglfs_mknod.c
	xglfs_open.c
	xglfs_opendir.c
	xglfs_read.c
	xglfs_readdir.c
	xglfs_readlink.c
	xglfs_release.c
	xglfs_releasedir.c
	xglfs_removexattr.c
	xglfs_rename.c
	xglfs_rmdir.c
	xglfs_setxattr.c
	xglfs_statfs.c
	xglfs_symlink.c
	xglfs_truncate.c
	xglfs_unlink.c
	xglfs_utimens.c
	xglfs_write.c)

target_link_libraries(xglfs
	pthread
	rt
	ln_pfcquirks
	ln_pfglfsquirks
	${GLUSTERFS-API_LIBRARIES}
	${FUSE_LIBRARIES}
	${GB_LD_EXTRA})

install(TARGETS xglfs
	RUNTIME DESTINATION bin)

