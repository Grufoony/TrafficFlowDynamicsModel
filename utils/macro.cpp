#include <fstream>

void macro() {
  // Style Setting
  gStyle->SetOptStat(0);

  auto canv = new TCanvas("canv", "canv", 4096, 2160);

  auto h1 = new TH1F("h1", "", 25, 1, 100);
  auto h2 = new TH1F("h2", "", 25, 1, 100);
  auto h3 = new TH1F("h3", "", 25, 1, 100);

  std::ifstream fIn;
  double data;
  fIn.open("../temp_data/3000_t.dat");
  while (fIn >> data) {
    h1->Fill(data);
  }
  fIn.close();
  h1->Scale(1. / h1->Integral(), "nosw2");
  fIn.open("../temp_data/5200_t.dat");
  while (fIn >> data) {
    h2->Fill(data);
  }
  fIn.close();
  h2->Scale(1. / h2->Integral(), "nosw2");
  fIn.open("../temp_data/10900_t.dat");
  while (fIn >> data) {
    h3->Fill(data);
  }
  fIn.close();
  h3->Scale(1. / h3->Integral(), "nosw2");

  h1->Fit("gaus");
  h1->Draw("HIST,SAME");
  canv->Print("../data/img/normal.png");

  // auto f2 = new TF1("f2","[0]*TMath::Exp([1]*x)", 30, 100);
  // auto f2 = new TF1("f2","[0]*TMath::Gaus(TMath::Log(x), [1], [2])/x", 0,
  // 100); f2->SetParameters(4.5, 30, 100); h2->Fit(f2, "R");
  h2->Fit("gaus");
  h2->Draw("HIST,SAME");
  canv->Print("../data/img/lognormal.png");

  auto f3 =
      new TF1("f3", "[0]*TMath::Gaus(x, [1], [2])+[3]*TMath::Gaus(x, [4], [5])",
              0, 100);
  f3->SetParameters(6e-3, 19, 8, 2e-3, 40, 8);

  h3->Fit(f3, "R");
  h3->Draw("HIST,SAME");
  canv->Print("../data/img/bimodal.png");

  delete h1;
  delete h2;
  delete h3;
  delete canv;
}