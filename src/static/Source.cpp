#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char** argv) {

    std::ifstream f;
    f.open("out.csv");   /* open file with filename as argument */
    if (!f.is_open()) {    /* validate file open for reading */
        std::cerr << "error: file open failed '" << argv[1] << "'.\n";
        return 0;
    }


    std::string line, val;                  /* string for line & value */
    std::vector<std::vector<int>> array;    /* vector of vector<int>  */

    while (std::getline(f, line)) {        /* read each line */
        std::vector<int> v;                 /* row vector v */
        std::stringstream s(line);         /* stringstream line */
        while (getline(s, val, ','))       /* get each value (',' delimited) */
            v.push_back(std::stoi(val));  /* add to row vector */
        array.push_back(v);                /* add row vector to array */
    }

    std::vector<int> flat_array;            /* create 1D vector */
    for (auto& row : array) {               /* iterate over rows */
        for (auto& val : row)               /* iterate over vals */
            flat_array.push_back(val);      /* add to 1D vector */
    }

    int* array1d = flat_array.data();       /* get pointer to data */
    int size = flat_array.size();           /* get size of 1D array */

    std::ofstream out_file("out.bin", std::ios::binary); /* create binary file */
    out_file.write((char*)&size, sizeof(size)); /* write size of 1D array to file */
    out_file.write((char*)array1d, size * sizeof(int)); /* write 1D array to file */
    out_file.close(); /* close file */

    return 0;
}
