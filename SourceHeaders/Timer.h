#ifndef TIMER_H
#define TIMER_H
#include <chrono>
#include <iostream>

class Timer
{
public:
	Timer() 
	{
		std::cout << "Timer started!\n";
		start = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		std::cout << "Done!\n\nElapsed time : " << duration/1000.0f << " milliseconds!" << std::endl;
	}
	const void nanonow()
	{
		end = std::chrono::high_resolution_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		std::cout << "Up until this point, it took : " << duration << " nanoseconds!\n" << std::endl;
	}
	const void refresh(){ start = std::chrono::high_resolution_clock::now(); }
	const void now()
	{
		end = std::chrono::high_resolution_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		std::cout << "Up until this point, it took : " << duration/1000.0f << " milliseconds!\n" << std::endl;
	}
private:
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
};


#endif