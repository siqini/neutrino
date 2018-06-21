void macro1()
{
    // read data points from the flux histogram

    TFile *flux=new TFile("flux.root");
    TH1D *hist=(TH1D*)flux->Get("numu_CV_AV_TPC");
    TH1D histh=*hist;
    int size=histh.GetNbinsX();
    Double_t *hbins=new double[size];
    for (Int_t i=0;i<size;i++)    hbins[i]=histh.GetBinContent(i);
    // double hhbins[size]= *hbins;


    {ofstream outputFile;
    ofstream fhist;
    string filename="flux_data.csv"; //the file that contains the bin heights
    fhist.open("flux_data.csv");
    for (int i=0;i<size;i++) fhist<<hbins[i]<<" "<<endl;
    fhist.close();
    // hist->Draw();
    }


    // read data points at bin centers from the cross section graph
    TFile *x_section=new TFile("cross_section.root");
    TGraph *xsec=(TGraph*) x_section->Get("qel_cc_n");
    TGraph xsecn=*xsec;
    Double_t *yvalues=new double[size];
    for (Int_t i=0;i<size;i++) yvalues[i]=xsecn.Eval((50.*i+25.)*(pow(10.,-3)));
    // double yyvalues[size]= *yvalues;


    {ofstream outputFile;
    ofstream fxsec;
    string filename="cross_section_data.csv";
    fxsec.open("cross_section_data.csv");
    for (int i=0;i<size;i++) fxsec<<yvalues[i]<<" "<<endl;
    fxsec.close();
    // xsec->Draw();
    }

    //distance scaling
    double dis_scale=pow((470./600.),2);
    std::cout<<"The distance scaler is "<<dis_scale<<endl;

    //proton-on-target (POT) adjustment
    double POT=6e20;
    double POT_scaling_factor=POT/1e6;
    std::cout<<"The POT scaling factor is "<<POT_scaling_factor<<endl;

    //area unit adjustment
    double area_scaling_factor=1e-38 * 1e-4;
    std::cout<<"The area scaling factor is "<<area_scaling_factor<<endl;

    //number of argon atoms
    double molar_mass=39.948;
    double active_mass=4.76e8;
    double n_moles=active_mass/molar_mass;
    double avogadro=6.02e23;
    double n_argon=n_moles * avogadro;
    std::cout<<"The number of argons is "<<n_argon<<endl;

    //compute!
    Double_t *n_initial_flux=new double[size];
    for (int i=0;i<size;i++) n_initial_flux[i]=hbins[i] * yvalues[i]*dis_scale*POT_scaling_factor*area_scaling_factor*n_argon;
    {
      ofstream outputFile;
      ofstream f_initial_flux;
      string filename="initial_flux_data.csv";
      f_initial_flux.open("initial_flux_data.csv");
      for (int i=0;i<size;i++) f_initial_flux<<n_initial_flux[i]<<" "<<endl;
      f_initial_flux.close();
    }}
/*
    Double_t *energy=new double[size];
    for (int i=0;i<size;i++)
    {energy[i]=(50.*i+25.)*(pow(10.,-3));
    //std::cout<<"the energy for "<<i<<" is "<< energy[i]<<endl;}
    double L=600.;
    // double theta=;
    double dms=1.;
    double sin_squared=0.5;

    Double_t *n_oscillated_flux=new double[size];
    Double_t *probability=new double[size];
    for (int i=0;i<size;i++){
    double surv_prob, sin_ratio;
    // sin_squared = pow ((sin(2.*theta)),2.);
    sin_ratio = pow((sin(dms*L/(4*energy[i]))),2.);
    surv_prob=1-sin_squared*sin_ratio;
    probability[i]=surv_prob;
     n_oscillated_flux[i]=n_initial_flux[i]*surv_prob;
   }

   {
     ofstream outputFile;
     ofstream f_oscillated_flux;
     string filename="oscillated_flux_data.csv";
     f_oscillated_flux.open("oscillated_flux_data.csv");
     for (int i=0;i<size;i++) f_oscillated_flux<<n_oscillated_flux[i]<<" "<<endl;
     f_oscillated_flux.close();
   }

   double chi_squared=0.;

   for (int i=0;i<size;i++){
     if (n_initial_flux[i]!= 0.){
     double diff= n_initial_flux[i]-n_oscillated_flux[i];
     double diff_squared = pow(diff,2.);
     double contribution = diff_squared/(n_initial_flux[i]);
     chi_squared += contribution;
     //std::cout<< "Contribution from " << i << " is "<< contribution<<endl;
     }
     else continue;
   }





for (int i=0;i<size;i++){
  std::cout<<"the initial flux for "<<i<< " is "<<n_initial_flux[i]<<endl;
}

for (int i=0;i<size;i++){
  std::cout<<"the oscillated flux for "<<i<< " is "<<n_oscillated_flux[i]<<endl;
}

for (int i=0;i<size;i++){
  std::cout<<"the difference for "<<i<< " is "<<n_initial_flux[i]-n_oscillated_flux[i]<<endl;
}
*/
//for (int i=0;i<size;i++){
//  std::cout<<"the probability for "<<i<< " is "<<probability[i]<<endl;
//}
/*

   std::cout<<"The chi^2 value is "<<chi_squared<< ". ALLONS-Y! "<<std::endl;}}

*/






  /*
  TCanvas *c= new TCanvas("c", "Initial and Oscillated Fluxes", 0,0,800,800);
  */


  // draw the initial flux histogram

  /*
  auto ini_flux_hist=new TH1D("initial_flux","Initial Flux;E(GeV);N_{events}",200,0.,10.);
  for (int i=0;i<200;i++)
  ini_flux_hist->Fill(i,n_initial_flux[i]);
  ini_flux_hist->Draw();
  */
  /*
  auto osc_flux_hist=new TH1D("osc_flux","Oscillated Flux;E(GeV);N_{events}",200,0.,10.);
  for (int i=0;i<200;i++)
  osc_flux_hist->Fill(i, n_oscillated_flux[i]);
  osc_flux_hist->Draw();
  */
