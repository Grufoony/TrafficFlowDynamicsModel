#include <fstream>

void macro() {
    auto canv = new TCanvas("canv", "canv", 4096, 2160);
    canv->Divide(2,2);
    auto h1 = new TH1F("h1", "titolo", 25, 1, 100);
    auto h2 = new TH1F("h2", "titolo", 25, 1, 100);
    auto h3 = new TH1F("h3", "titolo", 25, 1, 100);
    std::ifstream fIn;
    double data;
    fIn.open("./temp_data/3000_t.dat");
    while(fIn >> data) {
        h1->Fill(data);
    }
    fIn.close();
    h1->Scale(1./h1->Integral(), "nosw2");
    fIn.open("./temp_data/5200_t.dat");
    while(fIn >> data) {
        h2->Fill(data);
    }
    fIn.close();
    fIn.open("./temp_data/10900_t.dat");
    while(fIn >> data) {
        h3->Fill(data);
    }
    fIn.close();

    canv->cd(1);
    h1->Fit("gaus");
    h1->Draw("hist, same");
    canv->cd(2);
    // auto f2 = new TF1("f2","[0]*TMath::Exp([1]*x)", 30, 100);
    auto f2 = new TF1("f2","[0]*TMath::Gaus(TMath::Log(x), [1], [2])/x", 0, 100);
    f2->SetParameters(4.5, 30, 100);
    h2->Fit(f2, "R");
    // h2->Fit("gaus");
    h2->Draw("HIST,SAME");
    canv->cd(3);
    // auto g1= new TF1("g1","gaus", 0, 100);
    // g1->SetParameters(2, 19, 8);
    // auto g2= new TF1("g2","gaus", 0, 100);
    // g2->SetParameters(2, 60, 8);
    auto f3 = new TF1("f3", "[0]*TMath::Gaus(x, [1], [2])+[3]*TMath::Gaus(x, [4], [5])", 0, 100);
    f3->SetParameters(6, 19, 8, 2, 40, 8);

    h3->Fit(f3, "R");
    h3->Draw("HIST,SAME");


    canv->Print("./prova.png");
    delete h1;
    delete h2;
    delete h3;
    delete canv;
}