
// reading a text file

/*
void csv_to_1d_array () {
    double data[200];
    std::ifstream file("initial_flux_data.csv");
    for (int i=0;i<200;i++) {
      std::string line;
      std::getline(file, line);
      if ( !file.good()) break;
      //std::stringstream iss(line);
      std::stringstream convertor(line);
      convertor>>data[i];
    }
    for (int i=0;i<200;i++){
      std::cout<<" The initial flux event number for "<<i<<" is "<<data[i]<<std::endl;
    }
    }

int main(){
  csv_to_1d_array();
  return 0;
}
*/
/*
int main(){
  std::ifstream file("initial_flux_data.csv");//, std::ios::in);
  std::vector<double> vflux;
  //check to see if the file is opened correctly
  //if (!file.is_open()){
  //  std::cerr<<"Opening failed!"<<std::endl;
  //    exit(1);
//}

  double num=0.;
  while (file>>num){
    vflux.push_back(num);
  }
  for (int i=0;i<vflux.size();i++)
  std::cout<<i<<" "<<vflux[i]<<std::endl;
  return 0;
}
*/
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

int main() {
    std::ifstream myfile;
    myfile.open("initial.txt", ios::in|ios::out|ios::binary);
    std::vector<double> initial_flux_vec;

    //check to see that the file was opened correctly:
    if (!myfile.is_open()) {
        std::cerr << "There was a problem opening the input file!\n";
        exit(1);//exit or do additional error checking
    }

    double entry = 0.0;
    //keep storing values from the text file so long as data exists:
    while (myfile >> entry) {
        initial_flux_vec.push_back(entry);
    }

    double* initial_flux_arr = initial_flux_vec.data();


    //verify that the scores were stored correctly:
    for (int i = 0; i < initial_flux_vec.size(); ++i) {
        std::cout << initial_flux_arr[i] << std::endl;
    }

    return 0;
}
