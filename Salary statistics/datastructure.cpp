// Definition of Datastructure class, hw3 of TIE-20100/TIE-20106

#include "datastructure.hpp"

// For debug
#include <iostream>
using std::cerr;
using std::endl;

// Remove after implement own sort func
#include <algorithm>

// Helper function to return a random (enough) number in range start-end
// I.e. random_in_range(1,4) produces a random number between 1, 2, 3, or 4.
template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = rand() % range;
    return static_cast<Type>(start+num);
}

// Comparison used on merge function. Depending on bool sort_salary the function
// compares the salary or the name of person and returns the expected bool.
bool merge_comparison(Person* B_left, Person* B_right, bool sort_salary) {
    if ( sort_salary ) {
        return ( B_left->salary <= B_right->salary );
    }
    else {
        return ( B_left->name <= B_right->name );
    }
}

// Merge function which gets two slices of A. Left one from start to middle and right
// on from middle to end. The full slice is copied to B, and the subslices, which are
// already sorted, are merged. The function was implemented based on the pseudocode on
// the slides. The extra boolen sort_salary is used to indicate whether they should be
// merged/sort by salary or name.
void merge( vector<Person*>& A, int start, int middle, int end, bool sort_salary ) {
    vector<Person*> B( A.begin()+start, A.begin()+end + 1 );
    int A_idx = start;
    int B_left_idx = start;
    int B_right_idx = middle + 1;
    while ( ( B_left_idx <= middle ) && ( B_right_idx <= end ) ) {
        if ( merge_comparison( B.at(B_left_idx-start), B.at(B_right_idx-start), sort_salary ) ) {
            A.at(A_idx) = B.at(B_left_idx - start);
            B_left_idx++;
        }
        else {
            A.at(A_idx) = B.at(B_right_idx - start);
            B_right_idx++;
        }
        A_idx++;
    }
    if ( B_left_idx > middle ) B_right_idx = 0;
    else B_right_idx = middle - end;
    for ( B_left_idx =  A_idx; B_left_idx <= end; B_left_idx++ ) {
        A.at(B_left_idx) = B.at(B_left_idx+B_right_idx -start);
    }
}

// Function that divides the vector A recursively and merge the slices to sort it.
// The function was implemented based on the pseudocode on the slides. The extra
// boolen sort_salary is used to indicate whether they should be merged/sort by salary
// or name.
void merge_sort( vector<Person*>& A, int start, int end, bool sort_salary) {
    if ( start < end ) {
        int middle = (start + end)/2;
        merge_sort(A, start, middle, sort_salary);
        merge_sort(A, (middle + 1), end, sort_salary);
        merge(A, start, middle, end, sort_salary);
    }
}


// Sort vector in salary order. If Total number of workers is equal to the number of
// workers whose was not sorted, sort the whole vector with merge sort. If part of the
// vector is already sorted, sort just the final part and merge with the previous
// sorted one.
void sort_salary(vector<Person*>& workers_salary_, unsigned int& num_workers_,
                 unsigned int& workers_salary_not_sorted_, bool& salary_is_sort_){
    // Sort the whole vector
    if (num_workers_ == workers_salary_not_sorted_) {
        merge_sort(workers_salary_, 0, num_workers_-1, true);
    }
    // Sort only the unsorted slice and merge with the whole vector.
    else {
        merge_sort(workers_salary_, num_workers_-workers_salary_not_sorted_,
                   num_workers_-1, true);
        merge(workers_salary_, 0,num_workers_-workers_salary_not_sorted_-1,
              num_workers_-1, true);

    }
    salary_is_sort_ = true;
    workers_salary_not_sorted_ = 0;
}

// Sort vector in alpha order. If Total number of workers is equal to the number of
// workers which were not sorted, sort the whole vector with merge sort. If part of the
// vector is already sorted, sort just the final part and merge with the previous
// sorted one.
void sort_alpha(vector<Person*>& workers_alpha_, unsigned int& num_workers_,
                unsigned int& workers_alpha_not_sorted_, bool& alpha_is_sort_){
    // Sort the whole vector
    if (num_workers_ == workers_alpha_not_sorted_) {
        merge_sort(workers_alpha_, 0, num_workers_-1, false);
    }
    // Sort only the unsorted slice and merge with the whole vector.
    else {
        merge_sort(workers_alpha_, num_workers_-workers_alpha_not_sorted_,
                   num_workers_-1, false);
        merge(workers_alpha_, 0, num_workers_-workers_alpha_not_sorted_-1,
              num_workers_-1, false);
    }
    alpha_is_sort_ = true;
    workers_alpha_not_sorted_ = 0;
}

// Constructor for the class.
Datastructure::Datastructure() :

    workers_salary_({}),
    workers_alpha_({}),
    unitialized_(new Person()),
    max_salary_(new Person()),
    min_salary_(new Person()),
    num_workers_(0),
    workers_salary_not_sorted_(0),
    workers_alpha_not_sorted_(0),
    salary_is_sort_(false),
    alpha_is_sort_(false) {
}

