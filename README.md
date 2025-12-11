# GrandOrgue for FreeBSD

Last updated on 2025-12-10.

This is a port of the church-organ sample-based simulator "GrandOrgue" v3.14.2 for FreeBSD 13+. If you are running Linux, Windows or MacOS, see [the official GrandOrgue repository on Github](https://github.com/GrandOrgue).

License: GPL2 and later.

## Current status

The sound rendering part of the software and the user interface are working fine. The MIDI part is now working, using our custom version of RtMidi only.

The demo sample set is fully usable.

![screenshot](resources/fbsd.png)

## Hardware requirements

### Soundcard

You will need a card to produce the sound and, if it does not support MIDI, another one to add it. The sound stack under FreeBSD, may be managed

1. either natively by the kernel: kernel modules "sound", "snd_xxx" for various cards - I suggest a SoundBlaster one - "snd_uaudio" for any USB-based MIDI interface

2. and/or by OSS (Open Sound System) which brings a list of extra supported cards and provides kernel modules named "osscore", "oss_xxx" as well as user-space tools

The trick is that native kernel modules and OSS ones are not compatible. But you can use the user-space part of OSS together with the native modules. GrandOrgue generates its sound output using OSS user-space libraries.

So you cannot have, for example, one soundcard supported natively and one other for MIDI that is only supported by OSS. Your soundcard(s) must be supported either natively **OR** by OSS.

Furthermore, if your hardware is only supported by OSS, you need to recompile the FreeBSD kernel such that the module "sound" is not statically linked to the kernel. You will then be able to choose between the native sound framework (main module is "sound") and the OSS one (main module is "osscore").

Note that the large church-organs with multiple channels may require up to 128GB of RAM to work. For smallest ones with 16-bit samples, 4GB may be sufficient. Likewise, more CPU cores is better. To store these organs, you will also need some space on your hard-drive; any kind church organ may add about 5-10 GB of data for the sample set itself and the cache.

### MIDI

Your soundcard may have a game/MIDI port but you will not be able to use it because for now, MIDI support in FreeBSD is broken (see below for more details). You have to use a MIDI-to-USB adapter. Do not buy a too cheap one, some are very sensitive to electromagnetic noise.

### The hardware I'm currently running on

- old server motherboard with 32GB ECC Ram and a quad-core Opteron 2379HE
- old dedicated 160 GB SSD
- a SoundBlaster Audigy 2 PCI sound-card plugged into a PCIe-to-PCI bridge card

## Software requirements

### FreeBSD

I recently switched to FreeBSD 15.0, which was released on 4, December 2025.

I made some pull requests to enable the support of MIDI on PCI sound cards in FreeBSD. I hope this work will be integrated into FreeBSD 15.1; see the [freebsd-src/midi-all repository on my Github account](https://github.com/n-p-soft/freebsd-src/tree/midi-all/sys/dev/sound/midi). For now, unless you want to compile a custom Kernel after applying these patches, as explained below, you can use one USB-to-MIDI adapter.

### Building FreeBSD 15.0 with MIDI support for PCI cards

In the directory "fbsd/15.0" of this repository, you will find the patched files needed to build a FreeBSD 15.0 kernel with MIDI support for PCI cards. To build the kernel on amd64, assuming the FreeBSD sources are stored under /usr/src, run:

	cp -R fbsd/15.0/sound /usr/src/sys/dev
	make -C /usr/src -j4 buildkernel
	su root -c 'make -C /usr/src installkernel'

In addition, you can use the file fbsd/15.0/GENERIC as a custom kernel config suitable for a PC dedicated to GrandOrgue on AMD/Intel x86-64 architecture. Before starting 'make buildkernel', run:

	mv /usr/src/sys/amd64/conf/GENERIC /usr/src/sys/amd64/conf/GENERIC.sav
	cp fbsd/15.0/GENERIC /usr/src/sys/amd64/conf

This config should be used if your soundcard is supported by OSS only.

### Build system

Install the package (or the port) cmake.

### Window manager

Install your favorite one (for example fluxbox, xfce).

Also needed for the graphical stack:

- wxWidgets toolkit (package wx30-gtk3)
- ImageMagick7

### Audio subsystem

- OSS: if your soundcard needs it, install the package or build the port audio/oss. The modules are stored under /usr/local/lib/oss/modules.
- portaudio
- wavpack
- ZitaConvolver

### MIDI

On UNIX-like systems such as FreeBSD or Linux, the Jack server is often used to manage MIDI devices, together with the jack_umidi tool (or jack_midi: see my Github account to get it).  This was not working for me under FreeBSD 13.5: too much latency.

So I made a custom version of the library RtMidi, which is embedded in the source tree of GrandOrgue here, and allows to directly access the raw MIDI devices, for much better efficiency:

	MIDI raw device /dev/midiXXX => custom RtMidi (rtmidi-direct)

You may have to install the Jack server to compile GrandOrgue, but it will not be used for MIDI: package jackit, or port audio/jack.

## How to build

In the directory storing the sources, run:

	mkdir build
	cd build
	cmake ..
	make -j4
	su root -c 'make install'

The third step will fail if you are missing one dependency. Install it and retry from this step.

## How to run it

On success, run the executable GrandOrgue from your window manager. There is a demo organ to test your config.

Before this, you must ensure the modules for your sound hardware are loaded.

For a natively supported sound or MIDI card, modules to load are "sound", "snd_xxx" and/or "snd_uaudio" (for USB-to-MIDI adapters). For a SoundBlaster Live or Audigy, it is "snd_emu10k1" while for an Audigy 2, it is "snd_emu10kx" for example.

	su root -c 'kldload sound'
	su root -c 'kldload snd_uaudio'
	su root -c 'kldload snd_emu10kx'

For OSS-supported hardware, modules are "osscore" and "oss_xxx", depending on the card.

See the man pages for sound(4), snd_uaudio(4), oss(4)..

You should add some lines

	MODULE_load="YES"

to the file /boot/loader.conf so that these modules are loaded on startup.

## Notes

### Submodules included in the tree

I've included the libraries RtAudio and RtMidi in this source tree, so that corresponding packages are not needed in the system, nor git submodules.

### Touchscreen

It is convenient to have a touch screen to toggle the stops of the organ. The module to load is "wmt".

## Author

(c) Nicolas Provost, 2025 for the changes needed to run on FreeBSD and the MIDI layer.

Original authors: see the GrandOrgue repository on github.com; you will find help there to use the software and download sample sets.


