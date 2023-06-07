#ifndef MATRIXES_H
#define MATRIXES_H
#include <omp.h>
#include <cmath>
#include "Tuples.h"

const static int max_threads = omp_get_max_threads();

// Classes implementation

class Matrix2x2f
{
public:
	Matrix2x2f(float a00, float a01, float a10, float a11)
	{
		matrix[0] = a00; matrix[1] = a01;
		matrix[2] = a10; matrix[3] = a11;
	}
	Matrix2x2f()
	{
		for (int i = 0; i < 4; i++)	
				matrix[i] = 0;
	}
	void setConstant(float constant)
	{
		for (int i = 0; i < 4; i++)
			matrix[i] = constant;
	}
	void setConstantAsIdentity(float constant)
	{
		matrix[0] = constant; matrix[1] = 0.0f;
		matrix[2] = 0.0f;	  matrix[3] = constant;
	}
	bool isEqual(const Matrix2x2f& b) const
	{
		for (size_t i = 0; i < 4; i++)
		{	
			if (matrix[i] - b.matrix[i] > ERRORMARGIN || b.matrix[i] - matrix[i] > ERRORMARGIN)
					return false;
		}
		return true;
	}
	Matrix2x2f operator*(const Matrix2x2f& b) const
	{
		return
		{
			matrix[0] * b[0] + matrix[1] * b[2], matrix[0] * b[1] + matrix[1] * b[3],
			matrix[0] * b[1] + matrix[1] * b[3], matrix[2] * b[1] + matrix[3] * b[3]
		};
	}
	const float& operator[](int index) const { return matrix[index]; }
	float& operator[](int index) { return matrix[index]; }
	friend std::ostream& operator<<(std::ostream& os, const Matrix2x2f& a)
	{
		for (int i = 0; i < 4; i++) 
		{
			os << a[i] << " ";
			if(i == 1 || i == 3)
				os << std::endl;
		}
		return os;
	}
private:
	float matrix[4];
};

class Matrix3x3f
{
public:
	Matrix3x3f(float a00, float a01, float a02, float a10, float a11,  float a12, float a20, float a21, float a22)
	{
		matrix[0] = a00; matrix[1] = a01; matrix[2] = a02;
		matrix[3] = a10; matrix[4] = a11; matrix[5] = a12;
		matrix[6] = a20; matrix[7] = a21; matrix[8] = a22;
	}
	Matrix3x3f()
	{
		for (int i = 0; i < 9; i++)
			matrix[i] = 0.0f;
	}
	void setConstant(float constant)
	{
		for (int i = 0; i < 9; i++)
			matrix[i] = constant;
	}
	void setConstantAsIdentity(float constant)
	{
		matrix[0] = constant; matrix[1] = 0.0f;		 matrix[2] = 0.0f;
		matrix[3] = 0.0f;	  matrix[4] = constant;	 matrix[5] = 0.0f;
		matrix[6] = 0.0f;	  matrix[7] = 0.0f;		 matrix[8] = constant;
	}
	bool isEqual(const Matrix3x3f& b) const
	{
		for (int i = 0; i < 9; i++)
		{
			if (matrix[i] - b.matrix[i] > ERRORMARGIN || b.matrix[i] - matrix[i] > ERRORMARGIN)
				return false;
		}
		return true;
	}
	inline const Matrix2x2f subMatrix(int row, int column) const
	{
		Matrix2x2f a;

		int aIndex = 0;
		for (int i = 0; i < 3; i++) 
		{
			if (i == row)
				continue;
			for (int j = 0; j < 3; j++)
			{
				if (j == column)
					continue;

				a[aIndex++] = matrix[i * 3 + j];
			}
		}
		return a;
	}
	Matrix3x3f operator*(const Matrix3x3f& b) const
	{
		return
		{
			matrix[0] * b.matrix[0] + matrix[1] * b.matrix[3] + matrix[2] * b.matrix[6], // [0]
			matrix[0] * b.matrix[1] + matrix[1] * b.matrix[4] + matrix[2] * b.matrix[7], // [1]
			matrix[0] * b.matrix[2] + matrix[1] * b.matrix[5] + matrix[2] * b.matrix[8], // [2]
			matrix[3] * b.matrix[0] + matrix[4] * b.matrix[3] + matrix[5] * b.matrix[6], // [3]
			matrix[3] * b.matrix[1] + matrix[4] * b.matrix[4] + matrix[5] * b.matrix[7], // [4]
			matrix[3] * b.matrix[2] + matrix[4] * b.matrix[5] + matrix[5] * b.matrix[8], // [5]
			matrix[6] * b.matrix[0] + matrix[7] * b.matrix[3] + matrix[8] * b.matrix[6], // [6]
			matrix[6] * b.matrix[1] + matrix[7] * b.matrix[4] + matrix[8] * b.matrix[7], // [7]
			matrix[6] * b.matrix[2] + matrix[7] * b.matrix[5] + matrix[8] * b.matrix[8]	 // [8]
		};
	}
	const float& operator[](int index) const { return matrix[index]; }
	float& operator[](int index) { return matrix[index]; }
	friend std::ostream& operator<<(std::ostream& os, const Matrix3x3f& a)
	{
		for (int i = 0; i < 9; i++) 
		{
			os << a[i] << " ";
			if(i == 2 || i == 5 || i == 8)
				os << std::endl;
		}
		return os;
	}

private:
	float matrix[9];
};

