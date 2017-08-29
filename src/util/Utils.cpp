#include <sstream>
#include <fstream>
#include "../util/Utils.h"
#include "../util/Defines.h"
#include "../memory/memory_manager.h"
#include "../memory/linear_allocator.h"

using namespace std;
using namespace igad;

string igad::ReadFile(const string& filename)
{
	unsigned int length;
	char* buffer;

	ifstream is;
	is.open(filename.c_str(), ifstream::in);

	// Check for bad paths
	if (!is.is_open())
	{
		LOG("Unable to open file: %s\n", filename.c_str());
		return "";
	}

	// get length of file:
	is.seekg(0, ios::end);
	length = static_cast<unsigned int>(is.tellg());
	is.seekg(0, ios::beg);

	// allocate memory
	memory::LinearAllocator* allocator = memory::MemoryManager::Instance()->ConstructAllocator<memory::LinearAllocator>(length + 50);

	buffer = memory::MemoryManager::AllocateArray<char>(allocator, length + 1);
	memset(buffer, 0, length + 1);

	// read data as a block:
	is.read(buffer, length);
	is.close();

	// NB: specifying a length parameter will solve any null byte issues, even when
	// reading arbitrary binary data.
	string ret = string(buffer, length + 1);
	
	memory::MemoryManager::Instance()->DestructAllocator(allocator);

	return ret;
}

bool igad::SaveFile(const std::string& filename,
	const std::string& text)
{
	ofstream os;
	os.open(filename.c_str(), ofstream::out);

	// Check for bad paths
	if (!os.is_open())
	{
#ifdef DEBUG
		LOG("Unable to open file for writing: %s\n", filename.c_str());
#endif
		return false;
	}


	// get length of file:
	os.write(text.c_str(), text.size());

	return true;
}

igad::SphericalCoordinates igad::CartesianToSpherical(const Vector3& cvec)
{
	SphericalCoordinates scoord;
	scoord.r		= sqrt(cvec.x*cvec.x + cvec.y*cvec.y + cvec.z*cvec.z);
	scoord.theta	= acos(cvec.y / scoord.r);
	float ratio		= cvec.z / cvec.x;
	scoord.fi		= isnan(ratio) ? 0.0f : atan(ratio);
	return scoord;
}

Vector3 igad::SphericalToCartesian(const SphericalCoordinates& scoord)
{
	Vector3 cvec;	
	cvec.x = scoord.r * sin(scoord.theta) * cosf(scoord.fi);
	cvec.y = scoord.r * cosf(scoord.theta);
	cvec.z = scoord.r * sinf(scoord.theta) * sinf(scoord.fi);
	return cvec;
}
