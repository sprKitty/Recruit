#pragma once


class State
{
public:
	State() {}
	virtual ~State() {}

	virtual const bool Execute() = 0;

private:

};