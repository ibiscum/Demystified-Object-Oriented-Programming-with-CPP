// (c) Dorothy R. Kirk. All Rights Reserved.
// Purpose: To illustrate a nested exception class within a user defined class.

#include <iostream>
#include <iomanip>

using std::cout;    // preferred to: using namespace std;
using std::endl;
using std::setprecision;
using std::set_terminate;
using std::string;
using std::exception;

constexpr int MAX = 5;

void AppSpecificTerminate()
{
    cout << "Uncaught exception. Program terminating" << endl;
    exit(1);
}

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
    // dynamically allocate memory for any pointer data members here
}

Person::Person(const string &fn, const string &ln, char mi, const string &t) :
               firstName(fn), lastName(ln), middleInitial(mi), title(t)
{
   // dynamically allocate memory for any pointer data members here
}

Person::Person(const Person &p) :
               firstName(p.firstName), lastName(p.lastName),
               middleInitial(p.middleInitial), title(p.title)
{
   // deep copy any pointer data members here
}

Person::~Person()
{
    // release memory for any dynamically allocated data members
}

void Person::ModifyTitle(const string &newTitle)
{
    title = newTitle;     // assignment between strings ensures a deep assignment
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
    // data members
    float gpa;
    string currentCourse;
    const string studentId;  
public:
    // member function prototypes
    Student();  // default constructor
    Student(const string &, const string &, char, const string &, float, const string &, const string &);
    Student(const Student &);  // copy constructor
    virtual ~Student();  // destructor
    void EarnPhD();  
    bool TakePrerequisites();
    // inline function definitions
    float GetGpa() const { return gpa; }
    const string &GetCurrentCourse() const { return currentCourse; }
    const string &GetStudentId() const { return studentId; }
    void SetCurrentCourse(const string &); // prototype only
  
    // In the derived class, the keyword virtual is optional, 
    // but recommended for internal documentation
    virtual void Print() const override;
    virtual void IsA() const override;
    // note: we choose not to redefine // Person::Greeting(const string &) const
    virtual void Validate();  // newly introduced virtual fn in Student
    virtual void Graduate();

    class StudentException   // over-simplified nested exception class 
    {
    private:
        int number;
    public:
        StudentException(int num): number(num) { }
        ~StudentException() { }
        int GetNum() const { return number;}
    }; 
};

inline void Student::SetCurrentCourse(const string &c)
{
   currentCourse = c;
}

Student::Student() : gpa(0.0), currentCourse(""), studentId ("")
{
   // note: since studentId is const, if the Student is default constructed, this id will always be empty.
   // Another approach, would be to generate a unique id always and use this in both constructors
   // dynamically allocate memory for any pointer data members here
}

// Alternate constructor member function definition
Student::Student(const string &fn, const string &ln, char mi, const string &t,
       float avg, const string &course, const string &id) : Person(fn, ln, mi, t),
                       gpa(avg), currentCourse(course), studentId(id)
{
   // dynamically allocate memory for any pointer data members here
}

// Copy constructor definition
Student::Student(const Student &s) : Person(s),
                 gpa(s.gpa), currentCourse(s.currentCourse), studentId(s.studentId)
{
   // deep copy any pointer data members of derived class here

}

// destructor definition
Student::~Student()
{
   // release memory for any dynamically allocated data members

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

void Student::Validate()
{
    // check Student instance to see if standards are met; if not, throw an exception
    throw "Student does not meet prerequisites"; 
}

bool Student::TakePrerequisites()
{
    // Assume this function can correct the issue at hand
    // if not, it returns false
    return false;
}

void Student::Graduate()
{
    // Assume this method is fully implemented. Here, I'm forcing a throw of a Course 
    // unless the bad gpa requirement is first triggered

    if (gpa < 2.0)  // if Student does not meet gpa requirements, throw bad gpa
       throw gpa;
    // if Student is short credits, throw how many are missing
    // throw numCreditsMissing;  // assume this is an integer
    // or if Student is missing a course, construct and
    // then throw the missing Course as a referencable object
    // throw *(new Course("Intro. to Programming", 1234)); 
    // or throw a string containing a diagnostic message
    // throw (“Does not meet requirements”); 

    throw *(new StudentException(5));
}


int main()
{
    set_terminate(AppSpecificTerminate);  // register fn.

    Student s1("Ling", "Mau", 'I', "Ms.", 3.1, "C++", "55UD"); 

    try  
    {
        s1.Graduate();
    }
    catch (float err)
    {
        cout << "Too low gpa: " << err << endl;
        exit(1); // only if you can’t fix, exit gracefully
    } 
    catch (int err)
    {
        cout << "Missing " << err << " credits" << endl;
        exit(2);
    }
    catch (const char *err)
    {
        cout << err << endl;
        exit(4); 
    }
    catch (const Student::StudentException &err)
    {
        cout << "Error: " << err.GetNum() << endl;
        // if you can correct the error and continue the application, be    
        // sure to deallocate heap memory for err using:
        // delete &err;  // take addr of the ref to delete
        exit(5);   // Heap memory will be reclaimed if you choose to exit
    }
    catch (...) 
    {
        cout << "Exiting" << endl;
        exit(6);
    }


    cout << "Moving onward with remainder of code." << endl;

    return 0;
}

