
#pragma once


template <typename T>
class Singleton
{
protected:
	Singleton()				{ }
	~Singleton()			{ }

public:
	static T*	instance(void)
	{
		if (nullptr == _instance)
			_instance = new T;

		return _instance;
	}

	static void		desctroy(void)
	{
		if (nullptr != _instance)
			delete _instance;

		_instance = nullptr;
	}

private:
	static T*	_instance;
};

template<typename T> T*	Singleton<T>::_instance = nullptr;