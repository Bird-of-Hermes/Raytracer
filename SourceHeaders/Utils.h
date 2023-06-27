#ifndef UTILS_H
#define UTILS_H
#include <memory>
#include "Objects.h"
#include "Lights.h"

namespace Utils
{
	template<typename T>
	class Vector
	{
	public:
		void emplace_back(const T&& type)
		{
			T* newAlloc = new T[size + 1];

			// moves old vector to new vector
			for (size_t i = 0; i < size; i++)
			{
				newAlloc[i] = std::move(at[i]);
			}
			newAlloc[size] = std::move(type); // moves the argument to the last location on the vector

			delete[] at; // frees old vector
			at = newAlloc; // pointer to the new address
			++size;
		}
		void emplace_back(T* value) { emplace_back(std::move(value)); }
		inline const size_t Size() const { return size; }

		operator T*() { return at; }
		T& operator[](const size_t index) { return at[index]; }

		Vector() { size = 0; at = nullptr; }
		Vector(const size_t Size) : size(Size) { at = new T[Size]; }
		~Vector() { delete[] at; }

	public:
		T* at;
		size_t size;
	};
}
inline const unsigned int FastRand(unsigned int index)
{
	index = (index << 13) ^ index;
	return ((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

#endif