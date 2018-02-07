#ifndef STATES_ENEMY_H
#define STATES_ENEMY_H

#include "State.h"
#include "../Enemy/Enemy.h"

class StatePatrol : public State
{
	CEnemy *m_go;
public:
	StatePatrol(const std::string &stateID, CEnemy *go);
	virtual ~StatePatrol();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateChase : public State
{
	CEnemy *m_go;
public:
	StateChase(const std::string &stateID, CEnemy *go);
	virtual ~StateChase();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateSearch: public State
{
	CEnemy *m_go;
public:
	StateSearch(const std::string &stateID, CEnemy *go);
	virtual ~StateSearch();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateAttack : public State
{
	CEnemy *m_go;
public:
	StateAttack(const std::string &stateID, CEnemy *go);
	virtual ~StateAttack();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};



#endif