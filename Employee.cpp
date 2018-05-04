#define _SCL_SECURE_NO_WARNINGS 

#define _CRT_SECURE_NO_WARNINGS
#include "Employee.h"
#include <cassert>
#include <new>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <stack>
#include <vector>


using namespace std;

//Constructor
Employee::Employee(int id, const std::string& name, const std::string& address, const std::string& city, const std::string& state,const std::string& country, const std::string& phone, double salary): name(name), city(city),state(state), address(address), country(country), phone(phone) {
	this->id = id;
	this->salary = salary;
}
// To overwrite <<
ostream& operator<<(ostream& os, const Employee& e) {
	return os << '{' << e.id << ',' << e.name << ',' << e.address << ',' << e.city << ',' << e.state << ',' << e.country << ',' << e.salary << ',' << e.phone << '}';
}
int Employee::recSize() {
	return sizeof(EmployeeRec);
}
void Employee::display(std::ostream& os) const {// Write a readable Employee representation to a stream
	os << "id: " << id << endl;
	os << "name: " << name << endl;
	os << "address: " << address << endl;
	os << "city: " << city << endl;
	os << "state: " << state << endl;
	os << "country: " << country << endl;
	os << "phone: " << phone << endl;
	os << "salary: " << salary << endl;
	os << endl;
}
void Employee::write(std::ostream& os ) const { // Write a fixed-length record to current file position
	EmployeeRec outbuf;
	outbuf.id = id;
	strncpy(outbuf.name, name.c_str(), 30)[30] = '\0';
	strncpy(outbuf.address, address.c_str(), 25)[25] = '\0';
	strncpy(outbuf.city, city.c_str(), 20)[20] = '\0';
	strncpy(outbuf.state, state.c_str(), 20)[20] = '\0';
	strncpy(outbuf.country, country.c_str(), 20)[20] = '\0';
	strncpy(outbuf.phone, phone.c_str(), 20)[20] = '\0';
	outbuf.salary = salary;
	os.write(reinterpret_cast<const char*>(&outbuf), sizeof outbuf);
	
}
Employee* Employee::read(std::istream& is) { // Read record from current file position
	Employee::EmployeeRec inbuf;
	is.read(reinterpret_cast<char*>(&inbuf), sizeof inbuf);

	if (is) {
		Employee *e = new Employee(inbuf.id, inbuf.name, inbuf.address, inbuf.city, inbuf.state, inbuf.country, inbuf.phone, inbuf.salary);
	
		return e;
	}
	else
		return nullptr;

}
void Employee::store(std::iostream& ios) const { // Overwrite (or append) record in (to) file
	bool found = false;
	
	Employee *p = Employee::read(ios);
	while (p) {
		if (id == p->id) {
			ios.seekg(-Employee::recSize(), ios::cur);
			write(ios);
			found = true;
			break;
		}
		delete p;
		p = Employee::read(ios);
	}
	delete p;
	if (!found) {
		ios.clear();
		ios.seekg(0, ios::beg);
		write(ios);
	}
}
void Employee::toXML(std::ostream& os ) const { // Write XML record for Employee
	os << "<Employee>" << endl;
	os << "	" << "<id>" << id << "</id>" << endl;
	os << "	" << "<name>" << name << "</name>" << endl;
	if (address != "")
		os << "	" << "<address>" << address << "</address>" << endl;
	if (city != "")
		os << "	" << "<city>" << city << "</city>" << endl;
	if (state != "")
		os << "	" << "<state>" << state << "</state>" << endl;
	if (country != "")
		os << "	" << "<country>" << country << "</country>" << endl;
	if (phone != "")
		os << "	" << "<phone>" << phone << "</phone>" << endl;
	if (salary>0)
		os << "	" << "<salary>" << salary << "</salary>" << endl;
	os << "</Employee>" << endl;

}