class Matrix4x4f
{
public:
	Matrix4x4f(float a00, float a01, float a02, float a03, float a10, float a11, float a12, float a13, float a20, float a21, float a22, float a23, float a30, float a31, float a32, float a33)
	{
		matrix[0]  = a00; matrix[1]  = a01; matrix[2]  = a02; matrix[3]  = a03;
		matrix[4]  = a10; matrix[5]  = a11; matrix[6]  = a12; matrix[7]  = a13;
		matrix[8]  = a20; matrix[9]  = a21; matrix[10] = a22; matrix[11] = a23;
		matrix[12] = a30; matrix[13] = a31; matrix[14] = a32; matrix[15] = a33;
	}
	Matrix4x4f()
	{
		for (int i = 0; i < 16; i++)
			matrix[i] = 0.0f;
	}
	Matrix4x4f(const Matrix4x4f& other)	{ std::copy(std::begin(other.matrix), std::end(other.matrix), std::begin(matrix));}
	void setConstant(float constant)
	{
		for (int i = 0; i < 16; i++)
			matrix[i] = constant;
	}
	void setConstantAsIdentity(float constant)
	{
		matrix[0]  = constant; matrix[1]  = 0.0f;		 matrix[2]  = 0.0f;			matrix[3]  = 0.0f;
		matrix[4]  = 0.0f;	   matrix[5]  = constant;	 matrix[6]  = 0.0f;			matrix[7]  = 0.0f;
		matrix[8]  = 0.0f;	   matrix[9]  = 0.0f;		 matrix[10] = constant;		matrix[11] = 0.0f;
		matrix[12] = 0.0f;	   matrix[13] = 0.0f;		 matrix[14] = 0.0f;			matrix[15] = constant;
	}
	bool isEqual(const Matrix4x4f &b) const 
	{
		for (size_t i = 0; i < 16; i++)
		{
			if (matrix[i] - b.matrix[i] > ERRORMARGIN || b.matrix[i] - matrix[i] > ERRORMARGIN)
				return false;
		}
		return true;
	}
	inline const Matrix4x4f Invert()
	{
		Matrix4x4f inv // for visualization purposes
		{
			0,   1,  2,  3,
			4,   5,  6,  7,
			8,   9, 10, 11,
			12, 13, 14, 15
		};

		inv[0] = matrix[5] * matrix[10] * matrix[15] - matrix[5] * matrix[11] * matrix[14] -
				matrix[9] * matrix[6] * matrix[15] + matrix[9] * matrix[7] * matrix[14] +
				matrix[13] * matrix[6] * matrix[11] - 	matrix[13] * matrix[7] * matrix[10];

		inv[4] = -matrix[4] * matrix[10] * matrix[15] +
				matrix[4] * matrix[11] * matrix[14] +
				matrix[8] * matrix[6] * matrix[15] -
				matrix[8] * matrix[7] * matrix[14] -
				matrix[12] * matrix[6] * matrix[11] +
				matrix[12] * matrix[7] * matrix[10];

		inv[8] = matrix[4] * matrix[9] * matrix[15] -
			matrix[4] * matrix[11] * matrix[13] -
			matrix[8] * matrix[5] * matrix[15] +
			matrix[8] * matrix[7] * matrix[13] +
			matrix[12] * matrix[5] * matrix[11] -
			matrix[12] * matrix[7] * matrix[9];

		inv[12] = -matrix[4] * matrix[9] * matrix[14] +
			matrix[4] * matrix[10] * matrix[13] +
			matrix[8] * matrix[5] * matrix[14] -
			matrix[8] * matrix[6] * matrix[13] -
			matrix[12] * matrix[5] * matrix[10] +
			matrix[12] * matrix[6] * matrix[9];

		inv[1] = -matrix[1] * matrix[10] * matrix[15] +
			matrix[1] * matrix[11] * matrix[14] +
			matrix[9] * matrix[2] * matrix[15] -
			matrix[9] * matrix[3] * matrix[14] -
			matrix[13] * matrix[2] * matrix[11] +
			matrix[13] * matrix[3] * matrix[10];

		inv[5] = matrix[0] * matrix[10] * matrix[15] -
			matrix[0] * matrix[11] * matrix[14] -
			matrix[8] * matrix[2] * matrix[15] +
			matrix[8] * matrix[3] * matrix[14] +
			matrix[12] * matrix[2] * matrix[11] -
			matrix[12] * matrix[3] * matrix[10];

		inv[9] = -matrix[0] * matrix[9] * matrix[15] +
			matrix[0] * matrix[11] * matrix[13] +
			matrix[8] * matrix[1] * matrix[15] -
			matrix[8] * matrix[3] * matrix[13] -
			matrix[12] * matrix[1] * matrix[11] +
			matrix[12] * matrix[3] * matrix[9];

		inv[13] = matrix[0] * matrix[9] * matrix[14] -
			matrix[0] * matrix[10] * matrix[13] -
			matrix[8] * matrix[1] * matrix[14] +
			matrix[8] * matrix[2] * matrix[13] +
			matrix[12] * matrix[1] * matrix[10] -
			matrix[12] * matrix[2] * matrix[9];

		inv[2] = matrix[1] * matrix[6] * matrix[15] -
			matrix[1] * matrix[7] * matrix[14] -
			matrix[5] * matrix[2] * matrix[15] +
			matrix[5] * matrix[3] * matrix[14] +
			matrix[13] * matrix[2] * matrix[7] -
			matrix[13] * matrix[3] * matrix[6];

		inv[6] = -matrix[0] * matrix[6] * matrix[15] +
			matrix[0] * matrix[7] * matrix[14] +
			matrix[4] * matrix[2] * matrix[15] -
			matrix[4] * matrix[3] * matrix[14] -
			matrix[12] * matrix[2] * matrix[7] +
			matrix[12] * matrix[3] * matrix[6];

		inv[10] = matrix[0] * matrix[5] * matrix[15] -
			matrix[0] * matrix[7] * matrix[13] -
			matrix[4] * matrix[1] * matrix[15] +
			matrix[4] * matrix[3] * matrix[13] +
			matrix[12] * matrix[1] * matrix[7] -
			matrix[12] * matrix[3] * matrix[5];

		inv[14] = -matrix[0] * matrix[5] * matrix[14] +
			matrix[0] * matrix[6] * matrix[13] +
			matrix[4] * matrix[1] * matrix[14] -
			matrix[4] * matrix[2] * matrix[13] -
			matrix[12] * matrix[1] * matrix[6] +
			matrix[12] * matrix[2] * matrix[5];

		inv[3] = -matrix[1] * matrix[6] * matrix[11] +
			matrix[1] * matrix[7] * matrix[10] +
			matrix[5] * matrix[2] * matrix[11] -
			matrix[5] * matrix[3] * matrix[10] -
			matrix[9] * matrix[2] * matrix[7] +
			matrix[9] * matrix[3] * matrix[6];

		inv[7] = matrix[0] * matrix[6] * matrix[11] -
			matrix[0] * matrix[7] * matrix[10] -
			matrix[4] * matrix[2] * matrix[11] +
			matrix[4] * matrix[3] * matrix[10] +
			matrix[8] * matrix[2] * matrix[7] -
			matrix[8] * matrix[3] * matrix[6];

		inv[11] = -matrix[0] * matrix[5] * matrix[11] +
			matrix[0] * matrix[7] * matrix[9] +
			matrix[4] * matrix[1] * matrix[11] -
			matrix[4] * matrix[3] * matrix[9] -
			matrix[8] * matrix[1] * matrix[7] +
			matrix[8] * matrix[3] * matrix[5];

		inv[15] = matrix[0] * matrix[5] * matrix[10] -
			matrix[0] * matrix[6] * matrix[9] -
			matrix[4] * matrix[1] * matrix[10] +
			matrix[4] * matrix[2] * matrix[9] +
			matrix[8] * matrix[1] * matrix[6] -
			matrix[8] * matrix[2] * matrix[5];

		float det = matrix[0] * inv[0] + matrix[1] * inv[4] + matrix[2] * inv[8] + matrix[3] * inv[12];

		if (det == 0)
			throw std::runtime_error("Matrix determinant is 0");

		det = 1.0f / det;

		Matrix4x4f invOut;

		for (int i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;

		return invOut;
	}
	inline const Matrix4x4f Transposed() const
	{
		return
		{
			matrix[0], matrix[4], matrix[8],  matrix[12],
			matrix[1], matrix[5], matrix[9],  matrix[13],
			matrix[2], matrix[6], matrix[10], matrix[14],
			matrix[3], matrix[7], matrix[11], matrix[15]
		};
	}
	inline const Matrix3x3f subMatrix(int row, int column) const
	{
		Matrix3x3f a;

		int aIndex = 0;
		for (int i = 0; i < 4; i++)
		{
			if (i == row)
				continue;
			for (int j = 0; j < 4; j++)
			{
				if (j == column)
					continue;

				a[aIndex++] = matrix[i * 4 + j];
			}
		}
		return a;
	}
	Matrix4x4f& operator=(const Matrix4x4f& other)
	{
		if (this != &other)
			std::copy(std::begin(other.matrix), std::end(other.matrix), std::begin(matrix));
		
		return *this;
	}
	const float& operator[](int index) const { return matrix[index]; }
	float& operator[](int index) { return matrix[index]; }
	Matrix4x4f operator*(const Matrix4x4f& b) const
	{
		return 
		{
			matrix[0] * b.matrix[0] + matrix[1] * b.matrix[4] + matrix[2] * b.matrix[8] + matrix[3] * b.matrix[12],			// [0]
			matrix[0] * b.matrix[1] + matrix[1] * b.matrix[5] + matrix[2] * b.matrix[9] + matrix[3] * b.matrix[13],			// [1]
			matrix[0] * b.matrix[2] + matrix[1] * b.matrix[6] + matrix[2] * b.matrix[10] + matrix[3] * b.matrix[14],		// [2]
			matrix[0] * b.matrix[3] + matrix[1] * b.matrix[7] + matrix[2] * b.matrix[11] + matrix[3] * b.matrix[15],		// [3]
			matrix[4] * b.matrix[0] + matrix[5] * b.matrix[4] + matrix[6] * b.matrix[8] + matrix[7] * b.matrix[12],			// [4]
			matrix[4] * b.matrix[1] + matrix[5] * b.matrix[5] + matrix[6] * b.matrix[9] + matrix[7] * b.matrix[13],			// [5]
			matrix[4] * b.matrix[2] + matrix[5] * b.matrix[6] + matrix[6] * b.matrix[10] + matrix[7] * b.matrix[14],		// [6]
			matrix[4] * b.matrix[3] + matrix[5] * b.matrix[7] + matrix[6] * b.matrix[11] + matrix[7] * b.matrix[15],		// [7]
			matrix[8] * b.matrix[0] + matrix[9] * b.matrix[4] + matrix[10] * b.matrix[8] + matrix[11] * b.matrix[12],		// [8]
			matrix[8] * b.matrix[1] + matrix[9] * b.matrix[5] + matrix[10] * b.matrix[9] + matrix[11] * b.matrix[13],		// [9]
			matrix[8] * b.matrix[2] + matrix[9] * b.matrix[6] + matrix[10] * b.matrix[10] + matrix[11] * b.matrix[14],		// [10]
			matrix[8] * b.matrix[3] + matrix[9] * b.matrix[7] + matrix[10] * b.matrix[11] + matrix[11] * b.matrix[15],		// [11]
			matrix[12] * b.matrix[0] + matrix[13] * b.matrix[4] + matrix[14] * b.matrix[8] + matrix[15] * b.matrix[12],		// [12]
			matrix[12] * b.matrix[1] + matrix[13] * b.matrix[5] + matrix[14] * b.matrix[9] + matrix[15] * b.matrix[13],		// [13]
			matrix[12] * b.matrix[2] + matrix[13] * b.matrix[6] + matrix[14] * b.matrix[10] + matrix[15] * b.matrix[14],	// [14]
			matrix[12] * b.matrix[3] + matrix[13] * b.matrix[7] + matrix[14] * b.matrix[11] + matrix[15] * b.matrix[15]		// [15]
		};
	}
	Tuple::Pos operator*(const Tuple::Pos& b) const
	{
		return 
		{
			matrix[0] * b.m_x + matrix[1] * b.m_y + matrix[2] * b.m_z + matrix[3] * b.m_type, // m_x
			matrix[4] * b.m_x + matrix[5] * b.m_y + matrix[6] * b.m_z + matrix[7] * b.m_type, // m_y
			matrix[8] * b.m_x + matrix[9] * b.m_y + matrix[10] * b.m_z + matrix[11] * b.m_type, // m_z
			static_cast<int32_t>(matrix[12] * b.m_x + matrix[13] * b.m_y + matrix[14] * b.m_z + matrix[15] * b.m_type) // m_type
		};
	}
	friend std::ostream& operator<<(std::ostream& os, const Matrix4x4f& a)
	{
		for (int i = 0; i < 16; i++)
		{
			os << a.matrix[i] << " ";
			if(i == 3 || i == 7 || i == 11 || i == 15)
				os << std::endl;
		}
		return os;
	}

private:
	float matrix[16];
};

// Utilities for the classes

inline const Matrix4x4f IdentityMat4x4f()
{
	return 
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}
inline const float Determinant2x2(const Matrix2x2f & matrix)
{
	return { matrix[0] * matrix[3] - matrix[1] * matrix[2] }; // ab - cd
}
inline const float Determinant3x3(Matrix3x3f & matrix)
{
	return
	{
		  matrix[0] * (matrix[4] * matrix[8] - matrix[5] * matrix[7])
		- matrix[1] * (matrix[3] * matrix[8] - matrix[5] * matrix[6])
		+ matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6])
	};
}
inline const float Determinant3x3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22)
{
	return
	{
		  a00 * (a11 * a22 - a12 * a21)
		- a01 * (a10 * a22 - a12 * a20)
		+ a02 * (a10 * a21 - a11 * a20)
	};
}
inline const float Determinant4x4(Matrix4x4f & matrix)
{
	return
		matrix[0] * Determinant3x3(    matrix[5],  matrix[6],  matrix[7],
									   matrix[9],  matrix[10], matrix[11],
									   matrix[13], matrix[14], matrix[15]) -
		matrix[1] * Determinant3x3(    matrix[4],  matrix[6],  matrix[7],
									   matrix[8],  matrix[10], matrix[11],
									   matrix[12], matrix[14], matrix[15]) +
		matrix[2] * Determinant3x3(    matrix[4],  matrix[5],  matrix[7],
									   matrix[8],  matrix[9],  matrix[11],
									   matrix[12], matrix[13], matrix[15]) -
		matrix[3] * Determinant3x3(    matrix[4],  matrix[5],  matrix[6],
									   matrix[8],  matrix[9],  matrix[10],
									   matrix[12], matrix[13], matrix[14]);
}

