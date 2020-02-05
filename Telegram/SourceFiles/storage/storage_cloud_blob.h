/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "mtproto/dedicated_file_loader.h"

namespace Storage::CloudBlob {

constexpr auto kCloudLocationUsername = "tdhbcfiles"_cs;

struct Blob {
	int id = 0;
	int postId = 0;
	int size = 0;
	QString name;
};

struct Available {
	int size = 0;

	inline bool operator<(const Available &other) const {
		return size < other.size;
	}
	inline bool operator==(const Available &other) const {
		return size == other.size;
	}
};
struct Ready {
	inline bool operator<(const Ready &other) const {
		return false;
	}
	inline bool operator==(const Ready &other) const {
		return true;
	}
};
struct Active {
	inline bool operator<(const Active &other) const {
		return false;
	}
	inline bool operator==(const Active &other) const {
		return true;
	}
};
struct Failed {
	inline bool operator<(const Failed &other) const {
		return false;
	}
	inline bool operator==(const Failed &other) const {
		return true;
	}
};

using Loading = MTP::DedicatedLoader::Progress;
using BlobState = base::variant<
	Available,
	Ready,
	Active,
	Failed,
	Loading>;

bool UnpackBlob(
	const QString &path,
	const QString &folder,
	Fn<bool(const QString &)> checkNameCallback);

class BlobLoader : public QObject {
public:
	BlobLoader(
		QObject *parent,
		int id,
		MTP::DedicatedLoader::Location location,
		const QString &folder,
		int size);

	int id() const;

	rpl::producer<BlobState> state() const;
	virtual void destroy() = 0;
	virtual void unpack(const QString &path) = 0;

protected:
	void fail();

	const QString _folder;

private:
	void setImplementation(std::unique_ptr<MTP::DedicatedLoader> loader);

	int _id = 0;
	rpl::variable<BlobState> _state;

	MTP::WeakInstance _mtproto;
	std::unique_ptr<MTP::DedicatedLoader> _implementation;

};

} // namespace Storage::CloudBlob
