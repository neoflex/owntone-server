# Installation instructions for forked-daapd

This document contains instructions for installing forked-daapd from the git
tree.

The source for this version of forked-daapd can be found here:
[owntone/owntone-server](https://github.com/owntone/owntone-server.git)

## Quick version for Raspbian (Raspberry Pi)

See the instructions here:
[Improved forked-daapd (iTunes server) -
Raspberry Pi Forums](http://www.raspberrypi.org/phpBB3/viewtopic.php?t=49928)

## Quick version for Debian/Ubuntu users

If you are the lucky kind, this should get you all the required tools and
libraries:

```bash
sudo apt-get install \
  build-essential git autotools-dev autoconf automake libtool gettext gawk \
  gperf antlr3 libantlr3c-dev libconfuse-dev libunistring-dev libsqlite3-dev \
  libavcodec-dev libavformat-dev libavfilter-dev libswscale-dev libavutil-dev \
  libasound2-dev libmxml-dev libgcrypt20-dev libavahi-client-dev zlib1g-dev \
  libevent-dev libplist-dev libsodium-dev libjson-c-dev libwebsockets-dev \
  libcurl4-openssl-dev
```

Note that forked-daapd will also work with other versions and flavours of
libgcrypt and libcurl, so the above are just suggestions.

While forked-daapd will work with versions of libevent between 2.0.0 and 2.1.3,
it is recommended to use 2.1.4+. Otherwise you may not have support for
simultaneous streaming to multiple DAAP clients.

Optional packages:

 Feature             | Configure argument       | Packages
 --------------------|--------------------------|------------------------------------------------
 Chromecast          | `--enable-chromecast`    | libgnutls*-dev libprotobuf-c-dev
 Spotify             | `--enable-spotify`       | libspotify-dev
 Player web UI       | `--disable-webinterface` | libwebsockets-dev
 Live web UI         | `--with-libwebsockets`   | libwebsockets-dev
 Pulseaudio          | `--with-pulseaudio`      | libpulse-dev

Then run the following (adding configure arguments for optional features):

```bash
git clone https://github.com/owntone/owntone-server.git
cd forked-daapd
autoreconf -i
./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var
make
sudo make install
```

Finally, read the section [Long version - after installation
](#Long-version---after-installation) in the bottom of this document, which
describes configuration, setting up init scripts and adding a system user.
Also see the [README.md](README.md) for usage information.

## Quick version for Fedora

If you haven't already enabled the free RPM fusion packages do that, since you
will need ffmpeg. You can google how to do that. Then run:

```bash
sudo yum install \
  git automake autoconf gettext-devel gperf gawk libtool \
  sqlite-devel libconfuse-devel libunistring-devel mxml-devel libevent-devel \
  avahi-devel libgcrypt-devel zlib-devel alsa-lib-devel ffmpeg-devel \
  libplist-devel libsodium-devel json-c-devel libwebsockets-devel \
  libcurl-devel
```

Clone the forked-daapd repo:

```bash
git clone https://github.com/owntone/owntone-server.git
cd forked-daapd
```

Now you need to install ANTLR3, but you probably can't use the version that
comes with the package manager (but do try that first). Instead you can install
it by running this script:

```bash
scripts/antlr35_install.sh
```

Then run the following:

```bash
autoreconf -i
./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var
make
sudo make install
```

Finally, read the section [Long version - after installation
](#Long-version---after-installation) in the bottom of this document, which
describes configuration, setting up init scripts and adding a system user.
Also see the README for usage information.

## Quick version for FreeBSD

You can use the ports framework to handle dependencies and build options, so
the following is sufficient to build from source on FreeBSD:

```sh
cd /usr/ports/audio/forked-daapd
make install
```

Otherwise, there is a script in the 'scripts' folder that will at least attempt
to do all the work for you. And should the script not work for you,
you can still look through it and use it as an installation guide.

## Quick version for macOS (using Homebrew)

This workflow file used for building forked-daapd via Github actions includes
all the steps that you need to execute:
[.github/workflows/macos.yml](.github/workflows/macos.yml)

## "Quick" version for macOS (using macports)

Caution: macports requires many downloads and lots of time to install
(and sometimes build) ports... you'll want a decent network connection
and some patience!

Install macports (which requires Xcode):
  https://www.macports.org/install.php

Install Apple's Java (this enables java command on OSX 10.7+):
  https://support.apple.com/kb/DL1572?locale=en_US

Afterwards, you can optionally install Oracle's newer version, and then
  choose it using the Java pref in the System Preferences:
  http://www.oracle.com/technetwork/java/javase/downloads/index.html

```bash
sudo port install \
  autoconf automake libtool pkgconfig git gperf libgcrypt \
  libunistring libconfuse ffmpeg libevent json-c libwebsockets curl \
  libplist libsodium
```

Download, configure, build and install the Mini-XML library:
  http://www.msweet.org/projects.php/Mini-XML

Download, configure, build and install the libinotify library:
  https://github.com/libinotify-kqueue/libinotify-kqueue

Add the following to `.bashrc`:

```bash
# add /usr/local to pkg-config path
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:/opt/local/lib/pkgconfig
# libunistring doesn't support pkg-config, set overrides
export LIBUNISTRING_CFLAGS=-I/opt/local/include
export LIBUNISTRING_LIBS="-L/opt/local/lib -lunistring"
```

Optional features require the following additional ports:

 Feature             | Configure argument       | Ports
 --------------------|--------------------------|-------------------
 Chromecast          | `--enable-chromecast`    | gnutls protobuf-c
 Pulseaudio          | `--with-pulseaudio`      | pulseaudio

Clone the forked-daapd repo:

```bash
git clone https://github.com/owntone/owntone-server.git
cd forked-daapd
```

Install antlr3 and library using the included script:

```bash
scripts/antlr35_install.sh -p /usr/local
```

Finally, configure, build and install, adding configure arguments for
optional features:

```bash
autoreconf -i
./configure
make
sudo make install
```

Note: if for some reason you've installed the avahi port, you need to
add `--without-avahi` to configure above.

Edit `/usr/local/etc/forked-daapd.conf` and change the `uid` to a nice
system daemon (eg: unknown), and run the following:

```bash
sudo mkdir -p /usr/local/var/run
sudo mkdir -p /usr/local/var/log # or change logfile in conf
sudo chown unknown /usr/local/var/cache/forked-daapd # or change conf
```

Run forked-daapd:

```bash
sudo /usr/local/sbin/forked-daapd
```

Verify it's running (you need to <kbd>Ctrl</kbd>+<kbd>C</kbd> to stop
dns-sd):

```bash
dns-sd -B _daap._tcp
```

## Long version - requirements

Required tools:

- ANTLR v3 is required to build forked-daapd, along with its C runtime
  (libantlr3c). Use a version between 3.1.3 and 3.5 of ANTLR v3 and the
  matching C runtime version. Get it from <http://www.antlr3.org/>
- Java runtime: ANTLR is written in Java and as such a JRE is required to
  run the tool. The JRE is enough, you don't need a full JDK.
- autotools: autoconf 2.63+, automake 1.10+, libtool 2.2. Run `autoreconf -i`
  at the top of the source tree to generate the build system.
- gettext: libunistring requires iconv and gettext provides the autotools
  macro definitions for iconv.
- gperf

Libraries:

- libantlr3c (ANTLR3 C runtime, use the same version as antlr3)  
  from <https://github.com/antlr/website-antlr3/tree/gh-pages/download/C>
- Avahi client libraries (avahi-client), 0.6.24 minimum  
  from <http://avahi.org/>
- sqlite3 3.5.0+ with unlock notify API enabled (read below)  
  from <http://sqlite.org/download.html>
- libav 9+ or ffmpeg 0.11+  
  from <http://libav.org/> or <http://ffmpeg.org/>
- libconfuse  
  from <http://www.nongnu.org/confuse/>
- libevent 2.0+ (best with 2.1.4+)  
  from <http://libevent.org/>
- MiniXML (aka mxml or libmxml)  
  from <http://minixml.org/software.php>
- gcrypt 1.2.0+  
  from <http://gnupg.org/download/index.en.html#libgcrypt>
- zlib  
  from <http://zlib.net/>
- libunistring 0.9.3+  
  from <http://www.gnu.org/software/libunistring/#downloading>
- libjson-c  
  from <https://github.com/json-c/json-c/wiki>
- libcurl
  from <http://curl.haxx.se/libcurl/>
- libplist 0.16+
  from <http://github.com/JonathanBeck/libplist/downloads>
- libsodium
  from <https://download.libsodium.org/doc/>
- libasound (optional - ALSA local audio)
  often already installed as part of your distro
- libpulse (optional - Pulseaudio local audio)
  from <https://www.freedesktop.org/wiki/Software/PulseAudio/Download/>
- libspotify (optional - Spotify support)
  from <https://developer.spotify.com>
- libgnutls (optional - Chromecast support)
  from <http://www.gnutls.org/>
- libprotobuf-c (optional - Chromecast support)
  from <https://github.com/protobuf-c/protobuf-c/wiki>
- libwebsockets 2.0.2+ (optional - websocket support)
  from <https://libwebsockets.org/>

If using binary packages, remember that you need the development packages to
build forked-daapd (usually named -dev or -devel).

sqlite3 needs to be built with support for the unlock notify API; this isn't
always the case in binary packages, so you may need to rebuild sqlite3 to
enable the unlock notify API (you can check for the presence of the
sqlite3_unlock_notify symbol in the sqlite3 library). Refer to the sqlite3
documentation, look for `SQLITE_ENABLE_UNLOCK_NOTIFY`.

libav (or ffmpeg) is a central piece of forked-daapd and most other FLOSS
multimedia applications. The version of libav you use will potentially have a
great influence on your experience with forked-daapd.

## Long version - building and installing

Start by generating the build system by running `autoreconf -i`. This will
generate the configure script and `Makefile.in`.

The configure script will look for a wrapper called antlr3 in the PATH to
invoke ANTLR3. If your installation of ANTLR3 does not come with such a
wrapper, create one as follows:

```bash
#!/bin/sh
CLASSPATH=...
exec /path/to/java -cp $CLASSPATH org.antlr.Tool "$@"
```

Adjust the `CLASSPATH` as needed so that Java will find all the jars needed
by ANTLR3.

The parsers will be generated during the build, no manual intervention is
needed.

To display the configure options `run ./configure --help`.

Support for Spotify is optional. Use `--enable-spotify` to enable this feature.
If you enable this feature `libspotify/api.h` is required at compile time.
Forked-daapd uses runtime dynamic linking to the libspotify library, so even
though you compiled with `--enable-spotify`, the executable will still be able
to run on systems without libspotify (the Spotify features will then be disabled).

Support for LastFM scrobbling is optional. Use `--enable-lastfm` to enable this
feature.

Support for the MPD protocol is optional. Use `--disable-mpd` to disable this
feature.

Support for Chromecast devices is optional. Use `--enable-chromecast` to enable
this feature.

The player web interface is optional. Use `--disable-webinterface` to disable
this feature.
If enabled, `sudo make install` will install the prebuild html, js, css files.
The prebuild files are:

- `htdocs/index.html`
- `htdocs/player/*`

The source for the player web interface is located under the `web-src` folder and
requires nodejs >= 6.0 to be built. In the `web-src` folder run `npm install` to
install all dependencies for the player web interface. After that run `npm run build`.
This will build the web interface and update the `htdocs` folder.
(See [README_PLAYER_WEBINTERFACE.md](README_PLAYER_WEBINTERFACE.md) for more
informations)

Building with libwebsockets is required if you want the web interface. It will be enabled
if the library is present (with headers). Use `--without-libwebsockets` to disable.

Building with Pulseaudio is optional. It will be enabled if the library is
present (with headers). Use `--without-pulseaudio` to disable.

Recommended build settings:

```bash
./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var
```

After configure run the usual make, and if that went well, `sudo make install`.

You may see two kinds of warnings during make.
First, `/usr/bin/antlr3` may generate a long series of warnings that
begin like this:

```log
warning(24):  template error: context ...
```

Second, you may see compiler warnings that look like this:

```log
RSPLexer.c: In function `mESCAPED':
RSPLexer.c:2674:16: warning: unused variable `_type' [-Wunused-variable]
  ANTLR3_UINT32 _type;
                ^~~~~
```

You can safely ignore all of these warnings.

## Long version - after installation

After installation:

- edit the configuration file, `/etc/forked-daapd.conf`
- make sure the Avahi daemon is installed and running (Debian:
  `apt install avahi-daemon`)

Note that `sudo make install` will not install any system files to start the
service after boot, and it will not setup a system user.

forked-daapd will drop privileges to any user you'll specify in the
configuration file if it's started as root.

This user must have read permission on your library (you can create a group for
this and make the user a member of the group, for instance) and read/write
permissions on the database location (`$localstatedir/cache/forked-daapd` by
default).

If your system uses systemd then you might be able to use the service file
included, see `forked-daapd.service`.

Otherwise you might need an init script to start forked-daapd at boot. A simple
init script will do, forked-daapd daemonizes all by itself and creates a
pidfile under `/var/run`. Different distributions have different standards for
init scripts and some do not use init scripts anymore; check the documentation
for your distribution.

For dependency-based boot systems, here are the forked-daapd dependencies:

- local filesystems
- network filesystems, if needed in your setup (library on NFS, ...)
- networking
- NTP
- Avahi daemon

The LSB header below sums it up:

```bash
### BEGIN INIT INFO
# Provides:          forked-daapd
# Required-Start:    $local_fs $remote_fs $network $time
# Required-Stop:     $local_fs $remote_fs $network $time
# Should-Start:      avahi
# Should-Stop:       avahi
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: DAAP/DACP (iTunes) server, support for AirPlay and Spotify
# Description:       forked-daapd is an iTunes-compatible media server for
#                    sharing your media library over the local network with DAAP
#                    clients like iTunes. Like iTunes, it can be controlled by
#                    Apple Remote (and compatibles) and stream music directly to
#                    AirPlay devices. It also supports streaming to RSP clients
#                    (Roku devices) and streaming from Spotify.
### END INIT INFO
```
