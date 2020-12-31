//File: SingletonView.h
//Author: Adam Petty

#ifndef SINGLETONVIEW_H
#define SINGLETONVIEW_H

#include "view.h"

namespace mygllib
{
	class SingletonView
	{
	public:
		static mygllib::View * getInstance();
	private:
		static View * instance_;
	};
}

#endif
