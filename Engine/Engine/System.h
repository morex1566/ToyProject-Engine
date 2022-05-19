#pragma once
class System
{
private:
	// utility
	bool looping;
	
	// model

	System();

public:
	~System();
	
	// base loop func
	void begin();
	void run();
	void shutdown();
};

