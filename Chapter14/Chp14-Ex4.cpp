// (c) Dorothy R. Kirk. All Rights Reserved.
// Purpose: To illustrate STL deque 

#include <iostream>
#include <iomanip>
#include <deque>

using std::cout;   // preferred to: using namespace std;
using std::endl;
using std::setprecision;
using std::string;
using std::deque;

class Person
{
private: 
    string firstName;
    string lastName;
    char middleInitial;
    string title;  // Mr., Ms., Mrs., Miss, Dr., etc.
protected:
    void ModifyTitle(const string &); 
public:
    Person();   // default constructor
    Person(const string &, const string &, char, const string &);  
    Person(const Person &);  // copy constructor
    Person &operator=(const Person &); // overloaded assignment operator
    virtual ~Person();  // virtual destructor

    // inline function definitions
    const string &GetFirstName() const { return firstName; }  
    const string &GetLastName() const { return lastName; }    
    const string &GetTitle() const { return title; } 
    char GetMiddleInitial() const { return middleInitial; }

    // Virtual functions will not be inlined since their 
    // method must be determined at run time using v-table.
    virtual void Print() const; 
    virtual void IsA() const;  
    virtual void Greeting(const string &) const;
};

Person::Person() : firstName(""), lastName(""), middleInitial('\0'), title("")
{
}

Person::Person(const string &fn, const string &ln, char mi, const string &t) :
               firstName(fn), lastName(ln), middleInitial(mi), title(t)
{
}

Person::Person(const Person &p) : firstName(p.firstName), lastName(p.lastName),
                                  middleInitial(p.middleInitial), title(p.title)
{
}

Person::~Person()
{
}

Person &Person::operator=(const Person &p)
{
   // make sure we're not assigning an object to itself
   if (this != &p)
   {
      // delete any previously dynamically allocated data members here from the destination object
      // or call ~Person() to release this memory -- unconventional

      // Also, remember to reallocate memory for any data members that are pointers.

      // copy from source to destination object each data member
      firstName = p.firstName;
      lastName = p.lastName;
      middleInitial = p.middleInitial;
      title = p.title;
   }
   return *this;  // allow for cascaded assignments
}

void Person::ModifyTitle(const string &newTitle)
{
    title = newTitle;
}

void Person::Print() const
{
    cout << title << " " << firstName << " ";
    cout << middleInitial << ". " << lastName << endl;
}

void Person::IsA() const
{
    cout << "Person" << endl;
}

void Person::Greeting(const string &msg) const
{
    cout << msg << endl;
}


class Student : public Person
{
private: 
    float gpa;
    string currentCourse;
    string studentId;      // decided to make studentId not const (a design decision that makes copy constructor more productive, etc.)
    static int numStudents;
public:
    // member function prototypes
    Student();  // default constructor
    Student(const string &, const string &, char, const string &, float, const string &, const string &); 
    Student(const Student &);  // copy constructor
    Student &operator=(const Student &); // overloaded assignment operator
    virtual ~Student();  // destructor
    void EarnPhD();  
    // inline function definitions
    float GetGpa() const { return gpa; }
    const string &GetCurrentCourse() const { return currentCourse; }
    const string &GetStudentId() const { return studentId; }
    void SetCurrentCourse(const string &); // prototype only
  
    // In the derived class, the keyword virtual is optional, 
    // but recommended for internal documentation. Same for override.
    virtual void Print() const override;
    virtual void IsA() const override;
    // note: we choose not to redefine Person::Greeting(const string &); const
    static int GetNumberStudents() { return numStudents; }
};


int Student::numStudents = 0;  // definition of static data member


inline void Student::SetCurrentCourse(const string &c)
{
    currentCourse = c;
}

Student::Student() : gpa(0.0), currentCourse(""), studentId ("None")
{
    numStudents++;
}

Student::Student(const string &fn, const string &ln, char mi, const string &t, float avg, const string &course,
                 const string &id) : Person(fn, ln, mi, t), gpa(avg), currentCourse(course), studentId(id)
{
    numStudents++;
}

Student::Student(const Student &s) : Person(s), gpa(s.gpa), currentCourse(s.currentCourse), studentId(s.studentId)
{
    numStudents++;
}

// destructor definition
Student::~Student()
{
    numStudents--;
    // the embedded object studentId will also be destructed
}

// overloaded assignment operator
Student &Student::operator=(const Student &s)
{
   // make sure we're not assigning an object to itself
   if (this != &s)
   {
      Person::operator=(s);

      // delete any dynamically allocated data members in destination Student (or call ~Student() - unconventional)

      // remember to allocate any memory in destination for copies of source members

      // copy data members from source to designation object
      gpa = s.gpa;
      currentCourse = s.currentCourse;
      studentId = s.studentId;

   }
   return *this;  // allow for cascaded assignments
}

void Student::EarnPhD()
{
    ModifyTitle("Dr.");
}

void Student::Print() const
{   // need to use access functions as these data members are
    // defined in Person as private
    cout << GetTitle() << " " << GetFirstName() << " ";
    cout << GetMiddleInitial() << ". " << GetLastName();
    cout << " with id: " << studentId << " GPA: ";
    cout << setprecision(3) <<  " " << gpa;
    cout << " Course: " << currentCourse << endl;
}

void Student::IsA() const
{
    cout << "Student" << endl;
}


bool operator<(const Student &s1, const Student &s2)
{
    return (s1.GetGpa() < s2.GetGpa());
}

bool operator==(const Student &s1, const Student &s2)
{
    return (s1.GetGpa() == s2.GetGpa());
}

int main()
{
    deque<Student> studentBody;
    Student s1("Tim", "Lim", 'O', "Mr.", 3.2, "C++", "111UD");

    studentBody.push_back(Student("Hana", "Sato", 'U', "Dr.", 3.8, "C++", "178PSU")); 
    studentBody.push_back(Student("Sara", "Kato", 'B', "Dr.", 3.9, "C++", "272PSU")); 
    studentBody.push_front(Student("Giselle", "LeBrun", 'R', "Ms.", 3.4, "C++", "299TU")); 

    // Note, operator= is called both by insert (twice) and in the explicit assignment on the line below insert
    studentBody.insert(studentBody.begin() + 1, Student("Anne", "Brennan", 'B', "Ms.", 3.9, "C++", "299CU")); 
    studentBody[0] = s1;  // no bounds checking is done, be careful!

    while (studentBody.empty() == false)
    {
        studentBody.front().Print();
        studentBody.pop_front();
    }
    return 0;
}

