#include "LolSim.h"

Buff::Buff(void)
{
	stack = 0;
	TimeBased = true;
	hitcount = 0;
	duration = 0;
	Unique = true;
	Shield = 0;
}
bool Buff::isUnique() {
	return Unique;
}
bool Buff::isTimeBased() {
	return TimeBased;
}
void Buff::setName(std::string Name) {
	this->Name = Name;
}
std::string Buff::getName() {
	return Name;
}
void Buff::setShield(double shield) {
	this->Shield = shield;
}
double Buff::getShield() {
	return Shield;
}
void Buff::decreaseStack() { 
	stack--;
}

void Buff::setDuration(double duration) {
	if (log_level & 0x0100) {
    std::stringstream s;
		s << "Setting duration on " << Name << " for " << duration;
    LogS::ToLog(s.str());
	}
	this->duration = duration;
}
void Buff::setHitCount(int hitcount) {
	this->hitcount = hitcount;
}
void Buff::setAttackSpeed(double AttackSpeed) {
	this->AttackSpeed = AttackSpeed;
}

double Buff::getAttackSpeed() {
	return AttackSpeed;
}
double Buff::getDuration() { return duration; }
int Buff::getHitCount() {
	return hitcount;
}

void Buff::increaseStack() { 
	stack++;
}
int Buff::getStack() {
	return stack;
}

Buff::~Buff(void)
{
}
