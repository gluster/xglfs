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

* Linux (tested with 4.4.0)
* cmake (tested with 3.4.1)
* make (tested with GNU Make 4.1)
* gcc (tested with 5.3.0), clang (tested with 3.7.1) or ICC (tested with 16.0.1)
* FUSE (tested with 2.9.4)
* glusterfs-api (tested with 7.3.7.6)

### Compiling

First, initialize and update git submodules:

`git submodule update --init --recursive`

Then, create `build` folder, chdir to it, then run

`cmake ..`

or

`cmake -DCMAKE_BUILD_TYPE=Debug ..`

to build app with debug info. Then just type `make`.

Usage
-----

Syntax:

`xglfs <[tcp|udp]:server:[port]:volume:[GlusterFS_logfile]:[verbose]:[debug]:[syslog]:[foreground]> <mountpoint>`

Typical usage:

`xglfs :glusterfs.example.com::bigvolume::::1: /mnt/bigvolume`

Distribution and Contribution
-----------------------------

Distributed under terms and conditions of GNU GPL v3 (only).

The following people are involved in development:

* Oleksandr Natalenko &lt;oleksandr@natalenko.name&gt;

Mail them any suggestions, bugreports and comments.
