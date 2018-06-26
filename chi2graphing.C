void chi2graphing(){
  TCanvas *c=new TCanvas("c", "chi^2 distribution", 0, 0, 600, 400);
  c->SetLogx();
  c->SetLogy();
  //TPad *pad1 = new TPad("pad1", "This is pad1", 0,0,540,360);
  //pad1->Draw();
  //pad1->SetLogx();
  //pad1->SetLogy();
  TGraph2D *g=new TGraph2D("chi2.csv");
  g->SetTitle("chi^2 distribution; sin^2(2theta); delta m^2 (eV^2);chi^2");
  gStyle->SetPalette(1);
  //g->Draw("CONT5");
  g->Draw("COL2Z");
}
