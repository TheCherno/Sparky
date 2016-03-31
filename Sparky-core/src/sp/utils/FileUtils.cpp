#include "sp/sp.h"
#include "FileUtils.h"
#include <fstream>

namespace sp { namespace utils {

	String ReadFile(const String& filepath)
	{
		std::ifstream file{ filepath, std::ios_base::in || std::ios_base::binary };
		if ( !file.good( ) )
			SP_ASSERT( false, "Could not open file '", filepath, "'!" );
		std::string result{ std::istreambuf_iterator<char>( file ),
				    std::istreambuf_iterator<char>( ) };

		// Strip carriage returns
		result.erase( std::remove( result.begin( ), result.end( ), '\r' ), result.end( ) );
		return result;
	}

} }
