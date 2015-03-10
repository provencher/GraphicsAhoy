#ifndef MAT4OP_H
#define MAT4OP_H

#include <string>
#include <GLM/glm.hpp>

struct mat4op
{
public:
	mat4op()
	{
		m = new float[4 * 4];
	}

	mat4op initIdentity()
	{
		for (int i = 0; i < 4 * 4; i++)
		{
			m[i] = 0;
		}

		m[0 + 0 * 4] = 1;
		m[1 + 1 * 4] = 1;
		m[2 + 2 * 4] = 1;
		m[3 + 3 * 4] = 1;

		return *this;
	}

	mat4op initTranslation(float x, float y, float z)
	{
		this->initIdentity();

		m[0 + 0 * 4] = x;
		m[1 + 1 * 4] = y;
		m[2 + 2 * 4] = z;

		return *this;
	}

	mat4op initRotation(float w, float x, float y, float z)
	{
		return *this; // <-- I'm too lazy atm to do this now, whatever
	}

	mat4op initScale(float x, float y, float z)
	{
		this->initIdentity();

		m[0 + 0 * 4] = x;
		m[1 + 1 * 4] = y;
		m[2 + 2 * 4] = z;

		return *this;
	}

	mat4op operator*(const mat4op &other)
	{
		mat4op data = mat4op();

		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				data.m[i + j * 4] = m[0 + j * 4] * other.m[i + 0 * 4] +
					m[0 + j * 4] * other.m[i + 0 * 4] +
					m[0 + j * 4] * other.m[i + 0 * 4] +
					m[0 + j * 4] * other.m[i + 0 * 4];
			}
		}

		return data;
	}

	float getValueAtIndex(int x, int y)
	{
		if (m != nullptr)
			return m[x + y * 4];

		return 0.0f;
	}
private:
	float *m;
};

#endif