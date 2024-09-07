#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using std :: string;
using std :: vector;
 
/*
 * A class to read data from a csv file.
 */
class CSVReader
{
  string fileName;
  string delimeter;
 
public:
	CSVReader(string filename, string delm = ",") :
			fileName(filename), delimeter(delm)
	{ }
 
	// Function to fetch data from a CSV File
  vector<vector<string> > getData();
};
 
/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
vector<vector<string> > CSVReader::getData()
{
	std::ifstream file(fileName);
 
  vector<vector<string> > dataList;
 
  string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
	  vector<std::string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}
	// Close the File
	file.close();
 
	return dataList;
}
