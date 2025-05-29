target            =             arm-linux-eabi
build             =             x86_64-pc-linux-gnu
ENABLE_MULTILIB   ?= 			1

ifndef (${windows_host})
windows_host      =             0
endif
ifeq (${windows_host}, 1)
host              =             i686-w64-mingw32
#host              =             x86_64-w64-mingw32
#if host != build, prepare for compilation environment variables
export PATH:=$(CURDIR)/${target}_for_${build}/bin:${PATH}
else
#host              =             i686-pc-linux-gnu
host              =             ${build}
endif

BUILDDIR          =             $(CURDIR)/build/${target}_for_${host}
LOGDIR            =             $(CURDIR)/log
sysroot           =             $(BUILDDIR)/sysroot/
INSTALLDIR        =             $(CURDIR)/${target}_for_${host}
JOBS             :=             $(shell grep -c ^processor /proc/cpuinfo 2>/dev/null)
MAKE              =             make -j${JOBS}
DOWNLOAD_CMD      =             wget -c
BINUTILS_VER      =             2.39
GCC_VER           =             12.2.0
GMP_VER           =             6.2.1
MPC_VER           =             1.3.1
MPFR_VER          =             4.2.0
MUSL_VER          =             1.1.22
BUILD_TIME        =             $(shell date +"%s")

GNU_SITE          =             http://mirrors.ustc.edu.cn/gnu/
#GNU_SITE          =             https://mirrors.tuna.tsinghua.edu.cn/gnu/
#GNU_SITE          =             http://download.lan.rt-thread.com/download/rt-smart/mirror/toolchains
GCC_SITE          =             $(GNU_SITE)/gcc
BINUTILS_SITE     =             $(GNU_SITE)/binutils
GMP_SITE          =             $(GNU_SITE)/gmp
MPC_SITE          =             $(GNU_SITE)/mpc
MPFR_SITE         =             $(GNU_SITE)/mpfr


GCC_CONFIG        =             --disable-werror 
GCC_CONFIG       +=             --prefix=
GCC_CONFIG       +=             --target=${target} 
GCC_CONFIG       +=             --with-sysroot=/${target} 
GCC_CONFIG       +=             --with-build-sysroot=${sysroot}
#GCC_CONFIG       +=             --enable-languages=c,c++ 
#GCC_CONFIG       +=             --disable-multilib 

GCC_CONFIG       +=             --disable-gnu-indirect-function 
GCC_CONFIG       +=             --disable-shared
GCC_CONFIG       +=             --disable-threads
GCC_CONFIG       +=             --disable-tls
GCC_CONFIG       +=             --enable-languages=c
ifeq (${ENABLE_MULTILIB}, 1)
GCC_CONFIG       +=             --enable-multiarch
GCC_CONFIG       +=             --enable-multilib
GCC_CONFIG       +=             --with-multilib-list=rmprofile
else
GCC_CONFIG       +=             --disable-multilib
endif
#GCC_CONFIG       +=				--with-multilib-list="armv7-a/thumb,hard-float/soft-float"
GCC_CONFIG       +=             --disable-libmudflap
GCC_CONFIG       +=             --disable-libssp
GCC_CONFIG       +=             --disable-libquadmath
GCC_CONFIG       +=             --disable-libgomp
GCC_CONFIG       +=             --disable-libatomic
GCC_CONFIG       +=             --disable-nls
GCC_CONFIG       +=             --disable-doc
GCC_CONFIG       +=             --disable-tm-clone-registry

GCC_CONFIG       +=             --disable-libmudflap --disable-libsanitizer 
GCC_CONFIG       +=             --disable-libmpx 
GCC_CONFIG       +=             --enable-libstdcxx-time 
GCC_CONFIG       +=             --host=${host}

