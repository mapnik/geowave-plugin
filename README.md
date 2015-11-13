# GeoWave Plugin
##About

[![Build Status Linux](https://secure.travis-ci.org/mapnik/geowave-plugin.png)](http://travis-ci.org/mapnik/geowave-plugin)

GeoWave Plugin is an open source input plugin for Mapnik.

[GeoWave](http://ngageoint.github.io/geowave/) is an open source set of software that:
 * Adds multi-dimensional indexing capability to [Apache Accumulo](https://accumulo.apache.org)
 * Adds support for geographic objects and geospatial operators to [Apache Accumulo](https://accumulo.apache.org)
 * Provides Map-Reduce input and output formats for distributed processing and analysis of geospatial data

# Plugin Installation

## Dependencies

### Build System
 * C++ compiler supporting `-std=c++11` (e.g. >= g++ 4.8 or >= clang++ 3.4)
 * [GYP](https://gyp.gsrc.io/)

### Libraries
 * boost_system
    - >= 1.57
 * boost_thread
    - >= 1.57
 * mapnik
    - >= 3.0.8
 * jvm
    - >= JDK 7
 * geowave
    - >= 0.8.7
    - [source](http://s3.amazonaws.com/geowave-rpms/release/TARBALL/geowave-0.8.7-c8ef40c-jace-source.tar.gz)
    - [docs](http://ngageoint.github.io/geowave/documentation.html#generate-proxies-and-build-from-source)

### Test Libraries
 * gtest
    - >= 1.7.0

### Runtime Environment/Jars
 * Java Runtime Environment
    - >= 7
 * GeoWave Runtime Jar
    - >= 0.8.7
 * GeoWave Ingest Jar
    - >= 0.8.7
    - Required for test

## Configuration

We use GYP as our build system.  You can setup a custom configuration by specifying variables in config.gypi

Here is a list of variables which you can override:

 * boost_home
    - boost_includes
    - boost_libs
 * geowave_home
    - geowave_includes
    - geowave_libs
    - geowave_runtime_jar
 * java_home
    - java_include_path
    - java_include_path2
    - java_jvm_lib
 * mapnik_config
    - mapnik_includes
    - mapnik_libs
    - mapnik_name
 * gtest_includes
 * gtest_libs

Once you have created config.gypi, you are ready to proceed to the build step.

## Building

We provide a simple Makefile wrapper which can be used as follows:

    make && make install

This will compile the geowave.input plugin and install it to the directory pointed to by:

```bash
./mapnik_config --input-plugins
```

The release library is built by default, but you can build the debug library using:

    make debug

For more details, or to build directly with GYP, check out the contents of the Makefile.

## Testing

### Configuration

First, download the GeoWave runtime and ingest jars contained within the jace-source packages provided by GeoWave [here](http://ngageoint.github.io/geowave/packages.html).  

Next, create two environment variables which point to the runtime and ingest jars:

```bash
export GEOWAVE_RUNTIME_JAR=/path/to/jar
export GEOWAVE_INGEST_JAR=/path/to/jar
```

### Running Tests

Once configured, you can run the tests locally using the Makefile wrapper as follows:

    make test

