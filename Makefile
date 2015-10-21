all: ./build ./build/out/Release/geowave-plugin

./deps/gyp:
	git clone --depth 1 https://chromium.googlesource.com/external/gyp.git ./deps/gyp

ifneq (,$(wildcard config.gypi))
CONFIG = "--include=config.gypi"
endif

./build: ./deps/gyp
	deps/gyp/gyp geowave-plugin.gyp ${CONFIG} --depth=. -f make --generator-output=./build

./build/out/Release/geowave-plugin: ./build/
	make -C ./build V=1

clean:
	rm -rf ./build