Employee* Employee::retrieve(std::istream& is, int i) { // Search file for record by id

	Employee* emp = Employee::read(is);
	
	while (emp) {
		if (i == emp->id) {
			return emp;
		}
		delete emp;
		emp = Employee::read(is);
	}
	delete emp;
	return nullptr;
}
Employee* Employee::fromXML(std::istream& is) {// Read the XML record from a stream
	
	string line;
	string scope;
	string status= "Notstarted";
	char c;
	std::stack <string> s;
	int id = 0;
	double salary = 0.0;
	string name, address,city, state, country, phone;
	while (is.get(c)) {


		if (c != '\n') {

			line += c;

			if (c == '<') {
				if(status == "openingbracket")
					throw runtime_error("Missing <"+scope+"> tag");
				if(status == "Notstarted" || status == "closingbracket")
				status = "openingbracket";
				
				line = line.substr(0, line.length() - 1);
				line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
				line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);


				if (_stricmp(scope.c_str(), "id")==0 && line.length()>0) {
					if(id> 0)
						throw runtime_error("Multiple <id> tags");
					id = std::stoi(line);
					line.clear();
				}
				else if(_stricmp(scope.c_str(), "name") == 0 && line.length()>0) {
					if (name!="")
						throw runtime_error("Multiple <name> tags");
					name = line;
					line.clear();
				}
				else if(_stricmp(scope.c_str(), "address") == 0 && line.length()>0) {
					if (address != "")
						throw runtime_error("Multiple <address> tags");
					address = line;
					line.clear();
				}
				else if(_stricmp(scope.c_str(), "city") == 0 && line.length()>0) {
					if (city != "")
						throw runtime_error("Multiple <city> tags");
					city = line;
					line.clear();
				}
				else if (_stricmp(scope.c_str(), "state") == 0 && line.length()>0) {
					if (state != "")
						throw runtime_error("Multiple state tags");
					state = line;
					line.clear();
				}
				else if (_stricmp(scope.c_str(), "country") == 0 && line.length()>0) {
					if (country != "")
						throw runtime_error("Multiple <country> tags");
					country = line;
					line.clear();
				}
				else if (_stricmp(scope.c_str(), "phone") == 0 && line.length()>0) {
					if (phone != "")
						throw runtime_error("Multiple <phone> tags");
					phone = line;
					line.clear();
				}
				else if (_stricmp(scope.c_str(), "salary") == 0 && line.length()>0) {
					if (salary>0)
						throw runtime_error("Multiple <salary> tags");
					salary = std::stod(line);
					line.clear();
				}
				else if (line.length()>0) {
					throw runtime_error("Invalid <"+scope+"> tag");
				}

			}
			if (c == '>') {

				
				if(status == "slash"){
					s.pop();
					status = "closingbracket";
					
				}

				if (status == "openingbracket") {
					line = line.substr(0, line.length() - 1);
			
					if (!s.empty()) {
						if (_stricmp(s.top().c_str(), "Employee") != 0) {
							throw runtime_error("Missing </"+line+"> tag");
						}
						if (_stricmp(s.top().c_str(), line.c_str()) == 0) {
							throw runtime_error("Invalid tag: <Employee>");
						}
					}
					scope = line;
					s.push(scope);
					if (s.size() == 1 && _stricmp(s.top().c_str(), "Employee") != 0)
						throw runtime_error("Missing <Employee> tag");
					
					status = "closingbracket";
				}
				
				line.clear();
			}
			if (c == '/') {
				status = "slash";
				if (_stricmp(s.top().c_str(), "Employee") == 0) {
					if (id == 0)
						throw runtime_error("Missing <id> tag");
					if (name == "")
						throw runtime_error("Missing <Name> tag");
					return new Employee(id, name, address,city, state, country, phone, salary);
					
				}
				

			}

		}
	}
	return nullptr;
}

void Employee::setSalary(const double& d) {
	salary = d;
}
 double Employee::getSalary() {
	return salary;
}


