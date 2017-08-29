#pragma once

#include <string>
#include "../math/Vector3.h"
namespace igad
{

/// Read an entire text file into a string. Only use for small files.
///
/// @filename   Full path to the file
///
std::string ReadFile(const std::string& filename);

/// Save text to a file
/// @filename   Full path to the file
/// @text       Text to save
///
bool SaveFile(const std::string& filename, const std::string& text);

/// Spherical coordinates defined with radius and two angles
struct SphericalCoordinates
{
	/// Radius
	float r;

	/// Inclination
	float theta;

	/// Azimuth
	float fi;
};


/// Convert a cartesian 3D vector to spherical coordinates
SphericalCoordinates CartesianToSpherical(const Vector3& cartesianVector);

/// Converts from spherical coordinates to cartesian
Vector3 SphericalToCartesian(const SphericalCoordinates& sphericalCoord);


}
