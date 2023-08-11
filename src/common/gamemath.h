// gamemath.h
// Author: abramann

#pragma once

#include "types.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace gameMathNS
{
	inline XMVECTOR xmVectorSet(V3 vec)
	{
		return XMVectorSet(vec.x, vec.y, vec.z, 0);
	}

	inline void matrixTranspose(Matrix* mat1, Matrix* mat2)
	{
		*mat1 = XMMatrixTranspose(*mat2);
	}
	inline void matrixTranslation(Matrix* mat, float x, float y, float z)
	{
		*mat = XMMatrixTranslation(x, y, z);
	}

	inline void matrixScaling(Matrix* mat, float x, float y, float z)
	{
		*mat = XMMatrixScaling(x, y, z);
	}

	inline void matrixRotationX(Matrix* mat, float x)
	{
		*mat = XMMatrixRotationX(x);
	}

	inline void matrixRotationY(Matrix* mat, float y)
	{
		*mat = XMMatrixRotationY(y);
	}

	inline void matrixRotationZ(Matrix* mat, float z)
	{
		*mat = XMMatrixRotationZ(z);
	}

	inline void matrixLookAtLH(Matrix* eye, V3* position, V3* target, V3* up)
	{
		XMVECTOR pos = xmVectorSet(*position);
		XMVECTOR tar = xmVectorSet(*target);
		XMVECTOR u = xmVectorSet(*up);
		*eye = XMMatrixLookAtLH(pos, tar, u);
	}

	inline void matrixPerspectiveFovLH(Matrix* proj, float fov, float aspectRatio, float nearPlane,
		float farPlane)
	{
		*proj = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
	}

	inline void matrixIdentity(Matrix* mat)
	{
		*mat = XMMatrixIdentity();
	}
}