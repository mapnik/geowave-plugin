BUILDTYPE=Release

all: ./build/out/${BUILDTYPE}/lib.target/geowave.input

debug: BUILDTYPE=Debug
debug: all
release: all

./deps/gyp:
	git clone --depth 1 https://chromium.googlesource.com/external/gyp.git ./deps/gyp

ifneq (,$(wildcard config.gypi))
CONFIG = --include=config.gypi
endif

./build/out/${BUILDTYPE}: ./deps/gyp
	deps/gyp/gyp geowave-plugin.gyp ${CONFIG} --depth=. -f make --generator-output=./build

./build/out/${BUILDTYPE}/lib.target/libgeowave-plugin.so: ./build/out/${BUILDTYPE}
	make -C ./build V=1 BUILDTYPE=${BUILDTYPE}

./build/out/${BUILDTYPE}/lib.target/geowave.input: ./build/out/${BUILDTYPE}/lib.target/libgeowave-plugin.so
	mv ./build/out/${BUILDTYPE}/lib.target/libgeowave-plugin.so ./build/out/${BUILDTYPE}/lib.target/geowave.input

clean:
	rm -rf ./build

INSTALL_PATH=$(shell cat ./build/out/.plugin_install_path)

install: all
	cp ./build/out/${BUILDTYPE}/lib.target/geowave.input ${INSTALL_PATH}

test: all
	./build/out/${BUILDTYPE}/geowave-plugin-test ./build/out/${BUILDTYPE}/lib.target/geowave.input

.PHONY: clean install test
