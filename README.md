# ionospheric-ray-tracer
Simulate the behavior of radio waves in planetary ionospheres by applying a ray tracing method

## Quickstart ##
Run the main application in src/core/main.cpp to start the simulation.

## prerequisites ##
This project makes use of the google test library gtest as well as the multithreading lib boost/thread.
Gtest is included in the source but it is required to have boost/thread installed locally. See for more
information on the boost libraries: [boost getting started](http://www.boost.org/doc/libs/1_57_0/more/getting_started/index.html)

## Installation ##

The following section treats how to prepare your system for this package if you want to compile it from source.

### Linux (Ubuntu 14.04) ###

1. Install git if you haven't already:
```
sudo apt-get install git
```

2. Ensure that the required build packages and libraries are installed.
The minimum required gcc is gcc 4.9.
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install build-essential g++ gcc python-dev autotools-dev libicu-dev build-essential libbz2-dev libstdc++
```

Install the boost libraries. The minimum required version is libboost-1.55.
```
sudo aptitude install libboost-all-dev
```

The next step is to download and compile the json library. Note that the version number of gcc might vary on your system. In the example below I used gcc-4.9.2:
```
sudo apt-get install scons
git clone https://github.com/open-source-parsers/jsoncpp.git
cd ~/jsoncpp
scons platform=linux-gcc
sudo cp libs/linux-gcc-4.9.2/libjson_linux-gcc-4.9.2_libmt.so /usr/lib
```