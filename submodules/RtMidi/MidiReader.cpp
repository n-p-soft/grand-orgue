/*-
 * Copyright (c) 2025 Nicolas Provost <dev@nicolas-provost.fr>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdarg.h>
#include "MidiReader.h"

extern "C" {
#include "midi_reader.c"
}

int
MidiReader::getVersion ()
{
	return (MIDI_READER_VERSION);
}

int
MidiReader::poll ()
{
	return (midi_reader_poll (&this->reader));
}

MidiReader::MidiReader (MidiReaderFlags flags,
			const unsigned char *to_skip)
{
	midi_reader_init (&this->reader, flags, to_skip);
}

MidiReader::~MidiReader ()
{
	this->close ();
	this->clearQueue ();
}

bool
MidiReader::addSource (const char *path, int channel)
{
	return (midi_reader_add_source_path (&this->reader, path, channel));
}

bool
MidiReader::addSource (int fd, int channel)
{
	return (midi_reader_add_source (&this->reader, fd, channel));
}

bool
MidiReader::removeSource (int fd)
{
	return (midi_reader_remove_source (&this->reader, fd));
}

bool
MidiReader::setDumpFile (int fd)
{
	return (midi_reader_set_dump_fd (&this->reader, fd));
}

bool
MidiReader::setDumpFile (const char *path, bool trunc)
{
	return (midi_reader_set_dump_file (&this->reader, path, trunc));
}

void
MidiReader::setCallback (MidiReaderFunc cb, void *user_data)
{
	midi_reader_set_callback (&this->reader, cb, user_data);
}

void
MidiReader::resetFrame (MidiFrame& frame)
{
	midi_frame_reset (&frame);
}

void
MidiReader::close ()
{
	midi_reader_close (&this->reader);
}

void
MidiReader::dumpFrame (MidiFrame& frame, int fd)
{
	midi_frame_dump (&frame, fd);
}

bool
MidiReader::update ()
{
	return (midi_reader_update (&this->reader));
}

MidiFrame*
MidiReader::getNext ()
{
	return (midi_reader_get_next (&this->reader));
}

void
MidiReader::clearQueue ()
{
	midi_reader_clear_queue (&this->reader);
}

bool
MidiReader::expandFrame (MidiFrame& frame)
{
	return (midi_frame_expand_running (&frame));
}

int
MidiReader::inject (MidiFrame& frame)
{
	return (midi_reader_inject (&this->reader, &frame));
}

int
MidiReader::inject (int n, ...)
{
	va_list va;
	MidiFrame f;

	if (n <= 0 || n > MIDI_FRAME_MAX)
		return (0);
	resetFrame (f);
	va_start (va, n);
	for (int i = 0; i < n; i++)
		f.data[i] = va_arg (va, int);
	f.len = n;
	va_end (va);
	return (midi_reader_inject (&this->reader, &f));
}

bool
MidiReader::getStats (int n, MidiReaderStats& stats)
{
	return (midi_reader_get_stats (&this->reader, n, &stats));
}

void
MidiReader::resetStats (int n)
{
	midi_reader_reset_stats (&this->reader, n);
}

unsigned int
MidiReader::available ()
{
	return (this->reader.frames.len);
}

