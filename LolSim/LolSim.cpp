#include "LolSim.h"
#include <string.h>

typedef std::vector<Creep*> camp_vector;

bool isCreepAlive(camp_vector camp);
camp_vector generateCamp();
Creep* getTarget(Creep *source, camp_vector camp);
void Buffcount(Base * creature, double &nextAction);

int main() {
	LeeSin* Gwelican = new LeeSin();
	Gwelican->setName("Gwelican");
	Gwelican->initCamp("blue");
	camp_vector& camp = Gwelican->getTargets();
	double time = 0;
	while(!camp.empty()) {
		if(Gwelican->getHealth() < 0){
			LogS::ToLog(Gwelican->getName()+" died");
			break;
		}
		double nextAction = 10;
		//if (camp.size() == 0) {
		//std::cin.get();

		//return 0;
		//}
		if(!Gwelican->getNextSwing()){		//player swings
			(*camp.begin())->takeDamage(Gwelican,Gwelican->getDamage());
			Gwelican->setNextSwing(1/Gwelican->getAttackSpeed());
		}
		Buffcount(Gwelican, nextAction);		//buff calc for player

		for(int doublerun = 0; doublerun < 3; doublerun++) { // double run hax, to enable replaced skills
			std::vector<Skill*> skills = Gwelican->getSkills();		//skills turn
			for(std::vector<Skill*>::iterator skillit = skills.begin(); skillit != skills.end(); skillit++){
				if(!(*skillit)->getCooldown()){
					if (log_level & LOG_SKILL) {
						std::stringstream s;
						s  << (*skillit)->getName() << " activated: " << (*skillit)->getCooldown() << "/" << (*skillit)->getMaxCooldown();
						LogS::ToLog(s.str());
					}
					if ((*skillit)->isAoe()){		//AOE ALL THE CREEPS
						for(std::vector<Creep*>::iterator it2 = camp.begin(); it2 != camp.end(); it2++){
							if (log_level & LOG_SKILL) {
								LogS::ToLog("Hitting: " + (*it2)->getName());
							}
							(*it2)->takeDamage(Gwelican,(*skillit)->getDamage(Gwelican,(*it2)));						
						}
					}
					else {
						(*camp.begin())->takeDamage(Gwelican,(*skillit)->getDamage(Gwelican,(*camp.begin())));	//Single target skill
					}
					(*skillit)->setCooldown((*skillit)->getMaxCooldown());		//set skillCD
				}
				else{
					if ((*skillit)->getCooldown() < nextAction) {
						nextAction = (*skillit)->getCooldown();
						if (log_level & LOG_SKILL) {
							std::stringstream s;
							s << "Skill cooldown " << (*skillit)->getName() << nextAction;
							LogS::ToLog(s.str());
						}
					}
				}
			}
		}
		Gwelican->cdReduce(nextAction);		//time reduces
		for(std::vector<Creep*>::iterator it = camp.begin(); it!= camp.end(); it++)
			(*it)->cdReduce(nextAction);
		if (log_level & LOG_OTHER) {
			std::stringstream s;
			s << "Adding time: " << nextAction;
			LogS::ToLog(s.str());
		}
		LogS::addTime(nextAction);
		time += nextAction;		//sum
		for(int i = 0; i < camp.size(); i++){	// dead creeps are erased
			if(camp[i]->getHealth() < 0){
				LogS::ToLog(camp[i]->getName()+" died, removing");
				camp.erase(camp.begin()+i);
				continue;
			}
			if(!camp[i]->getNextSwing()){	//creature swings
				Gwelican->takeDamage(camp[i],camp[i]->getDamage());
				camp[i]->setNextSwing(1/camp[i]->getAttackSpeed());
			}
			Buffcount(camp[i], nextAction);		//buff calc for creeps (see below)
		}

	}
	delete Gwelican;
	if (log_level && LOG_OTHER) {
		std::stringstream s;
		s << "Time elapsed: " << time;
		LogS::ToLog(s.str());
	}
	std::cin.get();

}

void Buffcount(Base* creature, double & nextAction){
	for(int i = 0; i < creature->getBuffs().size(); i++) {		//buffremoval
		if (!creature->getBuffs()[i]->getDuration()){
			creature->removeBuff(creature->getBuffs()[i]); 
			continue;
		}
		if (creature->getBuffs()[i]->getDuration() < nextAction) {	//buff is nextaction
			nextAction = creature->getBuffs()[i]->getDuration();
			if (log_level && LOG_BUFF) {
				std::stringstream s;
				s << "Buff duration " << creature->getBuffs()[i]->getName() << nextAction;
				LogS::ToLog(s.str());
			}
		}
	}
	if (creature->getNextSwing() < nextAction) {		//swing might be next instead of buff
		nextAction = creature->getNextSwing();
	}
}

Creep* getTarget(Creep *source, camp_vector camp) {
	Creep *superCreep = new Creep();
	superCreep->setMaxHealth(0);
	for(std::vector<Creep*>::iterator it = camp.begin(); it != camp.end(); it++) {
		//	for each(Base *obj in camp) {
		if (source->isCreep() == false) {
			if ((*it)->isCreep() == true && superCreep->getMaxHealth() < (*it)->getMaxHealth() && (*it)->Alive()) {
				superCreep =  (*it);
			}
		} else {
			if ((*it)->isCreep() == false) {
				return (*it);
			}
		}
	}
	return superCreep;
}

bool isCreepAlive(camp_vector camp) {
	bool alive = false;
	for(std::vector<Creep *>::iterator it = camp.begin(); it != camp.end(); it++) {
		Base *obj = (*it);
		//	for each(Base *obj in camp) {
		if (obj->isCreep() == false && obj->Alive() == false) { // player dead, over
			return false;
		}
		if (obj->getHealth() > 0 && obj->isCreep()) {
			alive = true;
		}
	}
	return alive;
}

/*camp_vector generateCamp() {
camp_vector camp;
Creep* blue = new Creep();
Creep* kiswraith1 = new Creep();
Creep* kiswraith2 = new Creep();
LeeSin* p = new LeeSin();

p->setName("Gwelican");

blue->setName("BlueGolem");
blue->setMaxHealth(1275+175);
blue->setAttackDamage(110);
blue->setArmor(24);
blue->setAttackSpeed(0.613);
blue->setMovementSpeed(200);

kiswraith1->setName("Wraith1");
kiswraith1->setMaxHealth(350);
kiswraith1->setAttackDamage(18);
kiswraith1->setArmor(8);
kiswraith1->setAttackSpeed(0.679);

kiswraith2->setName("Wraith2");
kiswraith2->setMaxHealth(350);
kiswraith2->setAttackDamage(18);
kiswraith2->setArmor(8);
kiswraith2->setAttackSpeed(0.679);

blue->setNextSwing(5);
kiswraith1->setNextSwing(5);
kiswraith2->setNextSwing(5);

camp.push_back(blue);
camp.push_back(p);
camp.push_back(kiswraith1);
camp.push_back(kiswraith2);
return camp;
}*/
