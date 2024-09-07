/*
	The Ising Model code
 */

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// my personal headers
//#include "./ising_model_globals.cpp"
#include "./initialize.cpp"
#include "./include/my_random.h"

// define name spaces
using std :: cout;
using std :: endl;
using std :: ofstream;
// using std :: class;

// prototype functions
double get_total_energy();
void _debug_lattice();
void _debug_random(RandomNumber my_random);
void clean_up();
//////////////////////////////////////////////////////////////
// Generate VMD snapshot
//////////////////////////////////////////////////////////////
void get_vmd_snapshot(){

	int isite, ispin;
	char identity;

	COORD_FILE << "" << NSITES << endl;
	COORD_FILE << "atom,x,y,z" << endl;

	for (isite=0; isite<NSITES; isite++){

		ispin = SPIN[isite];
		if (ispin == 1) {
			identity = 'H';
		}
		else if (ispin == -1) {
			identity = 'O';
		}
		else {
			cout << "Error: Invalid spin: " << ispin << endl;
			throw std::runtime_error("Runtime Error!");
		}

		COORD_FILE << identity <<"  "<< double(SITE_COORD[isite][0]) <<"  "<< double(SITE_COORD[isite][1]) <<"  "<< double(SITE_COORD[isite][2]) << endl;
	}

}
//////////////////////////////////////////////////////////////
// Generate output
//////////////////////////////////////////////////////////////
void generate_output(int step, double total_en){

	int isite, total_mag=0;
	double inst_tot_en;

	// write files
	if ( (step >= EQSTEPS) && (step % SAMPLE_FREQ == 0) ){

		for (isite=0; isite<NSITES; isite++)
			total_mag = total_mag + SPIN[isite];

		RESULT_FILE << step << "," << total_en << "," << total_mag << endl; // results file
		get_vmd_snapshot();

		cout << "===== " << step << " =====" << endl;
		cout << "Total energy: " << total_en << endl;
		cout << "Magnetization: " << total_mag << endl;

		// validation
		inst_tot_en = get_total_energy();
		if (total_en !=  inst_tot_en){
			cout << "Total / Instantenous: " << total_en <<"\t"<< inst_tot_en << endl;
			throw std::runtime_error( "Energy calculation error" );
		}

	}

}
//////////////////////////////////////////////////////////////
// GET SITE ENERGY
//////////////////////////////////////////////////////////////
double get_site_energy(int isite){

	int ispin, ineigh, jsite, jspin;
	double isite_en;

	ispin = SPIN[isite];
	isite_en = 0.0;
	for (ineigh=0; ineigh<NEIGHBORS; ineigh++){ // loop over all neighbors of isite
		jsite = N1LIST[isite][ineigh];
		jspin = SPIN[jsite];
		isite_en = isite_en + double (-1) * EXCHANGE_ENERGY * ispin * jspin;
	}	// loop over all neighbors of isite

	return isite_en;
}
//////////////////////////////////////////////////////////////
// GET TOTAL ENERGY
//////////////////////////////////////////////////////////////
double get_total_energy(){

	int isite;
	double total_en;

	total_en = 0.0;
	for (isite=0; isite<NSITES; isite++)
		total_en = total_en + get_site_energy(isite);

	return double(total_en / double(2.0));

}
//////////////////////////////////////////////////////////////
// Ising Model MC
//////////////////////////////////////////////////////////////
void run_mc(){

	int isite, step, ineigh, sum_jspin;
	double total_en, delta_en, seed, acceptance_ratio, rand_num;
	RandomNumber random_generator;

	// initialize random number generator
	seed = time(0);
  random_generator.initialize(seed);

	// get the total energy of the system
	total_en = get_total_energy();
	get_vmd_snapshot();

	for (step=0;step<MCSTEPS; step++){ // loop over all the MC Steps

		for (isite=0; isite<NSITES; isite++){ // loop over all sites

			// calculate change in energy while spin flip
			sum_jspin = 0;
			for (ineigh=0; ineigh<NEIGHBORS; ineigh++)
				sum_jspin = sum_jspin + SPIN[ N1LIST[isite][ineigh] ];

			// Metropolis criteria
			delta_en = double(2) * EXCHANGE_ENERGY * SPIN[isite] * sum_jspin;
			acceptance_ratio = exp(-delta_en / TSTAR);
			rand_num = random_generator.get(0, 1);
			if (  acceptance_ratio >=  rand_num ){
				total_en = total_en + delta_en;
				SPIN[isite] = -1 * SPIN[isite];
			}

		}	// loop over all sites

		generate_output(step, total_en);

	}	// loop over all the MC Steps

}
//////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////
int main(){
	/* The main executable function	 */
	// 1. get inputs
	get_input();
	cout << "Read Input -- Done!" << endl;

	// 2. set the lattice parameters
	set_lattice(LATTICE_TYPE, INITIAL_SPIN);
	cout << "Lattice Setup -- Done!" << endl;

	// _debug_lattice();
	// _debug_random(random_number);

	run_mc();
	// finally: clean up the code
	clean_up();

	return 0;
}
//////////////////////////////////////////////////////////////
// CLEAN UP
//////////////////////////////////////////////////////////////
void clean_up(){

	// close the result file
	RESULT_FILE.close();
	COORD_FILE.close();

	// clean up memory
	delete[] SPIN;

	for (unsigned int i=0; i < sizeof N1LIST / sizeof(int*); i++)
		delete[] N1LIST[i];
	delete[] N1LIST;

	for (unsigned int i=0; i < sizeof SITE_COORD / sizeof(int*); i++)
		delete[] SITE_COORD[i];
	delete[] SITE_COORD;

}
//////////////////////////////////////////////////////////////
// DEBUG LATTICE
//////////////////////////////////////////////////////////////
void _debug_lattice(){

	for (int isite=0; isite<NSITES; isite++){
		int x = SITE_COORD[isite][0];
		int y = SITE_COORD[isite][1];
		int z = SITE_COORD[isite][2];

		cout << "=============================" << endl;
		cout << "**" << "\t" << x << "\t" << y << "\t" << z << "\t" << "\t" << isite << endl;

		for (int neigh=0; neigh<NEIGHBORS;neigh++){
			int neigh_site = N1LIST[isite][neigh];
			int x_neigh = SITE_COORD[neigh_site][0];
			int y_neigh = SITE_COORD[neigh_site][1];
			int z_neigh = SITE_COORD[neigh_site][2];
			cout << "neigh: " << neigh_site << "\t" << x_neigh << "\t" << y_neigh << "\t" << z_neigh << endl;
		}

	}
}
//////////////////////////////////////////////////////////////
// DEBUG Random Number
//////////////////////////////////////////////////////////////
void _debug_random(RandomNumber my_random){

	int count=1000000;
	double rand_sum = 0.0;

	ofstream myfile ("./data/random_numbers.csv");
  if (myfile.is_open()){

		for (int i=0; i<count; i++){
			double i_rand = my_random.get(0, 1);
			rand_sum = rand_sum + i_rand;
			myfile << i << "," <<  i_rand << endl;
		}
		myfile.close();							// close the file
	}
  else cout << "Unable to open file";

	cout << "Avg: " << rand_sum / double(count) << endl;

}
