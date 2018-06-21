#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

double chi2(double sin_squared, double dms){
  // read data in the initial flux text file into a 1D array
  std::ifstream myfile;
  myfile.open("initial.txt", std::ios::in|std::ios::out|std::ios::binary);
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

  double* ini_flux= initial_flux_vec.data();

// compute the probability array
  double prob_arr [200];
      for (int i=0;i<200;i++){
        double E=(50.*i+25.)*1e-3;
        double sin_ratio;
        sin_ratio=pow(sin(1.27*0.6*dms/E),2.);
        //std::cout<<" energy for "<<i<< " is "<<E<<std::endl;
        prob_arr[i]=1-sin_squared * sin_ratio;}
for (int i=0;i<200;i++){
  std::cout<<"probability "<<i<<" "<<prob_arr[i]<<std::endl;}

// compute the oscillated flux
double osc_flux[200];
  for (int i=0;i<200;i++)
    osc_flux[i]=prob_arr[i]*ini_flux[i];
for (int i=0;i<200;i++){
  std::cout<<"oscillated flux "<<i<<" "<<osc_flux[i]<<std::endl;
}

// compute chi^2
double diff[200];
double diff_squared[200];
double contri;
double chi_squared=0.;
for (int i=0;i<200;i++){
  diff[i]=ini_flux[i]-osc_flux[i];
}
for (int i=0;i<200;i++){
  diff_squared[i]=pow(diff[i],2.);
}
for (int i=0;i<200;i++){
  if (ini_flux[i]==0.) continue;
  else
  {
    contri=diff_squared[i]/ini_flux[i];
    chi_squared+=contri;
  }
}
//std::cout<< "Your chi squared for sin_squared of double the mixing angle= "<< sin_squared << " and the delta m squared ="<<dms<<" is "<<chi_squared<<" . Allons-y!";
return chi_squared;
}

int main(){
  std::cout<<" check!! " <<chi2(0.1,1.)<<std::endl;
  /*
  std::vector<double> chi2_vec;
  std::ofstream outputFile;
  std::ofstream chi2_file;
  std::string filename="chi2.csv";
  chi2_file.open("chi2.csv", std::ios::out);
  if(chi2_file.is_open()){
  for (double angle=0.01;angle<1.;angle+=0.005){
    for (double mass_diff=0.01;mass_diff<10.;mass_diff+=0.05){
      double chi2_entry=chi2(angle, mass_diff);
      chi2_vec.push_back(chi2_entry);

      chi2_file<<angle<<" "<<mass_diff<<" "<<chi2_entry<<std::endl;
    }
  }}
  */
  //chi2_file.close();
  //double* chi2_arr=chi2_vec.data();
  //std::cout<<"the size of the vector is "<<chi2_vec.size()<<std::endl;

//  {
//    std::ofstream outputFile;
  //  std::ofstream chi2_file;
    //std::string filename="chi2.csv";
    //chi2_file.open("chi2.csv");
    //for (int i=0;i<chi2_vec.size();i++) {
      //chi2_file<<chi2_arr[i]<<" "<<std::endl;
    //}

    //chi2_file.close();
  //}
  //for (int j=0;j<chi2_vec.size();j++){
  //  std::cout<<"the chi2 for "<<j<<" is "<<chi2_arr[j]<<std::endl;
  //}
  return 0;
}
