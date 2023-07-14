#ifndef UTILS_H
#define UTILS_H
#include <memory>
#include <vector>
#include "Objects.h"
#include "Lights.h"

class MManager
{
public:
	MManager(uint64_t bytes)
	{
		size = bytes;
		ptr = malloc(bytes);
	}

	void Reload(uint64_t bytes)
	{
		if(ptr != nullptr)
			free(ptr);
		next = 0;
		size = bytes;
		ptr = malloc(bytes);
	}

	template<typename T>
	void Next(T** typesize)
	{
		if (size > next)
			*typesize = reinterpret_cast<T*>(static_cast<uint8_t*>(ptr) + next);
		else
		{
			std::cerr << "Heap size exceeded, exiting program!\n";
			exit(EXIT_FAILURE);
		}
		next += sizeof(T);
	}
	~MManager() { free(ptr); }
	MManager(const MManager&) = delete;
	MManager& operator=(const MManager&) = delete;

private:
	void* ptr;
	uint64_t size;
	uint64_t next = 0;
}inline thread_local Mem(850'000'000ll); // 10,5 MB

// Basic/unsafe implementation, beware of errors
namespace Utils
{
	template<typename T>
	class Vector
	{
	public:
		void emplace_back(const T&& type)
		{
			T* newAlloc = new T[m_size + 1];

			// moves old vector to new vector
			for (uint32_t i = 0; i < m_size; i++)
			{
				newAlloc[i] = std::move(m_data[i]);
			}
			newAlloc[m_size] = std::move(type); // moves the argument to the last location on the vector

			delete[] m_data; // frees old vector
			m_data = newAlloc; // pointer to the new address
			++m_size;
		}
		void emplace_newptr(void* ptr)
		{
			T* newAlloc = new T[m_size + 1];

			// moves old vector to new vector
			for (uint32_t i = 0; i < m_size; i++)
			{
				newAlloc[i] = std::move(m_data[i]);
			}
			newAlloc[m_size] = newAlloc[m_size] = static_cast<T>(ptr); // moves the argument to the last location on the vector

			delete[] m_data; // frees old vector
			m_data = newAlloc; // pointer to the new address
			++m_size;
		}
		constexpr void fill_in(const T&& type)
		{
			if (m_place < m_size)
			{
				m_data[m_place] = std::move(type);
				m_place++;
			}
			else
			{
				std::cerr << "\nfill_in() method is inadequate for this use case. Consider switching to other container other than Utils::Vector\n";
				std::cerr << "m_place = " << m_place << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		operator std::vector<T>() const
		{
			std::vector<T> vec;
			for (size_t i = 0; i < m_size; i++)
			{
				vec.emplace_back(m_data[i]);
			}
			return vec;
		}
		operator T* () { return m_data; }
		T& operator[](const uint32_t index) { return m_data[index]; }
		constexpr T& operator[](const uint32_t index) const { return m_data[index]; }
		constexpr uint32_t size() const { return m_size; }
		constexpr T* begin() const { return m_data; }
		constexpr T* end() const { return m_data + m_size; }
		T* begin() { return m_data; }
		T* end() { return m_data + m_size; }

		Vector() { m_size = 0; m_data = nullptr; m_place = 0; }
		Vector(const uint32_t Size) : m_size(Size) { m_data = new T[Size]; m_place = 0; }
		~Vector() 
		{
			delete[] m_data;
		}

	private:
		T* m_data;
		uint32_t m_size;
		uint32_t m_place;
	};

	template<typename T, const uint32_t Size>
	class Static_Array
	{
	public:
		Static_Array() 
		{
			for (uint32_t i = 0; i < Size; i++)
				m_data[i] = T();
			
			m_place = 0;
		}
		T* begin() { return m_data; }
		T* end() { return m_data + Size; }
		T& operator[](const uint32_t index) { return m_data[index]; }
		const T& operator[](const uint32_t index) const { return m_data[index]; }
		constexpr uint32_t size() const { return Size; }
		constexpr void fill_in(const T&& type)
		{
			if (m_place < Size)
			{
				m_data[m_place] = std::move(type);
				m_place++;
			}
			else
			{
				std::cerr << "\nfill_in() method is inadequate for this use case. Consider switching to other container other than Utils::Static_Array\n";
				std::cerr << "m_place = " << m_place << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		constexpr void restart() 
		{
			for (uint32_t i = 0; i < Size; i++)
				m_data[i] = T();

			m_place = 0;
		}

	private:
		T m_data[Size];
		uint32_t m_place;
	};
}
inline constexpr unsigned int FastRand(unsigned int index)
{
	index = (index << 13) ^ index;
	return ((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff);
}
#endif