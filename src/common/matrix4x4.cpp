/*
D3DXMATRIX::operator () (UINT iRow, UINT iCol)
{
	return m[iRow][iCol];
}

D3DXINLINE FLOAT
D3DXMATRIX::operator () (UINT iRow, UINT iCol) const
{
	return m[iRow][iCol];
}


// casting operators
D3DXINLINE
D3DXMATRIX::operator FLOAT* ()
{
	return (FLOAT *)&_11;
}

D3DXINLINE
D3DXMATRIX::operator CONST FLOAT* () const
{
	return (CONST FLOAT *) &_11;
}


// assignment operators
D3DXINLINE D3DXMATRIX&
D3DXMATRIX::operator *= (CONST D3DXMATRIX& mat)
{
	D3DXMatrixMultiply(this, this, &mat);
	return *this;
}

D3DXINLINE D3DXMATRIX&
D3DXMATRIX::operator += (CONST D3DXMATRIX& mat)
{
	_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
	_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
	_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
	_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
	return *this;
}

D3DXINLINE D3DXMATRIX&
D3DXMATRIX::operator -= (CONST D3DXMATRIX& mat)
{
	_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
	_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
	_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
	_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
	return *this;
}

D3DXINLINE D3DXMATRIX&
D3DXMATRIX::operator *= (FLOAT f)
{
	_11 *= f; _12 *= f; _13 *= f; _14 *= f;
	_21 *= f; _22 *= f; _23 *= f; _24 *= f;
	_31 *= f; _32 *= f; _33 *= f; _34 *= f;
	_41 *= f; _42 *= f; _43 *= f; _44 *= f;
	return *this;
}

D3DXINLINE D3DXMATRIX&
D3DXMATRIX::operator /= (FLOAT f)
{
	FLOAT fInv = 1.0f / f;
	_11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
	_21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
	_31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
	_41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
	return *this;
}


// unary operators
D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator + () const
{
	return *this;
}

D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator - () const
{
	return D3DXMATRIX(-_11, -_12, -_13, -_14,
		-_21, -_22, -_23, -_24,
		-_31, -_32, -_33, -_34,
		-_41, -_42, -_43, -_44);
}


// binary operators
D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator * (CONST D3DXMATRIX& mat) const
{
	D3DXMATRIX matT;
	D3DXMatrixMultiply(&matT, this, &mat);
	return matT;
}

D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator + (CONST D3DXMATRIX& mat) const
{
	return D3DXMATRIX(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
		_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
		_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
		_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
}

D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator - (CONST D3DXMATRIX& mat) const
{
	return D3DXMATRIX(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
		_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
		_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
		_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
}

D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator * (FLOAT f) const
{
	return D3DXMATRIX(_11 * f, _12 * f, _13 * f, _14 * f,
		_21 * f, _22 * f, _23 * f, _24 * f,
		_31 * f, _32 * f, _33 * f, _34 * f,
		_41 * f, _42 * f, _43 * f, _44 * f);
}

D3DXINLINE D3DXMATRIX
D3DXMATRIX::operator / (FLOAT f) const
{
	FLOAT fInv = 1.0f / f;
	return D3DXMATRIX(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
		_21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
		_31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
		_41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);
}


D3DXINLINE D3DXMATRIX
operator * (FLOAT f, CONST D3DXMATRIX& mat)
{
	return D3DXMATRIX(f * mat._11, f * mat._12, f * mat._13, f * mat._14,
		f * mat._21, f * mat._22, f * mat._23, f * mat._24,
		f * mat._31, f * mat._32, f * mat._33, f * mat._34,
		f * mat._41, f * mat._42, f * mat._43, f * mat._44);
}


D3DXINLINE BOOL
D3DXMATRIX::operator == (CONST D3DXMATRIX& mat) const
{
	return 0 == memcmp(this, &mat, sizeof(D3DXMATRIX));
}

D3DXINLINE BOOL
D3DXMATRIX::operator != (CONST D3DXMATRIX& mat) const
{
	return 0 != memcmp(this, &mat, sizeof(D3DXMATRIX));
}
*/