// Matrix Transforms

inline const float toRadians(float degrees) { return degrees * 0.0174533f; }
inline const Matrix4x4f Translate(Tuple::Pos point)
{
	return
	{
		1.0f, 0.0f, 0.0f, point.m_x,
		0.0f, 1.0f, 0.0f, point.m_y,
		0.0f, 0.0f, 1.0f, point.m_z,
		0.0f, 0.0f, 0.0f, static_cast<float>(point.m_type)
	};
}
inline const Matrix4x4f Translate(float x, float y, float z)
{
	return
	{
		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f // vector is 0, point is 1
	};
}
inline const Matrix4x4f Scale(float x, float y, float z)
{
	return
	{
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
inline const Matrix4x4f Scale(Tuple::Pos vector)
{
	return
	{
		vector.m_x, 0.0f, 0.0f, 0.0f,
		0.0f, vector.m_y, 0.0f, 0.0f,
		0.0f, 0.0f, vector.m_z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
inline const Matrix4x4f RotateXaxis(float angle)
{
	return
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(toRadians(angle)), -sin(toRadians(angle)), 0.0f,
		0.0f, sin(toRadians(angle)), cos(toRadians(angle)), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
inline const Matrix4x4f RotateYaxis(float angle)
{
	return
	{
		cos(toRadians(angle)), 0.0f, sin(toRadians(angle)), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(toRadians(angle)), 0.0f, cos(toRadians(angle)), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
inline const Matrix4x4f RotateZaxis(float angle)
{
	return
	{
		cos(toRadians(angle)), -sin(toRadians(angle)), 0.0f, 0.0f,
		 sin(toRadians(angle)), cos(toRadians(angle)), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
inline const Matrix4x4f RotateXYZ(float angleX, float angleY, float angleZ)
{
	const float cosX = cos(toRadians(angleX));
	const float sinX = sin(toRadians(angleX));
	const float cosY = cos(toRadians(angleY));
	const float sinY = sin(toRadians(angleY));
	const float cosZ = cos(toRadians(angleZ));
	const float sinZ = sin(toRadians(angleZ));

	return
	{
		cosY * cosZ,                        -cosY * sinZ,                       sinY,         0.0f,
		sinX * sinY * cosZ + cosX * sinZ,    -sinX * sinY * sinZ + cosX * cosZ,   -sinX * cosY,  0.0f,
		-cosX * sinY * cosZ + sinX * sinZ,   cosX * sinY * sinZ + sinX * cosZ,    cosX * cosY,   0.0f,
		0.0f,                                0.0f,                                0.0f,         1.0f
	};
}
inline const Matrix4x4f Shear(float xy, float xz, float yx, float yz, float zx, float zy)
{
	return
	{
		1.0f,   xy,   xz, 0.0f,
		  yx, 1.0f,   yz, 0.0f,
		  zx,   zy, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
#endif