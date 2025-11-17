# GrandOrgue for FreeBSD

This is a port of the church-organ sample-based simulator "GrandOrgue" v3.14.2 for FreeBSD 13+. If you are running Linux, Windows or MacOS, see the original GrandOrgue repository on Github: [https://github.com/GrandOrgue].

License: GPL2 and later.

## Current status (2025/11/17): beta

The sound rendering part of the software and the user interface are working fine, but not always the MIDI part (so, you cannot play in this case..).

GrandOrgue gets its MIDI input from the RtMidi library, and various sources:

	MIDI-IN ==> jack_umidi or jack_midi server ==> Jack server ==> RtMidi
	             
	MIDI-IN ==> OSS ==> RtMidi

If you are using USB-MIDI adapter, you are on the (too) long way at the top.
I experienced too much latency in this case; I'm trying to correct this.
I hope you will have better luck using a MIDI card supported by OSS (see below).

## Hardware requirements

You will need a card to produce the sound and, if it does not support MIDI, another device to add it. The sound stack under FreeBSD, may be managed

1. either natively by the kernel: kernel modules "sound", "snd_xxx" (various cards, I suggest a SoundBlaster Audigy one), "snd_uaudio" (all USB-based MIDI interfaces),

2. and/or by OSS (Open Sound System) which brings a list of extra supported cards and provides kernel modules (named "osscore", "oss_xxx") as well as user-space tools.

The trick is that native kernel modules and OSS ones are not compatible. But you can use the user-space part of OSS together with the native modules. GrandOrgue generates its sound output using OSS user-space libraries.

So you cannot have, for example, a sound-card supported natively and one other for MIDI that is only supported by OSS. Your soundcard, and possibly a separate MIDI one, must be supported either natively **OR** by OSS.

Furthermore, if your hardware is only supported by OSS, you need to recompile the FreeBSD kernel such that the module "sound" is not statically linked to the kernel, so that you can choose between the native sound framework and the OSS one.

Note that the large church-organs with multiple channels may require up to 128GB of RAM to work. Smallest ones are working with 4GB. Conversely, more CPU cores are better. To store these organs, you will also need some space on your hard-drive; any kind church organ may add about 5-10 GB of data for the sample set itself and the cache.

## What I'm currently using

- FreeBSD 13.5
- old server motherboard with 32GB ECC Ram, a quad-core Opteron 2379HE, and a PCIe-to-PCI adapter for the soundcard
- old dedicated 160 GB SSD
- a SoundBlaster Audigy 2 sound-card (NOTE: this card has no official MIDI support until the actual kernel is corrected, but I also have one USB to MIDI adapter)

## Software requirements

### Build system

Install the package (or the port) cmake.

### Window manager

Install your favorite one (for example fluxbox, xfce).

Also needed for the graphical stack:

- wxWidgets toolkit (package wx30-gtk3)
- ImageMagick7

### OSS ###

Install the package or build the port audio/oss. The modules are stored under /usr/local/lib/oss/modules if you need them.

### Various packages or ports

- portaudio
- wavpack
- ZitaConvolver

### Jack

- jack (package jackit, or port audio/jack) and tool qjackctl
- jack_umidi or jack_midi (see my Github account) if you are using USB-MIDI
(more to come about this)

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

Before this, you must ensure the modules for your sound hardware are loaded (you may have to add lines < MODULE_load="YES" > to the file /boot/loader.conf):

- for a natively supported sound or MIDI card, modules to load are "sound", "snd_xxx" and/or "snd_uaudio" (for USB-to-MIDI adapters). For a SoundBlaster Live or Audigy, it is "snd_emu10k1" while for an Audigy 2, it is "snd_emu10kx" for example

	su root -c 'kldload sound'
	su root -c 'kldload snd_uaudio'
	su root -c 'kldload snd_emu10kx'

- for OSS-supported hardware, modules are "osscore" and "oss_xxx", depending on the card

See the man pages for sound(4), snd_uaudio(4), oss(4)..

You should add some lines

	MODULE_load="YES"

to the file /boot/loader.conf so that the modules are loader on startup.

(to come: Jack configuration)

## Notes

- I've included the libraries RtAudio and RtMidi in this source tree, such that corresponding packages are not needed in the system, nor git submodules

- I'm currently adding the "direct" API to RtMidi to enhance the MIDI stack

## Author

(c) Nicolas Provost, 2025 for the changes needed to run on FreeBSD and the MIDI layer.

Original authors: see the GrandOrgue repository on github.com; you will find help here to use the software and download sample sets.


