#pragma once

#include "Logger/logger.h"

class Container
{
public:
	Container()
	{
		LOG_INFO("default ctor is called");
	}
	Container(const Container& other)
	{
		LOG_WARN("copy ctor is called");
	}
	Container(Container&& other)
	{
		LOG_WARN("move ctor is called");
	}

	Container& operator=(const Container& other)
	{
		if (&other == this)
		{
			// do resource management
		}
		LOG_INFO("operator=(const Container& other)");
		return *this;
	}

	Container& operator=(Container&& other)
	{
		if (&other != this)
		{
			// do resource management
		}
		LOG_INFO("operator=(Container&& other)");
		return *this;
	}

};