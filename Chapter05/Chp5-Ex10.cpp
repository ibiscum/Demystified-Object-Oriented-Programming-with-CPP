// (c) Dorothy R. Kirk. All Rights Reserved.
// Purpose: To illustrate const member functions.

#include <iostream>
#include <utility>

using std::cout;   // preferred to: using namespace std;
using std::endl;
using std::string;

class Student
{
private:
	// data members
	string firstName;
	string lastName;
	char middleInitial;
	float gpa;
	string currentCourse;
	const int studentId;   // constant data member

public:
	// member function prototypes
	Student();  // default constructor
	Student(string , string , char, float, string , int);
	Student(const Student&);  // copy constructor
	~Student();  // destructor
	void Print() const; // most of these member functions are const

	[[nodiscard]] const string& GetFirstName() const
	{
		return firstName;
	}

	[[nodiscard]] const string& GetLastName() const
	{
		return lastName;
	}

	[[nodiscard]] char GetMiddleInitial() const
	{
		return middleInitial;
	}

	[[nodiscard]] float GetGpa() const
	{
		return gpa;
	}

	[[nodiscard]] const string& GetCurrentCourse() const
	{
		return currentCourse;
	}

	[[nodiscard]] int GetStudentId() const
	{
		return studentId;
	}

	void SetCurrentCourse(const string&);  // prototype only
};

inline void Student::SetCurrentCourse(const string& c)
{
	currentCourse = c;   // the implementation using the string is far simpler
}

// default constructor
Student::Student() : middleInitial('\0'), gpa(0.0), studentId(0)
{
	// note: you may still initialize data members here, but using member initialization list is more efficient
	// Because string data members, firstName, lastName, currentCourse are initialized with empty strings (they are member objects
	// which are default constructed), we do not need to include the default constructor specification for these members above.
}

// alternate constructor 
Student::Student(string fn, string ln, char mi, float avg, string course, int id) :
	firstName(std::move(fn)),
	lastName(std::move(ln)),
	middleInitial(mi),
	gpa(avg),
	currentCourse(std::move(course)),
	studentId(id)
{
	// note: for string data members, the above initialization is calling the constructor matching the signature of item in ()'s
	// this is inevitably more efficient than default constructing the data member and then assigning it with a new value in the method body
	// note: any pointer data members often need to be set within body of function (but can sometimes be set in init. list too)
}

// copy constructor 
Student::Student(const Student& s) : firstName(s.firstName), lastName(s.lastName), middleInitial(s.middleInitial),
									 gpa(s.gpa), currentCourse(s.currentCourse), studentId(s.studentId)
{
	// remember to do a deep copy for any data members that are pointers
}

Student::~Student()
{
	// remember to delete memory for any data members that are pointers
}

void Student::Print() const
{
	cout << firstName << " " << middleInitial << ". ";
	cout << lastName << " with id: " << studentId;
	cout << " and gpa: " << gpa << " is enrolled in: ";
	cout << currentCourse << endl;
}

int main()
{
	Student s1("Zack", "Moon", 'R', 3.85, "C++", 1378);
	cout << s1.GetFirstName() << " " << s1.GetLastName();
	cout << " Enrolled in " << s1.GetCurrentCourse() << endl;
	s1.SetCurrentCourse("Advanced C++ Programming");
	cout << s1.GetFirstName() << " " << s1.GetLastName();
	cout << " New course: " << s1.GetCurrentCourse() << endl;

	const Student s2("Gabby", "Doone", 'A', 3.9, "C++", 2239);
	s2.Print();
	// Not allowed, s2 is const
	// s2.SetCurrentCourse("Advanced C++ Programming");

	return 0;
}