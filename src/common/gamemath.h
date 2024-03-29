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

	inline float getDistance(const V3& v1, const V3& v2)
	{
		float dist = sqrtf(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
		return dist;
	}

	inline void matrixTranspose(Matrix* pOut, Matrix* pMat)
	{
		*pOut = XMMatrixTranspose(*pMat);
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

	inline Matrix V3ToMatrix(V3 v3, MatrixType type)
	{
		Matrix mat;
		const float &x = v3.x, &y = v3.y, &z = v3.z;
		switch (type)
		{
		case MATRIX_TYPE_TRANSLATE:
			gameMathNS::matrixTranslation(&mat, x, y, z);
			break;

		case MATRIX_TYPE_SCALL:
			gameMathNS::matrixScaling(&mat, x, y, z);
			break;

		case MATRIX_TYPE_ROTATE:
			gameMathNS::matrixRotationX(&mat, x);
			gameMathNS::matrixRotationY(&mat, y);
			gameMathNS::matrixRotationZ(&mat, z);
			break;

		default:
			break;
		}

		return mat;
	}

	inline float getAngle(const V3& a, const V3& b)
	{
		return atan2(a.x - b.x, a.y - b.y);
	}

	inline float getAngle(const Vector3D& vector)
	{
		return gameMathNS::getAngle(vector.begin, vector.end);
	}
}
