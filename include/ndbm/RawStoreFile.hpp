#ifndef NDBM_RAW_STORE_FILE_HPP
#define NDBM_RAW_STORE_FILE_HPP

#include <string>

#include <ndbm.h>

namespace ndbm {
	
	class RawStoreFile {
	
	public:
		
		RawStoreFile(const std::string& filenameStart, int flags, int mode);
		
		~RawStoreFile();
		
		void open();
	
		void close();
	
		datum fetch(datum key);
	
		int store(datum key, datum content, int mode);
	
	private:
		
		RawStoreFile(const RawStoreFile&);
		
		RawStoreFile& operator=(const RawStoreFile&);
				
		std::string _filenameStart;
		
		int _flags;
		
		int _mode;
		
		DBM* _dbm;
		
	};
	
}

#endif
