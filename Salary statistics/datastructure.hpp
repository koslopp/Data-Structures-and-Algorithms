// Definition of Datastructure class, hw1 of TIE-20100/TIE-20106

#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP

#include <string>
using std::string;

#include <vector>
using std::vector;

struct Person
{
    string name;
    int salary;

    // Constructors
    Person() : name("UNKNOWN"), salary(-1) {}
    Person(string name_in, int salary_in) : name(name_in), salary(salary_in) {}
};


class Datastructure
{
public:
    Datastructure();
    ~Datastructure();

    void add_person(string name, int salary);

    unsigned int size();
    void clear();

    vector<Person*> personnel_alphabetically();
    vector<Person*> personnel_salary_order();

    Person* min_salary();
    Person* max_salary();

    Person* median_salary();
    Person* first_quartile_salary();
    Person* third_quartile_salary();

private:
    vector<Person*> workers_salary_; // Vector of pointers in salary order
    vector<Person*> workers_alpha_; // Vector of pointers in alpha order

    Person* unitialized_; // Returned by methods when empty (no worker added)
    Person* max_salary_; // Save the worker with biggest salary
    Person* min_salary_; // Save the worker with lowest salary

    unsigned int num_workers_; // Total number of workers (size)
    // Number of workers in salary vector which were not sorted
    unsigned int workers_salary_not_sorted_;
    // Number of workers in alpha vector which were not sorted
    unsigned int workers_alpha_not_sorted_;

    bool salary_is_sort_; //  Indicates if vector in salary order is sorted
    bool alpha_is_sort_; //  Indicates if vector in alpha order is sorted
};

#endif // DATASTRUCTURE_HPP

