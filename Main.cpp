// Project:  XML Parser
// Name: Joseph Brinton
// Submitted: 02/22/18
// I declare that the following source code was written by me, or provided
// by the instructor for this project. I understand that copying source
// code from any other source, providing source code to another student, 
// or leaving my code on a public web site constitutes cheating.
// I acknowledge that  If I am found in violation of this policy this may result
// in a zero grade, a permanent record on file and possibly immediate failure of the class.
/* Reflection (1-2 paragraphs):  
	While this project was easier for me than than Project 2 it still took about the same amount of time. The most 
	time consuming part for me was the logic in the fromXML function. I think if I had planned and organized this
	function before beginning to code it may have ended up cleaner. Instead I kept all the logic inside one founction
	and didn't have time to separate it out into like in your example. Overall, I think my code turned out well, I
	think there are just a few concepts that I need to review and plan the stucture of each project ahead of coding.
*/

#ifdef NDEBUG
#undef NDEBUG
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <memory>
#include "Employee.h"



using namespace std;

int main(int argc, char* argv[]) {

	
	for (int i = 0; i < argc; ++i)
	cout << argv[i] << ' ';
	cout << endl;
	assert(argc > 1);

	assert(argv[1] != nullptr);
	std::vector<unique_ptr<Employee> > v;

	ifstream file(argv[1]);
	
	assert(file);
	if (file) {
		try {
			Employee* employ = Employee::fromXML(file);
			while (employ) {
				v.push_back(unique_ptr<Employee>(employ));
				employ = Employee::fromXML(file);
			}
		}
		catch (runtime_error err) {
			cout << err.what() <<  endl; 
			cin.get();
			return EXIT_FAILURE;

		}
		for (auto & e: v) {
			e->display(cout);
		}
		fstream employeestream("employee.bin", ios::in | ios::out | ios::binary | ios::trunc );
		
		for (auto & e : v) {
			e->write(employeestream);
		}

		for (auto & e : v) {
			e.reset(); // delete memory to prevent memory leak
		}

		v.clear();
		employeestream.clear();
		employeestream.seekg(0, ios::beg);

		Employee *p = Employee::read(employeestream);
		
		while (p) {
			v.push_back(unique_ptr<Employee>{p});
			 p = Employee::read(employeestream);
		} 
		
		
		for (auto & e : v) {
			e->toXML(cout);
		}
		employeestream.clear();                       
		employeestream.seekg(0, ios::beg);
	
		Employee * e = Employee::retrieve(employeestream, 12345);
		
		e->setSalary(150000.0);
	
		employeestream.clear();
		employeestream.seekg(0, ios::beg);
		e->store(employeestream);
		delete e;
		employeestream.clear();                       
		employeestream.seekg(0, ios::beg);


		Employee * d = Employee::retrieve(employeestream, 12345);

		cout << "Employee 12345 salary: " << d->getSalary() << endl;
		delete d;

		Employee  em = Employee (999, "Joe", "123 University Pkwy","Provo", "Utah", "US", "555-555-5555", 999999.0);
		v.push_back(unique_ptr<Employee>(&em));

		employeestream.clear();
		employeestream.seekg(0, ios::beg);
		em.store(employeestream);
		
		employeestream.clear();
		employeestream.seekg(0, ios::beg);
		Employee * l = Employee::retrieve(employeestream, 999);

		if (l) {
			cout << "Found: " << endl;
			l->display(cout);
		}
	}

	std::cin.get();
}
