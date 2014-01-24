#pragma once

#include <vector>
#include <map>

class GameLogic
{
public:
	bool getSwitch(int i) { return switches[i]; }
	void setSwitch(int i, bool b) { switches[i] = b; }

	int getInt(int i) { return int_vars[i]; }
	void setInt(int i, int n) { int_vars[i] = n; }

	std::string getString(int i) { string_vars[i]; }
	void setString(int i, std::string s) { string_vars[i] = s; }

	int getControl(std::string label) { return control_vars[label]; }
	void setControl(std::string label, int n) { control_vars[label] = n; }

private:
	//Persistent State
	std::vector<bool> switches;
	std::vector<int> int_vars;
	std::vector<std::string> string_vars;
	std::map<std::string, int> control_vars;
};