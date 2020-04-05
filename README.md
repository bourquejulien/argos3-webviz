
<br/>
<p align="center" style="background:#edeff3;">
        <img width="80%" style="max-width:540px" src="client/images/banner_light.png" alt="Webviz banner">
</p>

<br/>
<p align="center">
    <a href="https://github.com/NESTLab/argos3-webviz/blob/master/LICENSE" target="_blank">
        <img src="https://img.shields.io/github/license/NESTLab/argos3-webviz.svg" alt="GitHub license">
    </a>
    <a href="https://github.com/NESTLab/argos3-webviz/releases" target="_blank">
        <img src="https://img.shields.io/github/tag/NESTLab/argos3-webviz.svg" alt="GitHub tag (latest SemVer)">
    </a>
    <a href="https://github.com/NESTLab/argos3-webviz/commits/master" target="_blank">
        <img src="https://img.shields.io/github/commit-activity/m/NESTLab/argos3-webviz.svg" alt="GitHub commit activity">
    </a>
        <img src="https://img.shields.io/github/last-commit/NESTLab/argos3-webviz" alt="GitHub last commit">

</p>
<br/>


# ARGoS3-Webviz
A Web interface plugin for [ARGoS 3](https://www.argos-sim.info/).

| All builds | Ubuntu 16.04  | Ubuntu 18.04  | Mac OSX |
|:-:|:-:|:-:|:-:|
| [![Travis build](https://img.shields.io/travis/com/NESTLab/argos3-webviz)](https://travis-ci.com/NESTLab/argos3-webviz) | [![Ubuntu 16.04 build](https://travis-matrix-badges.herokuapp.com/repos/NESTLab/argos3-webviz/branches/master/1?use_travis_com=true)](https://travis-ci.com/NESTLab/argos3-webviz) | [![Ubuntu 18.04 build](https://travis-matrix-badges.herokuapp.com/repos/NESTLab/argos3-webviz/branches/master/2?use_travis_com=true)](https://travis-ci.com/NESTLab/argos3-webviz) | [![MacOSX build](https://travis-matrix-badges.herokuapp.com/repos/NESTLab/argos3-webviz/branches/master/3?use_travis_com=true)](https://travis-ci.com/NESTLab/argos3-webviz) |

# Features

![screencast](https://raw.githubusercontent.com/wiki/NESTLab/argos3-webviz/screencast.gif)

- All communication over Websockets
- SSL support (protocol `wss://`)
- Only single port needed(Easier for NAT/forwarding/docker)
- filterable channels (broadcasts, events, logs)
- easily extendable for custom robots/entities.
- Independent Web client files.
- Simple client protocol, can easily be implemented in any technology
- Using UWebSockets, which is blazing fast([Benchmarks](https://github.com/uNetworking/uWebSockets/blob/master/misc/websocket_lineup.png)).
- The event-loop is native epoll on Linux, native kqueue on macOS
# Installing

### Dependencies
#### Homebrew 
```console
$ brew install cmake git zlib openssl
```
#### Debian
```console
$ sudo apt install cmake git zlib1g-dev libssl-dev
```
#### Fedora
```console
$ sudo dnf install cmake git zlib-devel openssl-devel
```

You can [Download pre-compiled binaries from Releases](https://github.com/NESTLab/argos3-webviz/releases)

or

<details>
<summary style="font-size:18px">Installing from source</summary>
<br>

### Requirements
- A `UNIX` system (Linux or Mac OSX; Microsoft Windows is not supported)
- `ARGoS 3`
- `g++` >= 5.7 (on Linux)
- `clang` >= 3.1 (on MacOSX)
- `cmake` >= 3.5.1
- `zlib` >= 1.x
- `git` (for autoinstalling dependencies using Cmake `ExternalProject`)

**Optional dependency**
- `OpenSSL` >= 1.1 (for websockets over SSL)

Please [install all dependencies](#installing) before continuing


### Downloading the source-code
```console
$ git clone https://github.com/NESTLab/argos3-webviz
```

### Compiling
The compilation is configured through CMake.

```console
$ cd argos3-webviz
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ../src
$ make
$ sudo make install
```

You can use `-DCMAKE_BUILD_TYPE=Debug` instead of `Release` with the cmake command above to enable debugging.

</details>


# Usage
Edit your Argos Experiment file (.argos), and change the visualization tag as:
```xml
.. 
..
<visualization>
    <webviz />
    <!-- <qt-opengl /> -->
</visualization>
..
..
```
i.e. add `<webviz/>` in place of default `<qt-opengl />`


Then run the argos experiment as usual

```console
$ argos3 -c EXPERIMENT_FILE.argos
```
This starts argos experiment with the webviz server.

*Note:* If you do not have an experiment file, you can check [http://argos-sim.info/examples.php](http://argos-sim.info/examples.php)

or run an example project,
```console
$ argos3 -c src/testing/testexperiment.argos
```

## Web Client
The web client code is placed in `client` directory (or download it as zip from the [Releases](https://github.com/NESTLab/argos3-webviz/releases)). This folder needs to be *served* through an http server(for example `apache`, `nginx`, `lighthttpd`).

The easiest way is to use python's inbuilt server, as python is already installed in most of *nix systems.

Run these commands in the terminal
```bash
$ cd client
$ python3 -m http.server 8000
```
To host the files in folder client over http port 8000.


Now you can access the URL using any browser.

> [http://localhost:8000](http://localhost:8000)


*Visit [http static servers one-liners](https://gist.github.com/willurd/5720255) for alternatives to the python3 server shown above.*

<details>
<summary style="font-size:20px">Configuration</summary>
<br>
[You can check more documentation in `docs` folder](docs/README.md)

#### REQUIRED XML CONFIGURATION
```xml

  <visualization>
    <webviz />
  </visualization>
```

#### OPTIONAL XML CONFIGURATION 
with all the defaults:
```xml
  <visualization>
    <webviz port=3000
         broadcast_frequency=10
         ff_draw_frames_every=2
         autoplay="true"
         ssl_key_file="NULL"
         ssl_cert_file="NULL"
         ssl_ca_file="NULL"
         ssl_dh_params_file="NULL"
         ssl_cert_passphrase="NULL"
    />
  </visualization>
```

Where:

`port(unsigned short)`: is the network port to listen incoming traffic on (Websockets and HTTP both share the same port)
```
Default: 3000
Range: [1,65535]

Note: Ports less < 1024 need root privileges.
```

`broadcast_frequency(unsigned short)`: Frequency (in Hertz) at which to broadcast the updates(through websockets)
```
Default: 10
Range: [1,1000]
```
`ff_draw_frames_every(unsigned short)`: Number of steps to skip when in fast forward mode
```
Default: 2
```
`autoplay(bool)`: Allows user to auto-play the simulation at startup
```
Default: false
```

#### SSL CONFIGURATION

SSL can be used to host the server over "wss"(analogous to "https" for websockets).

**NOTE**: You need Webviz to be compiled with OpenSSL support to use SSL.

You might have to use any combination of the following to enable SSL, depending upon your implementation.

- ssl_key_file
- ssl_cert_file
- ssl_ca_file
- ssl_dh_params_file
- ssl_cert_passphrase

Where file parameters supports relative and absolute paths.

**NOTE**: Webviz need read access to the files.


[You can check more documentation in `docs` folder](docs/README.md)

</details>

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.
[Check full contributing info](docs/CONTRIBUTING.md)

## License
[MIT](https://choosealicense.com/licenses/mit/)

Licenses of libraries used are in their respective directories.


## Limitations
OpenGL Loop functions are currently neglected in this plugin, as they are QT-OpenGL specific.