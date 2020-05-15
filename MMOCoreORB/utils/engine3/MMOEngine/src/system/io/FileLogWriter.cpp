/*
** Copyright (C) 2007-2019 SWGEmu
** See file COPYING for copying conditions.
*/

#include "FileLogWriter.h"

#include <cerrno>
#include <string.h>

int FileLogWriter::write(const char* str, int len) {
	validateWriteable();

	if (doSynchronized) {
		writeMutex.lock();
	}

	auto result = fwrite(str, 1, len, file->getDescriptor());

	currentLoggedBytes.add(len);

	if (currentLoggedBytes >= maxLoggedBytes) {
		rotatefile();
	}

	if (doSynchronized) {
		writeMutex.unlock();
	}

	return result;
}

void FileLogWriter::rotatefile(bool force) const {
	if (!force && maxLoggedBytes == 0) {
		return;
	}

	if(file == nullptr || !file->exists() || !rotateMutex.tryLock()) {
		return;
	}

	if (file->size() == 0) {
		return;
	}

	Time now;
	String fileName = file->getName();
	String dir = file->getDirName();
	String baseFilename = file->getBaseName();
	String ext;

	int i = baseFilename.lastIndexOf('.');

	if (i > 0) {
		ext = baseFilename.subString(i);
		baseFilename = baseFilename.subString(0, i);
	}

	StringBuffer buf;
	buf << dir << File::directorySeparator() << rotatePrefix;

	File::mkpath(buf.toString());

	buf << baseFilename << "-" << now.getMiliTime() << ext;
	String destFileName = buf.toString();

	int err = std::rename(fileName.toCharArray(), destFileName.toCharArray());

	if (err != 0) {
		char buf[1024];
#ifdef PLATFORM_MAC
		char* msg = buf;
		strerror_r(errno, buf, sizeof(buf));
#else // PLATFORM_MAC
#ifndef PLATFORM_WIN
		char* msg = strerror_r(errno, buf, sizeof(buf));
#else // PLATFORM_WIN
		char* msg = "<unknown on windows>";
#endif // PLATFORM_WIN
#endif // PLATFORM_MAC

		// Need to avoid loop of calling logger that calls us, but we also need someone to notice this issue and resolve it
		System::err << "WARNING: Failed to rotate " << fileName << " to " << destFileName << " errno=" << errno << " (" << msg << ")" << endl;
	} else {
#if DEBUG_FILELOGWRITER
		System::out << "Rotated " << fileName << " to " << destFileName << " size=" << currentLoggedBytes << endl;
#endif // DEBUG_FILELOGWRITER
		file->reopen();
	}

	// Don't try again until we write another maxLoggedBytes
	currentLoggedBytes.set(0);
	rotateMutex.unlock();
}
