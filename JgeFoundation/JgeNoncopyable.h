#pragma once

namespace jge
{
	class JGE_DLL Noncopyable
	{
	public:
		Noncopyable();
		~Noncopyable();

	private:
		Noncopyable(const Noncopyable& value);
		Noncopyable& operator=(const Noncopyable& value);
	};
}