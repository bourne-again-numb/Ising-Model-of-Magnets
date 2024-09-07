#ifndef _LATTICES
#define _LATTICES

#include <iostream>

using std :: string;
using std :: cout;
using std :: endl;

//////////////////////////////////////////////////////////////
// Class to store lattices
//////////////////////////////////////////////////////////////
class CreateLattice{						/* Class to create Lattice */

 public:
	int lx, ly, lz, nsites, neighbors;
	int *arr_spin;
	int **arr_n1list, **arr_coords;
  //CreateLattice() { };
  //~CreateLattice();
	//////////////////////////////////////////////////////////////
	// Get the 2D SCC lattice
	//////////////////////////////////////////////////////////////
	void set_2d_scc(){

		int isite, x, y, z, x0, x1, y0, y1;

		// === Initialize arrays
		// get nsites
		nsites = lx * ly;

		// get the spin
		arr_spin = new int [nsites];

		// get the neigbor list
		lz = 1;
		neighbors = 4;
		arr_n1list = new int*[nsites];
		for (int i=0; i<nsites; i++)
			arr_n1list[i] = new int [neighbors];

		// get the site_cooord
		arr_coords = new int*[nsites];
		for (int i=0; i<nsites; i++)
			arr_coords[i] = new int [3];

		for (x=0; x<lx; x++){ // loop over x

			for (y=0; y<ly; y++){ // loop over y

				for (z=0; z<lz; z++){ // loop over z
				
					isite = x + lx * (y + ly * z);

					x0 = x - 1;
					if (x0 == -1)
						x0 = lx - 1;

					x1 = x + 1;
					if (x1 == lx)
						x1 = 0;
				
					y0 = y - 1;
					if (y0 == -1)
						y0 = ly - 1;
				
					y1 = y + 1;
					if (y1 == ly)
						y1 = 0;
				
					// set up arr_n1list array
					arr_n1list[isite][0] = x0 + lx * (y + ly * z);
					arr_n1list[isite][1] = x1 + lx * (y + ly * z);
					arr_n1list[isite][2] = x + lx * (y0 + ly * z);
					arr_n1list[isite][3] = x + lx * (y1 + ly * z);

					// set up arr_coords array
					arr_coords[isite][0] = x;
					arr_coords[isite][1] = y;
					arr_coords[isite][2] = z;

				} //loop over z

			} // loop over y

		} // loop over x

	}

	//////////////////////////////////////////////////////////////
	// Get the 3D SCC lattice
	//////////////////////////////////////////////////////////////
	void set_3d_scc(){

		int isite, x, y, z, x0, x1, y0, y1, z0, z1;

		// === Initialize arrays
		// get nsites
		nsites = lx * ly * lz;

		// get the spin
		arr_spin = new int [nsites];

		// get the neigbor list
		neighbors = 6;
		arr_n1list = new int*[nsites];
		for (int i=0; i<nsites; i++)
			arr_n1list[i] = new int [neighbors];

		// get the site_cooord
		arr_coords = new int*[nsites];
		for (int i=0; i<nsites; i++)
			arr_coords[i] = new int [3];

		for (x=0; x<lx; x++){ // loop over x

			for (y=0; y<ly; y++){ // loop over y

				for (z=0; z<lz; z++){ // loop over z

					isite = x + lx * (y + ly * z);

					x0 = x - 1;
					if (x0 == -1)
						x0 = lx - 1;

					x1 = x + 1;
					if (x1 == lx)
						x1 = 0;

					y0 = y - 1;
					if (y0 == -1)
						y0 = ly - 1;

					y1 = y + 1;
					if (y1 == ly)
						y1 = 0;

					z0 = z - 1;
					if (z0 == -1)
						z0 = lz - 1;

					z1 = z + 1;
					if (z1 == lz)
						z1 = 0;

					// set up arr_n1list array
					arr_n1list[isite][0] = x0 + lx * (y + ly * z);
					arr_n1list[isite][1] = x1 + lx * (y + ly * z);
					arr_n1list[isite][2] = x + lx * (y0 + ly * z);
					arr_n1list[isite][3] = x + lx * (y1 + ly * z);
					arr_n1list[isite][4] = x + lx * (y + ly * z0);
					arr_n1list[isite][5] = x + lx * (y + ly * z1);

					// set up arr_coords array
					arr_coords[isite][0] = x;
					arr_coords[isite][1] = y;
					arr_coords[isite][2] = z;

				} //loop over z

			} // loop over y

		} // loop over x

	}
	//////////////////////////////////////////////////////////////
	// Set the values for the class
	//////////////////////////////////////////////////////////////
	void set_values(int length, int width, int height){
		lx = length;
		ly = width;
		lz = height;
	}
};
// //////////////////////////////////////////////////////////////
// // Set the values for the class
// //////////////////////////////////////////////////////////////
// CreateLattice::~CreateLattice(){

//   if (arr_spin != nullptr)
//     delete arr_spin;

//   for (unsigned long i=0; i<sizeof(arr_n1list); i++){
//     if (arr_n1list[i] != nullptr)
//       delete arr_n1list[i];
//   }
//   delete [] arr_n1list;

//   for (unsigned long i=0; i<sizeof(arr_coords); i++){
//     if (arr_coords[i] != nullptr)
//       delete arr_coords[i];
//   }
//   delete [] arr_coords;

//   std::cout << "class destroyed" << std::endl;
    
// }
	
//////////////////////////////////////////////////////////////
// Top level function
//////////////////////////////////////////////////////////////
void set_lattice(CreateLattice* lattice_obj, int lx, int ly, int lz, string lattice_type){

  (*lattice_obj).set_values(lx, ly, lz);
  (*lattice_obj).set_2d_scc();
	
	if ( lattice_type == "2Dscc"){  // if the model is 2D simple cubic
		(*lattice_obj).set_2d_scc();
	}
	else if ( lattice_type == "3Dscc"){  // if the model is 3D simple cubic
		(*lattice_obj).set_3d_scc();
	}
	else {  // if the model is not simple cubic
		std :: cout << "Lattice type: " << lattice_type << std :: endl;
		throw std::invalid_argument("lattices.h: Invalid lattice type");
	}

}


#endif
