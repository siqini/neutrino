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
//for (int i=0;i<200;i++){
  //std::cout<<"probability "<<i<<" "<<prob_arr[i]<<std::endl;}

// compute the oscillated flux
double osc_flux[200];
  for (int i=0;i<200;i++)
    osc_flux[i]=prob_arr[i]*ini_flux[i];
//for (int i=0;i<200;i++){
  //std::cout<<"oscillated flux "<<i<<" "<<osc_flux[i]<<std::endl;
//}

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
    // with 30% normalization error
    // contri=diff_squared[i]/(ini_flux[i]+pow((0.3*ini_flux[i]),2));
    chi_squared+=contri;
  }
}
//std::cout<< "Your chi squared for sin_squared of double the mixing angle= "<< sin_squared << " and the delta m squared ="<<dms<<" is "<<chi_squared<<" . Allons-y!";
return chi_squared;
}

void chi2ori(){
  //select discrete sin^2(2theta) and delta m^2 values on a log scale
  //c.Divide(2,2);
  /*
  TFile *flux=new TFile("flux.root");
  TH1D *hist=(TH1D*)flux->Get("numu_CV_AV_TPC");
  c.cd(1);
  hist->Draw();

  TFile *x_sec=new TFile("cross_section.root");
  TGraph *x_sec_graph=(TGraph*)x_sec->Get("qel_cc_n");
  c.cd(2);
  x_sec_graph->Draw();
 */
  double angles[200];
  for (int i=0;i<200;i++){
    angles[i]=pow(10.,(-2.+0.01*i));
  }
  double masses[400];
  for (int i=0;i<400;i++){
    masses[i]=pow(10.,(-2+0.0075*i));
  }


  std::vector<double> chi2_vec;
  //std::vector<vector<double>> chi2_2d_vec;
  std::ofstream outputFile;
  std::ofstream chi2_file;
  std::string filename="chi2_new.csv";
  chi2_file.open("chi2_new.csv", std::ios::out);
  if(chi2_file.is_open()){
  for (int i=0;i<200;i++){
    for (int j=0;j<400;j++){
      double chi2_entry=chi2(angles[i], masses[j]);
      chi2_vec.push_back(chi2_entry);
      // write the chi^2 grid to a textfile
      chi2_file<<angles[i]<<" "<<masses[j]<<" "<<chi2_entry<<std::endl;
    }
  }}

// convert the vector to a 2d array
  double* chi2_arr=chi2_vec.data();
  double chi2_2d_arr[200][400];
  for (int i=0;i<chi2_vec.size();i++){
    chi2_2d_arr[i/400][i%400]=chi2_arr[i];
  }
  std::cout<<"the size of the vector is "<<chi2_vec.size()<<std::endl;
  //for (int i=0;i<200;i++){
  //  for (int j=0;j<400;j++){
  //    std::cout<< i << " "<< j << " "<< chi2_2d_arr[i][j]<<std::endl;
  //  }
  //}

  //create arrays of angles and masses
  /*
  double angles[198];
  double masses[200];
  for (int i=0;i<198;i++){
    angles[i]=0.01+0.005*i;
  }
  for (int i=0;i<200;i++){
    masses[i]=0.01+0.05*i;
  }
  */
  //void drawContours(Double_t **Chi, Int_t nX, Int_t nY, Double_t xs[], Double_t ys[]){
   //Contour values = {1.64, 7.74, 23.40}, corresponding to dX^2_90, dX^2_3sigma, and dX^2_5sigma, respectively.

  //find the smallest chi2 values
  double chi2_min=10.;
  for (int i=0;i<200;i++){
    for (int j=0;j<400;j++){
      if (chi2_2d_arr[i][j]<chi2_min) {chi2_min=chi2_2d_arr[i][j];}
    }
  }
  std::cout<<"chi2 min: "<<chi2_min<<std::endl;
  //update the chi2 array
  for (int i=0;i<200;i++){
    for (int j=0;j<400;j++){
      chi2_2d_arr[i][j]-=chi2_min;
    }
  }






   TCanvas *c=new TCanvas("c", "chi^2 distribution", 1000,1000);
   TH2D *select_contours=new TH2D("select_contours","Select Contours",199,angles,399,masses);
   int pts_counter=0;
   for (int i=0;i<200;i++){
     for (int j=0;j<400;j++){
       if((chi2_2d_arr[i][j+1]>1.64 && chi2_2d_arr[i][j-1]<1.64)||(chi2_2d_arr[i+1][j]>1.64 && chi2_2d_arr[i-1][j]<1.64)){
         pts_counter++;
        select_contours->Fill(angles[i],masses[j],1.64);
        //gr1->SetPoint(pts_counter1,angles[i],masses[j]);
        //pts1x.push_back(angles[i]);
        //pts1y.push_back(masses[j]);
        }
       if((chi2_2d_arr[i][j+1]>7.74 && chi2_2d_arr[i][j-1]<7.74)||(chi2_2d_arr[i+1][j]>7.74 && chi2_2d_arr[i-1][j]<7.74)){
         pts_counter++;
         select_contours->Fill(angles[i],masses[j],7.74);
        //gr2->SetPoint(pts_counter2,angles[i], masses[j]);
        //pts2x.push_back(angles[i]);
        //pts2y.push_back(masses[j]);
        }
       if((chi2_2d_arr[i][j+1]>23.4 && chi2_2d_arr[i][j-1]<23.4)||(chi2_2d_arr[i+1][j]>23.4 && chi2_2d_arr[i-1][j]<23.4)){
         pts_counter++;
         select_contours->Fill(angles[i],masses[j],23.4);
        //gr3->SetPoint(pts_counter3,angles[i],masses[j]);
        //pts3x.push_back(angles[i]);
        //pts3y.push_back(masses[j]);
        }
     }
   }
   gStyle->SetPalette(1);
   gPad->SetLogy();
   gPad->SetLogx();
   gPad->SetLogz();
   select_contours->SetTitle("Chi^2 Contours; sin^2(2theta); delta m^2 (eV^2)");
   select_contours->Draw("COLZ");





}
