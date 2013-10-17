#include "LolSim.h"
double LogS::time = 0;


void LogS::ToLog(std::string text) {
	std::cout << "[" << boost::format("%8d") % time << "] " << text << std::endl;
}
void LogS::addTime(double time2) {
	time += time2;
}
