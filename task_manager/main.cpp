#include <Windows.h>
#include <iostream>
#include <thread>
#include "task_manager.hpp"

namespace global
{
	bool running = true;
	std::unique_ptr<std::thread> seperate_thread;
}

void safe_exit()
{
	// cleanup before exiting
	global::seperate_thread->join();
	exit(0);
}

int main()
{
	auto task_manager = std::make_unique<management::task_manager>(2); // pass number of total tasks

	global::seperate_thread = std::make_unique<std::thread>([&task_manager]
	{
		while (true)
		{
			// get current progress of main thread in seperate thread
			auto progress = task_manager->get_progress();
			auto task = task_manager->get_task();
			auto status = task.status;
			auto is_error = task.error;

			if (is_error)
				break;
		}
	});

	task_manager->register_task<int>("doing difficult calculation", []()
	{
		auto sum = 3 + 5 - 2;
		return sum;
	}, safe_exit, true);

	task_manager->register_task<bool>("demonstrating error", [&task_manager]()
	{
		auto error_ocurred = true;
		if (error_ocurred)
			task_manager->exception("an error has ocurred");

		return true;
	}, safe_exit, true);
}