ifeq (${windows_host}, 1)
GCC_CONFIG       +=             --disable-bootstrap
GCC_CONFIG       +=             AR_FOR_TARGET=${target}-ar 
GCC_CONFIG       +=             AS_FOR_TARGET=${target}-as
GCC_CONFIG       +=             LD_FOR_TARGET=${target}-ld
GCC_CONFIG       +=             NM_FOR_TARGET=${target}-nm
GCC_CONFIG       +=             OBJCOPY_FOR_TARGET=${target}-objcopy 
GCC_CONFIG       +=             OBJDUMP_FOR_TARGET=${target}-objdump 
GCC_CONFIG       +=             RANLIB_FOR_TARGET=${target}-ranlib 
GCC_CONFIG       +=             READELF_FOR_TARGET=${target}-readelf 
GCC_CONFIG       +=             STRIP_FOR_TARGET=${target}-strip
else
GCC_CONFIG       +=             AR_FOR_TARGET=${BUILDDIR}/obj_binutils/binutils/ar 
GCC_CONFIG       +=             AS_FOR_TARGET=${BUILDDIR}/obj_binutils/gas/as-new 
GCC_CONFIG       +=             LD_FOR_TARGET=${BUILDDIR}/obj_binutils/ld/ld-new 
GCC_CONFIG       +=             NM_FOR_TARGET=${BUILDDIR}/obj_binutils/binutils/nm-new 
GCC_CONFIG       +=             OBJCOPY_FOR_TARGET=${BUILDDIR}/obj_binutils/binutils/objcopy 
GCC_CONFIG       +=             OBJDUMP_FOR_TARGET=${BUILDDIR}/obj_binutils/binutils/objdump 
GCC_CONFIG       +=             RANLIB_FOR_TARGET=${BUILDDIR}/obj_binutils/binutils/ranlib 
GCC_CONFIG       +=             READELF_FOR_TARGET=${BUILDDIR}/obj_binutils/binutils/readelf 
GCC_CONFIG       +=             STRIP_FOR_TARGET=${BUILDDIR}/obj_binutils/binutils/strip-new 
endif

ifeq (${windows_host}, 1)
XGCC = ${target}-gcc
else
XGCC_DIR = ../obj_gcc/gcc
XGCC = $(XGCC_DIR)/xgcc -B $(XGCC_DIR)
endif


ifeq (${mlibc_debug}, 1)
BUILD_CFLAGS = "-O0 -g -DDEBUG_LDSO"
else
BUILD_CFLAGS = "-O2"
endif

