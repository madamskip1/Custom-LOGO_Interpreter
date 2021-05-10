#include "RepeatStatement.h"


void RepeatStatement::setHowManyTime(std::shared_ptr<Expression> howMany)
{
	howManyTime = howMany;
}

std::shared_ptr<Expression> RepeatStatement::getHowManyTime()
{
	return howManyTime;
}
