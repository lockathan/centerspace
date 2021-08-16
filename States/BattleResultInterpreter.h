#ifndef BattleResultInterpreter_h
#define BattleResultInterpreter_h

class BattleResult;

class BattleResultInterpreter
{
public:

	virtual void interpretResult(BattleResult* result) = 0;
};
#endif