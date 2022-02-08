/* RASSE
 *
 * Kuvaus:
 *  Ohjelma lukee käynnistyessään ratikkalinjoihin liittyviä
 * tietoja tiedostosta, tallentaa ne ja antaa käyttäjälle
 * mahdollisuuden tehdä hakuja, lisäyksiä ja poistoja tietoihin
 * lukemalla syötteenä komentoja.
 *
 * Ohjelman komennot:
 * QUIT: Ohjelman suoritus päättyy.
 * LINES: Tulostaa aakkosjärjestyksessä kaikki linjat.
 * LINE <linja>: Tulostaa annetulle linjalle kuuluvat pysäkit.
 * STOPS: Tulostaa kaikki pysäkit aakkosjärjestyksessä.
 * STOP <pysäkki>: Tulostaa ne linjat, joille pysäkki kuuluu.
 * DISTANCE <linja> <pysäkki1> <pysäkki2>: Tulostaa annettujen
 * pysäkkien välisen etäisyyden.
 * ADDLINE <linja>: Lisää uuden linjan.
 * ADDSTOP <linja> <uusi pysäkki> <etäisyys>: Lisää annetulle linjalle
 * annetun pysäkin ja sen etäisyyden lähtöpysäkistä.
 * REMOVE <pysäkki>: Poistaa annetun pysäkin kaikilta linjoilta.
 *
 * Ohjelman kirjoittaja
 * Nimi: Niina Ahola
 * Opiskelijanumero: K439389
 * Käyttäjätunnus: bmniah
 * E-Mail: niina.ahola@tuni.fi
 *
 * */


#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

using Tramlines = std::map<std::string, std::map<std::string, double>>;
using Stops = std::map<std::string, double>;

// The most magnificent function in this whole program.
// Prints a RASSE
void print_rasse()
{
    std::cout <<
                 "=====//==================//===\n"
                 "  __<<__________________<<__   \n"
                 " | ____ ____ ____ ____ ____ |  \n"
                 " | |  | |  | |  | |  | |  | |  \n"
                 " |_|__|_|__|_|__|_|__|_|__|_|  \n"
                 ".|                  RASSE   |. \n"
                 ":|__________________________|: \n"
                 "___(o)(o)___(o)(o)___(o)(o)____\n"
                 "-------------------------------" << std::endl;
}

// Tarkastaa, onko annetun vektoreiden alkioissa hipsuja.
// Jos hipsut löytyy, yhdistetään sana seuraavan kanssa ja poistetaan hipsut.
// Palauttaa vektorin, jossa on yhdistetyt sanat ilman hipsuja.
std::vector<std::string> check_apostrophe(const std::vector<std::string>& v){

    std::vector<std::string> result;
    char delimiter = '"';

    for ( unsigned int i = 0; i < v.size(); ++i ){
        std::string word = v.at(i);
        if ( word.at(0) == delimiter && word.at(word.size()-1) == delimiter ){
            // ensimmäinen ja viimeinen merkki "
            result.push_back(word.substr(1, word.size()-2));
        } else if ( word.at(word.size()-1) == delimiter ) {
            continue;
        } else if ( word.at(0) == delimiter ){
            std::string add_word = v.at(i+1);
            std::string tmp_word = word.substr(1, word.size()-1) + ' ' + add_word.substr(0, add_word.size()-1);
            result.push_back(tmp_word);
        } else {
            result.push_back(word);
        }
    }
    return result;
}


// Erottelee annetulta riviltä osat erotinmerkin kohdalta (delimiter).
// Palauttaa vektorin, joka sisältää osat.
std::vector<std::string> split(const std::string& s, const char delimiter,
                               bool ignore_empty = false){
    std::vector<std::string> result;
    std::string tmp = s;

    while(tmp.find(delimiter) != std::string::npos) {
        std::string new_part = tmp.substr(0, tmp.find(delimiter));

        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        if(not (ignore_empty and new_part.empty())){
            result.push_back(new_part);
        }
    }
    if(not (ignore_empty and tmp.empty())) {
        result.push_back(tmp);
    }
    result = check_apostrophe(result);

    return result;
}

// Tarkastaa, onko etäisyys jo annetulla linjalla.
// Jos sama etäisyys löytyy, palauttaa true.
bool same_distance(Stops tramline, const double& distance){
    auto iter = tramline.begin();
    bool found = false;

    while(iter != tramline.end()){
        if (iter->second == distance){
            found = true;
        }
        iter++;
    }
    return found;
}

