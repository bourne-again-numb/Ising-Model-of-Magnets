#ifndef _INITIALIZE
#define _INITIALIZE


#include <iostream>
#include <stdexcept>
#include <filesystem>

#include "./include/lattices.h"
#include "./include/csv_reader.h"


/*
  Defing global variables
*/

using std :: ofstream;
using std :: string;

extern int LX, LY, LZ, NSITES, INITIAL_SPIN, NEIGHBORS;

extern int *SPIN;

extern int **N1LIST, **SITE_COORD;

extern double TSTAR, EXCHANGE_ENERGY;

extern int MCSTEPS, EQSTEPS, SAMPLE_FREQ;

extern string LATTICE_TYPE, INPUT_FILE;

extern ofstream RESULT_FILE, COORD_FILE;

/*
	Initialize the variables
 */

// name space variables
using std :: cout;
using std :: endl;
using std :: string;
using std :: ofstream;
using std :: vector;
using std :: stoi;
using std :: stod;

// using
int LX, LY, LZ, NSITES, INITIAL_SPIN, NEIGHBORS;
int *SPIN;
int **N1LIST, **SITE_COORD;
double TSTAR, EXCHANGE_ENERGY;
int MCSTEPS, EQSTEPS, SAMPLE_FREQ;
string LATTICE_TYPE, INPUT_FILE="./ising/input.csv";
ofstream RESULT_FILE, COORD_FILE;

//////////////////////////////////////////////////////////////
// Get the inputs from the file
//////////////////////////////////////////////////////////////
void get_input(){

	string variable, value;
	CSVReader reader(INPUT_FILE);
	vector<vector<string> > dataList = reader.getData(); // read the data from CSV file
	
	for(vector<string> vec : dataList){
		variable = vec[0];
		value = vec[1];
		cout << "Input: variable, value -- " << variable << "," << value<< endl;
		if (variable == "lattice_type")
			LATTICE_TYPE = string(value);
		else if (variable == "lx")
			LX = stoi(value);
		else if (variable == "ly")
			LY = stoi(value);
		else if (variable == "lz")
			LZ = stoi(value);
		else if (variable == "tstar")
			TSTAR = stod(value);
		else if (variable == "mcsteps")
			MCSTEPS = stoi(value);
		else if (variable == "eqsteps")
			EQSTEPS = stoi(value);
		else if (variable == "sample_freq")
			SAMPLE_FREQ = stoi(value);
		else if (variable == "initial_spin")
			INITIAL_SPIN = stoi(value);
		else if (variable == "exchange_energy")
			EXCHANGE_ENERGY = stod(value);
		else{
			cout << "Variable: " << variable << "\t" << "Value: " << value << endl;
			throw std::invalid_argument("Error! Invalid input.");
		}
	}

  RESULT_FILE.open ("./ising_model_results.csv");
  COORD_FILE.open ("./ising_model_vmd.xyz");

}

//////////////////////////////////////////////////////////////
// Get the lattice varibles
//////////////////////////////////////////////////////////////
void set_lattice(string lattice_type, int init_spin){

	int isite;
	Lattice lattice_obj;

	lattice_obj = get_lattice(LX, LY, LZ, &lattice_type);

	SPIN = lattice_obj.spin;
	N1LIST = lattice_obj.n1list;
	NSITES = lattice_obj.nsites;
	SITE_COORD = lattice_obj.site_coord;
	NEIGHBORS = lattice_obj.neighbors;

	for (isite=0; isite<NSITES; isite++)
		SPIN[isite] = init_spin;
	
}


#endif