MULTILIBS := $(shell ${BUILDDIR}/obj_gcc/gcc/xgcc -B ${BUILDDIR}/obj_gcc/gcc -print-multi-lib)
MULTILIB_DIRS := $(foreach mlib,${MULTILIBS},\"$(firstword $(subst ;, ,${mlib}))\")
MULTILIBS := $(foreach mlib,${MULTILIBS},"${mlib}")

all:

download:
	${DOWNLOAD_CMD} ${BINUTILS_SITE}/binutils-${BINUTILS_VER}.tar.gz > /dev/null
	${DOWNLOAD_CMD} ${GCC_SITE}/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz > /dev/null
	${DOWNLOAD_CMD} ${MPC_SITE}/mpc-${MPC_VER}.tar.gz > /dev/null
	${DOWNLOAD_CMD} ${MPFR_SITE}/mpfr-${MPFR_VER}.tar.gz > /dev/null
	${DOWNLOAD_CMD} ${GMP_SITE}/gmp-${GMP_VER}.tar.xz > /dev/null
	-rm -rf binutils-${BINUTILS_VER} gcc-${GCC_VER} mpc-${MPC_VER} mpfr-${MPFR_VER} gmp-${GMP_VER} 
	tar axvf binutils-${BINUTILS_VER}.tar.gz > /dev/null
	tar axvf gcc-${GCC_VER}.tar.gz > /dev/null
	tar axvf mpc-${MPC_VER}.tar.gz > /dev/null
	tar axvf mpfr-${MPFR_VER}.tar.gz > /dev/null
	tar axvf gmp-${GMP_VER}.tar.xz > /dev/null
	test ! -d patches/binutils-${BINUTILS_VER} || cat patches/binutils-${BINUTILS_VER}/* | (cd binutils-${BINUTILS_VER} && patch -p1)
	test ! -d patches/gcc-${GCC_VER} || cat patches/gcc-${GCC_VER}/* | (cd gcc-${GCC_VER} && patch -p1)
	echo "CI_COMMIT_SHA:"${CI_COMMIT_SHA}
	echo "CI_JOB_ID:"${CI_JOB_ID}
	sed -i 's/CI_COMMIT_SHA/${CI_COMMIT_SHA}/'  $(CURDIR)/gcc-${GCC_VER}/gcc/gcc.cc
	sed -i 's/CI_JOB_ID/${CI_JOB_ID}/'  $(CURDIR)/gcc-${GCC_VER}/gcc/gcc.cc
	sed -i 's/CI_PIPELINE_ID/${CI_PIPELINE_ID}/'  $(CURDIR)/gcc-${GCC_VER}/gcc/gcc.cc
	cp -f config.sub binutils-${BINUTILS_VER}
	cp -f config.sub gcc-${GCC_VER}
	cp -f config.sub mpc-${MPC_VER}
	cp -f config.sub mpfr-${MPFR_VER}
	cp -f config.sub gmp-${GMP_VER}

prepare:
	mkdir -p ${BUILDDIR}
	mkdir -p ${LOGDIR}
	ln -sfT $(CURDIR)/binutils-${BINUTILS_VER}/   ${BUILDDIR}/src_binutils
	ln -sfT $(CURDIR)/gcc-${GCC_VER}/             ${BUILDDIR}/src_gcc
	ln -sfT $(CURDIR)/mpc-${MPC_VER}/             ${BUILDDIR}/src_gcc/mpc
	ln -sfT $(CURDIR)/mpfr-${MPFR_VER}/           ${BUILDDIR}/src_gcc/mpfr
	ln -sfT $(CURDIR)/gmp-${GMP_VER}/             ${BUILDDIR}/src_gcc/gmp
	ln -sfT $(CURDIR)/../                         ${BUILDDIR}/src_mlibc
	mkdir -p ${sysroot}
	mkdir -p ${INSTALLDIR}
	ln -sf ${sysroot} ${sysroot}/usr 

build_binutils:
	cd ${BUILDDIR} && mkdir -p obj_binutils
	cd ${BUILDDIR}/obj_binutils && ../src_binutils/configure \
			--prefix= \
			--target=${target} \
			--with-sysroot=/${target} \
			--enable-multilib \
			--with-multilib-list=rmprofile
			--host=${host}
			> ${LOGDIR}/log.binutils.${host}.configure
	cd ${BUILDDIR}/obj_binutils && ${MAKE} MAKEINFO=true > ${LOGDIR}/log.binutils.${host}.make

build_all_gcc: 
	mkdir -p ${BUILDDIR}/obj_gcc
	#- ln -sf ${BUILDDIR}/src_mlibc/include ${sysroot}/
	cd ${BUILDDIR}/obj_gcc && ../src_gcc/configure ${GCC_CONFIG} > ${LOGDIR}/log.all-gcc.${host}.configure
	cd ${BUILDDIR}/obj_gcc && ${MAKE} all-gcc > ${LOGDIR}/log.all-gcc.${host}.make 

build_mlibc_headers:
	mkdir -p ${BUILDDIR}/obj_mlibc
	-rm -rf ${sysroot}/include
	cp -r ${BUILDDIR}/src_mlibc/include ${sysroot}/

build_all_target_libgcc:
	cd ${BUILDDIR}/obj_gcc && ${MAKE}  all-target-libgcc > ${LOGDIR}/log.all-target-libgcc.${host}.make

build_mlibc:
	@for mlib in ${MULTILIBS}; do \
		[ $${mlib} = ".;" ] && continue; \
		mlib_dir=$$(echo "$${mlib}" | cut -d';' -f1); \
		mlib_flags=$$(echo "$${mlib}" | cut -d';' -f2 | sed 's/@/ -/g'); \
		raw_arch=$$(echo "$${mlib_dir}" | cut -d'/' -f1); \
		arch=$$([ "$${raw_arch}" = "thumb" ] && echo "arm" || echo "$${raw_arch}"); \
		echo "Building $${mlib_dir} (ARCH=$${arch}) with flags: $${mlib_flags}"; \
		mkdir -p "${BUILDDIR}/src_mlibc/build/$${mlib_dir}"; \
		(cd "${BUILDDIR}/src_mlibc" && \
			make \
			ARCH="$${arch}" \
			CC='${BUILDDIR}/obj_gcc/gcc/xgcc -B ${BUILDDIR}/obj_gcc/gcc' \
			AR='${BUILDDIR}/obj_binutils/binutils/ar' \
			ARCH_FLAGS="$${mlib_flags}" \
			TARGET_DIR_ARCH="build/$${mlib_dir}") || exit 1; \
		mkdir -p "${sysroot}/lib/$${mlib_dir}"; \
		cp "${BUILDDIR}/src_mlibc/build/$${mlib_dir}/libmlibc.a" "${sysroot}/lib/$${mlib_dir}/libc.a"; \
		cp "${BUILDDIR}/src_mlibc/build/$${mlib_dir}/crtobj/"*.o "${sysroot}/lib/$${mlib_dir}/"; \
	done
ifeq (${MULTILIB}, 0)
	cd ${BUILDDIR}/src_mlibc && make
	-mkdir -p ${sysroot}/lib/
	cp ${BUILDDIR}/src_mlibc/build/arm/libmlibc.a ${sysroot}/lib/libc.a
	cp ${BUILDDIR}/src_mlibc/build/arm/crtobj/crt0.o ${sysroot}/lib/crt1.o
	cp ${BUILDDIR}/src_mlibc/build/arm/crtobj/crti.o ${sysroot}/lib/crti.o
	cp ${BUILDDIR}/src_mlibc/build/arm/crtobj/crtn.o ${sysroot}/lib/crtn.o
endif


build_gcc:
	mkdir -p ${BUILDDIR}/obj_gcc
	#cd ${BUILDDIR}/obj_gcc && ../src_gcc/configure ${GCC_CONFIG} > ${LOGDIR}/log.gcc.${host}.configure 
	cd ${BUILDDIR}/obj_gcc && ${MAKE}  all > ${LOGDIR}/log.gcc.${host}.make

build_mlibc_for_kernel:
	mkdir -p ${BUILDDIR}/obj_mlibc_for_kernel
	cd ${BUILDDIR}/obj_mlibc_for_kernel && ../src_mlibc/configure  --prefix= --host=${target} ${FULL_MLIBC_CONFIG} --disable-shared CFLAGS=${BUILD_CFLAGS}${MLIBC_KERNEL_CFLAGS} > ${LOGDIR}/log.mlibc_for_kernel.${host}.configure
	cd ${BUILDDIR}/obj_mlibc_for_kernel && ${MAKE} ${MAKE_MLIBC_CONFIG} > ${LOGDIR}/log.mlibc_for_kernel.${host}.make
	cd ${BUILDDIR}/obj_mlibc_for_kernel && cp lib/libc.a ${sysroot}/lib/libc_kernel.a > ${LOGDIR}/log.mlibc_for_kernel.${host}.install

install: 
	cd ${BUILDDIR}/obj_binutils  && ${MAKE} DESTDIR=${INSTALLDIR}/            install-strip         > ${LOGDIR}/log.binutils.${host}.install
	#cd ${BUILDDIR}/obj_mlibc      && ${MAKE} DESTDIR=${INSTALLDIR}/${target}   install               > ${LOGDIR}/log.mlibc.${host}.install-headers
	cp -r ${BUILDDIR}/src_mlibc/include ${INSTALLDIR}/${target}
	cd ${BUILDDIR}/obj_gcc       && ${MAKE} DESTDIR=${INSTALLDIR}/            install-strip         > ${LOGDIR}/log.gcc.${host}.install
	#@echo "Installing multilib libraries..."
	@for mlib in ${MULTILIBS}; do \
		[ $${mlib} = ".;" ] && continue; \
		mlib_dir=$$(echo "$${mlib}" | cut -d';' -f1); \
		mkdir -p ${INSTALLDIR}/${target}/lib/$${mlib_dir} && \
		cp -f ${sysroot}/lib/$${mlib_dir}/libc.a ${INSTALLDIR}/${target}/lib/$${mlib_dir}/libc.a && \
		cp -f ${sysroot}/lib/$${mlib_dir}/*.o ${INSTALLDIR}/${target}/lib/$${mlib_dir}/; \
	done

all:
	make download
	make prepare
	make build_binutils
	make build_mlibc_headers
	make build_all_gcc
	#- make build_all_target_libgcc
	make build_mlibc
	make build_all_target_libgcc
	#make build_mlibc_for_kernel
	make build_gcc
	make install

update_mlibc:
	make build_mlibc_headers
	make build_mlibc
	make build_mlibc_for_kernel
	cd ${BUILDDIR}/obj_mlibc && cp lib/libc.a ${INSTALLDIR}/${target}/lib/libc.a > ${LOGDIR}/log.mlibc.${host}.install
	cd ${BUILDDIR}/obj_mlibc && cp lib/libc.so ${INSTALLDIR}/${target}/lib/libc.so > ${LOGDIR}/log.mlibc.${host}.install
	cd ${BUILDDIR}/obj_mlibc_for_kernel && cp lib/libc.a ${INSTALLDIR}/${target}/lib/libc_kernel.a > ${LOGDIR}/log.mlibc_for_kernel.${host}.install

pack:
	tar zcvf arm-linux-mlibceabi.tar.gz arm-linux-mlibceabi/

.PHONY: install all
	install:

clean:
	rm -rf ${BUILDDIR}/obj_*
	rm -rf ${BUILDDIR}/sysroot
	rm -rf ${INSTALLDIR}