// Avaa annetun tiedoston ja lukee sen riveittäin.
// Tallentaa tiedot (linja, pysäkki, etäisyys) säiliöön.
bool read_file(const std::string& filename, Tramlines& tramlines){
    std::ifstream file(filename);

    if ( not file) {
        // tiedoston avaaminen epäonnistui
        std::cout << "Error: File could not be read." << std::endl;
        return false;
    } else {
        // tiedoston avaaminen onnistui
        std::string line;
        // käy läpi tiedoston rivit
        while (getline(file, line)) {
            std::vector<std::string> parts = split(line, ';', true);
            if (parts.size() != 2 && parts.size() != 3){
                // tiedosto on vääränlainen
                std::cout << "Error: Invalid format in file." << std::endl;
                return false;
            }
            std::string tramline = parts.at(0);
            std::string stop = parts.at(1);
            double distance = 0;

            if (parts.size() == 3) {
                distance = std::stod(parts.at(2));
            }
            // tallenna rivin tiedot annettuun säiliöön
            if (tramlines.find(tramline) == tramlines.end()) {
                // linjaa ei ole, lisätään se
                tramlines.insert({tramline, {}});
            }
            // Jos samalla linjalla kaksi samaa pysäkkiä tai kaksi samaa etäisyyttä.
            if (tramlines.at(tramline).find(stop) != tramlines.at(tramline).end() ||
                same_distance(tramlines.at(tramline), distance)){
                std::cout << "Error: Stop/line already exists." << std::endl;
                return false;
            }
            // lisää pysäkkitiedot olemassaolevaan linjaan
            tramlines.at(tramline).insert({stop, distance});
        }
    }
    file.close();
    return true;
}

// Tulosta kaikki linjat aakkosjärjestyksessä.
void print_lines(Tramlines tramlines){
    std::cout << "All tramlines in alphabetical order:" << std::endl;
    Tramlines::iterator iter = tramlines.begin();

    while ( iter != tramlines.end() ){
        std::cout << iter->first << std::endl;
        ++iter;
    }
}

// Tulostaa annetun reitin kaikki pysäkit etäisyyden mukaan järjestyksessä.
void print_route(Tramlines tramlines, std::string line){

    if ( tramlines.find(line) != tramlines.end() ){
        std::cout << "Line " << line << " goes through these stops in "
                     "the order they are listed: " << std::endl;

        std::map<double, std::string> tmp;

        Stops::iterator iter = tramlines.at(line).begin();
        while ( iter != tramlines.at(line).end() ){
            std::string stop_name = iter->first;
            double distance = iter->second;
            tmp.insert({distance, stop_name});
            ++iter;
        }

        std::map<double, std::string>::iterator tmp_iter = tmp.begin();
        while ( tmp_iter != tmp.end() ){
            std::cout << "- " << tmp_iter->second << " : " << tmp_iter->first << std::endl;
            ++tmp_iter;
        }
    } else {
        std::cout << "Error: Line could not be found." << std::endl;
    }
}

// Palauttaa joukon, jossa on kaikki pysäkit.
std::set<std::string> all_stops(const Tramlines& tramlines){
    std::set<std::string> stops;
    Tramlines::const_iterator line_iter = tramlines.begin();
    while ( line_iter != tramlines.end() ){
        std::string line = line_iter->first;
        Stops::const_iterator stop_iter = tramlines.at(line).begin();
        while ( stop_iter != tramlines.at(line).end() ){
            stops.insert(stop_iter->first);
            ++stop_iter;
        }
        ++line_iter;
    }
    return stops;
}

// Tulostaa kaikki annetut pysäkit aakkosjärjestyksessä.
void print_all_stops(Tramlines tramlines){
    std::cout << "All stops in alphabetical order:" << std::endl;
    std::set<std::string> stops = all_stops(tramlines);

    for ( std::string stop : stops ){
        std::cout << stop << std::endl;
    }
}

// Tulosta aakkosjärjestyksessä linjat joille pysäkki (stop) kuuluu.
void print_lines_stopping_at(Tramlines& tramlines, std::string stop){

    std::set<std::string> print_these;

    Tramlines::iterator iter = tramlines.begin();

    while ( iter != tramlines.end() ){
        std::string line_name = iter->first;

        // Jos annettu pysäkki löytyy linjan tiedoista, lisää se
        if (tramlines.at(line_name).find(stop) != tramlines.at(line_name).end()){
            print_these.insert(line_name);
        }
        ++iter;
    }
    if ( print_these.size() == 0 ){
        std::cout << "Error: Stop could not be found." << std::endl;
    } else {
        std::cout << "Stop " << stop << " can be found on the following "
                      "lines:" << std::endl;
        for ( std::string line : print_these ){
            std::cout << " - " << line << std::endl;
        }
    }
}

// Tulostaa annettujen pysäkkien välisen etäisyyden.
void print_distance(Tramlines& tramlines, std::string line_name,
                    std::string stop1, std::string stop2){
    // löytyykö ylipäänsä
    if ( tramlines.find(line_name) == tramlines.end() ){
        std::cout << "Error: Line could not be found." << std::endl;
    } else {
        Stops line = tramlines.at(line_name);
        if ( line.find(stop1) == line.end() ||
             line.find(stop2) == line.end() ){
            std::cout << "Error: Stop could not be found." << std::endl;
        } else {
            double distance;
            double distance1 = tramlines.at(line_name).at(stop1);
            double distance2 = tramlines.at(line_name).at(stop2);
            if ( distance1 >= distance2 ){
                 distance = distance1-distance2;
             } else {
                 distance = distance2-distance1;
                 }
                 std::cout << "Distance between " << stop1 << " and " << stop2
                          << " is " << distance << std::endl;
        }
    }
}

