
void smearing(){
  //monte carlo
std::clock_t t1,t2;
t1=std::clock();
TFile *flux=new TFile("flux.root");
TH1D *hist=(TH1D*)flux->Get("numu_CV_AV_TPC");
TH1D histh=*hist;
TH1D* hmc=(TH1D*)hist->Clone("h2");
hmc->Reset();
hmc->SetLineColor(kBlue);
double resolution=0.15;
for (int i=0;i<400000;i++){
  double energy=hist->GetRandom();
  Double_t smeared=gRandom->Gaus(energy, resolution*energy);
  hmc->Fill(smeared);
}

//normalize
Double_t norm=1;
hmc->Scale(norm/hmc->Integral("width"));

//convolution

TH1D* hconv=(TH1D*)hist->Clone("h3");
hconv->Reset();
hconv->SetLineColor(kGreen);
for (int i=0;i<hist->GetNbinsX();i++){
  Double_t total=0.;
  for (int j=0;j<hist->GetNbinsX();j++){
    double xi=hist->GetXaxis()->GetBinCenter(i+1);
    double xj=hist->GetXaxis()->GetBinCenter(j+1);
    double yj=histh.GetBinContent(j+1);
    total+=yj* ROOT::Math::gaussian_pdf(xi,xj*resolution,xj);
  }
  hconv->SetBinContent(i,total*hist->GetBinWidth(1));

}

TCanvas *c1=new TCanvas();
hmc->Draw("hist same");
TCanvas *c2=new TCanvas();
hconv->Draw("hist same");
t2=std::clock();
float td=(float)t2-(float)t1;
float seconds=td/CLOCKS_PER_SEC;
std::cout<<"run time: "<<seconds<<std::endl;
//c1.Draw();
}
