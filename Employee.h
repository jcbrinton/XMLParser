#ifndef EMPLOYEE_H         
#define EMPLOYEE_H

#define _SCL_SECURE_NO_WARNINGS  

#include <algorithm>    
#include <cassert>      
#include <cstddef>         
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <stack>
#include <vector>



class Employee
{
private:
	int id;
	std::string name;
	std::string address;
	std::string city;
	std::string state;
	std::string country;
	std::string phone;
	double salary;
	Employee() {
		id = -1;
	}
	struct EmployeeRec {
		int id;
		char name[31];
		char address[26];
		char city[21];
		char state[21];
		char country[21];
		char phone[21];
		double salary;
	};
public:

	Employee(int id, const std::string& name, const std::string& address, const std::string& city, const std::string& state, const std::string& country, const std::string& phone, double salary);
	void display(std::ostream&) const; // Write a readable Employee representation to a stream
	void write(std::ostream&) const; // Write a fixed-length record to current file position
	void store(std::iostream&) const; // Overwrite (or append) record in (to) file
	void toXML(std::ostream&) const; // Write XML record for Employee
	static Employee* read(std::istream&); // Read record from current file position
	static Employee* retrieve(std::istream&, int); // Search file for record by id
	static Employee* fromXML(std::istream&); // Read the XML record from a stream
	friend std::ostream& operator<<(std::ostream&, const Employee&);
	void setSalary(const double&);
	double getSalary();
	static int recSize();
};
#endif