// Lisää uusi linja ilman pysäkkejä.
void add_line(Tramlines& tramlines, std::string new_line){
    if ( tramlines.find(new_line) == tramlines.end() ){
        tramlines.insert({new_line, {}});
        std::cout << "Line was added." << std::endl;
    } else {
        std::cout << "Error: Stop/line already exists." << std::endl;
    }
}

// Lisää uusi pysäkki annetulle linjalle.
void add_stop(Tramlines& tramlines, std::string line, std::string new_stop,
              double distance){
    if ( tramlines.find(line) == tramlines.end() ){
        std::cout << "Error: Line could not be found." << std::endl;
    } else if ( tramlines.at(line).find(new_stop) != tramlines.at(line).end() ||
                same_distance(tramlines.at(line), distance) ) {
        std::cout << "Error: Stop/line already exists." << std::endl;
    } else {
        tramlines.at(line).insert({new_stop, distance});
        std::cout << "Stop was added." << std::endl;
    }
}

// Poista annettu pysäkki kaikilta linjoilta.
void remove_line(Tramlines& tramlines, std::string stop){
    Tramlines::iterator line_iter = tramlines.begin();

    std::set<std::string> stops = all_stops(tramlines);

    if ( stops.find(stop) == stops.end() ){
        std::cout << "Error: Stop could not be found." << std::endl;
    } else {
        while ( line_iter != tramlines.end() ){
            std::string line = line_iter->first;
            Stops stops = tramlines.at(line);
            if ( stops.find(stop) != stops.end()){
                tramlines.at(line).erase(stop);
            }
            ++line_iter;
         }
         std::cout << "Stop was removed from all lines." << std::endl;
    }
}

int main()
{
    print_rasse();
    // Kysy syötetiedoston nimi
    std::cout << "Give a name for input file: ";
    std::string filename;
    getline(std::cin, filename);
    Tramlines tramlines;
    if ( read_file(filename, tramlines) == false ){
        return EXIT_FAILURE;
    }

    bool ask_input = true;
    while (ask_input){
        // Kysy komento
        std::string input;
        std::cout << "tramway> ";
        getline(std::cin, input);

        std::vector<std::string> separated_input = split(input, ' ', true);
        std::string command = separated_input.at(0);

        // Muuta komentokäsky isoiksi kirjaimiksi
        for (unsigned int i = 0; i < command.size(); ++i){
            command[i] = toupper(command[i]);
        }

        // Eri komentojen suoritukset
        if (command == "QUIT"){
            return EXIT_SUCCESS;

        } else if (command == "LINES"){
            print_lines(tramlines);

        } else if (command == "LINE"){
            if ( separated_input.size() < 2 ){
                std::cout << "Error: Invalid input." << std::endl;
            } else {
                std::string line = separated_input.at(1);
                print_route(tramlines, line);
            }

        } else if (command == "STOPS"){
            print_all_stops(tramlines);

        } else if (command == "STOP"){
            if ( separated_input.size() < 2 ){
                std::cout << "Error: Invalid input." << std::endl;
            } else {
                std::string stop = separated_input.at(1);
                print_lines_stopping_at(tramlines, stop);
            }

        } else if (command == "DISTANCE"){
            if ( separated_input.size() < 4 ){
                std::cout << "Error: Invalid input." << std::endl;
            } else {
                std::string line = separated_input.at(1);
                std::string stop1 = separated_input.at(2);
                std::string stop2 = separated_input.at(3);
                print_distance(tramlines, line, stop1, stop2);
            }

        } else if (command == "ADDLINE"){
            if ( separated_input.size() < 2 ){
                std::cout << "Error: Invalid input." << std::endl;
            } else {
                std::string new_line = separated_input.at(1);
                add_line(tramlines, new_line);
            }

        } else if (command == "ADDSTOP"){
            if ( separated_input.size() <= 3 ){
                std::cout << "Error: Invalid input." << std::endl;
            } else {
                std::string line = separated_input.at(1);
                std::string new_stop = separated_input.at(2);
                double distance = 0;
                if ( separated_input.size() >= 4){
                    distance = stod(separated_input.at(3));
                }
                add_stop(tramlines, line, new_stop, distance);
            }

        } else if (command == "REMOVE"){
            if ( separated_input.size() < 2 ){
                std::cout << "Error: Invalid input." << std::endl;
            } else {
                std::string line = separated_input.at(1);
                remove_line(tramlines, line);
            }

        } else {
            std::cout << "Error: Invalid input." << std::endl;
        }
    }
}
