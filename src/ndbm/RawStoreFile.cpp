#include "ndbm/RawStoreFile.hpp"

#include <stdexcept>


namespace ndbm {
	
	RawStoreFile::RawStoreFile(const std::string& filenameStart, int flags, mode_t mode):
			_filenameStart(filenameStart),
			_flags(flags),
			_mode(mode),
			_dbm(NULL)
	{
	}
	
	RawStoreFile::~RawStoreFile() {
	}
	
	void RawStoreFile::open() {
		if (_dbm != NULL) {
			throw std::runtime_error("Fichier DBM deja ouvert");
		}
		
		const char* filenameStartPt = _filenameStart.c_str();
		_dbm = dbm_open(filenameStartPt, _flags, _mode);
		if (_dbm == NULL) {
			throw std::runtime_error("Erreur a l'ouverture de la base DBM");
		}
	}
	
	void RawStoreFile::close() {
		if (_dbm != NULL) {
			dbm_close(_dbm);
		}
	}
	
	datum RawStoreFile::fetch(datum key) {
		return dbm_fetch(_dbm, key);
	}
	
	int RawStoreFile::store(datum key, datum content, int mode) {
		return dbm_store(_dbm, key, content, mode);
	}
	
}
