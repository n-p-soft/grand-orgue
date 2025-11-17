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

#ifndef MIDI_READER_HPP
#define MIDI_READER_HPP

#include "midi_reader.h"

typedef midi_frame_state_t MidiFrameState;
typedef midi_reader_flags_t MidiReaderFlags;
typedef midi_reader_callback_t MidiReaderFunc;
typedef midi_frame_t MidiFrame;
typedef midi_reader_stats_t MidiReaderStats;

/* A MIDI reader. */
class MidiReader
{
	protected:

	midi_reader_t reader;

	public:

	/* Create a MIDI reader. 'to_skip' may be NULL or a pointer to a ZERO-
	 * terminated array of status bytes; frames starting with one of these
	 * bytes will be skipped. User should call method "addSurce".
	 */
	MidiReader (MidiReaderFlags flags, const unsigned char *to_skip);

	/* Destroy this MIDI reader. */
	virtual ~MidiReader ();

	/* Get the version of the library as a 3-digits number (100, 101,..). */
	static int getVersion ();

	/* Add a MIDI-in file descriptor to the reader. Return false on failure.
	 * If 'channel' is a value between 1 and 16, then the channel 'n' for
	 * all channel-type messages (0x8n-0xEn) is changed to this value.
	 */
	bool addSource (int fd, int channel);
			
	/* Same as other method "addSource" but using a file path that will be
	 * opened. */
	bool addSource (const char *path, int channel);

	/* Remove a MIDI-in file descriptor from the reader. Return false on
	 * failure. */
	bool removeSource (int fd);

	/* Set the file descriptor where to dump frames.
	 * Returns false on error.
	 * Dump file is closed when calling "close" method.
	 */
	bool setDumpFile (int fd);

	/* Set the file where to dump frames. 'path' will be opened and
	 * truncated if 'trunc' is true. Returns false on error.
	 * Dump file is closed when calling "close" method.
	 */
	bool setDumpFile (const char *path, bool trunc);

	/* Set a user callback function with optional argument. Note that when
	 * using a callback, you should call regularly method "getNext" or
	 * "clearQueue" so that the internal queue get not full.
	 * That is, when a callback is set and a MIDI frame is validated, the
	 * callback is invoked, then  the frame will be stored into the internal
	 * queue and dump'ed if MidiReaderFlags::COMPLETE is returned. Else the
	 * frame is not stored nor dump'ed.
	 * The callback occurs just before dumping the frame, so the data may be
	 * properly changed.
	 */
	void setCallback (MidiReaderFunc cb, void *userData);

	/* Close this MIDI reader. Note that method "getNext" may be called
	 * after this one until the frames already read and stored in the
	 * internal queue are exhausted, but no new frame will be read.
	 */
	void close ();

	/* Returns -1 no MIDI-in source is readable, 0 if there is no byte to
	 * read, else the count of MIDI-in devices having data to read from.
	 */
	int poll ();

	/* Reset a MIDI frame. */
	static void resetFrame (MidiFrame& frame);

	/* Dump a MIDI frame to a file descriptor (hexadecimal). */
	static void dumpFrame (MidiFrame& frame, int fd);

	/* Return true if there is a MIDI frame that was read. Should be called
	 * regularly to read new frames and store them in the internal queue.i
	 */
	bool update ();

	/* Return next valid MIDI frame read by the reader, or NULL if none. */
	MidiFrame* getNext ();

	/* Remove all recorded frames. */
	void clearQueue ();

	/* Expand given MIDI frame if it is a running status one. May return
	 * false on error, for example if there was not enough room available
	 * in the frame (but this should not occur often).
 	*/
	static bool expandFrame (MidiFrame& frame);

	/* Inject a MIDI frame that will be processed immediately.
	 * Beware that the frame will appear when calling "getNext" and thru
	 * the user callback, if any.
	 * Return the count of bytes processed. May be less than the length of
	 * the frame in case or error (erroneous frame, internal queue is full,
	 * ..).
	 * Only one valid frame may be injected at once.
 	 */
	int inject (MidiFrame& frame);

	/* Inject a frame of 'n' bytes.
	 * See other method "inject" for details.
	 */
	int inject (int n, ...);

	/* Get statistics for nth input source (0..; or -1 for cumulated).
	 * Return false on error.
	 */
	bool getStats (int n, MidiReaderStats& stats);

	/* Reset statistics for nth input source (0..; or -1 for global ones).
	 */
	void resetStats (int n);

	/* Get the count of frames in the internal queue. */
	unsigned int available ();
};

#endif /* MIDI_READER_HPP */
