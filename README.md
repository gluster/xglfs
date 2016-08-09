xglfs
=====

Description
-----------

GlusterFS API FUSE client.

Principles
----------

xglfs uses system FUSE library and GlusterFS API library
to provide access to GlusterFS volumes.

Compiling
---------

### Prerequisites

* Linux (tested with 4.7.0)
* cmake (tested with 3.6.1)
* make (tested with GNU Make 4.2.1)
* gcc (tested with 6.1.1), clang (tested with 3.8.1) or ICC (tested with 16.0.3)
* FUSE (tested with 2.9.7)
* glusterfs-api (tested with 7.3.8.1)

### Compiling

Create `build` folder, chdir to it, then run

`cmake ..`

or

`cmake -DCMAKE_BUILD_TYPE=Debug ..`

to build app with debug info. Then just type `make`.

Usage
-----

Syntax:

`xglfs --server=<IP|DOMAIN> --volume=VOLUME --mountpoint=MOUNTPOINT [--protocol=<tcp|udp>] [--port=24007] [--logfile=<file|stderr|null>] [--verbosity=7] [--foreground]`

Typical usage:

`xglfs --server=glusterfs.example.com --volume=bigdata --mountpoint=/mnt/bigdata`

Distribution and Contribution
-----------------------------

Distributed under terms and conditions of GNU GPL v3 (only).

The following people are involved in development:

* Oleksandr Natalenko &lt;oleksandr@natalenko.name&gt;

Mail them any suggestions, bugreports and comments.
