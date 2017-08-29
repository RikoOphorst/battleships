#include "debug_renderer.h"

#include <memory>

using namespace igad;

// Actualy creates a new DebugDraw, but the real work is in the Init not the ctor
// This serves as a manager
DebugRenderer igad::gDebugDraw3D;

////////////////////////////////////////////////////////////////////////////////
// No args constructor
////////////////////////////////////////////////////////////////////////////////
DebugRenderer::DebugRenderer()
{
    // Set values
    _linesCount = 0;
    _pointsCount = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////////////////////
DebugRenderer::~DebugRenderer() {}

////////////////////////////////////////////////////////////////////////////////
// Initialize - Acts as a constructor
////////////////////////////////////////////////////////////////////////////////
void DebugRenderer::Initialize()
{
	auto vxShader =
		"#version 430 core												\n\
		in vec3 a_position;												\n\
		in vec4 a_color;												\n\
		out vec4 v_color;												\n\
		uniform mat4 u_worldviewproj;									\n\
																		\n\
		void main()														\n\
		{																\n\
			v_color = a_color;											\n\
			gl_Position = u_worldviewproj * vec4(a_position, 1.0);		\n\
		}";

	auto psShader =
		"#version 430 core												\n\
		in vec4 v_color;												\n\
		out vec4 fragColor;												\n\
																		\n\
		void main()														\n\
		{																\n\
			fragColor = v_color;										\n\
		}";

		
	_shader = std::make_unique<Shader>();
	_shader->LoadBySource(vxShader, "", psShader);
	
	_attribColor = _shader->GetAttribute("a_color");
	_attribVertex = _shader->GetAttribute("a_position");
	_paramCamera = _shader->GetUniform("u_worldviewproj");
}

////////////////////////////////////////////////////////////////////////////////
// Draw
////////////////////////////////////////////////////////////////////////////////
void igad::DebugRenderer::Draw(Matrix44& vp)
{
	if (!_shader)
		return;

	_shader->Activate();

	_paramCamera->SetValue(vp);

	if (_linesCount > 0)
	{
		_attribVertex->SetAttributePointer(3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(VertexPosition3DColor),
			&_vertexArray[0].Position);
		//
		_attribColor->SetAttributePointer(4,
			GL_UNSIGNED_BYTE,
			GL_TRUE,
			sizeof(VertexPosition3DColor),
			&_vertexArray[0].Color);
		//
		glDrawArrays(GL_LINES, 0, _linesCount * 2);
		GL_GET_ERROR();
	}

	// Draw points too (try the same _shader)
	if (_pointsCount > 0)
	{
		_attribVertex->SetAttributePointer(3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(VertexPosition3DColor),
			&_pointArray[0].Position);
		//
		_attribColor->SetAttributePointer(4,
			GL_UNSIGNED_BYTE,
			GL_TRUE,
			sizeof(VertexPosition3DColor),
			&_pointArray[0].Color);
		//
		glDrawArrays(GL_POINTS, 0, _pointsCount);
		GL_GET_ERROR();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Clear
////////////////////////////////////////////////////////////////////////////////
void DebugRenderer::Clear()
{
	_linesCount = 0;
	_pointsCount = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Add line
////////////////////////////////////////////////////////////////////////////////
void DebugRenderer::AddLine(const Vector3& from, const Vector3& to, const Color& color)
{
	if (_linesCount < _maxLines)
    {
		_vertexArray[_linesCount * 2].Position     = from;
		_vertexArray[_linesCount * 2 + 1].Position = to;
        //
        _vertexArray[_linesCount * 2].Color       = color;
		_vertexArray[_linesCount * 2 + 1].Color   = color;
        //
		++_linesCount;
	}
    // else ignore
}

////////////////////////////////////////////////////////////////////////////////
// AddCircle
////////////////////////////////////////////////////////////////////////////////
void DebugRenderer::AddCircle(const Vector3& center,
                            float radius,
                            const Color& color,
                            int divs)
{
	float dt = Pi / float(divs);
	float t = 0.0f;
	
	Vector3 v0(center.x + radius * cos(t), center.y, center.z + radius * sin(t));
	for (; t < TwoPi - dt; t += dt)
	{
		Vector3 v1(center.x + radius * cos(t + dt), center.y, center.z + radius * sin(t + dt));
		//
		AddLine(v0, v1, color);
		v0 = v1;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Add Sphere
////////////////////////////////////////////////////////////////////////////////
void DebugRenderer::AddAxisSphere(const Vector3& center,
                                float radius,
                                const Color& xzColor,
                                const Color& xyColor,
                                const Color& yzColor,
                                int divs)
{
	float dt = Pi / float(divs);
	float t = 0.0f;
	
	Vector3 vh0(center.x + radius * cos(t), center.y, center.z + radius * sin(t));
	for (; t < TwoPi - dt; t += dt)
	{
		Vector3 vh1(center.x + radius * cos(t + dt),
                    center.y,
                    center.z + radius * sin(t + dt));
		//
		AddLine(vh0, vh1, xzColor);
		vh0 = vh1;
	}
    
    Vector3 vv0(center.x + radius * cos(t),
                center.y + radius * sin(t),
                center.z);
    t = 0.0f;
	for (; t < TwoPi - dt; t += dt)
	{
		Vector3 vv1(center.x + radius * cos(t + dt),
                    center.y + radius * sin(t + dt),
                    center.z);
		//
		AddLine(vv0, vv1, xyColor);
		vv0 = vv1;
	}
    
    vv0 = Vector3(center.x,
                  center.y + radius * sin(t),
                  center.z + radius * cos(t));
    t = 0.0f;
	for (; t < TwoPi - dt; t += dt)
	{
		Vector3 vv1(center.x,
                    center.y + radius * sin(t + dt),
                    center.z + radius * cos(t + dt));
		//
		AddLine(vv0, vv1, yzColor);
		vv0 = vv1;
	}

}


////////////////////////////////////////////////////////////////////////////////
// Add Axis
////////////////////////////////////////////////////////////////////////////////
void DebugRenderer::AddAxis(const Matrix44 &trans, float size)
{
    Vector3 x(size, 0, 0);
    Vector3 y(0, size, 0);
    Vector3 z(0, 0, size);
    Vector3 o(0, 0, 0);
    //
    x = trans * x;
    y = trans * y;
    z = trans * z;
    o = trans * o;
    //
    AddLine(o, x, Color::Red);          // X is red
    AddLine(o, y, Color::Green);        // Y is greeen
    AddLine(o, z, Color::Blue);         // Z is blue
}

////////////////////////////////////////////////////////////////////////////////
// Queues a 2d point for drawing
////////////////////////////////////////////////////////////////////////////////
void DebugRenderer::AddPoint(const Vector3& position, const Color& color)
{
	if (_pointsCount < _maxPoints)
	{
		_pointArray[_pointsCount].Position = position;
		_pointArray[_pointsCount].Color = color;
		//
		++_pointsCount;
	}
    // else ignore
}

