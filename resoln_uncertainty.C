#include<sstream>
#include<string>
#include<vector>
using namespace std;

Double_t* sample_universe(int index, double resolution){
  TFile* flux=new TFile("flux.root");
  TH1D* hist=(TH1D*)flux->Get("numu_CV_AV_TPC");
  //TH1D histh=*hist;
  ifstream myfile;
  myfile.open("initial.txt", ios::in|ios::out|ios::binary);
  vector<double> initial_flux_vec;

  if (!myfile.is_open()){
    cerr<< "There was a problem opening the initial events file!\n";
    exit(1);
  }

  double entry=0.0;
  while (myfile>>entry){
    initial_flux_vec.push_back(entry);
  }
  Double_t* null_events=initial_flux_vec.data();

  int nbins = hist->GetNbinsX();
  Double_t* smeared_events = new Double_t[nbins];
  for (int i=0;i<nbins;i++){
    double total=0.;
    for (int j=0;j<nbins;j++){
      double xi=0.025+0.05*i;
      double xj=0.025+0.05*j;
      double yj=null_events[j];
      double sigma=resolution*sqrt(xj);
      auto gaussian_scaler=ROOT::Math::gaussian_pdf(xi,sigma,xj);
      total+=yj*gaussian_scaler;
    }
    smeared_events[i]=total*0.05;
  }
  return null_events;
}

void resoln_uncertainty(){
  Double_t* hcv=sample_universe(0,0.25);
  TFile* flux=new TFile("flux.root");
  TH1D* hist=(TH1D*)flux->Get("numu_CV_AV_TPC");
  TH1D histh=*hist;
  int nbins=hist->GetNbinsX();
  int nuniverses=2000;

  TH2D *cov=new TH2D("cov", "covariance matrix;Bin index;Bin index",nbins,0.,(double)nbins,nbins,0.,(double)nbins);


  //loop over universes
  for (int i=0;i<nuniverses;i++){
    //an array given some randomly-sampled resolution
    auto res=gRandom->Gaus(0.15,0.05);
    Double_t* h=sample_universe(i,res);
  }

  //loop over all pairs of bins
  for (int i=0;i<nbins;i++){
    for (int j=0;j<nbins;j++){
      auto ci=hcv[i]-histh.GetBinContent(i+1);
      auto cj=hcv[j]-histh.GetBinContent(j+1);

      auto v=cov->GetBinContent(i+1,j+1)+(ci*cj)/nuniverses;
      cov->SetBinContent(i+1,j+1,v);
    }
  }
  cov->Draw("COLZ");
}