// Destructor for the class.
Datastructure::~Datastructure() {

    // Delete each person previously added on vector
    for ( vector<Person*>::iterator iter = workers_salary_.begin();
          iter != workers_salary_.end(); ++iter ) {
        delete *iter;
    }
    delete unitialized_; // Delete the unitialzed helper person
    delete max_salary_; // Delete the worker with biggest salary
    delete min_salary_; // Delete the worker with lowest salary
}

// Add new person/worker to class
void Datastructure::add_person(std::string name, int salary) {

    // Creates a pointer to a new worker
    Person* new_person = new Person(name, salary);

    // Verify if new worker has the maximum or minimum salary
    if ( num_workers_ == 0 ) {
        max_salary_ = new_person;
        min_salary_ = new_person;
    }
    else if ( new_person->salary > max_salary_->salary ) max_salary_ = new_person;
    else if ( new_person->salary < min_salary_->salary ) min_salary_ = new_person;

    // Add worker's pointer to both vector
    workers_salary_.push_back( new_person );
    workers_alpha_.push_back( new_person );

    num_workers_++; // Increment number total of workers
    // Increment number of workers in salary and alpha vector whose was not sorted
    workers_salary_not_sorted_++;
    workers_alpha_not_sorted_++;

    // Indicates that vectors are no longer sorted
    salary_is_sort_ = false;
    alpha_is_sort_ = false;
}

// Return the number of workers on vector
unsigned int Datastructure::size() {
    return num_workers_;
}

// Clear structure
void Datastructure::clear() {

    // Delete each person previously added on memory
    for ( vector<Person*>::iterator iter = workers_salary_.begin();
          iter != workers_salary_.end(); ++iter ) {
        delete *iter;
    }

    // Clear vector of pointers
    workers_salary_.clear();
    workers_alpha_.clear();

    num_workers_ = 0; // Reset number of workers
    workers_salary_not_sorted_ = 0;
    workers_alpha_not_sorted_ = 0;

    // Indicates that vectors are no longer sorted
    salary_is_sort_ = false;
    alpha_is_sort_ = false;

    // Point max and min salary to unkown worker
    max_salary_ = unitialized_;
    min_salary_ = unitialized_;
}

// Return vector of workers in alphabetical order.
vector<Person *> Datastructure::personnel_alphabetically() {
    // If there is no worker on vector, return an unitialized vector of person.
    if ( num_workers_ == 0 ) {
        vector<Person *> unkown = {unitialized_};
        return unkown;
    }
    // If the alpha vector is not sorted, sort it and return.
    else if ( !alpha_is_sort_ ) {
        sort_alpha(workers_alpha_, num_workers_, workers_alpha_not_sorted_, alpha_is_sort_);
    }
    return workers_alpha_;
}

vector<Person *> Datastructure::personnel_salary_order() {
    // If there is no worker on vector, return an unitialized vector of person.
    if ( num_workers_ == 0 ) {
        vector<Person *> unkown = {unitialized_};
        return unkown;
    }
    // If the salary vector is not sorted, sort it and return.
    else if ( !salary_is_sort_ ) {
        sort_salary(workers_salary_, num_workers_, workers_salary_not_sorted_, salary_is_sort_);
    }
    return workers_salary_;
}

// Returns the person with lowest salary
Person* Datastructure::min_salary() {
    // If there is no worker on vector, return an unitialized person.
    if ( num_workers_ == 0 ) return unitialized_;
    else return min_salary_;
}

// Returns the person with biggest salary
Person* Datastructure::max_salary() {
    // If there is no worker on vector, return an unitialized person.
    if ( num_workers_ == 0 ) return unitialized_;
    else return max_salary_;
}

Person* Datastructure::median_salary() {
    // If there is no worker on vector, return an unitialized person.
    if ( num_workers_ == 0 ) return unitialized_;
    // If the salary vector is not sorted, sort it and return median index
    else if ( !salary_is_sort_ ) {
        sort_salary(workers_salary_, num_workers_, workers_salary_not_sorted_, salary_is_sort_);
    }
    return workers_salary_.at( workers_salary_.size()/2 );
}

Person* Datastructure::first_quartile_salary() {
    // If there is no worker on vector, return an unitialized person.
    if ( num_workers_ == 0 ) return unitialized_;
    // If the salary vector is not sorted, sort it and return 1/4 index
    else if ( !salary_is_sort_ ) {
        sort_salary(workers_salary_, num_workers_, workers_salary_not_sorted_, salary_is_sort_);
    }
    return workers_salary_.at( workers_salary_.size()/4 );
}

Person* Datastructure::third_quartile_salary() {
    // If there is no worker on vector, return an unitialized person.
    if ( num_workers_ == 0 ) return unitialized_;
    // If the salary vector is not sorted, sort it and return 3/4 index
    else if ( !salary_is_sort_ ) {
        sort_salary(workers_salary_, num_workers_, workers_salary_not_sorted_, salary_is_sort_);
    }
    return workers_salary_.at( 3*workers_salary_.size()/4 );
}

