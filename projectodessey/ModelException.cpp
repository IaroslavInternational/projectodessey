#include "ModelException.h"
#include <sstream>

ModelException::ModelException( int line,const char* file,std::string note ) noexcept
	:
	EngineException( line,file ),
	note( std::move( note ) )
{}

const char* ModelException::what() const noexcept
{
	std::ostringstream oss;
	oss << EngineException::what() << std::endl
		<< "[Note] " << GetNote();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* ModelException::GetType() const noexcept
{
	return "Engine Model Exception";
}

const std::string& ModelException::GetNote() const noexcept
{
	return note;
}