#pragma once

#include "../util/color.h"
#include "../graphics/shader.h"
#include "../math/Vector3.h"
#include "../math/Matrix44.h"

#include <memory>

namespace igad
{

///
/// Debug drawing in 3d space (good for testing ideas and stuff). Once a drawing
/// max has been reached, all subsequent draw calls are silently ignored.
///
class DebugRenderer
{
	struct VertexPosition3DColor
	{
		Vector3 Position;
		Color Color;
	};
        
public:
    /// No args, for static creation
    DebugRenderer();
        
    /// Does nothing
    virtual ~DebugRenderer();

	/// Call to make debug draw, gets the _shader program id
	void Initialize();
                
    /// Queues a line segment for drawing
	void AddLine(	const Vector3& from,
                    const Vector3& to,
                    const Color& color = igad::Color::Red);
        
	/// Queues a circle for drawing
	void AddCircle(const Vector3& center,
                    float radius         = 1.0f,
                    const Color& color   = igad::Color::Red,
                    int divs             = 12);
        
    /// Queues a sphere for drawing
	void AddAxisSphere(const Vector3& center,
                        float radius         = 1.0f,
                        const Color& xzColor = igad::Color::Green,
                        const Color& xyColor = igad::Color::Blue,
                        const Color& yzColor = igad::Color::Red,
                        int divs             = 12);

    /// Queues a sphere for drawing
	void AddSphere(const Vector3& center,
                    float radius         = 1.0f,
                    const Color& color   = igad::Color::Red,
                    int divs             = 12)
    { AddAxisSphere(center, radius, color, color, color,divs); }
        
    /// Queues an axis display
    void AddAxis(const Matrix44& trans, float size);
		
	/// Queues a point for drawing
	void AddPoint(const Vector3& position, const Color& color = igad::Color::Red);
        
    /// Clears queued actions
	void Clear();

	/// Draw queue with a view projection matrix
	void Draw(Matrix44& vp);

protected:

	static int const _maxLines = 4096;
	static const int _maxPoints = 1024;

	// Lines
	int                     _linesCount;
	VertexPosition3DColor   _vertexArray[_maxLines * 2];

	// Points
	int                     _pointsCount;
	VertexPosition3DColor   _pointArray[_maxPoints];

	// Use a unique pointer to delete this Shader at the end of execution
	// and raw pointers to non-owning params an
	std::unique_ptr<Shader>	_shader;
	ShaderAttribute*	_attribColor	= nullptr;
	ShaderAttribute*	_attribVertex	= nullptr;
	ShaderUniform*	_paramCamera	= nullptr;
};
    
    /// Manager global
    extern DebugRenderer gDebugDraw3D;
}
