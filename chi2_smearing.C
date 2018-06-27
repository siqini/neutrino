#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

double chi2(double sin_squared, double dms){
  // read data in the initial flux text file into a 1D array
  double resolution=0.25;
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

//smear it!
//TFile *flux=new TFile("flux.root");
//TH1D *hist=(TH1D*)flux->Get("numu_CV_AV_TPC");
//TH1D histh=*hist;
double osc_flux_smeared[200];
for (int i=0;i<200;i++){
  double total=0.;
  for (int j=0;j<200;j++){
    double xi=0.025+0.05*i;
    double xj=0.025+0.05*j;
    double yj=osc_flux[j];
    auto gaussian_scaler=ROOT::Math::gaussian_pdf(xi,xj*resolution,xj);
    total += yj*gaussian_scaler;
  }
  osc_flux_smeared[i]=total*0.05;
}

double diff[200];
double diff_squared[200];
double errors[200];
double contri;
double chi_squared=0.;
for (int i=0;i<200;i++){
  diff[i]=ini_flux[i]-osc_flux[i];
}
for (int i=0;i<200;i++){
  diff_squared[i]=pow(diff[i],2.);
}
for (int i=0;i<200;i++){
  errors[i]=osc_flux_smeared[i] + (osc_flux_smeared[i]/pow(ini_flux[i],0.5));
}
for (int i=0;i<200;i++){
  if (ini_flux[i]==0.) continue;
  else
  {
    contri=diff_squared[i]/errors[i];
    chi_squared+=contri;
  }
}
//std::cout<< "Your chi squared for sin_squared of double the mixing angle= "<< sin_squared << " and the delta m squared ="<<dms<<" is "<<chi_squared<<" . Allons-y!";
return chi_squared;
}

void chi2_smearing(){
  std::clock_t t1,t2;
  t1=std::clock();
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

double chi2_sum=0.;
double chi2_avg;
for (int i=0;i<200;i++){
  for (int j=0;j<400;j++){
    chi2_sum+=chi2_2d_arr[i][j];
  }
}
chi2_avg=chi2_sum/(double)chi2_vec.size();



  double chi2_min=chi2_avg;
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
       /*
       if((chi2_2d_arr[i][j+1]>1.64 && chi2_2d_arr[i][j-1]<1.64)||(chi2_2d_arr[i+1][j]>1.64 && chi2_2d_arr[i-1][j]<1.64)){
         pts_counter++;
        select_contours->Fill(angles[i],masses[j],1.64);
        }
       if((chi2_2d_arr[i][j+1]>7.74 && chi2_2d_arr[i][j-1]<7.74)||(chi2_2d_arr[i+1][j]>7.74 && chi2_2d_arr[i-1][j]<7.74)){
         pts_counter++;
         select_contours->Fill(angles[i],masses[j],7.74);
        }
        */
       if((chi2_2d_arr[i][j+1]>23.4 && chi2_2d_arr[i][j-1]<23.4)||(chi2_2d_arr[i+1][j]>23.4 && chi2_2d_arr[i-1][j]<23.4)){
         pts_counter++;
         select_contours->Fill(angles[i],masses[j],23.4);
        }
     }
   }
   std::cout<<"number of filled points: "<<pts_counter<<std::endl;
   gStyle->SetPalette(1);
   gPad->SetLogy();
   gPad->SetLogx();
   gPad->SetLogz();
   select_contours->SetTitle("Chi^2 Contours; sin^2(2theta); delta m^2 (eV^2)");
   select_contours->Draw("COLZ");
   t2=std::clock();
   float td=(float)t2-(float)t1;
   float seconds=td/CLOCKS_PER_SEC;
   std::cout<<"run time: "<<seconds<<std::endl;


   /*
   TGraph2D *g=new TGraph2D("chi2_new.csv");
   g->SetTitle("chi^2 distribution; sin^2(2theta); delta m^2 (eV^2);chi^2");
   //c.cd(3);
   gPad->SetLogy();
   gPad->SetLogx();
   gPad->SetLogz();
   gStyle->SetPalette(1);

   g->Draw("CONT5");
   */
}
