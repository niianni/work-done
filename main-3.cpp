#include <cstdlib>
#include <iostream>
#include <vector>



void read_integers(std::vector<int>& ints, int count)
{
    int new_integer = 0;
    for(int i = 0; i < count; ++i)
    {
        std::cin >> new_integer;
        // TODO: Implement your solution here
        ints.push_back(new_integer);
    }
}

bool same_values(std::vector<int>& ints)
// true jos kaikki samoja
{
    for (unsigned long i = 0; i < ints.size()-1; ++i) {
        if(ints.at(i) != ints.at(i+1)){
            return false;
        }
    }
    return true;
}

bool is_ordered_non_strict_ascending(std::vector< int >& ints)
//true jos nouseva (edellinen <= seuraava)
{
    for (unsigned long i = 0; i < ints.size()-1; ++i) {
        if(ints.at(i) > ints.at(i+1)){
            return false;
        }
    }
    return true;
}

bool is_arithmetic_series(std::vector< int >& ints)
//true jos peräkkäisten alkioiden erotus on vakio
{
    int difference = ints.at(0)-ints.at(1);

    for (unsigned long i = 0; i < ints.size()-1; ++i) {
        if(ints.at(i)-ints.at(i+1) != difference){
            return false;
        }
    }
    return true;
}

bool is_geometric_series(std::vector< int >& ints)
//peräkkäisten termien suhdeluku on vakio
{
    //tarkastetaan sisältääkö jono nollia
    for(int number : ints){
        if (number == 0){
            return false;
        }
    }
    double m = ints.at(0);
    double n = ints.at(1);
    double ratio = m/n;

    for (unsigned long i = 0; i < ints.size()-1; ++i) {

        double h = ints.at(i);
        double j = ints.at(i+1);
        double test_ratio = h/j;

        if(test_ratio != ratio){
            return false;
        }
    }
    return true;
}

int main()
{
    std::cout << "How many integers are there? ";
    int how_many = 0;
    std::cin >> how_many;

    std::cout << "Enter the integers: ";
    std::vector<int> integers;
    read_integers(integers, how_many);

    if(same_values(integers))
        std::cout << "All the integers are the same" << std::endl;
    else
        std::cout << "All the integers are not the same" << std::endl;

    if(is_ordered_non_strict_ascending(integers))
        std::cout << "The integers are in a non-strict ascending order" << std::endl;
    else
        std::cout << "The integers are not in a non-strict ascending order" << std::endl;

    if(is_arithmetic_series(integers))
        std::cout << "The integers form an arithmetic series" << std::endl;
    else
        std::cout << "The integers do not form an arithmetic series" << std::endl;

    if(is_geometric_series(integers))
        std::cout << "The integers form a geometric series" << std::endl;
    else
        std::cout << "The integers do not form a geometric series" << std::endl;

    return EXIT_SUCCESS;
}
