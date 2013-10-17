#include "LolSim.h"

Summoner::Summoner(void):Skill()
{
}

void Summoner::onUse() {
  std::stringstream s;
  s << "Summoner used";
  LogS::ToLog(s.str());
}
Summoner::~Summoner(void)
{
}
